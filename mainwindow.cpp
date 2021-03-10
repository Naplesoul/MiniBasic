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
    setWindowTitle("MiniBasic");
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::load);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::save);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearCode);
    connect(ui->runButton, &QPushButton::clicked, this, &MainWindow::run);
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
                        load();
                        return true;
                    case (Qt::Key_S):
                        save();
                        return true;
                    case (Qt::Key_R):
                        run();
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
             if(k->key() == Qt::Key_Escape)
             {
                 qDebug() << "[KEY] esc pressed";
                 if(opType == SAVE || opType == LOAD)
                 {
                     opType = INPUT;
                     ui->resultBrowser->setPlainText("Exit saving/loading mode.\nPlease input command");
                     return true;
                 }
                 return false;
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
            loadFile(in);
            break;
        case SAVE:
            saveFile(in);
            break;
        case RUN:
            ui->resultBrowser->setText("Running now, please wait...");
            break;
        case CLEAR:
            ui->resultBrowser->setText("clearing now, please wait...");
            break;
        case INPUT:
            if(input(in))
            {
                ui->codeBrowser->setPlainText(code->printCode());
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
    int cmdEnd = -1;
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
        if(input[i] <= '9' && input[i] >= '0')
            isNumber = true;
        else
            isNumber = false;
    }
    QString cmd = input.left(cmdEnd + 1);
    if(isNumber)
    {
        if(cmdEnd == len - 1)
            return false;
        int lineNum = cmd.toInt();
        Line in(lineNum, input.mid(cmdEnd + 2));
        qDebug() << "[insert]" << lineNum << ',' << in.code;
        if(code->insert(in))
        {
            QString message = "Line " + cmd + " inserted";
            ui->resultBrowser->setPlainText(message);
            return true;
        }
        else
            return false;
    }
    if(cmd == "RUN")
    {
        run();
        return true;
    }
    if(cmd == "LOAD")
    {
        load();
        return true;
    }
    if(cmd == "SAVE")
    {
        save();
        return true;
    }
    if(cmd == "CLEAR")
    {
        opType = CLEAR;
        clearCode();
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
    if(cmd == "DEL")
    {
        int lineNum = input.mid(cmdEnd + 1).toInt();
        if(code->del(lineNum))
        {
            QString message = "Line" + input.mid(cmdEnd + 1) + " deleted";
            qDebug() << "[VAILD] valid delete";
            updateCodeBrowser();
            ui->resultBrowser->setPlainText(message);
            return true;
        }
        else
        {
            qDebug() << "[INVAILD] invalid input";
            ui->resultBrowser->setPlainText("Invaild input, please revise.\nPress del to clear input");
        }
    }
    return false;
}

void MainWindow::run()
{
    opType = RUN;
    runCode();
    opType = INPUT;
    ui->inputEdit->setFocus();
}

bool MainWindow::runCode()
{

}

void MainWindow::clearCode()
{
    code->clear();
    updateCodeBrowser();
    ui->resultBrowser->setPlainText("Code cleared");
    ui->inputEdit->setFocus();
}

void MainWindow::updateCodeBrowser()
{
    ui->codeBrowser->setPlainText(code->printCode());
}

void MainWindow::help()
{
    ui->resultBrowser->setPlainText("This is help...");
}

void MainWindow::quit()
{
    emit quitSignal();
}

void MainWindow::loadFile(QString &filename)
{
    switch(code->load(filename))
    {
        // normal case
        case 0:
            updateCodeBrowser();
            ui->resultBrowser->setPlainText("File loaded");
            qDebug() << "[FILE] file loaded";
            ui->inputEdit->clear();
            opType = INPUT;
            break;
        // unable to open
        case -1:
            ui->resultBrowser->setPlainText("Invaild file path, please revise.\nPress del to clear input");
            qDebug() << "[INVALID] invaild file path";
            break;
        // file contains errors
        case -2:
            ui->resultBrowser->setPlainText("File contains invaild line numbers.\nPress del to clear input");
            qDebug() << "[INVALID] invaild lineNum in file";
            break;
    }
}

void MainWindow::saveFile(QString &filename)
{
    if(code->save(filename))
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
}

void MainWindow::load()
{
    opType = LOAD;
    ui->resultBrowser->setPlainText("Please input file path.\nPress esc to exit loading mode");
    ui->inputEdit->setFocus();
}

void MainWindow::save()
{
    opType = SAVE;
    ui->resultBrowser->setPlainText("Please input file path.\nPress esc to exit saving mode");
    ui->inputEdit->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

