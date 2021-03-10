#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(QObject *parent = nullptr);

signals:

};

#endif // PROGRAM_H
