# FastCvLearn
Qt-based opencv real-time image processing framework 
@[TOC](基于qt的opencv实时图像处理框架FastCvLearn实战)
# 结果展示

![基于qt的opencv实时处理框架FastCvLearn](https://img-blog.csdnimg.cn/20210327230428690.gif#pic_center)

# 基础知识回顾
首先回顾一下基础知识。


uint8_t
![uint8_t](https://img-blog.csdnimg.cn/20210327140826749.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70)unsigned char
![unsigned char](https://img-blog.csdnimg.cn/20210327140849214.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70)

BIN:0001 0000
![BIN:0001 0000](https://img-blog.csdnimg.cn/2021032713595679.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70)
NOT BIN:0001 0000
![NOT BIN:0001 0000](https://img-blog.csdnimg.cn/20210327135913473.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70)
static_cast
![static_cast](https://img-blog.csdnimg.cn/20210327141330410.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70)    
# 手撕代码
## capture_thread

对于enum 类型，感觉很陌生，如何剖析？
```cpp 
    enum MASK_TYPE{
                   RECTANGLE = 0,
                   LANDMARKS,
                   GLASSES,
                   MUSTACHE,
                   MOUSE_NOSE,
                   MASK_COUNT,
    };
```

使用qDebug()函数即可。
```cpp
    qDebug()<<"CaptureThread::MASK_COUNT"<<CaptureThread::MASK_COUNT;
    qDebug()<<"CaptureThread::MOUSE_NOSE"<<CaptureThread::MOUSE_NOSE;
    qDebug()<<"CaptureThread::MUSTACHE"<<CaptureThread::MUSTACHE;
    qDebug()<<"CaptureThread::GLASSES"<<CaptureThread::GLASSES;
    qDebug()<<"CaptureThread::LANDMARKS"<<CaptureThread::LANDMARKS;
    qDebug()<<"CaptureThread::RECTANGLE"<<CaptureThread::RECTANGLE;
```

可以发现，原来其枚举值是整数。
```cpp
CaptureThread::MASK_COUNT 5
CaptureThread::MOUSE_NOSE 4
CaptureThread::MUSTACHE 3
CaptureThread::GLASSES 2
CaptureThread::LANDMARKS 1
CaptureThread::RECTANGLE 0

```

那么，再结合先前我们百科得到的static_cast类型转换函数的定义，就不难理解下面这句代码了。对int 类型i，进行类型转换为枚举类型CaptureThread::MASK_TYPE。

```cpp
static_cast<CaptureThread::MASK_TYPE>(i)
```


综合上述知识，下面这个函数可以理解为将uint8_t masks_flag与uint8_t bit进行位运算。其可作为多个算法的开关函数。
```cpp
uint8_t masks_flag;
masks_flag = 0;
    void updateMasksFlag(MASK_TYPE type, bool on_or_off) {
        uint8_t bit = 1 << type;
        if(on_or_off) {
            masks_flag |= bit;
        } else {
            masks_flag &= ~bit;
        }
    };
```

isMaskOn的位运算同理。
```cpp
    bool isMaskOn(MASK_TYPE type) {return (masks_flag & (1 << type)) != 0; };
```

updateMasksFlag使用和isMaskOn使用，实现的效果就算给实时的视频帧添加多个特效独立的开关。

![实时的视频帧添加多个特效独立的开关](https://img-blog.csdnimg.cn/20210327142704447.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70)

```cpp 
//updateMasksFlag使用
    QCheckBox *box = qobject_cast<QCheckBox*>(sender());
    for (int i = 0; i < CaptureThread::MASK_COUNT; i++){
        if (mask_checkboxes[i] == box) {
            capturer->updateMasksFlag(static_cast<CaptureThread::MASK_TYPE>(i), status != 0);
        }
    }
    
//isMaskOn使用
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
```


下面来看一下，特效装饰素材。
![MASK_TYPE](https://img-blog.csdnimg.cn/20210327133702554.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70#pic_center)
运用素材的方法：

在CaptureThread::run()线程里使用while函数，不停的调用分类器。注意帧数据的上锁和解锁，保证数据同步。

```cpp

void CaptureThread::run() {
    running = true;
    cv::VideoCapture cap(cameraID);
    // cv::VideoCapture cap("/home/kdr2/Videos/WIN_20190123_20_14_56_Pro.mp4");
    cv::Mat tmp_frame;

    frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

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

```

这里有个坑，cv::CascadeClassifier函数的输入需要是const cv::String格式的（即std::string），QString不行。    

```cpp
    const cv::String classifier_data = "haarcascades/haarcascade_frontalface_default.xml";
    classifier = new cv::CascadeClassifier(classifier_data);
```

保存图像bug，
本想借鉴如下toStdString()方式，发现运行不成功，toStdString(时间带冒号格式)方式的字符串，cv::imwrite调用后，写入图像无结果。
```cpp
    QString photo_path = "";
    cv::imwrite(photo_path.toStdString(), frame);
```
后来发现是字符串格式问题，测试发现这句bool writeResult = cv::imwrite("a:a.jpg", frame);不行。
走过一些弯路后，发现时间日期格式修改后即可，将"yyyy-MM-dd+HH:mm:ss"修改为"yyyy-MM-dd+HH-mm-ss"。其中不能有冒号:。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210327225145495.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70)

接下来了解下，人脸识别的奥秘。
```cpp
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
```


classifier->detectMultiScale(gray_frame, faces, 1.3, 5);用来创建多尺度的分类器，检测到的对象作为矩形列表返回。

```cpp
detectMultiScale参数的理解可以参考:https://www.it610.com/article/1295047563740782592.htm
```
![detectMultiScale()](https://img-blog.csdnimg.cn/20210327145920782.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70)

cv::face::createFacemarkLBF()用来创建局部二值特征（LBF）。

```cpp
局部二值特征（LBF）具体可参考科普文：https://blog.csdn.net/qq_14845119/article/details/53575091
```
opencv官方文档介绍不多。
![cv::face::createFacemarkLBF()](https://img-blog.csdnimg.cn/20210327144427791.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70)
mark_detector->fit(frame, faces, shapes)用来画特征点。
![mark_detector->fit(frame, faces, shapes)](https://img-blog.csdnimg.cn/20210327144726304.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70)素材加载的方法：
这里QImage 与cv::Mat的相互转化值得借鉴。
```cpp

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
```

drawGlasses(frame, shapes[i]);
该函数对角度敏感，角度变化大时，失效了。选取的特征点是根据landmarks排序后位置来手工定义的。比如：resize和rotate中的marks[]中的数字。
```cpp
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
    cv::Rect rec(center.x - rotated.cols / 2, center.y - rotated.rows / 2, rotated.cols, rotated.rows);
    frame(rec) &= rotated;
}
```

cv::norm求解范数
一种非严密的解释就是，对应向量范数，向量空间中的向量都是有大小的，这个大小如何度量，就是用范数来度量的，不同的范数都可以来度量这个大小，就好比米和尺都可以来度量远近一样；对于矩阵范数，学过线性代数，我们知道，通过运算AX=B，可以将向量X变化为B，矩阵范数就是来度量这个变化大小的。
```cpp
OpenCV：norm-范数求解函数：https://jingyan.baidu.com/article/454316ab3d46d4f7a7c03a89.html
```
cv::resize
要缩小图像，通常使用INTER_AREA插值效果最佳，而要放大图像，通常使用c :: INTER_CUBIC（速度慢）或INTER_LINEAR（速度更快，但看起来仍然可以）最好。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210327152956100.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70)
double angle = -atan((marks[45].y - marks[36].y) / (marks[45].x - marks[36].x));
反正切求角度

 frame(rec) &= rotated;
 局部ROI区域添加蒙版图。




凑近看landmarks的序号的时候，发现程序闪退，发现是这面这个问题：
```cpp
OpenCV: terminate handler is called! The last OpenCV error is:
OpenCV(4.5.1) Error: Assertion failed (0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols && 0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows) in cv::Mat::Mat, file D:\work\OpenCV451Sln\opencv-4.5.1\modules\core\src\matrix.cpp, line 811
```

查看了下面的代码参考后，认定是rec的问题：

```cpp
https://github.com/Huang9495/roi-
```
解决方法：

```cpp
//    cv::Rect rec(center.x - rotated.cols / 2, center.y - rotated.rows / 2, rotated.cols, rotated.rows);
    cv::Rect rec((center.x - rotated.cols / 2)>0?(center.x - rotated.cols / 2):0,
                 (center.y - rotated.rows / 2)>0?(center.y - rotated.rows / 2):0,
                 rotated.cols>0?rotated.cols:0,
                 rotated.rows>0?rotated.rows:0);
    frame(rec) &= rotated;
```

该方式解决后，胡子和花猫的素材，凑近后仍有程序自动退出问题。解决方向是对的，后续有需要再优化吧。


![基于qt的opencv实时处理框架FastCvLearn](https://img-blog.csdnimg.cn/20210327225322458.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lhbnR1Z3VpZ3V6aVBHSg==,size_16,color_FFFFFF,t_70)参考链接：
1 : [Qt-5-and-OpenCV-4-Computer-Vision-Projects.](https://github.com/PacktPublishing/Qt-5-and-OpenCV-4-Computer-Vision-Projects)
2 : [FastCvLearn代码仓库，欢迎stars](https://github.com/pengguanjun/FastCvLearn)

