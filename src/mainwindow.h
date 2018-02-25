#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "data.h"
#include "inputtable.h"
#include "dialog.h"
#include "figure.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void inputOkButton();
    void showFollowerRadius(QString fshape);
    void forbidOffsetEdit(QString lr);
    bool plotDistanceDiagramm();
    void displayCam();

private:
    bool inputCheck(CAM::inputError &error,int &layout,
               double &followerR,double &basicR,double &e);
    Ui::MainWindow *ui;
    Data *data;
    Dialog *myDialog;
    int pointNum;
    void tab1Initial();

};

#endif // MAINWINDOW_H
