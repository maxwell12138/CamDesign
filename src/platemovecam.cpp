#include "platemovecam.h"
#include <queue>
#include "rpn.h"
//#include "def.h"
#include "dialog.h"
#include <QPainter>
#include <iostream>
PlateMoveCam::PlateMoveCam(const int pointNum):BasicCam(pointNum)
{
    x=new double[pointNum];
    y=new double[pointNum];
    contourX_In=new double[pointNum];
    contourY_In=new double[pointNum];
    contourX_Out=new double[pointNum];
    contourY_Out=new double[pointNum];
    distance=new double[pointNum];
    angle=new double[pointNum];

    centerPath=NULL;
    contourPath_In=NULL;
    contourPath_Out=NULL;
    speed=NULL;
    acceleration=NULL;

    currentIndex=0;
}



PlateMoveCam::~PlateMoveCam()
{

    if(x)  delete[] x;
    if(y)  delete[] y;

    if(centerPath)    delete centerPath;
    if(contourPath_In)  delete contourPath_In;
    if(contourPath_Out) delete contourPath_Out;

    if(contourX_In)  delete[] contourX_In;
    if(contourY_In)  delete[] contourY_In;
    if(contourX_Out)  delete[] contourX_Out;
    if(contourY_Out)  delete[] contourY_Out;

    if(distance)  delete[] distance;
    if(speed)     delete[] speed;
    if(acceleration)  delete[] acceleration;
    if(angle)      delete[] angle;
}

void PlateMoveCam::setDistance(double *distance)
{
    if(this->distance)   delete this->distance;
    this->distance=distance;
}


void PlateMoveCam::centralCurveCal()
{
    double resolution=2*M_PI/pointNum;
    if(layout&PMC::plate){
        for(int i=0;i<pointNum;i++){
           x[i]=(basicDistance+distance[i])*sin(-resolution*i);
           y[i]=(basicDistance+distance[i])*cos(resolution*i);
        }
    }else{
        if((layout&PMC::clockwise)&&(layout&PMC::left)){
            m1=-1;
            m2=1;
        }else if((layout&PMC::anticlockwise)&&(layout&PMC::right)){
            m1=1;
            m2=-1;
        }else if((layout&PMC::anticlockwise)&&(layout&PMC::left)){
            m1=-1;
            m2=-1;
        }else if((layout&PMC::clockwise)&&(layout&PMC::right)){
            m1=1;
            m2=1;
        }else if(layout&PMC::middle){
            m1=0;
            m2=1;
        }
        m3=-m1*m2;
        for(int i=0;i<this->pointNum;i++){
            x[i]=m1*offset*cos(m2*resolution*i)-(basicDistance+distance[i])*sin(m2*resolution*i);
            y[i]=m1*offset*sin(m2*resolution*i)+(basicDistance+distance[i])*cos(m2*resolution*i);
        }
    }
}

void PlateMoveCam::contourCurveCal()
{
    double resolution=360.0/this->pointNum;
    if(layout&PMC::roller){

        long double deltaX,deltaY,deltaXY;
        for(int i=1;i<pointNum-1;i++){
            deltaX=(x[i+1]-x[i-1])/(resolution*2);
            deltaY=(y[i+1]-y[i-1])/(resolution*2);
            deltaXY=sqrt(deltaX*deltaX+deltaY*deltaY);
            contourX_Out[i]=x[i]+followerRadius*deltaY/deltaXY;
            contourY_Out[i]=y[i]-followerRadius*deltaX/deltaXY;
            contourX_In[i]=x[i]-followerRadius*deltaY/deltaXY;
            contourY_In[i]=y[i]+followerRadius*deltaX/deltaXY;
        }
        deltaX=(x[1]-x[pointNum-1])/(resolution*2);
        deltaY=(y[1]-y[pointNum-1])/(resolution*2);
        deltaXY=sqrt(deltaX*deltaX+deltaY*deltaY);
        contourX_Out[0]=x[0]+followerRadius*deltaY/deltaXY;
        contourY_Out[0]=y[0]-followerRadius*deltaX/deltaXY;
        contourX_In[0]=x[0]-followerRadius*deltaY/deltaXY;
        contourY_In[0]=y[0]+followerRadius*deltaX/deltaXY;

        deltaX=(x[0]-x[pointNum-2])/(resolution*2);
        deltaY=(y[0]-y[pointNum-2])/(resolution*2);
        deltaXY=sqrt(deltaX*deltaX+deltaY*deltaY);
        contourX_Out[pointNum-1]=x[pointNum-1]+followerRadius*deltaY/deltaXY;
        contourY_Out[pointNum-1]=y[pointNum-1]-followerRadius*deltaX/deltaXY;
        contourX_In[pointNum-1]=x[pointNum-1]-followerRadius*deltaY/deltaXY;
        contourY_In[pointNum-1]=y[pointNum-1]+followerRadius*deltaX/deltaXY;

    }else{
        for(int i=0;i<pointNum;i++){
            contourX_In[i]=x[i];
            contourY_Out[i]=y[i];
            contourX_In[i]=x[i];
            contourY_Out[i]=y[i];
        }
    }

}


void PlateMoveCam::speedCal()
{

}
void PlateMoveCam::accelerationCal()
{

}


void PlateMoveCam::getScaleAndCenter(QSizeF frameSize, double &scale, QPointF &displayCenter)
{
    double height,width;
    double maxD,minD;
    minAndMaxDistance(minD,maxD);
    //double maxDistance=basicDistance-minD+maxD;
    //basicDis=sqrt(basicRadius*basicRadius+maxDistance*maxDistance)+basicRadius;
    width=basicRadius+followerRadius;
    width=width*2;
    height=(basicRadius+followerRadius)+(basicDistance-minD+maxD+followerRadius+20);

    displayCenter.setX(frameSize.width()/2);
    displayCenter.setY(frameSize.height()*(1-(width/2)/height));

    double scaleX,scaleY;
    scaleX=frameSize.width()/width;
    scaleY=frameSize.height()/height;
    if(scaleX>scaleY){
        scale=scaleY*0.9;
    }else{
        scale=scaleX*0.9;
    }
}



void PlateMoveCam::centerPathCal(double scale)
{
    if(this->centerPath) {
        delete centerPath;
    }
    this->centerPath=new QPainterPath(QPointF(x[0]*scale,-y[0]*scale));
    for(int i=1;i<this->pointNum;i++){
        centerPath->lineTo(x[i]*scale,-y[i]*scale);
    }
    centerPath->lineTo(x[0]*scale,-y[0]*scale);
}


void PlateMoveCam::contourPathCal(double scale)
{
    if(this->contourPath_In) delete contourPath_In;
    this->contourPath_In=new QPainterPath(QPointF(contourX_In[0]*scale,-contourY_In[0]*scale));
    for(int i=1;i<this->pointNum;i++){
        contourPath_In->lineTo(contourX_In[i]*scale,-contourY_In[i]*scale);
    }
    contourPath_In->lineTo(contourX_In[0]*scale,-contourY_In[0]*scale);

    if(this->contourPath_Out) delete contourPath_Out;
    this->contourPath_Out=new QPainterPath(QPointF(contourX_Out[0]*scale,-contourY_Out[0]*scale));
    for(int i=1;i<this->pointNum;i++){
        contourPath_Out->lineTo(contourX_Out[i]*scale,-contourY_Out[i]*scale);
    }
    contourPath_Out->lineTo(contourX_Out[0]*scale,-contourY_Out[0]*scale);
}


void PlateMoveCam::displayInDialog(QWidget *widget, QPointF displayCenter, double scale)
{
    static double previousScale=0.0;
    if(scale<=0) return;

    if(scale!=previousScale){
        this->centerPathCal(scale);
        this->contourPathCal(scale);
    }

    double resolution=360.0/this->pointNum;

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(CAM::SolidLineWidth);

    //cam
    QPainter camPainter(widget);
    camPainter.setPen(pen);
    camPainter.setRenderHint(QPainter::Antialiasing,true);
    camPainter.translate(displayCenter.x(),displayCenter.y());

    if(layout&PMC::anticlockwise){
        camPainter.rotate(-resolution*currentIndex);
    }else{
        camPainter.rotate(resolution*currentIndex);
    }
    //camPainter.drawEllipse(-this->basicRadius*scale-this->followerRadius*scale,-this->basicRadius*scale-this->followerRadius*scale,this->basicRadius*2*scale+this->followerRadius*2*scale,this->basicRadius*2*scale+this->followerRadius*2*scale);

    if(layout&PMC::roller){
        if(layout&PMC::followerInside){
            camPainter.drawPath(*contourPath_In);
        }else if(layout&PMC::followerOutside){
            camPainter.drawPath(*contourPath_Out);
        }else if(layout&PMC::followerEmbedded){
            camPainter.drawPath(*contourPath_In);
            camPainter.drawPath(*contourPath_Out);
        }

        camPainter.save();
        pen.setStyle(Qt::DotLine);
        pen.setWidth(CAM::dotLineWidth);
        camPainter.setPen(pen);
        camPainter.drawPath(*centerPath);
        camPainter.restore();
    }else{
        camPainter.drawPath(*centerPath);
    }

    camPainter.drawPoint(0,0);
    double r=this->basicRadius*scale/5.0;

    pen.setStyle(Qt::SolidLine);
    pen.setWidth(CAM::SolidLineWidth);
    camPainter.setPen(pen);
    camPainter.drawEllipse(-r,-r,2*r,2*r);
    camPainter.eraseRect(-r/3.0,-8*r/7.0,2*r/3.0,4*r/9.0);
    camPainter.drawRect(-r/3.0,-8*r/7.0,2*r/3.0,4*r/9.0);

    const int sliderLength=20;
    double min,max;
    minAndMaxDistance(min,max);

    QPainter follower(widget);
    follower.setPen(pen);
    follower.translate(displayCenter);
    //follower move
    if(layout&PMC::anticlockwise){
        follower.translate(this->offset*scale*(m1),-(this->basicDistance+this->distance[currentIndex])*scale);//(data->offset,-data->basicDistance-data->distance[index]);
    }else{
        follower.translate(this->offset*scale*(m1),-(this->basicDistance+this->distance[pointNum-currentIndex])*scale);//(data->offset,-data->basicDistance-data->distance[index]);
    }
    if(layout&PMC::roller){
        follower.drawLine(0,0,0,-(max-min+followerRadius+followerRadius/5.0+sliderLength)*scale);                                //-this->followerRadius*scale
        follower.drawEllipse(-this->followerRadius*scale,-this->followerRadius*scale,this->followerRadius*scale*2,this->followerRadius*scale*2);
    }else if(layout&PMC::plate){
        follower.drawLine(0,-CAM::SolidLineWidth,0,-CAM::SolidLineWidth-(max-min+followerRadius+followerRadius/5.0+sliderLength)*scale);                                //-this->followerRadius*scale
        follower.drawLine(-(this->offset+10)*scale,-CAM::SolidLineWidth,10*scale,-CAM::SolidLineWidth);
    }else if(layout&PMC::needle){
        follower.drawLine(0,0,0,-(max-min+followerRadius+followerRadius/5.0+sliderLength)*scale);
    }


    QPainter slider(widget);
    slider.setPen(pen);
    slider.translate(displayCenter);
    slider.translate(this->offset*scale*(m1),-(basicDistance+max-min+followerRadius+4)*scale);
    slider.setBrush(Qt::gray);
    slider.drawRect(-CAM::SolidLineWidth*2-sliderLength/5.0,-sliderLength,sliderLength/5.0,sliderLength);
    slider.drawRect(CAM::SolidLineWidth*2,-sliderLength,sliderLength/5.0,sliderLength);

    previousScale=scale;
}

const double *PlateMoveCam::getInfo(CAM::infoType info)
{
    switch (info) {
    case CAM::distance:
        return this->distance;
    case CAM::angle:
        return this->angle;
    default:
        return 0;
    }
}

void PlateMoveCam::angleCal()
{
    double deltaPhi=2*M_PI/pointNum;
    if(layout&PMC::roller||layout&PMC::needle){
        for(int i=1;i<pointNum-1;i++){
            //angle[i]=180.0/M_PI*atan(((distance[i+1]-distance[i-1])/(2*deltaPhi)-m3*offset)/(basicDistance+distance[i]));
            angle[i]=((distance[i+1]-distance[i-1])/(deltaPhi*2)-m3*offset)/(basicDistance+distance[i]);
        }
        angle[0]=((distance[1]-distance[pointNum-1])/(2*deltaPhi)-m3*offset)/(basicDistance+distance[0]);
        angle[pointNum-1]=((distance[0]-distance[pointNum-2])/(2*deltaPhi)-m3*offset)/(basicDistance+distance[pointNum-1]);
        for(int i=0;i<pointNum;i++){
            if(angle[i]<0) angle[i]=-angle[i];
            angle[i]=180.0/M_PI*atan(angle[i]);
        }
    }else if(layout&PMC::plate){
        for(int i=0;i<pointNum;i++){
            angle[i]=0;
        }
    }/*else if(layout&PMC::needle){
        if(offset==0){
            for(int i=0;i<pointNum;i++){
               angle[i]=0;
            }
            return;
        }

        for(int i=0;i<pointNum;i++){
            angle[i]=offset/(basicDistance+distance[i]);
            if(angle[i]<0) angle[i]=-angle[i];
            angle[i]=180.0/M_PI*atan(angle[i]);
        }
    }*/

}





void PlateMoveCam::minAndMaxDistance(double &min, double &max)const
{
    max=distance[0];
    min=distance[0];
    for(int i=1;i<pointNum;i++){
        if(max<distance[i]){
            max=distance[i];
        }
        if(min>distance[i]){
            min=distance[i];
        }
    }
}



void PlateMoveCam::basicDistanceCal(){
    double temp=sqrt(basicRadius*basicRadius-offset*offset);
    if(temp>0){
        this->basicDistance=temp;
    }
}
