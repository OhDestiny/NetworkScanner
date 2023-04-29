#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <helpwindow.h>
#include <aboutauthor.h>
#include <aboutmachine.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // 定义帮助文档点击的槽函数
private slots:
    void on_click_helpWindow();                // 点击帮助文档事件的槽函数
    void on_click_aboutAuthor();               // 点击关于作者的槽函数
    void on_click_aboutMachine();              // 点击关于本机的槽函数
};
#endif // MAINWINDOW_H
