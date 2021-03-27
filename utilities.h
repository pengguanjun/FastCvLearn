#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <QString>
using namespace std;

#define USE_DEBUG

#if defined(Q_OS_WIN32) && defined(USE_DEBUG)
#define USERDEBUG
#endif
#include "opencv2/opencv.hpp"

#include "capture_thread.h"
class Utilities
{
 public:
    static QString getDataPath();
    static QString newPhotoName();
    static QString getPhotoPath(QString name, QString postfix);
};

#endif
