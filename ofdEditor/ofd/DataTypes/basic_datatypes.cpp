#include "basic_datatype.h"
#include "../ofd_global.h"   // 生成库文件时需要

CT_Base * ID_Table::getItem(int key) {
    if (id_pool.contains(key))
        return id_pool[key];
    else return NULL;
}

bool ID_Table::contains(int key) {
    return id_pool.contains(key);
}

int ID_Table::key(CT_Base * value) {
    return id_pool.key(value, 0);
}

CT_Base * ID_Table::remove(int key) {
    if (id_pool.contains(key))
        return id_pool.take(key);
    else return NULL;
}

bool ID_Table::registerItem(int key, CT_Base * value) {
    if (id_pool.contains(key))
        return false;
    else id_pool.insert(key, value);
    return true;
}

bool ID_Table::registerItem(ST_ID key, CT_Base * value) {
    return registerItem(key.getID(), value);
}

int ID_Table::size() {
    return id_pool.size();
}
