#ifndef HPOINTTERM_H
#define HPOINTTERM_H

#include <QObject>
#include "publicdata.h"
class HPointTerm : public QObject
{
    Q_OBJECT
public:
    explicit HPointTerm(QObject *parent = 0);
    ~HPointTerm();

public:
    POINTTERM* getPointTerm();
    quint16 pointTermID();
    QString pointTermName();
    uchar   pointTermType();

public:
    POINTTERM pointTerm;
};

#endif // HPOINTTERM_H
