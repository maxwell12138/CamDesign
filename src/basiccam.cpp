#include "basiccam.h"

BasicCam::BasicCam(const int pointNum):pointNum(pointNum)
{
    this->currentIndex=0;
}

BasicCam::~BasicCam()
{

}

void BasicCam::increaseCurrentIndex(){
    this->currentIndex++;
    this->currentIndex=this->currentIndex%pointNum;
}

