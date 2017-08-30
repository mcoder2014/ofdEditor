#ifndef COMMONDT_H
#define COMMONDT_H
#include <QString>
#include <QStringList>
#include <QVector>
#include "../ofd_global.h"   // 生成库文件时需要
#include <QDebug>
#include <QMap>
#include "../ofdexceptions.h"

//6种基本数据类型
class OFDSHARED_EXPORT ST_Loc
{
    QString title;  //该路径的标签，比如资源路径为"Res"，页面路径为"Page"（与XML的标签等同）
    QString abs_path;   //绝对路径
    QString rela_path;
public:
    ST_Loc(){}
    ST_Loc(QString tag, QString relative_path, QString current_path) : //构造方法，包括对路径的解析
        title(tag), rela_path(relative_path)
    {
        //        qDebug() << "cur = " << current_path << "rela = " << relative_path << endl;
        current_path.replace("\\", "/");    //路径分隔符标准化
        //        qDebug() << "cur = " << current_path << endl;
        if (relative_path.startsWith("../"))
        {   //从父路径开始
            int i, cnt = 0;
            for (i = current_path.length() - 1; i >= 0; i--)
            {
                if (current_path[i] == '/')
                    cnt++;
                //                qDebug() <<"Cnt = " << cnt << endl;
                if (cnt == 2) break;
            }
            current_path.chop(current_path.length() - i - 1);
            //            qDebug() <<"chop = " << current_path.length() - i << endl;
            relative_path.remove(0, 3);
        }
        else
        { //从当前路径开始
            int i, cnt = 0;
            for (i = current_path.length() - 1; i >= 0; i--)
            {
                if (current_path[i] == '/')
                    cnt++;
                //                qDebug() <<"Cnt = " << cnt << endl;
                if (cnt == 1) break;
            }
            current_path.chop(current_path.length() - i - 1); //消去当前路径后面的无用内容
            if (relative_path.startsWith("./"))  //消去相对路径前端的"./"或者"/"
                relative_path.remove(0, 2);
            else if (relative_path.startsWith("/"))
                relative_path.remove(0, 1);
        }
        //        qDebug() << "cur = " << current_path << "rela = " << relative_path << endl;
        abs_path = current_path + relative_path;
    }   //构造方法，包括对路径的解析

    ST_Loc(QString tag, QString _abs_path)
        : title(tag), abs_path(_abs_path)
    {  //构造方法，在直接已有绝对路径时
        abs_path.replace("\\", "/");
    }

    operator QString()
    {    //可以直接把ST_Loc作为一个路径字符串使用
        return abs_path;
    }

    QString getPath()
    {     //返回绝对路径，以“\\”为分隔符
        return abs_path;
    }

    QString getRelativePath()
    {
        return rela_path;
    }

    QString getTitle()
    {    //返回当该路径的标签
        return title;
    }

    bool isNull()
    {
        return abs_path.length() == 0;
    }
};

class OFDSHARED_EXPORT ST_Array
{
    //以QStringList的形式来实现（因为容器类型的多样性，不如返回QString，具体的类型留给调用者去处理）
public:
    QString title;
    QStringList elements;
    QString all_content;
public: //对QStringList的一些简单封装（在有需要时再拓展接口）
    ST_Array() {}

    ST_Array(QString const & tag, QString const & elements_collection, QString const & separator = " ") :
        title(tag), elements(elements_collection.split(separator)),all_content(elements_collection) {}
    int size() {    //元素个数
        return elements.size();
    }

    int length() {  //元素个数
        return elements.length();
    }

    QString operator[](int index) { //随机访问元素
        return elements[index];
    }

    bool contains(QString const & value) {  //判断值为value的元素是否存在
        return elements.contains(value);
    }

    bool isNull() {
        return elements.size() == 0;
    }

    QString getAllContent() {
        return all_content;
    }
};


class OFDSHARED_EXPORT ST_ID
{
    long id;
    bool is_null;
public:

    ST_ID()
    {
        is_null = true;
        id = 0;
    }

    ST_ID(int _id)
    {
        id = _id;
        is_null = false;

    }
    operator long() { return id; }
    long getID() { return id; }
    bool isNull()
    {
        return is_null;
    }

};

class OFDSHARED_EXPORT ST_RefID
{
    long ref_id;
    bool is_null;
public:
    ST_RefID()
    {
        is_null = true;
        ref_id = 0;
    }
    ST_RefID(int _ref_id)
    {
        ref_id = _ref_id;
        is_null = false;
    }
    operator long() { return ref_id; }
    long getRefID() { return ref_id; }
    bool isNull()
    {
        return is_null;
    }
};

class OFDSHARED_EXPORT ST_Pos
{
    //坐标位置，简单封装即可
    double x, y;
    bool is_null;
public:
    ST_Pos() { is_null = true; x = y = 0; }
    ST_Pos(double _x, double _y) :
        x(_x), y(_y), is_null(false) {}
    double getX() const { return x; }
    double getY() const { return y; }
    void setX(double _x) { x = _x; }
    void setY(double _y) { y = _y; }
    bool isNull() { return is_null; }
};

class OFDSHARED_EXPORT ST_Box
{
    //坐标位置+边长，简单封装即可
    double start_x, start_y, delta_x, delta_y;
    bool is_null;
public:
    ST_Box() { is_null = true;  start_x = start_y = delta_x = delta_y = 0; }
    ST_Box(double _start_x, double _start_y, double _delta_x, double _delta_y) :
        start_x(_start_x), start_y(_start_y), delta_x(_delta_x), delta_y(_delta_y), is_null(false) {}
    double getX() { return start_x; }
    double getY() { return start_y; }
    double getDeltaX() { return delta_x; }
    double getDeltaY() { return delta_y; }
    void setX(double _x) { start_x = _x; }
    void setY(double _y) { start_y = _y; }
    void setDeltaX(double _delta_x) { delta_x = _delta_x; }
    void setDeltaY(double _delta_y) { delta_y = _delta_y; }
    bool isNull() { return is_null; }
};

class CT_Base;

class OFDSHARED_EXPORT ID_Table
{
    //为方便ID的管理，为两者建一个基类
private:
    QMap<int, CT_Base *> id_pool;     //凡是出现过的ID，全都在其中记录
public:
    friend class ST_ID;
    friend class ST_RefID;

    CT_Base * getItem(int key);        //若该key对应的东西不存在，则返回空指针

    bool contains(int key);

    int key(CT_Base * value);         //返回以value为值的键值。若不存在，则返回0

    CT_Base * remove(int key);         //从集合中删去键值为key的元素，并将其返回。若不存在，则返回空指针

    //bool insert(int key, CT_Base * value);         //向集合中添加一项，若成功则返回true,否则返回false

    int size();                         //返回集合中项的个数

    bool registerItem(ST_ID key, CT_Base * value);        //在集合中注册一个新的ID，若成功返回true，否则返回false
    bool registerItem(int key, CT_Base * value);          //重载
};

class OFDSHARED_EXPORT CT_Base
{ //所有含有ID属性的元素的基类
    ST_ID id;
public:

    ST_ID getID()
    {
        return id;
    }

    void setID(ST_ID new_id, ID_Table * _id_table)
    {
        if (_id_table->contains(new_id.getID()))
            throw InvalidIDException("试图注册重复的ID: " + QString::number(new_id));
        id = new_id;
        _id_table->registerItem(new_id.getID(), this);
    }

};


#endif // COMMONDT_H
