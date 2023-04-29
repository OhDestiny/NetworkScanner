#include "helpwindow.h"
#include "ui_helpwindow.h"

helpWindow::helpWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpWindow)
{
    ui->setupUi(this);

    // 设置帮助文档的标题
    setWindowTitle("帮助文档");

    // 设置窗口的图标 ": + 前缀名 + 文件名"
    // ui->->setIcon(QIcon(":/n/res/helpWindow.png"));
}

helpWindow::~helpWindow()
{
    delete ui;
}
