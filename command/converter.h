#ifndef _CONVERTER_H
#define _CONVERTER_H
#include"../data/dict.h"
#include<map>
#include<string>

class converter{
  std::map<std::string,int> commandmp;
  std::map<int,std::string> rcommandmp;
  std::vector<std::string> result;
  std::string::iterator tmpit;
  std::string cm;
  //initial rcommandmp
  void rmp();
    public:
  converter();
  //void loadcommandmp;
  std::vector<std::string> translateCommand2data(std::string cm);
  //method
  int findCommand(std::string _command);
  std::string findKey();
  std::string findContent();

  std::string translatedata2Command(std::string *data);
};
#endif
