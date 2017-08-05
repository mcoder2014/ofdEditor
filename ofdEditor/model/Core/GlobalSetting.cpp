#include "GlobalSetting.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <string>

#include "json/json.h"
#include "json/json-forwards.h"
#include "jsoncpp.cpp"

using std::string;
using Json::Reader;
using Json::Value;

// 初始化静态成员变量
GlobalSetting* GlobalSetting::m_instance = NULL;

GlobalSetting::GlobalSetting(QObject *parent)
    : QObject(parent)
{
    this->settingChanged = false;
    this->filepath = "config.json";
    loadGlobalSetting(this->filepath);

}

///
/// \brief GlobalSetting::loadGlobalSetting
///     从给定的文件路径中读取系统的全局参数
/// \param filePath 完整路径，以'/' 为分隔符
/// \author chaoqun
/// \date 2017/08/04
///
void GlobalSetting::loadGlobalSetting(QString filePath)
{
    QFile qfile;
    qfile.setFileName(filePath);        // 设置文件路径

    // 如果文件不存在，或者无法打开，使用默认配置
    if(!qfile.exists())
    {
        qDebug() << "This global setting file is not exists,"
                    << " program will use default setting";

        this->setDefaultSetting();  // 使用默认配置
        this->SaveSetting();        // 保存配置
        return;
    }
    // 打开文件
    if(!qfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Program could not open this global setting file,"
                 << " we will use default setting";

        this->setDefaultSetting();
        this->SaveSetting();        // 保存配置
        return;
    }

    QTextStream txtInput(&qfile);       // 设置文件流
    QString qstr = txtInput.readAll();   // 读取文件的全部内容
    string str = qstr.toStdString();    // 读取出标准字符

    Reader reader;    // 解析器
    Value value;      // 值

    if(reader.parse(str,value))
    {
        try
        {
            // 如果json 解析成功
            Value app = value["App"];   // App相关
            Value font = value["Font"]; // 字体相关
            Value page = value["Page"]; // 页面设置相关
            Value paragraph = value["Paragraph"];    // 段落设置相关

            // App
            string str_creator = app["Creator"].asString(); // 编辑器
            string str_creatorVersion = app["CreatorVersion"].asString();   // 软件版本
            string str_docType = app["DocType"].asString(); // 文档标准
            string str_docVersion = app["DocVersion"].asString();   // 文档标准版本
            string str_viewMode = app["ViewMode"].asString();       // 系统开启时默认的视图

            this->creator= QString::fromStdString(str_creator);
            this->creatorVersion = QString::fromStdString(str_creatorVersion);
            this->docType = QString::fromStdString(str_docType);
            this->docVersion = QString::fromStdString(str_docVersion);
            this->viewMode = QString::fromStdString(str_viewMode);
            this->undoSize = app["UndoSize"].asInt();       // 读取撤销恢复的最大数量

            // Font
            string str_fontFamily = font["FontFamily"].asString();  // 字体类型
            double double_fontSize = font["FontSize"].asDouble();   // 字号

            this->fontFamily = QString::fromStdString(str_fontFamily);
            this->fontSize = double_fontSize;

            // 页面设置相关
            double double_width = page["Width"].asDouble();     // 页面物理宽度
            double double_height = page["Height"].asDouble();   // 页面的物理高度
            double double_workWidth = page["WorkWidth"].asDouble(); // 页面的工作区宽度
            double double_workHeight = page["WorkHeight"].asDouble();   // 页面的工作区高度
            double double_workX = page["WorkX"].asDouble();     // x
            double double_workY = page["WorkY"].asDouble();     // y
            string str_pageType = page["PageType"].asString();  // 页面类型

            this->physicalWidth = double_width;
            this->physicalHeight = double_height;
            this->contentWidth = double_workWidth;
            this->contentHeight = double_workHeight;
            this->contentX = double_workX;
            this->contentY = double_workY;
            this->pageType = QString::fromStdString(str_pageType);

            //段落相关
            string str_horizontal = paragraph["Horizontal"].asString();         // 水平对齐模式
            double double_paraIndent = paragraph["ParaIndent"].asDouble();      // 段落默认缩进
            double double_firstIndent = paragraph["FirstIndent"].asDouble();    // 段落首行缩进
            double double_spaceBefore = paragraph["SpaceBefore"].asDouble();    // 段前间距
            double double_spaceAfter = paragraph["SpaceAfter"].asDouble();      // 段后间距
            string str_lineHeight = paragraph["LineHeight"].asString();         // 行高类型
            double double_lineHeightValue = paragraph["LineHeightValue"].asDouble();    // 行高值

            this->horizontal = QString::fromStdString(str_horizontal);
            this->paraIndent = double_paraIndent;
            this->firstIndent = double_firstIndent;
            this->spaceBefore = double_spaceBefore;
            this->spaceAfter = double_spaceAfter;
            this->lineHeight = QString::fromStdString(str_lineHeight);
            this->lineHeightValue = double_lineHeightValue;

            qDebug() << "The global setting file is explained successfully.";

        }
        catch(...)
        {
            // 如果有抛出异常
            qDebug() << "Program explan this global setting file failed,"
                     << "and program throws an exception,"
                     << "we will use default setting.";
            this->setDefaultSetting();
            this->SaveSetting();        // 保存配置
        }

    }
    else
    {
        // 如果json解析失败
        qDebug() << "Program explan this global setting file failed,"
                 << "we will use default setting.";
        this->setDefaultSetting();
        this->SaveSetting();        // 保存配置
    }

    qfile.close();  // 关闭文件
}

///
/// \brief GlobalSetting::exportGlobalSetting
///     将当前系统的全局设置保存到指定文件
/// \param filePath
/// \author chaoqun
/// \date 2017/08/04
///
void GlobalSetting::exportGlobalSetting(QString filePath)
{
    QFile qfile;
    qfile.setFileName(filePath);

    if(!qfile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Program could not creat the global setting file";
        return;
    }

    QTextStream txtOutput(&qfile);  // 用来作为输出流
    Json::StyledWriter swriter;     // 用来将value转换为 string

    Value root;
    Value app;          // 应用程序
    Value font;         // 字体
    Value page;         // 页面
    Value paragraph;    // 段落

    // App
    app["Creator"] = this->creator.toStdString();
    app["CreatorVersion"] = this->creatorVersion.toStdString();
    app["DocType"] = this->docType.toStdString();
    app["DocVersion"] = this->docVersion.toStdString();
    app["ViewMode"] = this->viewMode.toStdString();
    app["UndoSize"] = this->undoSize;

    // Font
    font["FontFamily"] = this->fontFamily.toStdString();
    font["FontSize"] = this->fontSize;

    // Page
    page["Width"] = this->physicalWidth;
    page["Height"] = this->physicalHeight;
    page["WorkWidth"] = this->contentWidth;
    page["WorkHeight"] = this->contentHeight;
    page["WorkX"] = this->contentX;
    page["WorkY"] = this->contentY;
    page["PageType"] = this->pageType.toStdString();

    // Paragraph
    paragraph["Horizontal"] = this->horizontal.toStdString();
    paragraph["ParaIndent"] = this->paraIndent;
    paragraph["FirstIndent"] = this->firstIndent;
    paragraph["SpaceBefore"] = this->spaceBefore;
    paragraph["SpaceAfter"] = this->spaceAfter;
    paragraph["LineHeight"] = this->lineHeight.toStdString();
    paragraph["LineHeightValue"] = this->lineHeightValue;

        root["App"] = app;
        root["Font"] = font;
        root["Page"] = page;
        root["Paragraph"] = paragraph;

    txtOutput << QString::fromStdString(root.toStyledString()) <<endl;
    qfile.close();
}

///
/// \brief GlobalSetting::setDefaultSetting
/// 当没有配置文件时，使用默认配置
/// \author chaoqun
/// \date 2017/08/04
void GlobalSetting::setDefaultSetting()
{
    // App
    this->creator = QString::fromStdString("OFDEditor");
    this->creatorVersion = QString::fromStdString("1.0");
    this->docType = QString::fromStdString("OFD");
    this->docVersion = QString::fromStdString("1.0");
    this->viewMode = QString::fromStdString("Edit");
    this->undoSize = 10;

    // Font
    this->fontFamily = tr("Test");
    this->fontSize = 12;

    // Page
    this->physicalWidth = 210;
    this->physicalHeight = 297;
    this->contentWidth = 210;
    this->contentHeight = 297;
    this->contentX = 0;
    this->contentY = 0;
    this->pageType = QString::fromStdString("A4");

    //Paragraph
    this->horizontal = QString::fromStdString("AlignLeft");
    this->paraIndent = 0;
    this->firstIndent = 0;
    this->spaceBefore = 0;
    this->spaceAfter = 0;
    this->lineHeight = QString::fromStdString("SingleHeight");
    this->lineHeightValue = 0;

}

///
/// \brief GlobalSetting::getInstance
///         获得唯一的全局设置对象实例
/// \return
/// \author chaoqun
/// \date 2017/08/03
GlobalSetting *GlobalSetting::getInstance()
{
    if( m_instance != NULL )
    {
        return m_instance;
    }

    m_instance = new GlobalSetting();       // 新建实例
    return m_instance;
}

///
/// \brief GlobalSetting::DestoryInstance
/// 销毁实例-估计用不上
/// \author chaoqun
/// \date 2017/08/04
///
void GlobalSetting::DestoryInstance()
{
    delete m_instance;
    m_instance = NULL;
}

///
/// \brief GlobalSetting::SaveSetting
///     将软件设置的更改保存到文件中去
/// \author chaoqun
/// \date 2017/08/04
///
void GlobalSetting::SaveSetting()
{
    this->exportGlobalSetting(this->filepath);
}

void GlobalSetting::setCreator(QString creator)
{
    if(this->creator != creator)
    {
        this->settingChanged = true;
        this->creator = creator;
    }

}

void GlobalSetting::setCreatorVersion(QString creatorVersion)
{
    if(this->creatorVersion != creatorVersion)
    {
        this->settingChanged = true;
        this->creatorVersion = creatorVersion;
    }

}

void GlobalSetting::setDocType(QString docType)
{
    if(this->docType != docType)
    {
        this->settingChanged = true;
        this->docType = docType;
    }
}

void GlobalSetting::setDocVersion(QString docVersion)
{
    if(this->docVersion != docVersion)
    {
        this->settingChanged = true;
        this->docVersion = docVersion;
    }

}

void GlobalSetting::setViewMode(QString viewMode)
{
    if(this->viewMode != viewMode)
    {
        this->settingChanged = true;
        this->viewMode = viewMode;
    }

}

void GlobalSetting::setUndoSize(int undosize)
{
    if(this->undoSize != undosize)
    {
        this->settingChanged = true;
        this->undoSize = undosize;
    }
}

void GlobalSetting::setFontFamily(QString fontFamily)
{
    if(this->fontFamily != fontFamily)
    {
        this->settingChanged = true;
        this->fontFamily = fontFamily;
    }

}

void GlobalSetting::setFontSize(double fontSize)
{
    if(this->fontSize != fontSize)
    {
        this->settingChanged = true;
        this->fontSize = fontSize;
    }

}

void GlobalSetting::setPhysicalWidth(double physicalWidth)
{
    if(this->physicalWidth != physicalWidth)
    {
        this->settingChanged = true;
        this->physicalWidth = physicalWidth;
    }

}

void GlobalSetting::setPhysicalHeight(double physicalHeight)
{
    if(this->physicalHeight != physicalHeight)
    {
        this->settingChanged = true;
        this->physicalHeight = physicalHeight;
    }

}

void GlobalSetting::setContentWidth(double contentWidth)
{
    if(this->contentWidth != contentWidth)
    {
        this->settingChanged = true;
        this->contentWidth = contentWidth;
    }

}

void GlobalSetting::setContentHeight(double contentHeight)
{
    if(this->contentX != contentHeight)
    {
        this->settingChanged = true;
        this->contentHeight = contentHeight;
    }
}

void GlobalSetting::setContentX(double contentX)
{
    if(this->contentX != contentX)
    {
        this->settingChanged = true;
        this->contentX = contentX;
    }
}

void GlobalSetting::setContentY(double contentY)
{
    if(this->contentY != contentY)
    {
        this->settingChanged = true;
        this->contentY = contentY;
    }
}

void GlobalSetting::setPageType(QString pageType)
{
    if(this->pageType != pageType)
    {
        this->settingChanged = true;
        this->pageType = pageType;
    }
}

void GlobalSetting::setHorizontal(QString horizontal)
{
    if(this->horizontal != horizontal)
    {
        this->settingChanged = true;
        this->horizontal = horizontal;
    }

}

void GlobalSetting::setParaIndent(double paraIndent)
{
    if(this->paraIndent != paraIndent)
    {
        this->settingChanged = true;
        this->paraIndent = paraIndent;
    }

}

void GlobalSetting::setFirstIndent(double firstIndent)
{
    if(this->firstIndent != firstIndent)
    {
        this->settingChanged = true;
        this->firstIndent = firstIndent;
    }
}

void GlobalSetting::setSpaceBefore(double spaceBefore)
{
    if(this->spaceBefore != spaceBefore)
    {
        this->settingChanged = true;
        this->spaceBefore = spaceBefore;
    }
}

void GlobalSetting::setSpaceAfter(double spaceAfter)
{
    if(this->spaceAfter != spaceAfter)
    {
        this->settingChanged = true;
        this->spaceAfter = spaceAfter;
    }
}

void GlobalSetting::setLineHeight(QString lineHeight)
{
    if(this->lineHeight != lineHeight)
    {
        this->settingChanged = true;
        this->lineHeight = lineHeight;
    }
}

void GlobalSetting::setLineHeightValue(double lineHeightValue)
{
    if(this->lineHeightValue != lineHeightValue)
    {
        this->settingChanged = true;
        this->lineHeightValue = lineHeightValue;
    }
}

void GlobalSetting::setfilePath(QString path)
{
    this->filepath = path;
}
