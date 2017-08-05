#ifndef GLOBALSETTING_H
#define GLOBALSETTING_H

#include <QObject>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include "model_global.h"

///
/// \brief The GlobalSetting class
///     用来做系统的全局设置，没有设置多线程的锁结构，因此多线程慎用
///
class MODELSHARED_EXPORT GlobalSetting
        : public QObject
{
    Q_OBJECT
private:
    explicit GlobalSetting(QObject *parent = 0);

    static GlobalSetting* m_instance;


    void loadGlobalSetting(QString filePath);// 从文件中读取设置信息

    void exportGlobalSetting(QString filePath);// 将设置信息写回文件
    void setDefaultSetting();                  // 在代码中纪录默认设置，防止设置文件丢失

public:
    static GlobalSetting* getInstance();    // 获得单例
    static void DestoryInstance();          // 销毁单例


    QString getFilePath(){return this->filepath;}   // 返回配置文件的存储路径
    bool isChanged(){return this->settingChanged;}           // 设置内容是否发生过改变
    // 获得各种类型的全局变量

    // 应用程序信息
    QString getCreator(){return this->creator;}
    QString getCreatorVersion(){return this->creatorVersion;}
    QString getDocType(){return this->docType;}
    QString getDocVersion(){return this->docVersion;}
    QString getViewMode(){return this->viewMode;}
    int getUndoSize(){return this->undoSize;}

    // 字体
    QString getFontFamily(){return this->fontFamily;}
    double getFontSize(){return this->fontSize;}

    // 纸张样式
    double getPhysicalWidth(){return this->physicalWidth;}
    double getPhysicalHeight(){return this->physicalHeight;}
    double getContentWidth(){return this->contentWidth;}
    double getContentHeight(){return this->contentHeight;}
    double getContentX(){return this->contentX;}
    double getContentY(){return this->contentY;}
    QString getPageType(){return this->pageType;}

    // 段落
    QString getHorizontal(){return this->horizontal;}
    double getParaIndent(){return this->paraIndent;}
    double getFirstIndent(){return this->firstIndent;}
    double getSpaceBefore(){return this->spaceBefore;}
    double getSpaceAfter(){return this->spaceAfter;}
    QString getLineHeight(){return this->lineHeight;}
    double getLineHeightValue(){return this->lineHeightValue;}

public slots:
    void setfilePath(QString path);  // 设置文件路径
    void SaveSetting();              // 将设置保存回文件

    // app
    void setCreator(QString creator);
    void setCreatorVersion(QString creatorVersion);
    void setDocType(QString docType);
    void setDocVersion(QString docVersion);
    void setViewMode(QString viewMode);
    void setUndoSize(int undosize);

    // font
    void setFontFamily(QString fontFamily);
    void setFontSize(double fontSize);

    // page
    void setPhysicalWidth(double physicalWidth);
    void setPhysicalHeight(double physicalHeight);
    void setContentWidth(double contentWidth);
    void setContentHeight(double contentHeight);
    void setContentX(double contentX);
    void setContentY(double contentY);
    void setPageType(QString pageType);

    // paragraph
    void setHorizontal(QString horizontal);
    void setParaIndent(double paraIndent);
    void setFirstIndent(double firstIndent);
    void setSpaceBefore(double spaceBefore);
    void setSpaceAfter(double spaceAfter);
    void setLineHeight(QString lineHeight);
    void setLineHeightValue(double lineHeightValue);

private:
    QString filepath;                       // 存放设置文件的路径
    QTextCharFormat charFormat;             // 存放字体格式
    QTextBlockFormat blockFormat;           // 段落格式

    bool settingChanged;                    // 全局设置的内容是否发生改变

    // App 信息
    QString creator;               // 应用程序信息
    QString creatorVersion;        // 应用程序版本
    QString docType;               // 文档标准
    QString docVersion;            // 文档标准版本
    QString viewMode;              // 首次打开软件的阅读模式
    int undoSize;                  // 撤销恢复栈的最大容量 0 为不限容量

    // Font
    QString fontFamily;             // 字体类型
    double fontSize;                // 字体大小

    //Page
    double physicalWidth;           // 页面物理宽度
    double physicalHeight;          // 页面物理高度
    double contentWidth;            // 内容宽度
    double contentHeight;           // 内容高度
    double contentX;                // 内容水平位置
    double contentY;                // 内容竖直位置
    QString pageType;               // 页面大小类型

    // Paragraph
    QString horizontal;             // 水平对齐策略
    double paraIndent;              // 段落缩进
    double firstIndent;             // 首行缩进
    double spaceBefore;             // 段前空白
    double spaceAfter;              // 段后空白
    QString lineHeight;             // 行高策略
    double lineHeightValue;         // 行高值


signals:

public slots:

};


#endif // GLOBALSETTING_H
