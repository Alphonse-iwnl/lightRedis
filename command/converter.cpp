#include"converter.h"

converter::converter(){
    //initial mp

    commandmp.insert(std::pair<std::string,int>("SADD",0));
    commandmp.insert(std::pair<std::string,int>("AADD",1));
    commandmp.insert(std::pair<std::string,int>("LADD",2));
    commandmp.insert(std::pair<std::string,int>("DEL",3));
    commandmp.insert(std::pair<std::string,int>("FIND",4));
    commandmp.insert(std::pair<std::string,int>("UPDATE",5));
    commandmp.insert(std::pair<std::string,int>("CREATE",6));
    commandmp.insert(std::pair<std::string,int>("SELECT",7));
    commandmp.insert(std::pair<std::string,int>("QUIT",8));
    commandmp.insert(std::pair<std::string,int>("FLUSHDB",9));

}

std::vector<std::string> converter::translateCommand2data(std::string _cm){
    cm=_cm;
    auto cmbegin=cm.begin();
    while(cmbegin!=cm.end()||*cmbegin!=' '){
        cmbegin++;
    }

    std::string command(cm.begin(),cmbegin);
    //find in mp and return  
    converter::result.push_back(std::to_string(findCommand(command)));
    (cmbegin==cm.end())?tmpit=cmbegin:tmpit=cmbegin++;
    converter::result.push_back(findKey());
    converter::result.push_back(findContent());
    return converter::result; 
}

int converter::findCommand(std::string _command){
    std::map<std::string,int>::iterator iter;
    if((iter=converter::commandmp.find(_command))!=commandmp.end())
        return (int )(iter->second);
    else return -1;   
}

std::string converter::findKey(){
    auto sbegin=converter::tmpit;
    while(tmpit!=cm.end()||*tmpit!=' ')
        tmpit++;

    if(tmpit!=cm.end()){
        std::string key(sbegin,tmpit);      
        tmpit++;
        return key;
    } else return NULL;
}

std::string converter::findContent(){
    if(tmpit!=cm.end())      
        return std::string(tmpit,cm.end());
    else return NULL;
}

void converter::rmp(){
    for(auto it=commandmp.begin();it!=commandmp.end();it++)
        rcommandmp.insert(std::pair<int,std::string>(it->second,it->first)); 
}

std::string converter::translatedata2Command(std::string *data){
    std::string result=0;
    result.append((converter::rcommandmp.find(std::stoi(data[0])))->second);
    result.append(" ");
    result.append(data[1]);
    result.append(" ");
    result.append(data[2]);
    return result;
}
