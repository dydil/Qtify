HEADERS += \
    $$files($$PWD/models/*.h)  \
    $$files($$PWD/private/*.h) \
    $$PWD/RequestHandler.h

SOURCES += \
    $$files($$PWD/models/*.cpp)  \
    $$files($$PWD/private/*.cpp) \
    $$PWD/RequestHandler.cpp

INCLUDEPATH += $$PWD

CONFIG += c++14

QT += networkauth
