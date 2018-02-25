#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "platemovecam.h"
#include "basiccam.h"
#include "rpn.h"
#include <QSettings>
#include <QMessageBox>
#include "def.h"
using namespace CAM;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    data=0;
    myDialog=0;
    ui->setupUi(this);
    tab1Initial();
    ui->widget->resize(740,170);
    ui->widget->autoMatch();
    this->setWindowIcon(QIcon("/new/prefix1/cam.ico"));
}

MainWindow::~MainWindow()
{
    if(data)      delete data;
    if(myDialog)  delete myDialog;
    delete ui;
}

void MainWindow::inputOkButton()
{
    double followerR=0.0;
    double basicR=0.0;
    double e=0.0;


    inputError error;
    int layout=PMC::null;

    if(!this->inputCheck(error,layout,followerR,basicR,e)){
        QMessageBox::critical(this, tr("Parameter Input Error"),errorName(error),
                                       QMessageBox::Yes,QMessageBox::Yes);
        return;

    }else{
        if(data)    delete data;
        this->data=new Data(pointNum,plateMoveCam);

        BasicCam *basic=data->getCam();
        PlateMoveCam *cam=dynamic_cast<PlateMoveCam *>(basic);
        cam->setFollowerRadius(followerR);
        cam->setBasicRadius(basicR);
        cam->setOffset(e);

        cam->setTypeAttr(layout);
        cam->basicDistanceCal();
        cam->setDistance(ui->tableWidget->distanceCalculation(pointNum));
        cam->centralCurveCal();
        cam->contourCurveCal();
        cam->centerPathCal(1.0);
        cam->contourPathCal(1.0);
        cam->angleCal();
    }
    if(plotDistanceDiagramm()){
        myDialog=new Dialog(this->data->getCam(),this);
    }

}

void MainWindow::showFollowerRadius(QString fshape)
{
    if(0==fshape.compare(QString("roller"))){
        //radius lineEdit show
        ui->PMCfollowerRadiusEdit->setVisible(true);
        ui->PMCfollowerRadiusLabel->setVisible(true);
        ui->PMCfollowerRadiusEdit->clear();
        //layout comboBox show
        ui->PMCfollowerLayoutComboBox->setVisible(true);
        ui->PMCfollowerLayoutLabel->setVisible(true);
    }else{
        //radius lineEdit hide
        ui->PMCfollowerRadiusEdit->setVisible(false);
        ui->PMCfollowerRadiusLabel->setVisible(false);

        //layout comboBox hide
        ui->PMCfollowerLayoutComboBox->setVisible(false);
        ui->PMCfollowerLayoutLabel->setVisible(false);
    }
}

void MainWindow::forbidOffsetEdit(QString lr)
{
    static double offset=0.0;
    if(0==lr.compare(QString("Middle"))){
        offset=str2double(ui->PMCoffsetEdit->text());
        ui->PMCoffsetEdit->setText("0");
        ui->PMCoffsetEdit->setEnabled(false);
    }else{
        ui->PMCoffsetEdit->setText(QString::number(offset));
        ui->PMCoffsetEdit->setEnabled(true);
    }
}

bool MainWindow::plotDistanceDiagramm()
{
    CAM::inputError error;
    if(ui->tableWidget->tableRead(error)==NULL){
        QMessageBox::critical(this, tr("Input Error"),errorName(error),QMessageBox::Yes,QMessageBox::Yes);
        return false;
    }
    double *distance=ui->tableWidget->distanceCalculation(this->pointNum);

    ui->widget->setTitle(QString("Distance"));
    QPointF *points=new QPointF[pointNum]();
    for(int i=0;i<pointNum;i++){
        points[i].setX(i*360.0/pointNum);
        points[i].setY(distance[i]);
    }
    ui->widget->clear();
    ui->widget->resize(740,170);
    ui->widget->autoMatch();
    ui->widget->inputCurve(points,pointNum);
    ui->widget->setInteractive(true);
    ui->widget->setTitle(QString("Distance"));
    ui->widget->repaint();
    return true;
}

void MainWindow::displayCam()
{

}

bool MainWindow::inputCheck(inputError &error, int &layout, double &followerR, double &basicR, double &e)
{
    //input rotation direction
    layout=0x0000;
    QString direction=ui->PMCRotionDirectionComboBox->currentText();
    if(0==direction.compare(QString("anticlockwise"))){
        layout=layout|PMC::anticlockwise;
    }else{
        layout=layout|PMC::clockwise;
    }
    /*
    //input in or outside
    QString io=ui->PMCfollowerLayoutComboBox->currentText();
    if(0==io.compare(QString("outside"))){
        layout=layout|PMC::followerOutside;
    }else if(0==io.compare(QString("inside"))){
        layout+=PMC::followerInside;
    }else{
        layout+=PMC::followerEmbedded;
    }
    */

    //input follower type and follower radius
    QString follower=ui->PMCfollowerShapeComboBox->currentText();
    if(0==follower.compare(QString("needle"))){
        followerR=0;
        layout=layout|PMC::needle;
    }else if(0==follower.compare(QString("roller"))){
        layout=layout|PMC::roller;
        if(ui->PMCfollowerRadiusEdit->text().isEmpty()){
            error._error=emptyInput;
            ui->PMCfollowerRadiusEdit->setFocus();
            return false;
        }
        followerR=str2double(ui->PMCfollowerRadiusEdit->text());
        if(followerR<0){
            error._error=negativeWert;
            ui->PMCfollowerRadiusEdit->setFocus();
            return false;
        }
    }else{
        layout=layout|PMC::plate;
        followerR=0;
    }

    //input basic radius
    if(ui->PMCbasicRadiusEdit->text().isEmpty()){
        error._error=emptyInput;
        ui->PMCbasicRadiusEdit->setFocus();
        //error._widget=ui->PMCbasicRadiusEdit;
        return false;
    }
    basicR=str2double(ui->PMCbasicRadiusEdit->text());
    if(basicR<0){
        error._error=negativeWert;
        ui->PMCbasicRadiusEdit->setFocus();
        //error._widget=ui->PMCbasicRadiusEdit;
        return false;
    }

    //input offset
    QString offsetLayout=ui->PMCoffsetComboBox->currentText();
    if(0==offsetLayout.compare(QString("left"))){
        layout=layout|PMC::left;
    }else if(0==offsetLayout.compare(QString("right"))){
        layout=layout|PMC::right;
    }else{
        layout=layout|PMC::middle;
    }

    if(layout&PMC::middle){
        e=0.0;
    }else if(ui->PMCoffsetEdit->text().isEmpty()){
        error._error=emptyInput;
        ui->PMCoffsetEdit->setFocus();
        return false;
    }else{
        e=str2double(ui->PMCoffsetEdit->text());
        if(e<0){
            error._error=negativeWert;
            ui->PMCoffsetEdit->setFocus();
            return false;
        }
    }


    //follower layout
    if(layout&PMC::roller){
        QString io=ui->PMCfollowerLayoutComboBox->currentText();
        if(0==io.compare(QString("outside"))){
            layout=layout|PMC::followerOutside;
        }else if(0==io.compare(QString("inside"))){
            layout=layout|PMC::followerInside;
        }else{
            layout=layout|PMC::followerEmbedded;
        }
    }
    return true;
}


void MainWindow::tab1Initial()
{
    QSettings *configIni = new QSettings("configuration.ini", QSettings::IniFormat);
    this->pointNum=(int)str2double(configIni->value("/system/pointNum").toString().toStdString());

    ui->PMCbasicRadiusEdit->setText("60");
    ui->PMCfollowerRadiusEdit->setText("5");
    ui->PMCoffsetEdit->setText("10");
    ui->PMCfollowerShapeComboBox->setCurrentText("roller");
}



