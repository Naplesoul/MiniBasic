#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include "initialize.h"
#include "code.h"
#include "program.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void help();
    void quit();
    void load();
    void save();
    void run();
    void clear();
    ~MainWindow();

protected:
    // filter shortcut keys: ctrl+L, ctrl+S
    bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::MainWindow *ui;
    Code *code;
    Program *program;
    QString inputOfProgram;
    QString outputOfProgram;
    Status status;

    void clearCode();
    void loadFile(const QString &filename);
    void saveFile(const QString &filename);
    void runCode();
    void runSingle(QString &cmd);
    void runSingleCode();

    void handleInput();
    bool input(QString &input);
    void updateCodeBrowser();

    void programInput(QString &input);
//    void print(QString &output);
    void printTree(QString output);

    bool isSingleCmd = false;


    QString helpText = "Basic commands:\nRUN --Execute the program\nLOAD --Load code from a file\nLIST --List code in the Code box\nCLEAR --Clear code and context\nHELP --Get help text\nQUIT --Quit the interpreter\n\nTo insert a line, type line number with a space before the command:\n1 PRINT 666\nLine number should be between 1 & 1000000\n\nOnly typing a line number will delete this line\n\nINPUT, PRINT, LET command can be executed immediately on the current context";

signals:
    void quitSignal();
};
#endif // MAINWINDOW_H
