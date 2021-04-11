#include "program.h"

Program::Program(QObject *parent) : QObject(parent)
{
    setParent(parent);
}

bool Program::parseStatements(const QList<Line> &code)
{
    QList<Line> codes = code;
    statements.clear();
    QString fun;
    QString content;
    for(auto it = codes.begin(); it != codes.end(); ++it)
    {
        QString tree = "";
        int len = (*it).code.size();
        int funEnd = -1;
        for(int i = 0; i < len; ++i)
        {
            if((*it).code[i] == ' ')
            {
                funEnd = i - 1;
                break;
            }
            if(i == len - 1)
                funEnd = i;
        }
        if(funEnd >= 0)
        {
            fun = (*it).code.left(funEnd + 1);
            content = (*it).code.mid(funEnd + 2);

            if(fun == "REM")
            {
//                statements.push_back(RemStmt((*it).lineNum));
                tree += QString::number((*it).lineNum) + " REM\n";
                tree += "\t" + (*it).code;
                emit printTree(tree);
                continue;
            }
            else if(fun == "LET")
            {
                LetStmt* newStatement = new LetStmt((*it).lineNum);
                try
                {
                    newStatement->parse(content.trimmed());
                } catch (QString)
                {
                    newStatement->isValid = false;
                    statements.push_back(newStatement);
                    emit printTree(QString::number((*it).lineNum) + " Error\n");
                    continue;
                }
                statements.push_back(newStatement);
                emit printTree(newStatement->printTree());
                continue;
            }
            else if(fun == "PRINT")
            {
                PrintStmt* newStatement = new PrintStmt((*it).lineNum);
                try
                {
                    newStatement->parse(content.trimmed());
                } catch (QString)
                {
                    newStatement->isValid = false;
                    statements.push_back(newStatement);
                    emit printTree(QString::number((*it).lineNum) + " Error\n");
                    continue;
                }
                statements.push_back(newStatement);
                emit printTree(newStatement->printTree());
                continue;
            }
            else if(fun == "INPUT")
            {
                InputStmt* newStatement = new InputStmt((*it).lineNum);
                try
                {
                    newStatement->parse(content.trimmed());
                } catch (QString)
                {
                    newStatement->isValid = false;
                    statements.push_back(newStatement);
                    emit printTree(QString::number((*it).lineNum) + " Error\n");
                    continue;
                }
                statements.push_back(newStatement);
                emit printTree(newStatement->printTree());
                continue;
            }
            else if(fun == "GOTO")
            {
                GotoStmt* newStatement = new GotoStmt((*it).lineNum);
                try
                {
                    newStatement->parse(content.trimmed());
                } catch (QString)
                {
                    newStatement->isValid = false;
                    statements.push_back(newStatement);
                    emit printTree(QString::number((*it).lineNum) + " Error\n");
                    continue;
                }
                statements.push_back(newStatement);
                emit printTree(newStatement->printTree());
                continue;
            }
            else if(fun == "IF")
            {
                IfStmt* newStatement = new IfStmt((*it).lineNum);
                try
                {
                    newStatement->parse(content.trimmed());
                } catch (QString)
                {
                    newStatement->isValid = false;
                    statements.push_back(newStatement);
                    emit printTree(QString::number((*it).lineNum) + " Error\n");
                    continue;
                }
                statements.push_back(newStatement);
                emit printTree(newStatement->printTree());
                continue;
            }
            else if(fun == "END")
            {
                statements.push_back(new EndStmt((*it).lineNum));
                tree += QString::number((*it).lineNum) + " END\n";
                emit printTree(tree);
                continue;
            }
        }
//        throw int(1);
//        throw QString("Syntax error in Line ") + QString::number((*it).lineNum);
        EndStmt* newStatement = new EndStmt((*it).lineNum);
        newStatement->isValid = false;
        statements.push_back(newStatement);
        emit printTree(QString::number((*it).lineNum) + " Error\n");
    }
    return true;
}

bool Program::run(QString &input, QString &output)
{
    QList<Statement*>::iterator it = statements.begin();
    for(; it != statements.end(); ++it)
    {
        if((*it)->lineNum == pc)
            break;
    }
    while(it != statements.end())
    {
        if(!(*it)->isValid)
            throw QString("[Syntax Error in Line " + QString::number((*it)->lineNum) + "]");
        int oriPc = pc;
        if((*it)->type == ENDSTMT)
            return true;

        // ask for input
        if(!(*it)->run(evaluationContext, pc, input, output))
            return false;
        if(pc == oriPc)
        {
            ++it;
            if(it != statements.end())
                pc = (*it)->lineNum;
            continue;
        }
        else
        {
            // move it to target pc
            for(it = statements.begin(); it != statements.end(); ++it)
            {
                if((*it)->lineNum == pc)
                    break;
            }
            if(it == statements.end())
                throw QString("Target line does not exist");
        }
    }
    return true;
}

void Program::initialize()
{
    if(statements.empty())
        throw QString("No code exist");
//    evaluationContext.clear();
    pc = statements.first()->lineNum;
}

void Program::clearContext()
{
    evaluationContext.clear();
}
