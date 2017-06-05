# ofdEditor

## 开发环境（实时更新）
 ~~Qt5.8 + msvc2015~~
```
win32 : Qt 4.8.6 + MinGW 4.8.2
中标麒麟 6.0 : Qt 4.8.6 + Gcc 4.8.3
```

## 目标运行环境
`windows 10 ` `ubuntu 16.04` `中标麒麟6.0`

## 功能需求
1. 创建文档：生成基本OFD框架；
2. 页面设置与分页：设置纸张形式和分页机制；
3. 文字录入：在控制区域内实现文字的增、改、删；
4. 版面控制：设置版心、字体、颜色、字号、行距、字距等；
5. 段落控制：实现段落缩进、居左、居中、居右、撑满等。
6. 图像控制：实现图像的插入和文字环绕；
7. 表格编辑：实现表格插入、绘制和文字表格内排版。
8. 账号管理：能够注册、登录、注销用户账号；
9. 文件云端备份功能：能够将文件备份在云端、实现多版本保存；
10. ~~协同编辑：两个或多个用户能够同时编辑同一份文档。~~

## 工程布局（实时更新）
本工程将采用Qt 多工程组织的形式，将按文件功能将代码文件分为若干个不同的子工程。<br>
子工程主要分为两个类型`app`和`lib`，`app`类型为可执行文件，`lib`类型为库文件。

```
|-model     -> 系统工作时的数据模型及显示渲染
|-ofd       -> ofd，主要用与ofd文件读取导出，以及ofd读取初期所使用的数据结构
|-start     -> start，主要存放启动程序已经程序的一些基本的界面文件。

|-test_chaoqun -> 江超群的测试工程
|-test_pan     -> 潘言星的测试工程
|-test_yang    -> 杨语晨的测试工程
```

## Qt 帮助文档
Qt的帮助文档极为丰富，看完之后能够对Qt的功能产生系统性的了解，这样设计软件时，思路就会更加的清晰。

| 文件名 | 作用 |
| --- | --- |
| Qt Data Types.pdf | Qt的一些基础数据类型 |
| Creating Custom Qt Types.pdf | 创建自定义的Qt 类 |
| MainWindow.pdf | 主窗口相关的 |
| Widget Tutorial.pdf | Widget 的简要教程 |
| Layout.pdf | 布局 |
| State Machine.pdf | 状态机 |
| Sighals and Slots.pdf | 信号槽 |
| Event System.pdf | 事件系统 |
| Drag and Drop.pdf | 拖拽操作处理 |
| Painting System.pdf | 绘画系统 |
| Qgraphics view framework.pdf | Qt的快速响应的图形渲染方式 |
| Qt 富文本框架.pdf | Qt富文本系统 |
| Creating Qt Plugins.pdf | 制作Qt插件 |
| Model View Programming.pdf | 模型视图结构编程 |
| Model View Tutorial.pdf | 模型视图结构教学 |
