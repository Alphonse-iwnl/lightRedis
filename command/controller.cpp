#include"controller.h"

controller::controller(){
    dictidx=0;

}

std::string controller::handleCommand(std::vector<std::string> data){
    int status=-1;
    int cm=std::stoi(data[0]);
    if(cm<3&&data.size()==3){
       if(controller::addHandle(cm,data[1],data[2]))
           return std::string("add success");
    }   else if(cm==3&&data.size()==2){
       if(controller::delHandle(data[1]))
           return std::string("delete success");
    }  else if(cm==4&&data.size()==2){
        //find void*
        std::string *result=(std::string *)controller::findHandle(data[1]); 
        if(result!=NULL)
            return *result;
    }
    else if(cm==5&&data.size()==3){
       return std::to_string(status=controller::updateHandle(data[1],data[2]));
    }   else if(cm==6)
        return std::to_string(status=controller::createHandle());
    else if(cm==7&&data.size()==2){
        controller::dictidx=std::stoi(data[1]);
        return std::string("change success");
    }   else if(cm==8){
        //close clifd
        
    }
    else if(cm==9){
       return  std::to_string(status=controller::flushdbHandle());
    }
    else return std::string("bad command");//no enough para or no command
}

void controller::splitString(const std::string &val,std::vector<std::string> &v,const std::string &c){
    std::string::size_type pos1,pos2;
    pos2=val.find(c);
    pos1=0;
    while(std::string::npos!=pos2){
        v.push_back(val.substr(pos1,pos2-pos1));

        pos1=pos2+c.size();
        pos2=val.find(c,pos1);
    }
    if(pos1!=val.length())
        v.push_back(val.substr(pos1));//not find && val.length()!=0
}

int controller::addHandle(int cm,std::string key,std::string val){
    robj okey,oval;
    std::vector<std::string> tmp;

    okey.objtype=_string;  
    okey.ptr=&key; 
    if(cm==0){// string-string
        oval.objtype=_string;
        oval.ptr=&val;        
    }else if(cm==1){//string-array
        oval.objtype=_set;
        controller::splitString(val,tmp,",");
        std::string *sarr=new std::string[tmp.size()+1];
        for(size_t i=0;i!=tmp.size();i++)
            sarr[i]=tmp[i];
        sarr[tmp.size()]="\0";
        oval.ptr=sarr;
    }else if(cm==2){//string-list
        oval.objtype=_list;
        controller::splitString(val,tmp,",");
        rlist rl;
        for(size_t i=0;i!=tmp.size();i++){
            listNode newN;
            newN.prev=NULL;
            newN.next=NULL;
            newN.value=&tmp[i];
            rl.listAppend(&newN);
        } 
        oval.ptr=&rl;
    }
    controller::dcdb.dictAdd(dictidx,&okey,&oval);
    //no return for current
}

int controller::delHandle(std::string key){
    robj okey;
    okey.objtype=_string;
    okey.ptr=&key;
    controller::dcdb.dictDelete(dictidx,&okey);     
}

void * controller::findHandle(std::string key){
    robj okey;
    okey.objtype=_string;
    okey.ptr=&key;
    void *fetch=NULL;
    void *result=NULL;
    fetch=controller::dcdb.dictFetchValue(controller::dictidx,&okey);
    if(fetch==NULL)
        return fetch;
    else if(((robj *)fetch)->objtype==_string)
        return ((robj *)fetch)->ptr;//consider to return string 
    else if (((robj *)fetch)->objtype==_set){
        std::string tmp;
        std::string *p=((std::string *)(((robj *)fetch)->ptr));
        while(*p++!="\0"){
            tmp+=*((std::string *)(((robj *)fetch)->ptr));
        }
        result=&tmp;
        return result;
    }
    else if(((robj *)fetch)->objtype==_list){
        listNode *p=(listNode *)(((robj *)fetch)->ptr);
        std::string tmp;
        while(p!=NULL){
            tmp+=*((std::string *)p->value);
            p=p->next;  
        }
        result=&tmp;
        return result;
    }
}

int controller::updateHandle(std::string key,std::string newval){
    robj okey;
    okey.objtype=_string;
    okey.ptr=&key;
    std::vector<std::string> tmp;
    void *fetch=NULL;
    std::string *newv;
    listNode *head=NULL;
    listNode *p=NULL;
    fetch=controller::dcdb.dictFetchValue(controller::dictidx,&okey);
    if(fetch!=NULL){
        robj val;
        val.objtype=((robj *)fetch)->objtype;
        switch(val.objtype){
            case _string:
                ((robj *)fetch)->ptr=NULL;
                ((robj *)fetch)->ptr=&newval;
                break;
            case _set:
                controller::splitString(newval,tmp,",");
                newv=new std::string[tmp.size()+1];
                for(std::size_t i=0;i<tmp.size();i++){
                    newv[i]=tmp[i];
                }
                newv[tmp.size()]="\0";
                ((robj *)fetch)->ptr=newv;
                break;  
            case _list:
                controller::splitString(newval,tmp,",");
                head->next=NULL;
                head->prev=NULL;
                head->value=&tmp[0];
                p=head;
                for(auto i=++tmp.begin();i!=tmp.end();i++){
                    listNode *node=NULL;
                    node->next=NULL;
                    node->prev=p;
                    node->value=&i;
                    p=p->next;
                }
                ((robj *)fetch)->ptr=head;
                break;
            default:break;
        };
    }
    else {
          controller::addHandle(0,key,newval);//defalut string-string
    }
}

int controller::createHandle(){
      return controller::dictidx=controller::dcdb.dictCreate();
}

int controller::flushdbHandle(){
      return controller::dcdb.dictRelese(controller::dictidx);
}
