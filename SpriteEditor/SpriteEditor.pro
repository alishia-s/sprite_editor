QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canvas.cpp \
    frame.cpp \
    framesview.cpp \
    insertframes.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    palette.cpp \
    preview.cpp \
    swapframes.cpp \
    tools.cpp

HEADERS += \
    canvas.h \
    frame.h \
    framesview.h \
    insertframes.h \
    mainwindow.h \
    model.h \
    palette.h \
    preview.h \
    swapframes.h \
    tools.h

FORMS += \
    canvas.ui \
    framesview.ui \
    insertframes.ui \
    mainwindow.ui \
    palette.ui \
    preview.ui \
    swapframes.ui \
    tools.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    madelynscooltxtfile

RESOURCES += \
    resources.qrc \
    resources.qrc
