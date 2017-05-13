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
    Doc/DocBlock.cpp \
    Doc/DocGraph.cpp \
    Doc/DocImage.cpp \
    Doc/DocLayer.cpp \
    Doc/DocPage.cpp \
    Doc/DocParagraph.cpp \
    Doc/DocPassage.cpp \
    Doc/DocPicture.cpp \
    Doc/DocTable.cpp \
    Doc/DocTableCell.cpp \
    Doc/DocTableRow.cpp \
    Doc/DocTemplate.cpp \
    Doc/DocText.cpp \
    Doc/DocTextBlock.cpp \
    Doc/DocDrawParam.cpp \
    Doc/DocParaStyle.cpp \
    Doc/DocTextStyle.cpp \
    Widget/PassageWidget.cpp \
    Tool/UnitTool.cpp

HEADERS += model.h\
        model_global.h \
    Doc/DocBlock.h \
    Doc/DocGraph.h \
    Doc/DocImage.h \
    Doc/DocLayer.h \
    Doc/DocPage.h \
    Doc/DocParagraph.h \
    Doc/DocPassage.h \
    Doc/DocPicture.h \
    Doc/DocTable.h \
    Doc/DocTableCell.h \
    Doc/DocTableRow.h \
    Doc/DocTemplate.h \
    Doc/DocText.h \
    Doc/DocTextBlock.h \
    Doc/DocDrawParam.h \
    Doc/DocBasicTypes.h \
    Doc/DocParaStyle.h \
    Doc/DocTextStyle.h \
    Widget/PassageWidget.h \
    Tool/UnitTool.h

DESTDIR = ../bin     # 生成文件在这
MOC_DIR = ./moc     # Q_OBJECT 类转换后的文件
RCC_DIR = ./rcc     # .qrc 文件转换后存放路径
OBJECTS_DIR += ./tmp   # .obj 文件存放路径

INCLUDEPATH += $$PWD/../ofd \
               $$PWD/../model

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
