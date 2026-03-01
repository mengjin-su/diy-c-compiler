#ifndef _NLIST_H
#define _NLIST_H
#include "common.h"

class Nnode {
	public:
		Nnode(int _type, char *_name=NULL, node *np1=NULL, node *np2=NULL);
		~Nnode();
	
		int     type;			// Nnode type - DEFINE/ENUM
		char   *name;			// Nnode name
		node   *np[2];			// parameters used in definition
		attrib *attr;			// attributes
		Nnode  *next;			// link pointer to the same block
		Nnode  *parent;			// link pointer to the parent
		
		int 	nops(void) 		{ return (np[0] == NULL)? 0:
										 (np[1] == NULL)? 1: 2; }
};

class Nlist {
	private:
		Nnode *list;
		Nnode *find(char *name, int type);

	public:
		Nlist ();
		~Nlist();
		void    addLayer(void);
		void    delLayer(void);
		Nnode  *add(char *name, int type, node *np1=NULL, node *np2=NULL);
		void    del(char *name);
		Nnode  *search(char *name, int type=0);
		attrib *search(attrib *attr);
};

#endif
