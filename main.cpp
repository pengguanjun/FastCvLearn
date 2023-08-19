
#pragma execution_character_set("utf-8")

#include <QApplication>
#include "mainwindow.h"
#include <QTextCodec>
#include <QDebug>
#include "MainWindow.h"
#include "app/quistyle.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //载入样式表
    QUIStyle::setStyle(QUIStyle::Style_LightBlue);

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#if _MSC_VER
    QTextCodec* codec = QTextCodec::codecForName("gbk");
#else
    QTextCodec* codec = QTextCodec::codecForName("utf-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec* codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
#endif


    MainWindow window;
    window.setWindowTitle("Facetious");
    window.show();
    return app.exec();
}
