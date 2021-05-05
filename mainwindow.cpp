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
    connect(ui->debugButton, &QPushButton::clicked, this, &MainWindow::debug);
    connect(ui->stepButton, &QPushButton::clicked, this, &MainWindow::step);

    ui->stepButton->setEnabled(false);

    connect(program, &Program::printTree, this, &MainWindow::printTree);
}

// filter shortcut keys: ctrl+L: LOAD, ctrl+S: SAVE, Ctrl+R: RUN, Ctrl+Del: Delete
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
                    case (Qt::Key_Delete):
                        ui->inputEdit->clear();
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
            if (isDebugMode) {
                ui->resultBrowser->append("[Debugging now, please wait...]");
                break;
            }
            if(input(in))
            {
//                ui->codeBrowser->setPlainText(code->printCode());
                ui->inputEdit->clear();
            }
            else
                ui->resultBrowser->setPlainText("[Invaild input]\n[Press Ctrl + Del to clear input]");
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
// finding the end of line number or commands
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
// get the line number or command
    QString cmd = input.left(cmdEnd + 1);
    if(isNumber)
    {
        int lineNum = cmd.toInt();
        // if the line only contains a number
        if(cmdEnd == len - 1)
        {
            if(code->del(lineNum) && lineNum <= 1000000 && lineNum >= 1)
            {
                QString message = "[Line" + input.mid(cmdEnd + 1) + " deleted]";
                updateCodeBrowser();
                ui->resultBrowser->setPlainText(message);
                return true;
            }
            else
                return false;
        }

        Line in(lineNum, input.mid(cmdEnd + 2));
        if(code->insert(in) && lineNum <= 1000000 && lineNum >= 1)
        {
            QString message = "[Line " + cmd + " inserted]";
            updateCodeBrowser();
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
        int lineNum = input.midRef(cmdEnd + 1).toInt();
        if(code->del(lineNum))
        {
            QString message = "[Line" + input.mid(cmdEnd + 1) + " deleted]";
            updateCodeBrowser();
            ui->resultBrowser->setPlainText(message);
            return true;
        }
        else
            return false;
    }
    // handle single commands that would be executed immediately
    if(cmd == "LET" || cmd == "PRINT" || cmd == "INPUT" || cmd == "PRINTF" || cmd == "INPUTS")
    {
        isSingleCmd = true;
        runSingle(input);
        return true;
    }
    return false;
}

void MainWindow::debug()
{
    isDebugMode = true;
    ui->debugButton->setEnabled(false);
    ui->stepButton->setEnabled(true);
    status = RUN;
    ui->resultBrowser->clear();
    ui->treeBrowser->clear();
    inputOfProgram.clear();
    outputOfProgram.clear();
    try
    {
        program->parseStatements(code->getCode());
        ui->treeBrowser->clear();
        program->initialize();
        highLightWrongAndNext();
        printNextTree();
    }
    catch(QString err)
    {
        ui->resultBrowser->setPlainText(err);
        status = INPUT;
        ui->inputEdit->setFocus();
        return;
    }
    status = INPUT;
}

void MainWindow::step()
{
    try {
        switch (program->step(inputOfProgram, outputOfProgram)) {
            case 0:
                highLightWrong();
                outputOfProgram += "\n[Program executed successfully]";
                ui->resultBrowser->setPlainText(outputOfProgram);
                updateContextBrowser();
                outputOfProgram.clear();
                status = INPUT;
                isDebugMode = false;
                ui->debugButton->setEnabled(true);
                ui->stepButton->setEnabled(false);
                return;
            case 1:
                updateContextBrowser();
                ui->resultBrowser->setPlainText(outputOfProgram);
                highLightWrongAndNext();
                printNextTree();
                break;
            case -1:
                status = WAIT_FOR_INPUT;
                ui->resultBrowser->setPlainText("[Ask for input]");
                ui->inputEdit->setFocus();
                break;

        }
    }  catch (QString err) {
        highLightWrong();
        updateContextBrowser();
        ui->resultBrowser->setPlainText(err);
        status = INPUT;
        isDebugMode = false;
        ui->debugButton->setEnabled(true);
        ui->stepButton->setEnabled(false);
        ui->inputEdit->setFocus();
        inputOfProgram.clear();
        outputOfProgram.clear();
        return;
    }
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
        highLightWrong();
        program->initialize();
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
        // program->run() return false if needs input, throw QString if contains errors
        if(program->run(inputOfProgram, outputOfProgram))
        {
            outputOfProgram += "\n[Program executed successfully]";
            ui->resultBrowser->setPlainText(outputOfProgram);
            outputOfProgram.clear();
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
    updateContextBrowser();
    inputOfProgram.clear();
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
        highLightWrong();
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
    updateContextBrowser();
    inputOfProgram.clear();
    outputOfProgram.clear();
    ui->inputEdit->setFocus();
}

void MainWindow::programInput(QString &input)
{
    inputOfProgram += " " + input;
    status = RUN;
    ui->inputEdit->clear();
    if (isDebugMode)
        step();
    else if (isSingleCmd)
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
    ui->codeBrowser->setText(code->printCode());
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
            ui->inputEdit->clear();
            break;
        // unable to open
        case -1:
            ui->resultBrowser->setPlainText("[Invaild file path]");
            break;
        // file contains errors
        case -2:
            ui->treeBrowser->clear();
            ui->resultBrowser->setPlainText("[File contains invaild line numbers]");
            break;
    }
}

void MainWindow::saveFile(const QString &filename)
{
    if(code->save(filename))
    {
        ui->resultBrowser->setPlainText("[File saved]");
        ui->inputEdit->clear();
        status = INPUT;
    }
    else
        ui->resultBrowser->setPlainText("[Invaild file path]");
}

void MainWindow::load()
{
    status = LOAD;
    QString filename = QFileDialog::getOpenFileName(this, "choose a file to open");
    loadFile(filename);
    status = INPUT;
    ui->inputEdit->setFocus();
}

void MainWindow::save()
{
    status = SAVE;
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
    updateContextBrowser();
    status = INPUT;
}

void MainWindow::highLightWrong()
{
    QStringList codeString = code->getStringLines();
    QList<bool> validity = program->getValidity();
    QString text;
    auto validityIt = validity.begin();
    for (auto codeIt = codeString.begin(); codeIt != codeString.end(); ++codeIt) {
        if (*validityIt)
            text += "<p style=\"line-height:0.59\">" + *codeIt + "</p>\n";
        else
            text += "<p style=\"color:#ff2525;line-height:0.59\">" + *codeIt + "</p>\n";
        ++validityIt;
    }
    ui->codeBrowser->setHtml(text);
}

void MainWindow::highLightWrongAndNext()
{
    int nextPos = program->getNextPos();
    QStringList codeString = code->getStringLines();
    QList<bool> validity = program->getValidity();
    QString text;
    int i = 0;
    auto validityIt = validity.begin();
    for (auto codeIt = codeString.begin(); codeIt != codeString.end(); ++codeIt) {
        if (*validityIt) {
            if (i == nextPos)
                text += "<p style=\"background:#64ff64;line-height:0.59\">" + *codeIt + "</p>\n";
            else
                text += "<p style=\"line-height:0.59\">" + *codeIt + "</p>\n";
        }
        else {
            if (i == nextPos)
                text += "<p style=\"background:#64ff64;color:#ff2525;line-height:0.59\">" + *codeIt + "</p>\n";
            else
                text += "<p style=\"color:#ff2525;line-height:0.59\">" + *codeIt + "</p>\n";
        }
        ++validityIt;
        ++i;
    }
    ui->codeBrowser->setHtml(text);
}

void MainWindow::updateContextBrowser()
{
    ui->contextBrowser->setText(program->printContext());
}

void MainWindow::printNextTree()
{
    ui->treeBrowser->clear();
    program->printNextTree();
}

MainWindow::~MainWindow()
{
    delete ui;
}

