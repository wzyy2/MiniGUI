/*
 ** $Id$
 **
 ** mgvisual.cpp:
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
 **
 ** All rights reserved by Feynman Software.
 **
 ** Current maintainer: dongjunjie
 **
 ** Create date: 2010-08-25
 */

#include "mgvisual.h"

BOOL MGQuery::Is(const char* name)
{
    if (name == NULL)
        return FALSE;
    if (name[0] == '*')
        return TRUE;

    const char* query_name = GetQueryName();

    if (!query_name)
        return FALSE;

    while (*query_name && *name) {
        if (*name == '*')
            return TRUE;

        while (*query_name && *query_name != '.' && *name && *name != '.' && *query_name == *name) {
            query_name++;
            name++;
        }

        if (*query_name == '\0' && *name == '\0')
            return TRUE;
        else if (*query_name == '\0' || *name == '\0' || *query_name != *name)
            return FALSE;

        query_name++;
        name++;
    }

    return TRUE;
}
