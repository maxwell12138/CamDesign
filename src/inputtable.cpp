#include "InputTable.h"
#include <QAction>
#include <queue>
#include <QTableWidget>
#include <QMessageBox>
#include <QHeaderView>
#include <QCheckBox>
#include "def.h"
#include "rpn.h"
using namespace std;
InputTable::InputTable(QWidget *parent):QTableWidget(parent)
{
    tableInitial();
    popMenuInitial();
}

InputTable::~InputTable()
{
    delete popMenu;
}

//show popmenu at cursor
void InputTable::contextMenuEvent(QContextMenuEvent *)
{
    popMenu->move(cursor().pos());
    popMenu->show();
}

double *InputTable::distanceCalculation(const int pointNum){

    CAM::inputError error;
    CAM::distanceExpression *expression=this->tableRead(error);
    if(expression==NULL){
        QMessageBox::critical(this, tr("Distance Input Error"),errorName(error),QMessageBox::Yes ,QMessageBox::Yes);

        return NULL;
    }
    double *distance=new double[pointNum];
    for(int i=0;i<pointNum;i++){
        distance[i]=0.0;
    }
    double resolution=360.0/pointNum;
    double *a=distance;
    std::queue<CAM::member> temp;
    double x=0.0;
    double baseY=0.0;
    double baseX=0.0;

    for(int i=0;i<this->rowCount();i++){
        int startIndex=expression[i]._start*pointNum/360;
        int endIndex=expression[i]._end*pointNum/360;

        x=expression[i]._start;
        temp=rpn(expression[i]._expression);
        for(int j=startIndex;j<endIndex;j++){
            a=distance+j;//alias pointer of distance
            double num=calculate(temp,(x-baseX));
            *a=baseY+num;//assignment for distance
            x+=resolution;
        }

        if(expression[i+1]._absolute){//(i+1)<=(this->rowCount()-1)&&
            baseX=0.0;
            baseY=0.0;
        }else{
            baseY=calculate(temp,expression[i]._end-baseX)+baseY;
            baseX=expression[i]._end;
            //baseY=distance[endIndex-1];
        }
    }
    delete expression;
    return distance;
}


CAM::distanceExpression *InputTable::tableRead(CAM::inputError &error)
{
    for(int i=0;i<this->rowCount();i++){
        for(int j=1;j<this->columnCount();j++){
            //if item is empty,return a null pointer and a error
            if(this->item(i,j)->text().isEmpty()){
                error._error=CAM::emptyInput;
                return NULL;
            }
        }
    }
    const int num=this->rowCount();
    CAM::distanceExpression *expression=new CAM::distanceExpression[num];
    for(int i=0;i<num;i++){
        expression[i]._absolute=dynamic_cast<QCheckBox*>(this->cellWidget(i,0))->isChecked();
        expression[i]._start=str2double(this->item(i,1)->text());
        expression[i]._end=str2double(this->item(i,2)->text());
        expression[i]._expression=this->item(i,3)->text().toStdString();

        if(expression[i]._start>360||expression[i]._start<0){
            error._error=CAM::outRange;
            this->editItem(this->item(i,1));
            return NULL;
        }else if(expression[i]._end>360||expression[i]._end<0){
            error._error=CAM::outRange;
            this->editItem(this->item(i,2));
            return NULL;
        }
        if(expression[i]._start>=expression[i]._end){
            error._error=CAM::falseInterval;
            this->editItem(this->item(i,1));
            return NULL;
        }
        if(i>0){
            if(expression[i]._start<expression[i-1]._end){
                error._error=CAM::intervalOverlap;
                this->editItem(this->item(i,1));
                return NULL;
            }
        }
    }
    return expression;
}

/*
inline std::string InputTable::readAt(int row, int column)
{
    return this->item(row,column)->text().toStdString();
}
*/


void InputTable::tableInitial()
{
    this->setRowCount(7);
    this->setColumnCount(4);
    QStringList header;

    header<<"absolute"<<"from"<<"to"<<"expression";
    this->horizontalHeader()->resizeSection(0,60);
    this->horizontalHeader()->resizeSection(1,50);
    this->horizontalHeader()->resizeSection(2,50);
    this->horizontalHeader()->setStretchLastSection(true);
    this->setHorizontalHeaderLabels(header);


    this->setCellWidget(0,0,new QCheckBox(this));
    this->setItem(0,1,new QTableWidgetItem("0"));
    this->setItem(0,2,new QTableWidgetItem("30"));
    this->setItem(0,3,new QTableWidgetItem("10-10*cos(x)"));//("sin(x)"));


    this->setCellWidget(1,0,new QCheckBox(this));
    this->setItem(1,1,new QTableWidgetItem("30"));
    this->setItem(1,2,new QTableWidgetItem("90"));
    this->setItem(1,3,new QTableWidgetItem("x/12"));

    this->setCellWidget(2,0,new QCheckBox(this));
    this->setItem(2,1,new QTableWidgetItem("90"));
    this->setItem(2,2,new QTableWidgetItem("120"));
    this->setItem(2,3,new QTableWidgetItem("10*cos(x-30)-sqrt(3)*5"));


    this->setCellWidget(3,0,new QCheckBox(this));
    this->setItem(3,1,new QTableWidgetItem("120"));
    this->setItem(3,2,new QTableWidgetItem("240"));
    this->setItem(3,3,new QTableWidgetItem("0"));

    this->setCellWidget(4,0,new QCheckBox(this));
    this->setItem(4,1,new QTableWidgetItem("240"));
    this->setItem(4,2,new QTableWidgetItem("270"));
    this->setItem(4,3,new QTableWidgetItem("10*cos(x)-10"));

    this->setCellWidget(5,0,new QCheckBox(this));
    this->setItem(5,1,new QTableWidgetItem("270"));
    this->setItem(5,2,new QTableWidgetItem("330"));
    this->setItem(5,3,new QTableWidgetItem("~x/12"));

    this->setCellWidget(6,0,new QCheckBox(this));
    this->setItem(6,1,new QTableWidgetItem("330"));
    this->setItem(6,2,new QTableWidgetItem("360"));
    this->setItem(6,3,new QTableWidgetItem("10*sqrt(0.75)-10*cos(x-30)"));
    for(int i=0;i<this->rowCount();i++){
        for(int j=1;j<this->columnCount();j++){
            this->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void InputTable::popMenuInitial()
{
    popMenu=new QMenu(this);
    QAction *actionAdd=new QAction("add",popMenu);
    QAction *actionDelete=new QAction("delete",popMenu);
    popMenu->addAction(actionAdd);
    popMenu->addAction(actionDelete);
    QAction *actionDeleteThisRow=new QAction("deleteThis",popMenu);
    popMenu->addAction(actionDeleteThisRow);
    QAction *actionInsertUpRow=new QAction("insertUp",popMenu);
    popMenu->addAction(actionInsertUpRow);
    QAction *actionInsertDownRow=new QAction("insertDown",popMenu);
    popMenu->addAction(actionInsertDownRow);
    connect(actionAdd,SIGNAL(triggered()),this,SLOT(addRow()));
    connect(actionDelete,SIGNAL(triggered()),this,SLOT(deleteRow()));
    connect(actionDeleteThisRow,SIGNAL(triggered()),this,SLOT(deleteThisRow()));
    connect(actionInsertUpRow,SIGNAL(triggered()),this,SLOT(insertUpRow()));
    connect(actionInsertDownRow,SIGNAL(triggered()),this,SLOT(insertDownRow()));
    actionAdd=NULL;
    actionDelete=NULL;
    actionDeleteThisRow=NULL;
    actionInsertUpRow=NULL;
    actionInsertDownRow=NULL;
    //delete actionAdd;
    //delete actionDelete;
    //delete actionDeleteThisRow;
    //delete actionInsertUpRow;
    //delete actionInsertDownRow;
}

void InputTable::addRow()
{
    int row = this->rowCount();
    this->setRowCount(row+1);
    this->setCellWidget(row,0,new QCheckBox(this));
}

void InputTable::deleteRow()
{
    int row = this->rowCount();
    if(row<=0) return;
    this->setRowCount(row-1);
}

void InputTable::deleteThisRow()
{
    int rowIndex = this->currentRow();
    this->removeRow(rowIndex);
}

void InputTable::insertUpRow()
{
    int rowIndex = this->currentRow();
    this->insertRow(rowIndex);
    this->setCellWidget(rowIndex,0,new QCheckBox(this));
}

void InputTable::insertDownRow()
{
    int rowIndex = this->currentRow();
    this->insertRow(rowIndex+1);
    this->setCellWidget(rowIndex+1,0,new QCheckBox(this));
}

