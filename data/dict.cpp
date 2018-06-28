#include"dict.h"
dic::dic(){
    //dic::seed=0xabcd;
    dic::dictCreate();
}

//used for dictType
unsigned int dic::murmurhash2(const void *key){

    const unsigned int seed=0xabcd;
    const unsigned int m=0x5bd1e995;
    const int r=24;

    int len=strlen((const char *)((const robj *)key)->ptr);

    unsigned int h=seed*len;

    const char *data=(const char *)((const robj *)key)->ptr;

    while(len>=4){
        unsigned int k=*(unsigned int *)data;

        k*=m;
        k^=k>>r;
        k*=m;

        h*=m;
        h^=k;

        data+=4;
        len-=4;
    }

    switch(len){
        case 3:h^=data[2]<<16;
        case 2:h^=data[1]<<8;
        case 1:h^=data[0];
               h*=m;
    };

    h^=h>>13;
    h*=m;
    h^=h>>15;

    return h;
}

void * dic::KeyDup(const void *key){
    const void * tmp=key;
    return ((const robj *)tmp)->ptr;
}

void * dic::ValDup(const void *obj){
    const void *tmp=obj;
    return ((const robj *)tmp)->ptr;
}

int dic::keyCompare(const void *key1,const void *key2){
    if(strcmp((const char *)((const robj *)key1)->ptr,(const char *)((const robj *)key2)->ptr)==0)
        return 0;
    else return -1;
}

void dic::KeyDestructor(void *key){
    delete (char *)key;
}

//cant solve how delete void*


int dic::dictCreate(){
    dict dt;
    dt.rehashidx=-1;
    dictht dct1,dct2;
    dct1.size=INITSIZE;
    dct1.sizemask=dct1.size-1;
    dct1.used=0;
    dct1.table=new dictEntry*[dct1.size];
    dct2.size=0;
    dct2.sizemask=-1;
    dct2.used=0;
    dt.ht[0]=dct1;
    dt.ht[1]=dct2;
    dc.push_back(dt);
    //return dc.end()-1-dc.begin();
}

//key and obj supposed to be a robj
int dic::dictAdd(int dictidx,void *key,void *obj){
    dc[dictidx].type->hashFunction=dic::murmurhash2;
    int hash=dc[dictidx].type->hashFunction(key);
    int index=hash&dc[dictidx].ht[0].sizemask;
    if(dic::dict_find(dictidx,0,key)==NULL){
        if(dc[dictidx].ht[0].table[index]==NULL){
            dc[dictidx].ht[0].table[index]->key=key;
            dc[dictidx].ht[0].table[index]->v.val=obj;
            dc[dictidx].ht[0].table[index]->next=NULL;
        }else{
            dictEntry *tmp=dc[dictidx].ht[0].table[index];  
            while(tmp!=NULL)
                tmp=tmp->next;
            dictEntry *newEntry=NULL;
            newEntry->key=key;
            newEntry->v.val=obj;
            newEntry->next=NULL;
            tmp->next=newEntry;
        }
        dc[dictidx].ht[0].used++;
        if(dc[dictidx].ht[0].used/dc[dictidx].ht[0].size>=1)
            dic::rehash(dictidx);
        //if rehash

        return index;}  
    else return -1;//already in 
}

//find dictEntry from key
void * dic::dict_find(int dictidx,int tableidx,void *key){
    dictEntry *fetch=NULL; 
    for(unsigned int i=0;i<dc[dictidx].ht[tableidx].size;i++){
        fetch=dc[dictidx].ht[tableidx].table[i]; 
        while(fetch!=NULL)
        {
            if(strcmp((char *)((const robj *)fetch->key)->ptr,(char *)((const robj *)key)->ptr)==0)
                return (void *)fetch;
            fetch=fetch->next;
        }    
    }
    return NULL;
}

void * dic::dictFetchValue(int dictidx,void *key){
    dictEntry *fetch=NULL;
    if(dc[dictidx].rehashidx==-1){
        fetch=(dictEntry *)dic::dict_find(dictidx,0,key);
        return (fetch!=NULL)?fetch->v.val:fetch;
    }else{
        //if rehash cheak ht[1] after ht[0]
        fetch=(dictEntry *)dic::dict_find(dictidx,0,key);
        if(fetch!=NULL)
            return fetch->v.val;
        else if((fetch=(dictEntry *)dic::dict_find(dictidx,1,key))!=NULL)
            return fetch->v.val;
        return NULL;
    }
}

int dic::dictReplace(int dictidx,dictEntry newde){
    dictEntry *fetch=NULL;
    if(dc[dictidx].rehashidx==-1){    
        fetch=(dictEntry *)dic::dict_find(dictidx,0,newde.key);
        if(fetch!=NULL){
            fetch->v.val=newde.v.val;
            return 1;
        }else{
            int index=dic::dictAdd(dictidx,newde.key,newde.v.val);
            return index; 
        }
    }
    else{
        fetch=(dictEntry *)dic::dict_find(dictidx,0,newde.key);
        if(fetch!=NULL){
            fetch->v.val=newde.v.val;
            return 1;
        } else if((fetch=(dictEntry *)dic::dict_find(dictidx,1,newde.key))!=NULL){
            fetch->v.val=newde.v.val;
            return 1;
        }
        return dic::dictAdd(dictidx,newde.key,newde.v.val);
    }
}  

int dic::dictDelete(int dictidx,void *key){
    dictEntry *delEntry=NULL;
    if(dc[dictidx].rehashidx==-1){
        delEntry=(dictEntry *)dic::dict_find(dictidx,0,key);
        if(delEntry!=NULL){
            delete (std::string *)delEntry->key;
            delEntry->key=NULL;
            if(((robj *)delEntry->v.val)->objtype==_string)
            {
                delete (std::string *)delEntry->v.val;
            }else if(((robj *)delEntry->v.val)->objtype==_set){
                delete[] (std::string *)delEntry->v.val; 
            }else if(((robj *)delEntry->v.val)->objtype==_list){
                ((rlist *)delEntry->v.val)->listDel();
            }
            delEntry->v.val=NULL;
            delete delEntry;
            delEntry=NULL;
            return 1;
        }
    }else {
        delEntry=(dictEntry *)dic::dict_find(dictidx,0,key);
        if(delEntry!=NULL){
            delete delEntry;
            delEntry=NULL;
            return 1;
        }
        else if((delEntry=(dictEntry *)dic::dict_find(dictidx,1,key))!=NULL){
            if(delEntry!=NULL){
                delete delEntry;
                delEntry=NULL;
                return 1;
            } 
        }  
    }
    return -1;
}

std::vector<struct dict> dic::diclist(){
    return dc;
}

int dic::dictRelese(int dictidx){
    dictEntry *fetch=NULL;
    dictEntry *tmp=NULL;
    //relese ht
    for(unsigned int i=0;i<dc[dictidx].ht[0].size;i++){
        fetch=dc[dictidx].ht[0].table[i];
        while(fetch!=NULL){
            tmp=fetch;
            fetch=fetch->next;
            delete tmp;
            tmp=NULL;
        }
    }  
    if(dc[dictidx].rehashidx!=-1){
        for(unsigned int i=0;i<dc[dictidx].ht[1].size;i++){
            fetch=dc[dictidx].ht[1].table[i];
            while(fetch!=NULL){
                tmp=fetch;
                fetch=fetch->next;
                delete tmp;
                tmp=NULL;
            }
        }  
    } 
    //delete database
    dc.erase(dc.begin()+dictidx);

    return 1;
}  

int dic::rehash(int dictidx){
    int i=1,index;
    dictEntry *tmp=NULL;
    dc[dictidx].type->hashFunction=dic::murmurhash2;
    unsigned long newsize=dc[dictidx].ht[0].size*2;
    unsigned long tmpsize=pow(2,i);
    while(newsize>tmpsize){
        tmpsize=pow(2,++i);
    }
    newsize=tmpsize;

    dc[dictidx].ht[1].size=newsize;
    dc[dictidx].ht[1].sizemask=newsize-1;
    dc[dictidx].ht[1].table=new dictEntry*[newsize];

    dictEntry *fetch=NULL;
    for(unsigned int i=0;i<dc[dictidx].ht[0].size;i++){
        fetch=dc[dictidx].ht[0].table[i];
        while(fetch!=NULL){
            index=dc[dictidx].type->hashFunction(fetch->key)&dc[dictidx].ht[1].sizemask;
            tmp=dc[dictidx].ht[1].table[index];
            if(tmp==NULL)
                dc[dictidx].ht[1].table[index]=fetch;
            else
            {
                while(tmp->next!=NULL){
                    tmp=tmp->next;
                }
                tmp->next=fetch;
            }
            fetch=fetch->next;
        }
    }
    //ht[0] -> ht[1] done

    //relese ht[0]
    fetch=tmp=NULL;    
    for(unsigned int i=0;i<dc[dictidx].ht[0].size;i++){
        fetch=dc[dictidx].ht[0].table[i];
        while(fetch!=NULL){
            tmp=fetch;
            fetch=fetch->next;
            delete tmp;
            tmp=NULL;
        }
    }  

    //override =
    dc[dictidx].ht[0]=dc[dictidx].ht[1];


    dictht *newht=NULL;
    newht->table=NULL;
    newht->size=0;
    newht->sizemask=0;
    newht->used=0;
    dc[dictidx].ht[1]=*newht;

    return 1;
}
