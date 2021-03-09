#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , code(new Code(this))
    , opType(INPUT)
{
    ui->setupUi(this);
    ui->inputEdit->setFocus();
    ui->inputEdit->installEventFilter(this);
}

// filter shortcut keys: ctrl+L, ctrl+S
bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->inputEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->modifiers() == Qt::ControlModifier)
             {
                 switch(k->key())
                 {
                    case (Qt::Key_L):
                        ui->resultBrowser->setText("Please input file path");
                        opType = LOAD;
                        return true;
                    case (Qt::Key_S):
                        ui->resultBrowser->setText("请输入保存文件的路径");
                        opType = SAVE;
                        return true;
                    case (Qt::Key_R):
                        opType = RUN;
                        runCode();
                        return true;
                 }
             }
             if(k->key() == Qt::Key_Return)
             {
                 qDebug() << "[KEY] enter pressed";
                 handleInput();
                 return true;
             }
             if(k->key() == Qt::Key_Delete)
             {
                 qDebug() << "[KEY] delete pressed";
                 ui->inputEdit->clear();
                 return true;
             }
        }
    }
    return false;
}

// handle cmd input during different states
void MainWindow::handleInput()
{
    QString in = ui->inputEdit->toPlainText();
    switch(opType)
    {
        case LOAD:
            if(code->load(in))
            {
                updateCodeBrowser();
                ui->resultBrowser->setPlainText("File loaded");
                qDebug() << "[FILE] file loaded";
                ui->inputEdit->clear();
                opType = INPUT;
            }
            else
            {
                ui->resultBrowser->setPlainText("Invaild file path, please revise.\nPress del to clear input");
                qDebug() << "[INVALID] invaild file path";
            }
            break;
        case SAVE:
            if(code->save(in))
            {
                ui->resultBrowser->setPlainText("File saved");
                qDebug() << "[FILE] file saved";
                ui->inputEdit->clear();
                opType = INPUT;
            }
            else
            {
                ui->resultBrowser->setPlainText("Invaild file path, please revise.\nPress del to clear input");
                qDebug() << "[INVALID] invaild file path";
            }
            break;
        case RUN:
            ui->resultBrowser->setText("请等待程序运行");
            break;
        case CLEAR:
            ui->resultBrowser->setText("请等待清除完成");
            break;
        case INPUT:
            if(input(in))
            {
                qDebug() << "[VALID] input accepted\n[inputEdit] clear";
                ui->inputEdit->clear();
            }
            else
            {
                qDebug() << "[INVAILD] invalid input";
                ui->resultBrowser->setPlainText("Invaild input, please revise.\nPress del to clear input");
            }
            break;
    }
}

bool MainWindow::input(QString &input)
{
    bool isNumber = false;
    int cmdEnd = 0;
    int len = input.size();
    for(int i = 0; i < len; ++i)
    {
        if(i == len - 1)
        {
            cmdEnd = i;
            break;
        }
        if(input[i] == ' ')
        {
            cmdEnd = i - 1;
            break;
        }
        if(input[i] <= '9' && input[i] >= 0)
            isNumber = true;
        else
            isNumber = false;
    }
    if(cmdEnd != 0)
    {
        QString cmd = input.left(cmdEnd + 1);
        if(isNumber)
        {
            if(cmdEnd == len - 1)
                return false;
            int lineNum = cmd.toInt();
            Line in(lineNum, input.mid(cmdEnd + 2));
            qDebug() << "[insert]" << lineNum << ',' << in.code;
            return code->insert(in);
        }
        if(cmd == "RUN")
        {
            runCode();
            return true;
        }
        if(cmd == "LOAD")
        {
            opType = LOAD;
            ui->resultBrowser->setPlainText("Please input file path");
            return true;
        }
        if(cmd == "SAVE")
        {
            opType = SAVE;
            ui->resultBrowser->setPlainText("Please input file path");
            return true;
        }
        if(cmd == "CLEAR")
        {
            opType = CLEAR;
            code->clear();
            opType = INPUT;
            return true;
        }
        if(cmd == "HELP")
        {
            help();
            return true;
        }
        if(cmd == "QUIT")
        {
            quit();
            return true;
        }
    }
    return false;
}

void MainWindow::runCode()
{

}

void MainWindow::updateCodeBrowser()
{

}

void MainWindow::help()
{

}

void MainWindow::quit()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

