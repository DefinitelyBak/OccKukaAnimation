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
    myanimation.cpp \
    occQt.cpp       \
    occView.cpp

CONFIG += c++11

HEADERS  += \
    myanimation.h \
    occQt.h \
    occView.h

FORMS    += \
    occQt.ui

RESOURCES += \
    .qrc

CASROOT = "C:\\Users\\lea-k\\Desktop\\Visual code\\OpenCASCADE-7.7.0-vc14-64\\opencascade-7.7.0"
#CASROOT = "D:\\Programs\\OpenCASCADE-7.7.0-vc14-64\\opencascade-7.7.0"

INCLUDEPATH += $${CASROOT}/inc
LIBS += -L$${CASROOT}/win64/vc14/lib/

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
    -lTKFillet  \
    -lTKernel -lTKMath -lTKService -lTKV3d -lTKOpenGl \
            -lTKBRep -lTKIGES -lTKSTL -lTKVRML -lTKSTEP -lTKSTEPAttr -lTKSTEP209 \
            -lTKSTEPBase -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
            -lTKXSBase -lTKShHealing -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
            -lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset \
            -lTKXDESTEP \

