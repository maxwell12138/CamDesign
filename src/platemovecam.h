#ifndef PLATECAM_H
#define PLATECAM_H
#include "basiccam.h"
#include "def.h"
#include "math.h"
#include "figure.h"
using namespace CAM;

class PlateMoveCam : public BasicCam
{
public:
    PlateMoveCam(const int pointNum);
    virtual ~PlateMoveCam();

    //void distanceCalculation(distanceExpression *expression,int num);
    void setDistance(double *distance);
    void basicDistanceCal();

    void centralCurveCal();
    void contourCurveCal();
    void angleCal();

    void speedCal();
    void accelerationCal();

    void getScaleAndCenter(QSizeF frameSize,double &scale,QPointF &displayCenter);

    void centerPathCal(double scale);
    void contourPathCal(double scale);
    void displayInDialog(QWidget *widget, QPointF displayCenter=QPointF(0,0), double scale=1.0);
    //void displayDistance(QCustomPlot *widget);
    //void displayAngle(QWidget *widget);
    //void displaySpeed(QWidget *widget);
    //void displayAcceleration(QWidget *widget);
    //void displayInfo(Figure *canvas, CAM::infoType type);
    const double *getInfo(CAM::infoType info);



    inline void setFollowerRadius(double f)                 {if(f>=0)followerRadius=f;}
    inline double getFollowerRadius()                       {return followerRadius;}
    inline void setOffset(double e)                         {if(e>=0)this->offset=e;}
    inline double getOffset()                               {return offset;}
    inline void setBasicRadius(double r)                    {if(r>=0)basicRadius=r;}
    inline double getBasicRadius()                          {return basicRadius;}

    int m1,m2,m3;

private:
    void minAndMaxDistance(double &min,double &max) const;
    //int currentIndex;
    QPainterPath *centerPath;
    QPainterPath *contourPath_In;
    QPainterPath *contourPath_Out;
    //int layout;       //variable in base class
    double followerRadius;
    double offset;
    double basicRadius;//
    double basicDistance;//
    double *x,*y;
    double *contourX_In,*contourY_In;
    double *contourX_Out,*contourY_Out;
    double *distance,*speed,*angle,*acceleration;
};



#endif // PLATECAM_H
