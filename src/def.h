#ifndef DEF
#define DEF
#include <iostream>
#include <string>
#include <QString>
#include <QWidget>
namespace CAM {
    enum infoType{distance,angle,speed,acceleration};

    typedef struct _coordinate{
        double _x;
        double _y;
    }coordinate;
    typedef struct _distanceExpression{
        double _start;
        double _end;
        std::string _expression;
        bool _absolute;
    }distanceExpression;

    enum memberType{number,_operator,l_operator,r_operator,constant,bracket,variable,undefined};
    typedef struct _member{
        memberType _type;
        std::string _str;
    }member;

    enum camType{plateMoveCam};
    enum followerType{movement=0x0000,swing=0x0001};

    enum inputErrorType{offsetToLarge,negativeWert,followerToLarge,emptyInput,outRange,falseInterval,intervalOverlap};
    typedef struct _inputError{
        inputErrorType _error;
    }inputError;
    enum lineWidth{dotLineWidth=1,SolidLineWidth=2};
}
namespace PMC {
    enum layout{null=0x0000,
           clockwise=0x0001,anticlockwise=0x0002,
           left=0x0004,right=0x0008,middle=0x0010,
           followerInside=0x0020,followerOutside=0x0040,followerEmbedded=0x0080,
           needle=0x0100,roller=0x0200,plate=0x0400};
}

#endif // DEF

