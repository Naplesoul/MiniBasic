#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "initialize.h"
#include "code.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void handleInput();
    ~MainWindow();

protected:
    bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::MainWindow *ui;
    Code *code;
    OPTYPE opType;

    void runCode();

};
#endif // MAINWINDOW_H
