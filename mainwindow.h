#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
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
    bool input(QString &input);
    void updateCodeBrowser();
    void help();
    void quit();
    void clearCode();
    void loadFile(QString &filename);
    void saveFile(QString &filename);
    void load();
    void save();
    void run();
    ~MainWindow();

protected:
    // filter shortcut keys: ctrl+L, ctrl+S
    bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::MainWindow *ui;
    Code *code;
    OPTYPE opType;

    bool runCode();

signals:
    void quitSignal();

};
#endif // MAINWINDOW_H
