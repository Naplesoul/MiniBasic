#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , codeEditor(new CodeEditor(this))
{
    ui->setupUi(this);
    ui->inputEdit->installEventFilter(this);

}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->inputEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)
             {
                 submitInput();
                 ui->inputEdit->clear();
                 return true;
             }
        }
    }
    return false;
}

void MainWindow::submitInput()
{
    QString input = ui->inputEdit->toPlainText();
    codeEditor->receiveInput(input);
}

MainWindow::~MainWindow()
{
    delete ui;
}

