#-------------------------------------------------
#
# Project created by QtCreator 2017-04-13T17:20:28
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_pan
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
#    DataTypes/CT_Action.cpp \
#    DataTypes/CT_Clip.cpp \
#    DataTypes/Color/CT_AxialShd.cpp \
#    DataTypes/Color/CT_Color.cpp \
#    DataTypes/Color/CT_ColorSpace.cpp \
#    DataTypes/Color/CT_Pattern.cpp \
#    DataTypes/Color/CT_RadialShd.cpp \
#    DataTypes/image/CT_Clip.cpp \
#    DataTypes/image/CT_DrawParam.cpp \
#    DataTypes/image/CT_GraphicUnit.cpp \
#    DataTypes/image/CT_Image.cpp \
#    DataTypes/image/CT_Path.cpp \
    ofd_parser.cpp \
    DataTypes/basic_datatypes.cpp

HEADERS  += \
    mainwindow.h \
    DataTypes/basic_datatype.h \
#    DataTypes/CT_Action.h \
#    DataTypes/Color/CT_AxialShd.h \
    DataTypes/Color/CT_Color.h \
#    DataTypes/Color/CT_ColorSpace.h \
#    DataTypes/Color/CT_Pattern.h \
#    DataTypes/Color/CT_RadialShd.h \
    DataTypes/document/CT_CommonData.h \
    DataTypes/document/CT_DocInfo.h \
#    DataTypes/document/CT_OutlineElem.h \
#    DataTypes/document/CT_Outlines.h \
    DataTypes/document/DocBody.h \
    DataTypes/document/Document.h \
    DataTypes/document/OFD.h \
#    DataTypes/image/CT_Clip.h \
#    DataTypes/image/CT_DrawParam.h \
    DataTypes/image/CT_GraphicUnit.h \
    DataTypes/image/CT_Image.h \
    DataTypes/image/CT_Path.h \
    DataTypes/page/CT_Layer.h \
    DataTypes/page/CT_PageArea.h \
    DataTypes/page/CT_PageBlock.h \
    DataTypes/page/CT_Pages.h \
    DataTypes/page/Page.h \
    ofd_parser.h \
    DataTypes/document/res.h\
#    DataTypes/text/CT_CGTransform.h \
#    DataTypes/text/CT_Font.h \
    DataTypes/text/CT_Text.h \
    DataTypes/text/TextCode.h \
    ofd_global.h

DESTDIR = ../bin     # 生成文件在这
MOC_DIR = ./moc     # Q_OBJECT 类转换后的文件
RCC_DIR = ./rcc     # .qrc 文件转换后存放路径
OBJECTS_DIR += ./tmp   # .obj 文件存放路径

# 引用，可以直接使用 include<文件名> 方式引入头文件
INCLUDEPATH += ../model \
               ../ofd

FORMS    += mainwindow.ui \
    mainwindow.ui

unix{
    LIBS += ../bin/libmodel.so\
            ../bin/libofd.so

}

win32{
    #LIBS += ../bin/model.lib \
    #        ../bin/ofd.lib
}
