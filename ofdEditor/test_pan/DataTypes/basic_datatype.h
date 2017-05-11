#ifndef COMMONDT_H
#define COMMONDT_H
#include <QString>
#include <QStringList>
#include <QVector>
#include "../ofd_global.h"   // 生成库文件时需要


//6种基本数据类型
class OFDSHARED_EXPORT ST_Loc {
    QString title;  //该路径的标签，比如资源路径为"Res"，页面路径为"Page"（与XML的标签等同）
    QString abs_path;   //绝对路径
public:
    ST_Loc(){}
    ST_Loc(QString tag, QString relative_path, QString current_path) : //构造方法，包括对路径的解析
        title(tag) {
    //        qDebug() << "cur = " << current_path << "rela = " << relative_path << endl;
        current_path.replace("\\", "/");    //路径分隔符标准化
    //        qDebug() << "cur = " << current_path << endl;
        if (relative_path.startsWith("../")) {   //从父路径开始
            int i, cnt = 0;
            for (i = current_path.length() - 1; i >= 0; i--) {
                if (current_path[i] == '/')
                        cnt++;
    //                qDebug() <<"Cnt = " << cnt << endl;
                if (cnt == 2) break;
            }
            current_path.chop(current_path.length() - i - 1);
    //            qDebug() <<"chop = " << current_path.length() - i << endl;
            relative_path.remove(0, 3);
        } else { //从当前路径开始
            int i, cnt = 0;
            for (i = current_path.length() - 1; i >= 0; i--) {
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
        abs_path.replace("/", "\\");    //路径分隔符标准化
    }   //构造方法，包括对路径的解析
    ST_Loc(QString tag, QString _abs_path) : title(tag), abs_path(_abs_path) {  //构造方法，在直接已有绝对路径时
        abs_path.replace("/", "\\");
    }

    operator QString() {    //可以直接把ST_Loc作为一个路径字符串使用
        return abs_path;
    }
    QString getPath() {     //返回绝对路径，以“\\”为分隔符
        return abs_path;
    }

    QString getTitle() {    //返回当该路径的标签
        return title;
    }

    bool isNull() {
        return abs_path.isNull();
    }
};

class OFDSHARED_EXPORT ST_Array {   //以QStringList的形式来实现（因为容器类型的多样性，不如返回QString，具体的类型留给调用者去处理）
    QString title;
    QStringList elements;
public: //对QStringList的一些简单封装（在有需要时再拓展接口）
    ST_Array() {}
    ST_Array(QString const & tag, QString const & elements_collection, QString const & separator = " ") :
        title(tag), elements(elements_collection.split(separator)) {}
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
};

class ID_Table {    //为方便ID的管理，为两者建一个基类
protected:
    static QVector<int> id_set;     //凡是出现过的ID，全都在其中记录
};



class OFDSHARED_EXPORT ST_ID : public ID_Table {
    long id;
    bool is_null;
public:
    ST_ID() {
        is_null = true;
        id = 0;
    }

    ST_ID(int _id) {
        if (id_set.contains(_id)){
            //！！！出现重复ID，报错处理
        }
        id = _id;
        is_null = false;
    }
    operator long() { return id; }
    long getID() { return id; }
    bool isNull() {
        return is_null;
    }
};

class OFDSHARED_EXPORT ST_RefID : public ID_Table {
    long ref_id;
    bool is_null;
public:
    ST_RefID() {
        is_null = true;
        ref_id = 0;
    }
    ST_RefID(int _ref_id) {
        if (!id_set.contains(_ref_id)){
            //！！！不存在的ID，报错处理
        }
        ref_id = _ref_id;
    }
    operator long() { return ref_id; }
    long getRefID() { return ref_id; }
    bool isNull() {
        return is_null;
    }
};

class OFDSHARED_EXPORT ST_Pos { //坐标位置，简单封装即可
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

class OFDSHARED_EXPORT ST_Box { //坐标位置+边长，简单封装即可
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


class OFDSHARED_EXPORT CT_Base { //所有含有ID属性的元素的基类
    ST_ID id;
public:
    ST_ID getID() {
        return id;
    }
    void setID(ST_ID new_id) {
        id = new_id;
    }
};
#endif // COMMONDT_H
