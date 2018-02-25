#ifndef DATA_H
#define DATA_H
#include "def.h"
//#include "platemovecam.h"
#include "basiccam.h"
class Data
{
public:
    Data(const int number,CAM::camType type);
    virtual ~Data();

    void createCam(CAM::camType type);
    inline BasicCam *getCam()               {return this->cam;}//               {return this->cam;}



    inline void setCamType(CAM::camType type)    {this->type=type;}
    inline const CAM::camType getCamType()             {return type;}

public:
    const int pointNum;
private:
    BasicCam *cam;
    CAM::camType type;

};

#endif // DATA_H
