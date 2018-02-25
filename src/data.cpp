#include "data.h"
#include "platemovecam.h"
#include "def.h"


Data::Data(const int number,CAM::camType type):pointNum(number),type(type)
{
    this->createCam(type);
}


Data::~Data()
{
    if(cam)    delete cam;
}

void Data::createCam(CAM::camType type)
{
    this->type=type;
    if(this->cam==NULL){
        delete this->cam;
    }
    switch (type) {

    case CAM::plateMoveCam:
        this->cam=new PlateMoveCam(this->pointNum);
        break;

    default:
        this->cam=new PlateMoveCam(this->pointNum);
        break;
    }
}













