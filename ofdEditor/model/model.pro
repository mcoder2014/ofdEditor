#-------------------------------------------------
#
# Project created by QtCreator 2017-04-13T16:22:20
#
#-------------------------------------------------

# 这个文件是为了建立ofd文件的模型,是一个静态类库

QT       += widgets xmlpatterns xml

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
    Doc/DocLayer.cpp \
    Doc/DocPage.cpp \
    Doc/DocPassage.cpp \
    Doc/DocPicture.cpp \
    Doc/DocTable.cpp \
    Doc/DocTableCell.cpp \
    Doc/DocText.cpp \
    Doc/DocTextBlock.cpp \
    Doc/DocTextStyle.cpp \
    Tool/UnitTool.cpp \
    Doc/DocPageScene.cpp \
    Widget/ParagraphFormatDialog.cpp \
    Widget/FontSettingDialog.cpp \
    Convert/OFD_DocConvertor.cpp \
    Convert/MergeCT_Texts.cpp \
    Convert/Doc_OFDConvertor.cpp \
    Convert/Objects/MinTextUnit.cpp \
    Widget/DocInfoDialog.cpp \
    Command/SetTextBlodCmd.cpp \
    Doc/DocImageBlock.cpp \
    Widget/imagepropertiesdialog.cpp \
    Widget/PageDialog.cpp \
    Widget/InsertTableDialog.cpp \
    Widget/FindAndReplaceDock.cpp \
    Widget/SelectTemplateDialog.cpp \
    Core/GlobalSetting.cpp \
    Widget/InsertPageDialog.cpp

HEADERS += model.h\
        model_global.h \
    Doc/DocBlock.h \
    Doc/DocGraph.h \
    Doc/DocLayer.h \
    Doc/DocPage.h \
    Doc/DocPassage.h \
    Doc/DocPicture.h \
    Doc/DocTable.h \
    Doc/DocTableCell.h \
    Doc/DocText.h \
    Doc/DocTextBlock.h \
    Doc/DocBasicTypes.h \
    Doc/DocTextStyle.h \
    Tool/UnitTool.h \
    Doc/DocPageScene.h \
    Widget/ParagraphFormatDialog.h \
    Widget/FontSettingDialog.h \
    Convert/OFD_DocConvertor.h \
    Convert/MergeCT_Texts.h \
    Convert/Doc_OFDConvertor.h \
    Convert/Objects/MinTextUnit.h \
    Command/SetTextBlodCmd.h \
    Doc/DocImageBlock.h \
    Widget/imagepropertiesdialog.h \
    Widget/PageDialog.h \
    Widget/FindAndReplaceDock.h \
    Widget/InsertTableDialog.h \
    Widget/SelectTemplateDialog.h \
    Core/GlobalSetting.h \
    Widget/DocInfoDialog.h \
    Widget/InsertPageDialog.h

DESTDIR = ../bin     # 生成文件在这
MOC_DIR = ./moc     # Q_OBJECT 类转换后的文件
RCC_DIR = ./rcc     # .qrc 文件转换后存放路径
OBJECTS_DIR += ./tmp   # .obj 文件存放路径

TRANSLATIONS = cn_model.ts

INCLUDEPATH += $$PWD/../ofd \
               $$PWD/../model \
               $$PWD/../libs/jsoncpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix{
    LIBS += -L../bin -lofd

}

win32{
    LIBS += -L../bin -lofd
}

FORMS += \
    Widget/ParagraphFormatDialog.ui \
    Widget/FontSettingDialog.ui \
    Widget/DocInfoDialog.ui \
    Widget/imagepropertiesdialog.ui \
    Widget/PageDialog.ui \
    Widget/InsertTableDialog.ui \
    Widget/FindAndReplaceDock.ui \
    Widget/SelectTemplateDialog.ui \
    Widget/InsertPageDialog.ui

RESOURCES += \
    resource.qrc
