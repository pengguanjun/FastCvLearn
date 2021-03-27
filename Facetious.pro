TEMPLATE = app
TARGET = Facetious
INCLUDEPATH += .

QT += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Input
HEADERS += mainwindow.h capture_thread.h utilities.h
SOURCES += main.cpp mainwindow.cpp capture_thread.cpp utilities.cpp

# DEFINES += OPENCV_DATA_DIR=\\\"/usr/share/opencv/\\\"
#DEFINES += OPENCV_DATA_DIR=\\\"/home/kdr2/programs/opencv/share/opencv4/\\\"


RESOURCES = images.qrc


# opencv config
INCLUDEPATH += C:/opencv451/include
DEPENDPATH += C:/opencv451/include

win32:CONFIG(release, debug|release): LIBS += -LC:/opencv451/vc15/lib/ \
opencv_aruco451.lib  \
opencv_bgsegm451.lib  \
opencv_bioinspired451.lib  \
opencv_ccalib451.lib  \
opencv_core451.lib  \
opencv_cudaarithm451.lib  \
opencv_cudabgsegm451.lib  \
opencv_cudacodec451.lib  \
opencv_cudafilters451.lib  \
opencv_cudaimgproc451.lib  \
opencv_cudalegacy451.lib  \
opencv_cudaobjdetect451.lib  \
opencv_cudaoptflow451.lib  \
opencv_cudastereo451.lib  \
opencv_cudawarping451.lib  \
opencv_cudev451.lib  \
opencv_cvv451.lib  \
opencv_datasets451.lib  \
opencv_dnn451.lib  \
opencv_dpm451.lib  \
opencv_face451.lib  \
opencv_flann451.lib  \
opencv_fuzzy451.lib  \
opencv_gapi451.lib  \
opencv_hfs451.lib  \
opencv_highgui451.lib  \
opencv_imgcodecs451.lib  \
opencv_imgproc451.lib  \
opencv_mcc451.lib  \
opencv_ml451.lib  \
opencv_objdetect451.lib  \
opencv_optflow451.lib  \
opencv_photo451.lib  \
opencv_plot451.lib  \
opencv_quality451.lib  \
opencv_rapid451.lib  \
opencv_reg451.lib  \
opencv_rgbd451.lib  \
opencv_saliency451.lib  \
opencv_shape451.lib  \
opencv_stereo451.lib  \
opencv_stitching451.lib  \
opencv_superres451.lib  \
opencv_text451.lib  \
opencv_tracking451.lib  \
opencv_video451.lib  \
opencv_videoio451.lib  \
opencv_videostab451.lib  \
opencv_ximgproc451.lib  \
opencv_xobjdetect451.lib  \
opencv_xphoto451.lib
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/opencv451/vc15/lib/  \
opencv_aruco451d.lib  \
opencv_bgsegm451d.lib  \
opencv_bioinspired451d.lib  \
opencv_ccalib451d.lib  \
opencv_core451d.lib  \
opencv_cudaarithm451d.lib  \
opencv_cudabgsegm451d.lib  \
opencv_cudacodec451d.lib  \
opencv_cudafilters451d.lib  \
opencv_cudaimgproc451d.lib  \
opencv_cudalegacy451d.lib  \
opencv_cudaobjdetect451d.lib  \
opencv_cudaoptflow451d.lib  \
opencv_cudastereo451d.lib  \
opencv_cudawarping451d.lib  \
opencv_cudev451d.lib  \
opencv_cvv451d.lib  \
opencv_datasets451d.lib  \
opencv_dnn451d.lib  \
opencv_dpm451d.lib  \
opencv_face451d.lib  \
opencv_flann451d.lib  \
opencv_fuzzy451d.lib  \
opencv_gapi451d.lib  \
opencv_hfs451d.lib  \
opencv_highgui451d.lib  \
opencv_imgcodecs451d.lib  \
opencv_imgproc451d.lib  \
opencv_mcc451d.lib  \
opencv_ml451d.lib  \
opencv_objdetect451d.lib  \
opencv_optflow451d.lib  \
opencv_photo451d.lib  \
opencv_plot451d.lib  \
opencv_quality451d.lib  \
opencv_rapid451d.lib  \
opencv_reg451d.lib  \
opencv_rgbd451d.lib  \
opencv_saliency451d.lib  \
opencv_shape451d.lib  \
opencv_stereo451d.lib  \
opencv_stitching451d.lib  \
opencv_superres451d.lib  \
opencv_text451d.lib  \
opencv_tracking451d.lib  \
opencv_video451d.lib  \
opencv_videoio451d.lib  \
opencv_videostab451d.lib  \
opencv_ximgproc451d.lib  \
opencv_xobjdetect451d.lib  \
opencv_xphoto451d.lib
else:unix:!macx: LIBS += -LC:/opencv451/vc15/lib/ -lopencv_core451 \
-lopencv_*451

