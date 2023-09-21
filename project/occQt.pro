QT       +=widgets core gui opengl openglwidgets #Подключение модулей Qt используемых в приложении

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = occQt #Название цели (приложения)
TEMPLATE = app #Тип приложения (библиотека, приложение или плагин)

SOURCES += main.cpp \ # *.cpp файлы используемые в проекте
    myanimation.cpp \
    occQt.cpp       \
    occView.cpp

CONFIG += c++11 # дополнительные параметры (стандарт языка)

HEADERS  += \ # используемые в проекте *.h файлы
    myanimation.h \
    occQt.h \
    occView.h

FORMS    += \ # используемые формы
    occQt.ui

RESOURCES += \ # папка, содержащая ресурсы проекта(иконки)
    .qrc

# Переменная содиржащая путь до библиотеки OpenCascade
#CASROOT = "C:\\Users\\lea-k\\Desktop\\Visual code\\OpenCASCADE-7.7.0-vc14-64\\opencascade-7.7.0"
CASROOT = "D:\\Programs\\OpenCASCADE-7.7.0-vc14-64\\opencascade-7.7.0"

INCLUDEPATH += $${CASROOT}/inc #Подключение *.h файлов библиотеки Open Cascade
LIBS += -L$${CASROOT}/win64/vc14/lib/ #Подключение пути до статических библиотек *.lib Open Cascade

#Подключение библиотек Open Cascade перечисленных ниже
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

