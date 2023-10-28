
INCLUDEPATH += $$PWD \
               $$PWD/include \
               $$PWD/include/opencv2
x64\vc16\lib
# 加载静态库:根据情况，加载对应的库
win32:CONFIG(release, debug|release): {
LIBS += $$PWD/x64/vc16/lib/*.lib
}
else:win32:CONFIG(debug, debug|release): {
LIBS += $$PWD/x64/vc16/lib/*.lib
}
