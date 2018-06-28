#include"list.h"

void * rlist::dupNode(  void *ptr){
    return ((listNode *)ptr)->value;
}

void rlist::_free(void *ptr){
    ((listNode *)ptr)->prev->next=((listNode *)ptr)->next;
    ((listNode *)ptr)->next->prev=((listNode *)ptr)->prev;
    ((listNode *)ptr)->next=nullptr;   
    ((listNode *)ptr)->prev=nullptr;   
    delete (listNode *)ptr;
}

int rlist::match(void *ptr,void *key){
    return (*(static_cast<std::string*>(((listNode *)ptr)->value))==*(static_cast<std::string*>(key)))?1:0;
}

rlist::rlist(){

}

unsigned long rlist::listLength(){
    length=0;
    if(rl->head!=NULL){
        view=rl->head;
        while(view!=nullptr){
            length++;
            view=view->next;
        }
    } 
    return rl->len=length;
}

listNode * rlist::listFirst(){
    return rl->head;
}

listNode * rlist::listLast(){
    if(rl->head!=NULL){
        if(rl->head->next!=NULL){
            view=rl->head;
            while(view->next!=NULL){
                view=view->next;
            }
            return view;
        }else{
            return rl->head;
        }
    }
    return NULL;
}

listNode * rlist::listPrevNode(void *node){
    return ((listNode *)node)->prev;  
}

listNode * rlist::listNextNode(void *node){  
    return ((listNode *)node)->next;  
}

void * rlist::listNodeValue(void *node){
    return ((listNode *)node)->value;
}

void * rlist::listSearchKey(void *value){
    if(rl->head!=NULL){
        view=rl->head;
        while(view!=NULL){
            if(rlist::match(view,value))
                return view;
            view=view->next;
        }
    }
    return NULL;
}

int rlist::listIndex(void *value){
    int index=-1;
    if(rl->head!=NULL){
        view=rl->head;
        while(view!=NULL){
            index++;
            if(rlist::match(view,value))
                return index;
            view=view->next;  
        }
    }
    return index;
}

bool rlist::listAppend(void *node){
    view=rlist::listLast();
    if(view!=NULL){
        view->next=(listNode *)node;
        ((listNode *)node)->prev=view;
        ((listNode *)node)->next=NULL;
        return true;
    }else if(view==NULL){
        rl->head=(listNode *)node;
        return true;
    }
    return false;
}

bool rlist::listInsertNode(void *node,unsigned long pos){
    unsigned long tmp=rlist::listLength();
    if(tmp>pos){
        view=rl->head;
        for(unsigned int i=1;i!=pos;i++)
            view=view->next;
        ((listNode *)node)->next=view->next;
        ((listNode *)node)->prev=view;
        view->next->prev=(listNode *)node;
        view->next=(listNode *)node; 
        return true;
    }else if(tmp==pos)
        return rlist::listAppend(node);
    else 
        return false;
}

bool rlist::listDelNode(int pos){
    long tmp=rlist::listLength();
    pos--;
    if(tmp>pos){
        view=rl->head;
        for(int i=0;i!=pos;i++)
            view=view->next;
        rlist::_free((void *)view);
        return true;
    } 
    return false;
}
 
void rlist::listDel(){
      view=rl->head;
      listNode *tmp=NULL;
      while(view!=NULL){
            tmp=view;
            view=view->next;
            delete tmp;
      }
}
