#ifndef _CONTROLLER_H
#define _CONTROLLER_H
#include"../data/dict.h"
#include<string>
#include<vector>

class controller{
    dic dcdb; 
    int dictidx;

    public:
    controller();
    std::string handleCommand(std::vector<std::string> data);

    int addHandle(int cm,std::string key,std::string val);
    void *findHandle(std::string key);
    int updateHandle(std::string key,std::string newval);
    int delHandle(std::string key);
    void selectHandle(std::string key);
    int createHandle();
    int flushdbHandle();
    void splitString(const std::string &val,std::vector<std::string> &v,const std::string &c);
};
#endif
