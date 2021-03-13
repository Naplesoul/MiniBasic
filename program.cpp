#include "program.h"

Program::Program(QObject *parent) : QObject(parent)
{
    setParent(parent);
}

bool Program::parseStatements(const QList<Line> &code)
{
    QString fun;
    QString content;
    for(auto it = code.begin(); it != code.end(); ++it)
    {
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
                statements.push_back(RemStmt((*it).lineNum));
                continue;
            }
            else if(fun == "LET")
            {
                LetStmt newStatement((*it).lineNum);
                newStatement.parse(content);
                statements.push_back(newStatement);
                continue;
            }
            else if(fun == "PRINT")
            {
                PrintStmt newStatement((*it).lineNum);
                newStatement.parse(content);
                statements.push_back(newStatement);
                continue;
            }
            else if(fun == "INPUT")
            {
                InputStmt newStatement((*it).lineNum);
                newStatement.parse(content);
                statements.push_back(newStatement);
                continue;
            }
            else if(fun == "GOTO")
            {
                GotoStmt newStatement((*it).lineNum);
                newStatement.parse(content);
                statements.push_back(newStatement);
                continue;
            }
            else if(fun == "IF")
            {
                IfStmt newStatement((*it).lineNum);
                newStatement.parse(content);
                statements.push_back(newStatement);
                continue;
            }
            else if(fun == "END")
            {
                statements.push_back(EndStmt((*it).lineNum));
                continue;
            }
        }
        throw QString("Syntax error in Line ") + QString::number((*it).lineNum);
    }
    return true;
}
