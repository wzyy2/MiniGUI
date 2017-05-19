/*
 ** $Id$
 **
 ** mglayout.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-08-21 
 */

#ifndef MINIGUI_LAYOUT_H
#define MINIGUI_LAYOUT_H

#include "mgvisual.h"

struct MGLength {
    MGLength()
    {
        type = 0;
        len = 0;
    }
    MGLength(int type, int len)
    {
        this->type = type & 0x3;
        this->len = len & 0x3FF;
    }
    unsigned short type : 2;
    unsigned short len : 14;

    enum {
        FIXED = 0,
        PERCENT,
        AUTOFIT,
        MORE
    };
};

class MGLayout : public MGVisual {
protected:
    short m_x;
    short m_y;
    short m_width;
    short m_height;

    unsigned int m_margin : 8;
    unsigned int m_pending : 8;
    unsigned int m_space : 8;
    unsigned int m_align : 2;
    unsigned int m_valign : 2;
    unsigned int m_flags : 4;

    MGVisual* m_parent;

public:
    enum {
        ALIGN_LEFT = 0,
        ALIGN_CENTER,
        ALIGN_RIGHT,

        VALIGN_TOP = 0,
        VALIGN_MIDDLE,
        VALIGN_BOTTOM
    };

    MGLayout()
    {
        m_parent = NULL;
        m_x = m_y = m_width = m_height = 0;
        m_margin = m_pending = 0;
        m_flags = 0;
        m_align = ALIGN_CENTER;
        m_valign = VALIGN_MIDDLE;
    }

    MGLayout(MGVisual* parent, int margin = 0, int pending = 0, int space = 0, int align = 0, int valign = 0)
    {
        m_parent = parent;
        m_margin = margin & 0xFF;
        m_pending = pending & 0xFF;
        m_align = align & 0x3;
        m_valign = valign & 0x3;
        m_space = space & 0xFF;
        m_flags = 0;
    }

    ~MGLayout() {}

    virtual void ReLayout() = 0;

    virtual BOOL SetBound(int x, int y, int width, int height, BOOL bUpdate = TRUE)
    {
        m_x = (short)x;
        m_y = (short)y;
        m_width = (short)width;
        m_height = (short)height;
        ReLayout();
        return TRUE;
    }

    virtual BOOL GetBound(PRECT prc)
    {
        if (!prc)
            return FALSE;
        prc->left = m_x;
        prc->top = m_y;
        prc->right = m_x + m_width;
        prc->bottom = m_y + m_height;
        return TRUE;
    }

    int GetWidth() { return m_width; }
    int GetHeight() { return m_height; }

    int GetMargin() { return m_margin; }
    int GetPending() { return m_pending; }
    int GetAlign() { return m_align; }
    int GetVAlign() { return m_valign; }

    MGVisual* GetParentOwner() { return m_parent; }

    HWND GetOwnerWindowHandle()
    {
        return m_parent ? m_parent->GetOwnerWindowHandle() : HWND_INVALID;
    }

    BOOL IsLayout() { return TRUE; }

protected:
    int CalcAlgin(int length, int need, int algin)
    {
        if (length == need)
            return 0;

        switch (algin) {
        case ALIGN_LEFT:
            return 0;
        case ALIGN_CENTER:
            return (length - need) / 2;
        case ALIGN_RIGHT:
            return length - need;
        }
        return 0;
    }
};

struct MGLayoutCell {
    MGLayoutCell()
    {
        visual = NULL;
        minWidth = maxWidth = 0;
        minHeight = maxHeight = 0;
        prev = next = NULL;
    }
    MGLayoutCell(MGVisual* v, MGLength width, MGLength height, int minWidth, int maxWidth, int minHeight, int maxHeight)
    {
        visual = v;
        this->width = width;
        this->height = height;
        this->minWidth = minWidth;
        this->maxWidth = maxWidth;
        this->minHeight = minHeight;
        this->maxHeight = maxHeight;
        prev = next = NULL;
    }
    ~MGLayoutCell()
    {
        //TODO delete visual
        if (visual)
            delete visual;
    }
    MGVisual* visual;
    MGLength width;
    unsigned short minWidth, maxWidth;
    MGLength height;
    unsigned short minHeight, maxHeight;
    MGLayoutCell *prev, *next;
    static MGLayoutCell* Append(MGLayoutCell* head, MGLayoutCell* cell);
    static void Free(MGLayoutCell* head);

    void Paint(HDC hdc)
    {
        if (visual)
            visual->Paint(hdc);
    }
};

#endif
