#ifndef MYTABLE_H
#define MYTABLE_H
#include <QTableWidget>
#include <QMenu>
#include "basiccam.h"
class InputTable : public QTableWidget
{
    Q_OBJECT

public:
    InputTable(QWidget *parent = 0);
    virtual ~InputTable();
    void contextMenuEvent(QContextMenuEvent *);
    CAM::distanceExpression *tableRead(CAM::inputError &error);
    double *distanceCalculation(const int pointNum);//CAM::distanceExpression *expression,

private:
    QMenu *popMenu;
    void tableInitial();
    void popMenuInitial();
private slots:
    void addRow();
    void deleteRow();
    void deleteThisRow();
    void insertUpRow();
    void insertDownRow();

};

#endif // MYTABLE_H
