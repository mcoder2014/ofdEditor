#-------------------------------------------------
#
# Project created by QtCreator 2017-04-13T16:22:20
#
#-------------------------------------------------

# 这个文件是为了建立ofd文件的模型,是一个静态类库

QT       += widgets

TARGET = model
TEMPLATE = lib

DEFINES += MODEL_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += model.cpp \
    DocImage.cpp \
    DocParagraph.cpp \
    DocTable.cpp \
    DocPassage.cpp \
    DocLayer.cpp \
    DocBlock.cpp \
    DocTextBlock.cpp \
    DocText.cpp \
    DocGraph.cpp \
    DocPicture.cpp \
    DocTableRow.cpp \
    DocTableCell.cpp \
    DocPage.cpp

HEADERS += model.h\
        model_global.h \
    DocImage.h \
    DocParagraph.h \
    DocTable.h \
    DocPassage.h \
    DocLayer.h \
    DocBlock.h \
    DocTextBlock.h \
    DocText.h \
    DocGraph.h \
    DocPicture.h \
    DocTableRow.h \
    DocTableCell.h \
    DocPage.h

DESTDIR = ../bin     # 生成文件在这
MOC_DIR = ./moc     # Q_OBJECT 类转换后的文件
RCC_DIR = ./rcc     # .qrc 文件转换后存放路径
OBJECTS_DIR += ./tmp   # .obj 文件存放路径

INCLUDEPATH += $$PWD/../ofd

unix {
    target.path = /usr/lib
    INSTALLS += target
}


unix{
    LIBS += ../bin/libofd.so

}

win32{
    LIBS += ../bin/ofd.lib
}
