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
    void debug();
    void step();
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
    void debugButtonPressed();

    void handleInput();
    bool input(QString &input);
    void updateCodeBrowser();

    void programInput(QString &input);
//    void print(QString &output);
    void printTree(QString output);

    bool isSingleCmd = false;
    bool isDebugMode = false;

    void highLightWrong();
    void highLightWrongAndNext();

    void updateContextBrowser();
    void printNextTree();

    QString helpText =  "Basic commands:\nRUN --Execute the program\n"
                        "SAVE --Save code to a file\n"
                        "LOAD --Load code from a file\n"
                        "LIST --List code in the Code box\n"
                        "CLEAR --Clear code and context\n"
                        "DEL --Delete a line\n"
                        "HELP --Get help text\n"
                        "QUIT --Quit the interpreter\n\n"
                        "Supported statements:\n"
                        "REM: Comments, e.g. 1 REM HELLO!\n"
                        "LET: Assignment, e.g. 1 LET N = 3  or  1 LET N = \"HELLO\"\n"
                        "PRINT: Print, e.g. 1 PRINT 5  or  1 PRINT N\n"
                        "PRINTF: Formatted print, e.g. 1 PRINTF \"HELLO, {}! {} {}\", \"WORLD\", 666, N\n"
                        "INPUT: Input an INT, e.g. 1 INPUT N\n"
                        "INPUTS: Input a STRING, e.g. 1 INPUT N\n"
                        "GOTO: Jump to a line, e.g. 1 GOTO 9\n"
                        "IF: if, <= and >= NOT supported, e.g. 1 IF N > 9 THEN 9\n"
                        "END: End of the program, can be omitted, e.g. 10 END\n\n"
                        "Shortcuts:\n"
                        "Ctrl + L --LOAD\n"
                        "Ctrl + S --SAVE\n"
                        "Ctrl + R --RUN\n"
                        "Ctrl + Del --Clear input box\n\n"
                        "To insert a line, type line number with a space before the command:\n"
                        "1 PRINT 666\n"
                        "Line number should be between 1 & 1000000\n\n"
                        "Only typing a line number will delete this line\n\n"
                        "INPUT, INPUTS, PRINT, PRINTF and LET command can be executed immediately on the current context";

signals:
    void quitSignal();
};
#endif // MAINWINDOW_H
