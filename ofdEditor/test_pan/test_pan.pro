#-------------------------------------------------
#
# Project created by QtCreator 2017-04-13T17:20:28
#
#-------------------------------------------------

QT       += core gui

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
    mainwindow.cpp

HEADERS  += \
    basic_datatype.h \
    mainwindow.h \
    text_object_classes.h \
    document_object_classes.h \
    document/ofd.h \
    document/ct_docinfo.h \
    document/docbody.h \
    document/ct_commondata.h \
    document/document.h \
    document/ct_outlineelem.h \
    document/ct_outlines.h \
    page/ct_pageblock.h \
    page/ct_layer.h \
    page/page.h \
    page/pages.h \
    text/ct_font.h \
    text/ct_cgtransform.h \
    text/textcode.h \
    text/ct_text.h

DESTDIR = ../bin     # 生成文件在这
MOC_DIR = ./moc     # Q_OBJECT 类转换后的文件
RCC_DIR = ./rcc     # .qrc 文件转换后存放路径
OBJECTS_DIR += ./tmp   # .obj 文件存放路径

# 引用，可以直接使用 include<文件名> 方式引入头文件
INCLUDEPATH += ../model \
               ../ofd

FORMS    += mainwindow.ui

unix{
    LIBS += ../bin/libmodel.so\
            ../bin/libofd.so

}

win32{
    LIBS += ../bin/model.lib \
            ../bin/ofd.lib
}
