﻿#include <QApplication>
#include <QImage>
#include <QTime>
#include <QDebug>

#include "utilities.h"
#include "capture_thread.h"

CaptureThread::CaptureThread(int camera, QMutex *lock):
    running(false), cameraID(camera), videoPath(""), data_lock(lock)
{
    frame_width = frame_height = 0;
    taking_photo = false;

    loadOrnaments();
    masks_flag = 0;
}

CaptureThread::CaptureThread(QString videoPath, QMutex *lock):
    running(false), cameraID(-1), videoPath(videoPath), data_lock(lock)
{
    frame_width = frame_height = 0;
    taking_photo = false;

    loadOrnaments();
    masks_flag = 0;
}

CaptureThread::~CaptureThread() {
}

void CaptureThread::run() {
    running = true;
    cv::VideoCapture cap(cameraID);
    // cv::VideoCapture cap("/home/kdr2/Videos/WIN_20190123_20_14_56_Pro.mp4");
    cv::Mat tmp_frame;

    frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    //classifier_data必须是const cv::String
    const cv::String classifier_data = "haarcascades/haarcascade_frontalface_default.xml";
    classifier = new cv::CascadeClassifier(classifier_data);

    mark_detector = cv::face::createFacemarkLBF();
    QString model_data = QApplication::instance()->applicationDirPath() + "/data/lbfmodel.yaml";
    qDebug()<<"model_data"<<model_data;
    mark_detector->loadModel(model_data.toStdString());
    while(running) {
        cap >> tmp_frame;
        if (tmp_frame.empty()) {
            break;
        }

        if(masks_flag > 0)
            detectFaces(tmp_frame);

        if(taking_photo) {
            takePhoto(tmp_frame);

#ifdef USERDEBUG
{
            if(!tmp_frame.data)  //判断是否有数据
            {
                qDebug()<<"run()--->tmp_frame null!";
            }
            qDebug()<<"run()--->tmp_frame exists data!";
}
#endif
        }

        cvtColor(tmp_frame, tmp_frame, cv::COLOR_BGR2RGB);
        data_lock->lock();
        frame = tmp_frame;
        data_lock->unlock();
        emit frameCaptured(&frame);
    }
    cap.release();
    delete classifier;
    classifier = nullptr;
    running = false;
}

void CaptureThread::takePhoto(cv::Mat &frame)
{
    QString photo_name = Utilities::newPhotoName();
    QString photo_path = Utilities::getPhotoPath(photo_name, "jpg");
    bool writeResult = cv::imwrite(photo_path.toStdString(), frame);
//    bool writeResult = cv::imwrite("a:a.jpg", frame);//冒号是错误格式
#ifdef USERDEBUG
{
    if(writeResult)
    {
        qDebug()<<"takePhoto--->writeResult ===   cv::imwrite success!";
    }
    if(!frame.data)  //判断是否有数据
    {
        qDebug()<<"takePhoto--->frame null!";
    }
    qDebug()<<"takePhoto--->frame exists data!";
    qDebug()<<"takePhoto--->photo_path.toStdString()"<< photo_path;
}
#endif
    emit photoTaken(photo_name);
    taking_photo = false;
}

void CaptureThread::detectFaces(cv::Mat &frame)
{
    vector<cv::Rect> faces;
    cv::Mat gray_frame;
    cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);
    classifier->detectMultiScale(gray_frame, faces, 1.3, 5);

    cv::Scalar color = cv::Scalar(0, 0, 255); // red

    // draw the circumscribe rectangles
    if (isMaskOn(RECTANGLE)) {
        for(size_t i = 0; i < faces.size(); i++) {
            cv::rectangle(frame, faces[i], color, 1);
        }
    }

    vector< vector<cv::Point2f> > shapes;
    if (mark_detector->fit(frame, faces, shapes)) {
        // draw facial land marks
        for (unsigned long i=0; i<faces.size(); i++) {
            if (isMaskOn(LANDMARKS)) {
                for(unsigned long k=0; k<shapes[i].size(); k++) {
                    cv::circle(frame, shapes[i][k], 2, color, cv::FILLED);
                     QString index = QString("%1").arg(k);
                     cv::putText(frame, index.toStdString(), shapes[i][k], cv::FONT_HERSHEY_SIMPLEX, 0.4, color, 2);
                }
            }
            if (isMaskOn(GLASSES))
                drawGlasses(frame, shapes[i]);
            if (isMaskOn(MUSTACHE))
                drawMustache(frame, shapes[i]);
            if (isMaskOn(MOUSE_NOSE))
                drawMouseNose(frame, shapes[i]);
        }
    }
}

void CaptureThread::loadOrnaments()
{
    QImage image;
    image.load(":/images/glasses.jpg");
    image = image.convertToFormat(QImage::Format_RGB888);
    glasses = cv::Mat(
        image.height(), image.width(), CV_8UC3,
        image.bits(), image.bytesPerLine()).clone();

    image.load(":/images/mustache.jpg");
    image = image.convertToFormat(QImage::Format_RGB888);
    mustache = cv::Mat(
        image.height(), image.width(), CV_8UC3,
        image.bits(), image.bytesPerLine()).clone();

    image.load(":/images/mouse-nose.jpg");
    image = image.convertToFormat(QImage::Format_RGB888);
    mouse_nose = cv::Mat(
        image.height(), image.width(), CV_8UC3,
        image.bits(), image.bytesPerLine()).clone();
}

void CaptureThread::drawGlasses(cv::Mat &frame, vector<cv::Point2f> &marks)
{
    // resize
    cv::Mat ornament;
    double distance = cv::norm(marks[45] - marks[36]) * 1.5;
    cv::resize(glasses, ornament, cv::Size(0, 0), distance / glasses.cols, distance / glasses.cols, cv::INTER_NEAREST);

    // rotate
    double angle = -atan((marks[45].y - marks[36].y) / (marks[45].x - marks[36].x));
    cv::Point2f center = cv::Point(ornament.cols/2, ornament.rows/2);
    cv::Mat rotateMatrix = cv::getRotationMatrix2D(center, angle * 180 / 3.14, 1.0);

    cv::Mat rotated;
    cv::warpAffine(
        ornament, rotated, rotateMatrix, ornament.size(),
        cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));

    // paint
    center = cv::Point((marks[45].x + marks[36].x) / 2, (marks[45].y + marks[36].y) / 2);
//    cv::Rect rec(center.x - rotated.cols / 2, center.y - rotated.rows / 2, rotated.cols, rotated.rows);
    cv::Rect rec((center.x - rotated.cols / 2)>0?(center.x - rotated.cols / 2):0,
                 (center.y - rotated.rows / 2)>0?(center.y - rotated.rows / 2):0,
                 rotated.cols>0?rotated.cols:0,
                 rotated.rows>0?rotated.rows:0);
    frame(rec) &= rotated;
}

void CaptureThread::drawMustache(cv::Mat &frame, vector<cv::Point2f> &marks)
{
    // resize
    cv::Mat ornament;
    double distance = cv::norm(marks[54] - marks[48]) * 1.5;
    cv::resize(mustache, ornament, cv::Size(0, 0), distance / mustache.cols, distance / mustache.cols, cv::INTER_NEAREST);

    // rotate
    double angle = -atan((marks[54].y - marks[48].y) / (marks[54].x - marks[48].x));
    cv::Point2f center = cv::Point(ornament.cols/2, ornament.rows/2);
    cv::Mat rotateMatrix = cv::getRotationMatrix2D(center, angle * 180 / 3.14, 1.0);

    cv::Mat rotated;
    cv::warpAffine(
        ornament, rotated, rotateMatrix, ornament.size(),
        cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));

    // paint
    center = cv::Point((marks[33].x + marks[51].x) / 2, (marks[33].y + marks[51].y) / 2);
//    cv::Rect rec(center.x - rotated.cols / 2, center.y - rotated.rows / 2, rotated.cols, rotated.rows);
    cv::Rect rec((center.x - rotated.cols / 2)>0?(center.x - rotated.cols / 2):0,
                 (center.y - rotated.rows / 2)>0?(center.y - rotated.rows / 2):0,
                 rotated.cols>0?rotated.cols:0,
                 rotated.rows>0?rotated.rows:0);
    frame(rec) &= rotated;
}

void CaptureThread::drawMouseNose(cv::Mat &frame, vector<cv::Point2f> &marks)
{
    // resize
    cv::Mat ornament;
    double distance = cv::norm(marks[13] - marks[3]);
    cv::resize(mouse_nose, ornament, cv::Size(0, 0), distance / mouse_nose.cols, distance / mouse_nose.cols, cv::INTER_NEAREST);

    // rotate
    double angle = -atan((marks[16].y - marks[0].y) / (marks[16].x - marks[0].x));
    cv::Point2f center = cv::Point(ornament.cols/2, ornament.rows/2);
    cv::Mat rotateMatrix = cv::getRotationMatrix2D(center, angle * 180 / 3.14, 1.0);

    cv::Mat rotated;
    cv::warpAffine(
        ornament, rotated, rotateMatrix, ornament.size(),
        cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));

    // paint
    center = marks[30];

//    cv::Rect rec(center.x - rotated.cols, center.y - rotated.rows / 2, rotated.cols, rotated.rows);
    cv::Rect rec((center.x - rotated.cols / 2)>0?(center.x - rotated.cols / 2):0,
                 (center.y - rotated.rows / 2)>0?(center.y - rotated.rows / 2):0,
                 rotated.cols>0?rotated.cols:0,
                 rotated.rows>0?rotated.rows:0);
    frame(rec) &= rotated;
}
