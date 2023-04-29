#include "aboutauthor.h"
#include "ui_aboutauthor.h"

AboutAuthor::AboutAuthor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutAuthor)
{
    ui->setupUi(this);

    // 设置关于作者界面的标题
    setWindowTitle("关于作者");
}

AboutAuthor::~AboutAuthor()
{
    delete ui;
}
