#include "code.h"

#include <fstream>
#include <iostream>
#include <QDebug>


CodeBuffer::CodeBuffer() {}

CodeBuffer::~CodeBuffer() {}

int CodeBuffer::size()
{
    return codeList.size();
}


bool CodeBuffer::insertLine(const Line &input)
{
    int lineNum = input.lineNum;
    QList<Line>::iterator it = codeList.begin();
    while(it != codeList.end() && (*it).lineNum < lineNum)
        ++it;
    if(it != codeList.end() && (*it).lineNum == lineNum)
        return false;
    codeList.insert(it, input);
    return true;
}

bool CodeBuffer::deleteLine(int lineNum)
{
    for(QList<Line>::iterator it = codeList.begin(); it != codeList.end(); ++it)
    {
        if((*it).lineNum == lineNum)
        {
            codeList.erase(it);
            return true;
        }
    }
    return false;
}

QString CodeBuffer::printCode()
{
    QString output = "";
    for(QList<Line>::iterator it = codeList.begin(); it != codeList.end(); ++it)
    {
        output += QString::number((*it).lineNum);
        output += " ";
        output += (*it).code;
        output += "\n";
    }
    return output;
}

void CodeBuffer::appendLine(const QString &text)
{
//    if(currentLineNum == 0)
//    {
//        codeList.push_back(text);
//        currentLineNum = 1;
//        return;
//    }
//    int cln = currentLineNum;
//    QList<QString>::iterator it = codeList.begin();
//    while(cln > 1)
//    {
//        if(it != codeList.end()) ++it;
//        --cln;
//    }
//    if(it != codeList.end()) ++it;
//    codeList.insert(it,text);
//    currentLineNum++;
}



void CodeBuffer::clear()
{
    codeList.clear();
}


Code::Code(QObject *parent) : QObject(parent)
{
    setParent(parent);
    codeBuffer = new CodeBuffer;
}

bool Code::insert(Line &input)
{
    return codeBuffer->insertLine(input);
}

bool Code::del(int lineNum)
{
    return codeBuffer->deleteLine(lineNum);
}

QString Code::printCode()
{
    return codeBuffer->printCode();
}

bool Code::load(const QString &filename)
{

}

bool Code::save(const QString &filename) const
{

}

void Code::clear()
{
    codeBuffer->clear();
}
