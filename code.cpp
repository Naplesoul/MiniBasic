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
    {
        (*it).code = input.code;
        return true;
    }
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

// returns 0: normal case
// -1: unable to open
// -2: file contains errors
int Code::load(const QString &filename)
{
    codeBuffer->clear();
    std::fstream file(filename.toStdString());
    if(!file)
        return -1;
    std::string line;
    while(std::getline(file, line))
    {
        if(line.empty())
            continue;
        QString Qline = QString::fromStdString(line);
        bool isNumber = false;
        int numEnd = -1;
        int len = Qline.size();
        for(int i = 0; i < len; ++i)
        {
            if(i == len - 1)
            {
                numEnd = i;
                break;
            }
            if(Qline[i] == ' ')
            {
                numEnd = i - 1;
                break;
            }
            if(Qline[i] <= '9' && Qline[i] >= '0')
                isNumber = true;
            else
                isNumber = false;
        }
        QString num = Qline.left(numEnd + 1);
        if(isNumber)
        {
            if(numEnd == len - 1)
                return -2;
            int lineNum = num.toInt();
            Line in(lineNum, Qline.mid(numEnd + 2));
            qDebug() << "[insert]" << lineNum << ',' << in.code;
            if(!insert(in))
                return -2;
        }
    }
    file.close();
    return 0;
}

bool Code::save(const QString &filename)
{
    std::ofstream file(filename.toStdString());
    if(!file)
        return false;
    file << printCode().toStdString();
    file.close();
    return true;
}

void Code::clear()
{
    codeBuffer->clear();
}
