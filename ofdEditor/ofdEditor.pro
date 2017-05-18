#-------------------------------------------------
#
# Project created by QtCreator 2017-04-13T16:22:20
#
#-------------------------------------------------

# 本工程的为多工程组织文件

TEMPLATE = subdirs      # 工程类型，多工程

#  子工程
SUBDIRS += \
    model \
    ofd   \
    start \
    #test_chaoqun\
#    test_pan \
    #test_yang

CONFIG += ordered warn_on  qt
# ordered 按照上边的顺序编译子工程，尽量减少两个工程之间的相互依赖关系，将依赖的工程先行编译
# warn_on 尽量充分的显示编译的信息
# qt The target is a Qt application or library and requires the Qt library and header files.
# The proper include and library paths for the Qt library will automatically be added to the project.
# This is defined by default, and can be fine-tuned with the \l{#qt}{QT} variable.
