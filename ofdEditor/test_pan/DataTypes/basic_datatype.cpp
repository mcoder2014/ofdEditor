#include "basic_datatype.h"

ST_Loc::ST_Loc(QString tag, QString relative_path, QString current_path) : //构造方法，包括对路径的解析
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
}
