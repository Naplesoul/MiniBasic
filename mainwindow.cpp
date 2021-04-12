#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , code(new Code(this))
    , program(new Program(this))
    , status(INPUT)
{
    ui->setupUi(this);
    ui->inputEdit->setFocus();
    ui->inputEdit->installEventFilter(this);
    setWindowTitle("MiniBasic");
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::load);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::save);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clear);
    connect(ui->runButton, &QPushButton::clicked, this, &MainWindow::run);

    connect(program, &Program::printTree, this, &MainWindow::printTree);
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
//                 qDebug() << "[KEY] enter pressed";
                 handleInput();
                 return true;
             }
             if(k->key() == Qt::Key_Delete)
             {
//                 qDebug() << "[KEY] delete pressed";
                 ui->inputEdit->clear();
                 return true;
             }
//             if(k->key() == Qt::Key_Escape)
//             {
//                 qDebug() << "[KEY] esc pressed";
//                 if(opType == SAVE || opType == LOAD)
//                 {
//                     opType = INPUT;
//                     ui->resultBrowser->setPlainText("Exit saving/loading mode.\nPlease input command");
//                     return true;
//                 }
//                 return false;
//             }
        }
    }
    return false;
}

// handle cmd input during different states
void MainWindow::handleInput()
{
    QString in = ui->inputEdit->toPlainText();
    switch(status)
    {
        case WAIT_FOR_INPUT:
            programInput(in);
            break;
        case RUN:
            ui->resultBrowser->setText("[Running now, please wait...]");
            break;
        case CLEAR:
            ui->resultBrowser->setText("[Clearing now, please wait...]");
            break;
        case INPUT:
            if(input(in))
            {
                ui->codeBrowser->setPlainText(code->printCode());
//                qDebug() << "[VALID] input accepted\n[inputEdit] clear";
                ui->inputEdit->clear();
            }
            else
            {
//                qDebug() << "[INVAILD] invalid input";
                ui->resultBrowser->setPlainText("[Invaild input]\n[Press del to clear input]");
            }
            break;
        default:
            return;
    }
}

bool MainWindow::input(QString &input)
{
    input = input.trimmed();
    bool isNumber = true;
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
        if(input[i] > '9' || input[i] < '0')
            isNumber = false;
    }
    QString cmd = input.left(cmdEnd + 1);
    if(isNumber)
    {
        int lineNum = cmd.toInt();
        if(cmdEnd == len - 1)
        {
            if(code->del(lineNum) && lineNum <= 1000000 && lineNum >= 1)
            {
                QString message = "[Line" + input.mid(cmdEnd + 1) + " deleted]";
//                qDebug() << "[VAILD] valid delete";
                updateCodeBrowser();
                ui->resultBrowser->setPlainText(message);
                return true;
            }
            else
            {
//                qDebug() << "[INVAILD] invalid input";
//                ui->resultBrowser->setPlainText("Invaild input, please revise.\nPress del to clear input");
                return false;
            }
        }

        Line in(lineNum, input.mid(cmdEnd + 2));
//        qDebug() << "[insert]" << lineNum << ',' << in.code;
        if(code->insert(in) && lineNum <= 1000000 && lineNum >= 1)
        {
            QString message = "[Line " + cmd + " inserted]";
            ui->resultBrowser->setPlainText(message);
            return true;
        }
        else
        {
//            qDebug() << "[INVAILD] invalid input";
//            ui->resultBrowser->setPlainText("Invaild input, please revise.\nPress del to clear input");
            return false;
        }
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
        clear();
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
    if(cmd == "LIST")
    {
        updateCodeBrowser();
        return true;
    }
    if(cmd == "DEL")
    {
        int lineNum = input.mid(cmdEnd + 1).toInt();
        if(code->del(lineNum))
        {
            QString message = "[Line" + input.mid(cmdEnd + 1) + " deleted]";
//            qDebug() << "[VAILD] valid delete";
            updateCodeBrowser();
            ui->resultBrowser->setPlainText(message);
            return true;
        }
        else
        {
//            qDebug() << "[INVAILD] invalid input";
            return false;
        }
    }
    if(cmd == "LET" || cmd == "PRINT" || cmd == "INPUT")
    {
        isSingleCmd = true;
        runSingle(input);
        return true;
    }
    return false;
}

void MainWindow::run()
{
    status = RUN;
    ui->resultBrowser->clear();
    ui->treeBrowser->clear();
    inputOfProgram.clear();
    outputOfProgram.clear();
    try
    {
        program->parseStatements(code->getCode());
        program->initialize();
//        program->clearContext();
    }
    catch(QString err)
    {
        ui->resultBrowser->setPlainText(err);
        status = INPUT;
        ui->inputEdit->setFocus();
        return;
    }
    runCode();
}

void MainWindow::runCode()
{
    try
    {
        if(program->run(inputOfProgram, outputOfProgram))
        {
            outputOfProgram += "\n[Program executed successfully]";
            ui->resultBrowser->setPlainText(outputOfProgram);
            status = INPUT;
        }
        else
        {
            status = WAIT_FOR_INPUT;
            ui->resultBrowser->setPlainText("[Ask for input]");
        }
    }
    catch(QString err)
    {
        ui->resultBrowser->setPlainText(err);
        status = INPUT;
        ui->inputEdit->setFocus();
        return;
    }
    inputOfProgram.clear();
    outputOfProgram.clear();
    ui->inputEdit->setFocus();
}

void MainWindow::runSingle(QString &cmd)
{
    status = RUN;
    ui->resultBrowser->clear();
    ui->treeBrowser->clear();
    inputOfProgram.clear();
    outputOfProgram.clear();
    QList<Line> singleCmd;
    singleCmd.push_back(Line(1, cmd));
    try
    {
        program->parseStatements(singleCmd);
        program->initialize();
        runSingleCode();
    }
    catch(QString)
    {
        ui->resultBrowser->setPlainText("[Invalid Command]");
        status = INPUT;
        ui->treeBrowser->setPlainText("1 Error\n");
    }
    ui->inputEdit->setFocus();
}

void MainWindow::runSingleCode()
{
    try
    {
        if(program->run(inputOfProgram, outputOfProgram))
        {
            outputOfProgram += "\n[Command executed successfully]";
            ui->resultBrowser->setPlainText(outputOfProgram);
            status = INPUT;
            isSingleCmd = false;
        }
        else
        {
            status = WAIT_FOR_INPUT;
            ui->resultBrowser->setPlainText("[Ask for input]");
        }
    }
    catch(QString)
    {
        ui->resultBrowser->setPlainText("[Invalid Command]");
        status = INPUT;
        ui->treeBrowser->setPlainText("1 Error\n");
    }
    inputOfProgram.clear();
    outputOfProgram.clear();
    ui->inputEdit->setFocus();
}

void MainWindow::programInput(QString &input)
{
    inputOfProgram += " " + input;
    status = RUN;
    ui->inputEdit->clear();
    if(isSingleCmd)
        runSingleCode();
    else
        runCode();
}

void MainWindow::clearCode()
{
    code->clear();
    ui->treeBrowser->clear();
    updateCodeBrowser();
    ui->resultBrowser->setPlainText("[Code & context cleared]");
    ui->inputEdit->setFocus();
}

void MainWindow::updateCodeBrowser()
{
    ui->codeBrowser->setPlainText(code->printCode());
}

void MainWindow::help()
{
    ui->resultBrowser->setPlainText(helpText);
}

void MainWindow::quit()
{
    emit quitSignal();
}

void MainWindow::loadFile(const QString &filename)
{
    switch(code->load(filename))
    {
        // normal case
        case 0:
            updateCodeBrowser();
            ui->treeBrowser->clear();
            ui->resultBrowser->setPlainText("[File loaded]");
//            qDebug() << "[FILE] file loaded";
            ui->inputEdit->clear();
            break;
        // unable to open
        case -1:
            ui->resultBrowser->setPlainText("[Invaild file path]");
//            qDebug() << "[INVALID] invaild file path";
            break;
        // file contains errors
        case -2:
            ui->treeBrowser->clear();
            ui->resultBrowser->setPlainText("[File contains invaild line numbers]");
//            qDebug() << "[INVALID] invaild lineNum in file";
            break;
    }
}

void MainWindow::saveFile(const QString &filename)
{
    if(code->save(filename))
    {
        ui->resultBrowser->setPlainText("[File saved]");
//        qDebug() << "[FILE] file saved";
        ui->inputEdit->clear();
        status = INPUT;
    }
    else
    {
        ui->resultBrowser->setPlainText("[Invaild file path]");
//        qDebug() << "[INVALID] invaild file path";
    }
}

void MainWindow::load()
{
    status = LOAD;
//    ui->resultBrowser->setPlainText("Please input file path.\nPress esc to exit loading mode");
    QString filename = QFileDialog::getOpenFileName(this, "choose a file to open");
    loadFile(filename);
    status = INPUT;
    ui->inputEdit->setFocus();
}

void MainWindow::save()
{
    status = SAVE;
//    ui->resultBrowser->setPlainText("Please input file path.\nPress esc to exit saving mode");
    QString filename = QFileDialog::getSaveFileName(this, "choose a file to save");
    saveFile(filename);
    status = INPUT;
    ui->inputEdit->setFocus();
}

void MainWindow::printTree(QString output)
{
    ui->treeBrowser->append(output);
}

void MainWindow::clear()
{
    status = CLEAR;
    clearCode();
    program->clearContext();
    status = INPUT;
}

MainWindow::~MainWindow()
{
    delete ui;
}

