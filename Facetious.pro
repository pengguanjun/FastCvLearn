TEMPLATE = app
TARGET = Facetious
INCLUDEPATH += .
INCLUDEPATH += ./app

QT += core gui multimedia
//pro文件引入模块
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

# Input
HEADERS += mainwindow.h capture_thread.h utilities.h \
    app/flatui.h \
    app/quiconfig.h \
    app/quistyle.h
SOURCES += main.cpp mainwindow.cpp capture_thread.cpp utilities.cpp \
    app/flatui.cpp \
    app/quiconfig.cpp \
    app/quistyle.cpp

RESOURCES = images.qrc \
    qss.qrc

# 设置可执行文件输出路径
win32:CONFIG(release, debug|release): {
# exe文件生成路径，默认采用工程下路径，这样可无需重复拷贝给力dll库文件和配置文件等
DESTDIR = ../FastCvLearn/install/bin/release
}
else:win32:CONFIG(debug, debug|release): {
# exe文件生成路径，默认采用工程下路径，这样可无需重复拷贝给力dll库文件和配置文件等
DESTDIR = ../FastCvLearn/install/bin/debug
}

exeInstallOut.path = $$DESTDIR
INSTALLS += exeInstallOut

include(3rd/3rd.pri)

# opencv config
#INCLUDEPATH += E:/qt6Work/OPENCV/install/include/ \
#               E:/qt6Work/OPENCV/install/include/opencv2/

#DEPENDPATH += E:/qt6Work/OPENCV/install/include/ \
#              E:/qt6Work/OPENCV/install/include/opencv2/


#win32:CONFIG(release, debug|release): LIBS += -LE:/qt6Work/OPENCV/install/x64/vc16/lib/ \
#opencv_aruco460.lib  \
#opencv_bgsegm460.lib  \
#opencv_bioinspired460.lib  \
#opencv_ccalib460.lib  \
#opencv_core460.lib  \
##opencv_cudaarithm460.lib  \
##opencv_cudabgsegm460.lib  \
##opencv_cudacodec460.lib  \
##opencv_cudafilters460.lib  \
##opencv_cudaimgproc460.lib  \
##opencv_cudalegacy460.lib  \
##opencv_cudaobjdetect460.lib  \
##opencv_cudaoptflow460.lib  \
##opencv_cudastereo460.lib  \
##opencv_cudawarping460.lib  \
##opencv_cudev460.lib  \
##opencv_cvv460.lib  \
#opencv_datasets460.lib  \
#opencv_dnn460.lib  \
#opencv_dpm460.lib  \
#opencv_face460.lib  \
#opencv_flann460.lib  \
#opencv_fuzzy460.lib  \
#opencv_gapi460.lib  \
#opencv_hfs460.lib  \
#opencv_highgui460.lib  \
#opencv_imgcodecs460.lib  \
#opencv_imgproc460.lib  \
#opencv_mcc460.lib  \
#opencv_ml460.lib  \
#opencv_objdetect460.lib  \
#opencv_optflow460.lib  \
#opencv_photo460.lib  \
#opencv_plot460.lib  \
#opencv_quality460.lib  \
#opencv_rapid460.lib  \
#opencv_reg460.lib  \
#opencv_rgbd460.lib  \
#opencv_saliency460.lib  \
#opencv_shape460.lib  \
#opencv_stereo460.lib  \
#opencv_stitching460.lib  \
#opencv_superres460.lib  \
#opencv_text460.lib  \
#opencv_tracking460.lib  \
#opencv_video460.lib  \
#opencv_videoio460.lib  \
#opencv_videostab460.lib  \
#opencv_ximgproc460.lib  \
#opencv_xobjdetect460.lib  \
#opencv_xphoto460.lib
#else:win32:CONFIG(debug, debug|release): LIBS += -LE:/qt6Work/OPENCV/install/x64/vc16/lib/  \
#opencv_aruco460d.lib  \
#opencv_bgsegm460d.lib  \
#opencv_bioinspired460d.lib  \
#opencv_ccalib460d.lib  \
#opencv_core460d.lib  \
##opencv_cudaarithm460d.lib  \
##opencv_cudabgsegm460d.lib  \
##opencv_cudacodec460d.lib  \
##opencv_cudafilters460d.lib  \
##opencv_cudaimgproc460d.lib  \
##opencv_cudalegacy460d.lib  \
##opencv_cudaobjdetect460d.lib  \
##opencv_cudaoptflow460d.lib  \
##opencv_cudastereo460d.lib  \
##opencv_cudawarping460d.lib  \
##opencv_cudev460d.lib  \
##opencv_cvv460d.lib  \
#opencv_datasets460d.lib  \
#opencv_dnn460d.lib  \
#opencv_dpm460d.lib  \
#opencv_face460d.lib  \
#opencv_flann460d.lib  \
#opencv_fuzzy460d.lib  \
#opencv_gapi460d.lib  \
#opencv_hfs460d.lib  \
#opencv_highgui460d.lib  \
#opencv_imgcodecs460d.lib  \
#opencv_imgproc460d.lib  \
#opencv_mcc460d.lib  \
#opencv_ml460d.lib  \
#opencv_objdetect460d.lib  \
#opencv_optflow460d.lib  \
#opencv_photo460d.lib  \
#opencv_plot460d.lib  \
#opencv_quality460d.lib  \
#opencv_rapid460d.lib  \
#opencv_reg460d.lib  \
#opencv_rgbd460d.lib  \
#opencv_saliency460d.lib  \
#opencv_shape460d.lib  \
#opencv_stereo460d.lib  \
#opencv_stitching460d.lib  \
#opencv_superres460d.lib  \
#opencv_text460d.lib  \
#opencv_tracking460d.lib  \
#opencv_video460d.lib  \
#opencv_videoio460d.lib  \
#opencv_videostab460d.lib  \
#opencv_ximgproc460d.lib  \
#opencv_xobjdetect460d.lib  \
#opencv_xphoto460d.lib
#else:unix:!macx: LIBS += -LE:/qt6Work/OPENCV/install/x64/vc16/lib/ -lopencv_core460 \
#-lopencv_*460

