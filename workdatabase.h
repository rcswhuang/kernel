#ifndef HWORKDATABASE_H
#define HWORKDATABASE_H

#include <QObject>

class HWorkDataBase : public QObject
{
    Q_OBJECT
public:
    explicit HWorkDataBase(QObject *parent = 0);

signals:

public slots:
};

#endif // HWORKDATABASE_H
