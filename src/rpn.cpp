#include <cctype>
#include "rpn.h"
#include <QString>
using namespace CAM;
using std::string;
using std::queue;
using std::stack;


std::queue<member> rpn(string &str){
    preTreat(str);
    std::stack<member> stackSymbol;
    std::queue<member> stackNumber;

    //string str="sin(x)";
    //"sin(pi/2)+0+3!-2*3+1%";
    std::string temp="";
    member tempMember;
    //int x[]={1,2,3,4,5,6};
    bool bool_l_operator=false;//the variable will be set true if left operator ist just now used

    int i=0;
    while(str[i]){
        //get operate number
        if((str[i]>='0'&&str[i]<='9')||str[i]=='.'){
            while((str[i]>='0'&&str[i]<='9')||str[i]=='.'){
                temp.append(std::string(1,str[i]));
                i++;
            }
            tempMember._type=number;
            tempMember._str=temp;
            stackNumber.push(tempMember);
        }else if((str[i]>='a'&&str[i]<='z')||(str[i]>='A'&&str[i]<='Z')||str[i]=='~'){
            if(str[i]=='~'){
                tempMember._type=l_operator;
                tempMember._str=std::string(1,'~');
                stackSymbol.push(tempMember);
                bool_l_operator=true;
                i++;
            }else{
                //get function name
                while((str[i]>='a'&&str[i]<='z')||(str[i]>='A'&&str[i]<='Z')){
                    temp.append(std::string(1,str[i]));
                    i++;
                }
                //check funcion type
                if((temp=="sin")||(temp=="cos")||(temp=="tan")||(temp=="exp")||(temp=="sqrt")){
                    tempMember._type=l_operator;
                    tempMember._str=temp;
                    stackSymbol.push(tempMember);
                    bool_l_operator=true;
                }else if(temp=="x"){
                    tempMember._type=variable;
                    tempMember._str=temp;
                    stackNumber.push(tempMember);
                }else if(temp=="pi"){
                    tempMember._type=constant;
                    tempMember._str=temp;
                    stackNumber.push(tempMember);
                }else{
                    //erase unknown function
                    str.erase(i-temp.length(),temp.length());
                    i=i-temp.length();
                }
            }


        }else if(str[i]=='('){
            tempMember._type=bracket;
            tempMember._str=std::string(1,str[i]);
            stackSymbol.push(tempMember);
            i++;
        }else if(str[i]==')'){
            if("("!=stackSymbol.top()._str){
                for(tempMember=stackSymbol.top();tempMember._str!="(";tempMember=stackSymbol.top()){
                    stackNumber.push(tempMember);
                    stackSymbol.pop();
                }
            }
            stackSymbol.pop();
            i++;


        }else if(str[i]=='+'||str[i]=='-'||str[i]=='*'||str[i]=='/'||str[i]=='^'){
            tempMember._type=_operator;
            tempMember._str=std::string(1,str[i]);
            if(stackSymbol.empty()){
                stackSymbol.push(tempMember);
            }else{
                //if the priority of operator is lower than stack top,pop stack until the priority of top
                //is not higher than operator
                while(precedence(tempMember._str)<=precedence(stackSymbol.top()._str)){
                    stackNumber.push(stackSymbol.top());
                    stackSymbol.pop();
                    if(stackSymbol.empty()) break;
                }
                stackSymbol.push(tempMember);
            }
            i++;
        }else if(str[i]=='!'||str[i]=='%'){
            tempMember._type=r_operator;
            tempMember._str=std::string(1,str[i]);
            stackNumber.push(tempMember);
            i++;
        }else{
            str.erase(i,1);
        }
        while(!bool_l_operator&&!stackSymbol.empty()&&stackSymbol.top()._type==l_operator){// operator for left operand
            stackNumber.push(stackSymbol.top());
            stackSymbol.pop();
        }
        bool_l_operator=false;
        //reset temp data
        temp.clear();
        tempMember._str.clear();
        tempMember._type=undefined;
    }
    while(!stackSymbol.empty()){
        stackNumber.push(stackSymbol.top());
        stackSymbol.pop();
    }
    return stackNumber;
}


int precedence(std::string ch)
{
    if(ch == "(") return 1;
    else if(ch == "+" || ch == "-") return 3;
    else if(ch == "*" || ch == "/") return 4;
    else if(ch == "~" ) return 5;
    else if(ch == "%" || ch == "^"||ch == "!") return 6;
    else if(ch == ")" ) return 7;
    else return 0;
}

double calculate(std::queue<member> stackNumber,double x){
    std::stack<double> calculation;
    //string temp;
    double operand1=0.0;
    double operand2=0.0;
    double result;
    try{
        while(!stackNumber.empty()){
            switch (stackNumber.front()._type) {
            case number:            //0~9
                result=str2double(stackNumber.front()._str);
                break;
            case _operator:         //+-*/^
                operand2=calculation.top();
                calculation.pop();
                operand1=calculation.top();
                calculation.pop();
                if(stackNumber.front()._str=="+"){
                    result=operand1+operand2;
                }else if(stackNumber.front()._str=="-"){
                    result=operand1-operand2;
                }else if(stackNumber.front()._str=="*"){
                    result=operand1*operand2;
                }else if(stackNumber.front()._str=="/"){
                    result=operand1/operand2;
                }else if(stackNumber.front()._str=="^"){
                    result=pow(operand1,operand2);
                }
                break;

            case l_operator:    case r_operator:
                operand1=calculation.top();
                calculation.pop();
                if(stackNumber.front()._str=="sin"){
                    result=sin(operand1/180.0*M_PI);
                }else if(stackNumber.front()._str=="cos"){
                    result=cos(operand1/180.0*M_PI);
                }else if(stackNumber.front()._str=="tan"){
                    result=tan(operand1/180.0*M_PI);
                }else if(stackNumber.front()._str=="exp"){
                    result=exp(operand1);
                }else if(stackNumber.front()._str=="sqrt"){
                    result=sqrt(operand1);
                }else if(stackNumber.front()._str=="~"){
                    result=-operand1;
                }else if(stackNumber.front()._str=="!"){
                    result=fac(operand1);
                }else if(stackNumber.front()._str=="%"){
                    result=operand1/100;
                }
                break;
            case bracket:
                //no possible
                break;
            case constant:
                if(stackNumber.front()._str=="pi"){
                    result=M_PI;
                }
                break;
            case variable:
                result=x;
                break;
            default:
                break;
            }
            calculation.push(result);
            stackNumber.pop();
        }

    }catch(std::exception myEx){
        std::cout<<"Exception:"<<myEx.what()<<std::endl;
    }
    return calculation.top();
}


double str2double(std::string str){
    std::stringstream s;
    s<<str;
    double num=0.0;
    s>>num;
    return num;
}
double str2double(QString str){
    return str2double(str.toStdString());
}

double fac(int n){
    if(n<0) return -1;
    if(n==0||n==1)return 1;
    return n*fac(n-1);
}



double calculate(std::string str, double x)
{
    return calculate(rpn(str),x);
}


void preTreat(std::string &str)
{
    //differ negative symbol and minus
    if(str[0]=='-'){
        str[0]='~';
    }
    for(int i=0;i<str.length();i++){
        str[i]=tolower(str[i]);
        //erase space
        if(str[i]==' '){
            str.erase(i,1);
            i--;
        }
    }
    for(int i=0;i<str.length();i++){
        //mark variable x, except exp
        if(str[i]=='x'){
            if(i!=0||i!=str.length()-1){
                if(str[i-1]!='e'||str[i+1]!='p'){
                    str.insert(i+1,")");
                    str.insert(i,"(");
                    i+=2;
                }
            }
        }
    }
}

//enum inputError{offsetToLarge,negativeWert,followerToLarge,emptyInput,outRange,falseInterval};
QString errorName(inputError error){
    QString str;
    switch (error._error) {
    case offsetToLarge:
        str=QString("offset is to large");
        break;
    case negativeWert:
        str=QString("Wert cannot be negative");
        break;
    case followerToLarge:
        str=QString("the radius of follower is to large");
        break;
    case emptyInput:
        str=QString("input cannot be empty");
        break;
    case outRange:
        str=QString("input is out of allowed range");
        break;
    case falseInterval:
        str=QString("end of Interval is smaller than begin");
        break;
    case intervalOverlap:
        str=QString("Intervals overlap");
        break;
    default:
        str=QString("unknown error");
        break;
    }
    return str;
}
