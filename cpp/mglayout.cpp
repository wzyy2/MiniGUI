/*
 ** $Id$
 **
 ** mglayout.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-08-21 
 */

#include "mglayout.h"

MGLayoutCell* MGLayoutCell::Append(MGLayoutCell* head, MGLayoutCell* cell)
{
    if (!head && !cell)
        return NULL;

    if (!cell)
        return head;

    if (!head) {
        cell->prev = cell->next = NULL;
        return cell;
    }

    MGLayoutCell* c = head;
    while (c && c->next)
        c = c->next;
    c->next = cell;
    cell->prev = c;
    cell->next = NULL;
    return head;
}

void MGLayoutCell::Free(MGLayoutCell* head)
{
    if (!head)
        return;

    while (head) {
        MGLayoutCell* c = head;
        head = head->next;
        delete c;
    }
}
