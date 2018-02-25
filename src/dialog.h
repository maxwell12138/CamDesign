#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "basiccam.h"
#include "rpn.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(BasicCam *cam,QWidget *parent = 0);
    ~Dialog();
    bool isPause();

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *);
private:
    Ui::Dialog *ui;
    QPointF displayCenter;
    double scale;
    double basicScale;
    BasicCam *cam;

    QPointF pressPoint;
    int timerFlag;

private slots:
    void setAngle(int index);
    void reset();
    void pause();
    inline void setScale(double scale)         {this->scale=scale;}

protected slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // DIALOG_H
