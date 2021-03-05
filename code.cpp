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


void CodeBuffer::showLines(int from, int to)
{
    if(from > to)
    {
        throw "Number range error";
        return;
    }
    if(from < 1 || to > int(codeList.size()))
    {
        throw "Line number out of range";
        return;
    }
    QList<QString>::const_iterator it = codeList.begin();
    int linenum = from;
    while(from > 1)
    {
        if(it != codeList.end()) ++it;
        --from;
    }

    while(linenum <= to)
    {
        qDebug() << linenum << '\t' << *it << '\n';
        if(it != codeList.end()) ++it;
        ++linenum;
    }
    currentLineNum = to;
}

void CodeBuffer::deleteLines(int from, int to)
{
    if(from > to)
    {
        throw "Delete range error";
        return;
    }
    if(from < 1 || to > int(codeList.size()))
    {
        throw "Line number out of range";
        return;
    }
    QList<QString>::iterator it = codeList.begin();
    int truefrom = from;
    int linenum = from;
    bool islast = false;
    auto dit = it;
    while(from > 1)
    {
        if(it != codeList.end()) ++it;
        --from;
    }
    while(linenum <= to)
    {
        dit = it;
        if(it != codeList.end()) ++it;
        else islast = true;
        codeList.erase(dit);
        ++linenum;
    }
    if(islast) currentLineNum = truefrom - 1;
    else currentLineNum = truefrom;
    if(codeList.empty()) currentLineNum = 0;
}

void CodeBuffer::insertLine(const QString &text)
{
    if(currentLineNum == 0)
    {
        codeList.push_back(text);
        currentLineNum = 1;
        return;
    }
    int cln = currentLineNum;
    QList<QString>::iterator it = codeList.begin();
    while(cln > 1)
    {
        if(it != codeList.end()) ++it;
        --cln;
    }
    codeList.insert(it,text);
}

void CodeBuffer::appendLine(const QString &text)
{
    if(currentLineNum == 0)
    {
        codeList.push_back(text);
        currentLineNum = 1;
        return;
    }
    int cln = currentLineNum;
    QList<QString>::iterator it = codeList.begin();
    while(cln > 1)
    {
        if(it != codeList.end()) ++it;
        --cln;
    }
    if(it != codeList.end()) ++it;
    codeList.insert(it,text);
    currentLineNum++;
}

const QString &CodeBuffer::moveToLine(int idx)
{
    if(idx < 1 || idx > int(codeList.size()))
    {
        throw "Line number out of range";
    }
    currentLineNum = idx;
    int cln = currentLineNum;
    QList<QString>::const_iterator it = codeList.begin();
    while(cln > 1)
    {
        if(it != codeList.end()) ++it;
        --cln;
    }
    return *it;
}



Code::Code(QObject *parent) : QObject(parent)
{
    setParent(parent);
    codeBuffer = new CodeBuffer;
}

void Code::receiveInput(QString &input)
{

}
