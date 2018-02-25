#include "dialog.h"
#include "ui_dialog.h"
#include <QPainter>
#include "rpn.h"
#include <QMouseEvent>

Dialog::Dialog(BasicCam *cam, QWidget *parent) :
    QDialog(parent),cam(cam),ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->reset();
    this->basicScale=1.0;
    int num=cam->getPointNum();

    QPointF *data=new QPointF[num]();
    const double *d1=cam->getInfo(CAM::distance);
    for(int i=0;i<num;i++){
        data[i].setX(i*360.0/num);
        data[i].setY(d1[i]);
    }
    ui->distanceWidget->resize(300,180);
    ui->distanceWidget->autoMatch();
    ui->distanceWidget->setTracer(true);
    ui->distanceWidget->setInteractive(true);
    ui->distanceWidget->setTitle(QString("Distance"));
    ui->distanceWidget->inputCurve(data,num);

    QPointF *angle=new QPointF[num]();
    const double *d2=cam->getInfo(CAM::angle);
    for(int i=0;i<num;i++){
        angle[i].setX(i*360.0/num);
        angle[i].setY(d2[i]);
    }
    ui->angleWidget->resize(300,180);
    ui->angleWidget->autoMatch();
    ui->angleWidget->setTracer(true);
    ui->angleWidget->setInteractive(true);
    ui->angleWidget->setTitle(QString("Angle"));
    ui->angleWidget->inputCurve(angle,num);

    this->show();
    timerFlag=startTimer(20);
    ui->horizontalSlider->setRange(0,360);

}


Dialog::~Dialog()
{
    delete ui;
    if(timerFlag!=0) killTimer(timerFlag);
}

bool Dialog::isPause()
{
    if(timerFlag==0){
        return true;
    }else{
        return false;
    }
}



void Dialog::paintEvent(QPaintEvent *event)
{
    cam->displayInDialog(this,displayCenter,scale*basicScale);
    ui->distanceWidget->setTraceIndex(cam->getCurrretIndex());
    ui->angleWidget->setTraceIndex(cam->getCurrretIndex());//(cam->getCurrretIndex());
    if(timerFlag!=0) cam->increaseCurrentIndex();
}

void Dialog::timerEvent(QTimerEvent *)
{
    this->repaint();//draw cam and follower
}

void Dialog::setAngle(int index)
{
    cam->setCurrentIndex(index*2);
    repaint();
}

void Dialog::reset()
{
    cam->getScaleAndCenter(QSizeF(this->width()*2/3,this->height()),scale,displayCenter);
    repaint();
}

void Dialog::pause()
{
    if(timerFlag==0){
        timerFlag=startTimer(20);
    }else{
        killTimer(timerFlag);
        timerFlag=0;
    }
}


void Dialog::mousePressEvent(QMouseEvent* event)
{
    pressPoint=event->pos();
}

void Dialog::mouseReleaseEvent(QMouseEvent* event)
{

}

void Dialog::mouseDoubleClickEvent(QMouseEvent* event)
{

}

void Dialog::mouseMoveEvent(QMouseEvent* event)
{
    this->displayCenter=this->displayCenter+event->pos()-pressPoint;
    pressPoint=event->pos();
    if(timerFlag==0) repaint();
}

void Dialog::wheelEvent(QWheelEvent * event)
{
    double delta=event->delta()/1000.0;
    if(this->basicScale+delta<=0)    return;
    this->basicScale+=delta;
    if(timerFlag==0) repaint();
}
