#-------------------------------------------------
#
# Project created by QtCreator 2013-08-18T15:52:09
#
#-------------------------------------------------

QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = occQt
TEMPLATE = app

SOURCES += main.cpp \
    occQt.cpp       \
    occView.cpp

CONFIG += c++11

HEADERS  += \
    occQt.h \
    occView.h

FORMS    += \
    occQt.ui

RESOURCES += \
    occqt.qrc

CASROOT = C:/Users/lea-k/Desktop/Visual code/OpenCASCADE-7.7.0-vc14-64/opencascade-7.7.0

INCLUDEPATH += "C:/Users/lea-k/Desktop/Visual code/OpenCASCADE-7.7.0-vc14-64/opencascade-7.7.0/inc"
LIBS += -L"C:/Users/lea-k/Desktop/Visual code/OpenCASCADE-7.7.0-vc14-64/opencascade-7.7.0/win64/vc14/lib/"

win32 {
    DEFINES +=  \
        WNT
    INCLUDEPATH +=  \
        $$(CASROOT)/inc

    win32-msvc2010 {
        compiler=vc10
    }

    win32-msvc2012 {
        compiler=vc11
    }

    win32-msvc2013 {
        compiler=vc12
    }

    win32-msvc2015 {
        compiler=vc14
    }

    # Determine 32 / 64 bit and debug / release build
    !contains(QMAKE_TARGET.arch, x86_64) {
        CONFIG(debug, debug|release) {
            message("Debug 32 build")
            LIBS += -L$$(CASROOT)/win32/$$compiler/libd
        }
        else {
            message("Release 32 build")
            LIBS += -L$$(CASROOT)/win32/$$compiler/lib
        }
    }
    else {
        CONFIG(debug, debug|release) {
            message("Debug 64 build")
            LIBS += -L$$(CASROOT)/win64/$$compiler/libd
        }
        else {
            message("Release 64 build")
            LIBS += -L$$(CASROOT)/win64/$$compiler/lib
        }
    }
}

linux-g++ {
    INCLUDEPATH +=  \
        $$(CASROOT)/include/opencascade

    LIBS +=         \
        -L$$(CASROOT)/lib
}

LIBS +=         \
    -lTKernel   \
    -lTKMath    \
    -lTKG3d     \
    -lTKBRep    \
    -lTKGeomBase\
    -lTKGeomAlgo\
    -lTKTopAlgo \
    -lTKPrim    \
    -lTKBO      \
    -lTKBool    \
    -lTKOffset  \
    -lTKService \
    -lTKV3d     \
    -lTKOpenGl  \
    -lTKFillet
