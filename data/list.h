#ifndef _LIST_H_
#define _LIST_H_
#include<string>

typedef struct listNode{
	struct listNode *prev;
	struct listNode *next;
	void *value;	
};

typedef struct Rlist{
	listNode *head;
	listNode *tail;
	unsigned long len;
	void *(*dup)(void *ptr);
	void (*_free)(void *ptr);
	int (*match)(void *ptr,void *key);	
};

class rlist{
	struct Rlist *rl;
    unsigned long length;
	listNode *view;
	void *dupNode(void *ptr);
	void _free(void *ptr);
	int match(void *ptr,void *key);
	public:
	rlist();
	unsigned long listLength();
	listNode *listFirst();
	listNode *listLast();
	listNode *listPrevNode(void *node);
	listNode *listNextNode(void *node);
	void *listNodeValue(void *node);
	void *listSearchKey(void *value);
    int listIndex(void *value);    
    bool listAppend(void *node);
    bool listInsertNode(void *node,unsigned long pos);
    bool listDelNode(int pos);
    void listDel();
};

#endif
