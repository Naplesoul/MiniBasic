#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , code(new Code(this))
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
                        ui->resultBrowser->setText("请输入载入文件的路径");
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
                 handleInput();
                 return true;
             }
        }
    }
    return false;
}

// handle cmd input during different states
void MainWindow::handleInput()
{
    QString input = ui->inputEdit->toPlainText();
    switch(opType)
    {
        case LOAD:
            code->load(input);
            ui->inputEdit->clear();
            break;
        case SAVE:
            code->save(input);
            ui->inputEdit->clear();
            break;
        case RUN:
            ui->resultBrowser->setText("请等待程序运行");
            break;
        case CLEAR:
            ui->resultBrowser->setText("请等待清除完成");
            break;
        case INPUT:
            code->input(input);
            ui->inputEdit->clear();
            break;
    }
}

void MainWindow::runCode()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

