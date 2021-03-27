// mode: c++
#ifndef CAPTURE_THREAD_H
#define CAPTURE_THREAD_H

#include <QString>
#include <QThread>
#include <QMutex>

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/face/facemark.hpp"

using namespace std;

class CaptureThread : public QThread
{
    Q_OBJECT
public:
    explicit CaptureThread(int camera, QMutex *lock);
    explicit CaptureThread(QString videoPath, QMutex *lock);
    ~CaptureThread();
    void setRunning(bool run) {running = run; };
    void takePhoto() {taking_photo = true; }

    enum MASK_TYPE{
                   RECTANGLE = 0,
                   LANDMARKS,
                   GLASSES,
                   MUSTACHE,
                   MOUSE_NOSE,
                   MASK_COUNT,
    };

    void updateMasksFlag(MASK_TYPE type, bool on_or_off) {
        uint8_t bit = 1 << type;
        if(on_or_off) {
            masks_flag |= bit;
        } else {
            masks_flag &= ~bit;
        }
    };

protected:
    void run() override;

signals:
    void frameCaptured(cv::Mat *data);
    void photoTaken(QString name);

private:
    void takePhoto(cv::Mat &frame);
    void detectFaces(cv::Mat &frame);
    void loadOrnaments();
    void drawGlasses(cv::Mat &frame, vector<cv::Point2f> &marks);
    void drawMustache(cv::Mat &frame, vector<cv::Point2f> &marks);
    void drawMouseNose(cv::Mat &frame, vector<cv::Point2f> &marks);
    bool isMaskOn(MASK_TYPE type) {return (masks_flag & (1 << type)) != 0; };
private:
    bool running;
    int cameraID;
    QString videoPath;
    QMutex *data_lock;
    cv::Mat frame;

    int frame_width, frame_height;

    // take photos
    bool taking_photo;

    // face detection
    cv::CascadeClassifier *classifier;
    cv::Ptr<cv::face::Facemark> mark_detector;

    // mask ornaments
    cv::Mat glasses;
    cv::Mat mustache;
    cv::Mat mouse_nose;

    uint8_t masks_flag;
};

#endif // CAPTURE_THREAD_H
