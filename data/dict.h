#ifndef _DICT_H
#define _DICT_H
#include<vector>
#include<math.h>
#include<string.h>
#include"list.h"
#define INITSIZE 128

typedef enum  _type{
  _string,_list,_set    
};

typedef struct robj{
      enum _type objtype;
      void *ptr;
};

typedef struct dictEntry{
    void *key;//key usually string 
    union{//value ptr or int 
        void *val;
        int _i;   
    }v;
    struct dictEntry *next;
};

typedef struct dictType{
    unsigned int (*hashFunction)(const void *key);//murmurhash2
    void *(*keyDup)(void *privdata,const void *key);
    void *(*valDup)(void *privdata,const void *obj);
    int (*keyCompare)(void *privdata,const void *key1,const void *key2);
    void (*keyDestructor)(void *privdata,void *key);
    void (*valDestructor)(void *privdata,void *obj);
};

typedef struct dictht{
    dictEntry **table;
    unsigned long size;
    unsigned long sizemask;
    unsigned long used;
    dictht& operator=(const dictht& ht){
          this->size=ht.size;
          this->sizemask=ht.sizemask;
          this->used=ht.used;
          this->table=ht.table;
          return *this;
    }
};

typedef struct dict{
    dictType *type;
    void *privdata;
    dictht ht[2];
    int rehashidx;
};

class dic{
    std::vector<struct dict> dc;
    //static unsigned int  seed;
    int loadFactor;
    int rehashidx;//rehash signal

   public:
       dic();
       //type method
       static unsigned int murmurhash2(const void *key);
       void *dict_find(int dictidx,int tableidx,void *key);
       static void *KeyDup(/*void *privdata,*/const void *key);
       static void *ValDup(/*void *privdata,*/const void *obj);
       static int keyCompare(/*void *privdata,*/const void *key1,const void *key2);
       static void KeyDestructor(/*void *privdata,*/void *key);
      // void ValDestructor(/*void  *privdata,*/void *obj);
       
       //dict api
       int dictCreate();
       int dictAdd(int dictidx,void *key,void *obj);
       int dictReplace(int dictidx,dictEntry newde);
       void *dictFetchValue(int dictidx,void *key);
       void *dictGetRandomKey(int dictidx);
       int dictDelete(int dictidx,void *key);
       int dictRelese(int dictidx);
       std::vector<struct dict> diclist();

       //rehash
       int rehash(int dictidx);
};

#endif
