#ifndef BASICCAM_H
#define BASICCAM_H
#include "def.h"
#include "figure.h"
class BasicCam
{
public:
    BasicCam(const int pointNum);
    virtual ~BasicCam();
    virtual void setDistance(double *distance)=0;
    virtual void centralCurveCal()=0;
    virtual void contourCurveCal()=0;
    virtual void speedCal()=0;
    virtual void accelerationCal()=0;
    virtual void angleCal()=0;

    virtual void getScaleAndCenter(QSizeF frameSize, double &scale, QPointF &displayCenter)=0;

    virtual void displayInDialog(QWidget *widget,QPointF displayCenter=QPointF(0,0),double scale=1.0)=0;
    //virtual void displayInfo(Figure *canvas,CAM::infoType type)=0;
    virtual const double *getInfo(CAM::infoType info)=0;

    inline void setTypeAttr(int t)       {this->layout=t;}
    inline void addTypeAttr(int t)       {this->layout+=t;}
    inline const int getTypeAttr()       {return layout;}
    inline const int getPointNum()          {return this->pointNum;}
    inline void setCurrentIndex(int index)           {this->currentIndex=index;this->currentIndex=this->currentIndex%pointNum;}
    inline int getCurrretIndex()                    {return currentIndex;}
    void increaseCurrentIndex();


protected:
    const int pointNum;
    int layout;
    int currentIndex;

private:
};



#endif // BASICCAM_H
