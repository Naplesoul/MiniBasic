#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "initialize.h"
#include "codeeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void submitInput();
    ~MainWindow();

protected:
    bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::MainWindow *ui;
    CodeEditor *codeEditor;
};
#endif // MAINWINDOW_H
