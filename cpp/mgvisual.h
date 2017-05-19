/*
 ** $Id$
 **
 ** mgvisual.h:
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
 **
 ** All rights reserved by Feynman Software.
 **
 ** Current maintainer: dongjunjie
 **
 ** Create date: 2010-08-21
 */

#ifndef MINIGUI_VISUAL_H
#define MINIGUI_VISUAL_H

#include "mgcomm.h"

class MGQuery;

class MGVisual {
public:
    virtual ~MGVisual() {}

    virtual BOOL SetBound(int x, int y, int width, int height,
        BOOL bUpdate = TRUE)
        = 0;
    BOOL SetBound(const RECT* prc, BOOL bUpdate = TRUE)
    {
        if (!prc)
            return FALSE;
        return SetBound(prc->left, prc->top, RECTWP(prc), RECTHP(prc), bUpdate);
    }

    virtual BOOL GetBound(PRECT prc) = 0;

    virtual int GetWidth()
    {
        RECT rc;
        if (GetBound(&rc))
            return RECTW(rc);
        return 0;
    }
    virtual int GetHeight()
    {
        RECT rc;
        if (GetBound(&rc))
            return RECTH(rc);
        return 0;
    }

    virtual MGVisual* GetParentOwner() = 0;
    virtual HWND GetOwnerWindowHandle() = 0;

    virtual BOOL IsMGWnd() { return FALSE; }
    virtual BOOL IsLayout() { return FALSE; }
    virtual BOOL IsSplitter() { return FALSE; }

public:
    virtual BOOL OnChildrenQuery(MGQuery* pquery) { return FALSE; }
    virtual BOOL ProcessMouseMessage(int iMsg, int x, int y, DWORD flags,
        int* pret)
    {
        return FALSE;
    }
    virtual void Paint(HDC hdc) { return; }

protected:
    BOOL QueryParent(MGQuery* pquery)
    {
        if (!pquery)
            return FALSE;
        if (GetParentOwner())
            return GetParentOwner()->OnChildrenQuery(pquery);
        return FALSE;
    }

    BOOL PtInBound(int x, int y)
    {
        RECT rc;
        return (GetBound(&rc) && PtInRect(&rc, x, y));
    }
};

class MGQuery {
protected:
    MGVisual* m_sender;

public:
    MGQuery() { m_sender = NULL; }
    MGQuery(MGVisual* sender)
        : m_sender(sender)
    {
    }
    virtual ~MGQuery() {}

    virtual const char* GetQueryName() = 0;

    MGVisual* GetSender() { return m_sender; }
    void SetSender(MGVisual* sender) { m_sender = sender; }

    BOOL Is(const char* name);

    virtual void Set(const char* propName, void* value) {}
    virtual void* Get(const char* propName) { return NULL; }
};

#define MGQUERY_GET_PROP_MEMBER(strPropName, _prop_name, _member) \
    if (strcasecmp(strPropName, _prop_name) == 0)                 \
        return (void*)(_member);

#define MGQUERY_GET_PROP_MEMBER_PTR(strPropName, _prop_name, _member) \
    if (strcasecmp(strPropName, _prop_name) == 0)                     \
        return (void*)(&_member);

#define MGQUERY_SET_PROP_MEMBER(strPropName, _prop_name, _member, _type, \
                                _value)                                  \
    if (strcasecmp(strPropName, _prop_name) == 0)                        \
    _member = ((_type)(_value))

#define MGQUERY_SET_PROP_MEMBER_PTR(strPropName, _prop_name, _member, _type, \
                                    _value)                                  \
    if (strcasecmp(strPropName, _prop_name) == 0)                            \
    _member = *((_type*)(_value))

#endif
