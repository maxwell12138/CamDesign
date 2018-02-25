#ifndef RPN_H
#define RPN_H
#include <string>
#include <iostream>
#include <stack>
#include <queue>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <math.h>
#include <QString>
#include <string>
#include "def.h"



void preTreat(std::string &str);
std::queue<CAM::member> rpn(std::string &str);
int precedence(std::string ch);
double str2double(std::string str);
double str2double(QString str);
double  calculate(std::queue<CAM::member> stackNumber,double x);

double  calculate(std::string str,double x);
double fac(int n);

QString errorName(CAM::inputError error);
#endif // RPN

