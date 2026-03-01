#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "lnk.h"

line_t 	*linePtr = NULL;
int    	errorCnt = 0;
char	*curFile;

////////////////////////////////////////////////////////////////////////////////
char *MALLOC(int length)
{
    char *p = (char *)malloc(length);

    if ( p == NULL )
    {
        printf("run out memory!\n");
        exit(99);
    }

    memset(p, 0, length);
    return p;
}


////////////////////////////////////////////////////////////////////////////////
char *dupStr(char *str)
{
    char *p = MALLOC(strlen (str) + 1);
    strcpy(p, str);
    return p;
}

////////////////////////////////////////////////////////////////////////////////
line_t *newLine(int type, item_t *items)
{
    line_t *p = (line_t *)MALLOC(sizeof(line_t));

    p->type  = type;
    p->items = items;
    p->fname = dupStr(curFile);// (cur_file->name);
    return p;
}

////////////////////////////////////////////////////////////////////////////////
line_t *endLine(line_t *lp)
{
	while ( lp->next != NULL )
		lp = lp->next;

	return lp;
}

////////////////////////////////////////////////////////////////////////////////
void appendLine(line_t **head, line_t *node)
{
    line_t *tmp = *head;

    if ( tmp == NULL )
    {
        *head = node;
        return;
	}

    while ( tmp->next != NULL )
        tmp = tmp->next;

    tmp->next = node;
}

////////////////////////////////////////////////////////////////////////////////
void freeLine(line_t *lp)
{
	if ( lp == NULL )
		return;

	freeItems(lp->items);
    if ( lp->src ) free(lp->src);
    if ( lp->fname ) free(lp->fname);
    free(lp);
}

////////////////////////////////////////////////////////////////////////////////
void freeLines(line_t *lp)
{
	while ( lp != NULL )
	{
		line_t *tmp = lp->next;

		freeLine(lp);
		lp = tmp;
	}
}

int lineSize(line_t *lp)
{
	if ( lp == NULL )	return 0;

	int size = 0;
	item_t *ip;
	for (ip = lp->items; ip; ip = ip->next)
	{
		switch ( lp->type )
		{
			case 'B':
				size += ip->size;
				break;

			case 'R':
				if ( ip->type == TYPE_VALUE )
					return ip->data.val;
				break;

			case 'J':
			case 'U':
			case 'V':
				return lp->insert;
		}
	}
	return size;
}

////////////////////////////////////////////////////////////////////////////////
void freeItems(item_t *ip)
{
    while ( ip != NULL )
    {
        item_t *tmp = ip;
        ip = ip->next;
        freeItem(tmp);
    }
}


////////////////////////////////////////////////////////////////////////////////
void freeItem(item_t *ip)
{
    if ( ip->type == TYPE_SYMBOL ||
         ip->type == TYPE_STRING )
        free (ip->data.str);

    free(ip);
}

////////////////////////////////////////////////////////////////////////////////
item_t *newItem(int type)
{
    item_t *p = (item_t *)MALLOC(sizeof(item_t));
    p->type   = type;
    return p;
}

////////////////////////////////////////////////////////////////////////////////
item_t *valItem(long val)
{
    item_t *p   = newItem(TYPE_VALUE);
    p->data.val = val;
    return p;
}

////////////////////////////////////////////////////////////////////////////////
item_t *strItem(char *str)
{
    item_t *p   = newItem(TYPE_STRING);
    p->data.str = dupStr(str);
    return p;
}

////////////////////////////////////////////////////////////////////////////////
item_t *symItem(char *sym)
{
    item_t *p   = newItem(TYPE_SYMBOL);
    p->data.str = dupStr(sym);
    return p;
}

////////////////////////////////////////////////////////////////////////////////
item_t *appendItem(item_t *head, item_t *node)
{
    item_t *tmp = head;

    if ( tmp == NULL )
        return node;

    while ( tmp->next != NULL )
        tmp = tmp->next;

    tmp->next = node;
    return head;
}

////////////////////////////////////////////////////////////////////////////////
item_t *cloneItem(item_t *iptr)
{
	item_t *ip;

	if ( iptr == NULL )
		return NULL;

	ip = newItem(iptr->type);
	ip->left  = cloneItem(iptr->left);
	ip->right = cloneItem(iptr->right);

	switch ( iptr->type )
	{
		case TYPE_VALUE:
			ip->data.val = iptr->data.val;
			break;

		case TYPE_STRING:
		case TYPE_SYMBOL:
			ip->data.str = dupStr(iptr->data.str);
			break;
	}
	return ip;
}

////////////////////////////////////////////////////////////////////////////////
int itemCount(line_t *lp)
{
    int n = 0;
	item_t *ip;
    for (ip = lp->items; ip != NULL; ip = ip->next) n++;
    return n;
}

////////////////////////////////////////////////////////////////////////////////
item_t *itemPtr(line_t *lp, int index)
{
	item_t *ip = lp->items;
    while ( ip != NULL && index-- ) ip = ip->next;
    return ip;
}

char *skipSP(char *s)
{
	while ( s && (*s == ' ' || *s == '\t') )
		s++;

	return s;
}