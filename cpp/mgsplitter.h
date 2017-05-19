/*
 ** $Id$
 **
 ** mgspliter.h:
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
 **
 ** All rights reserved by Feynman Software.
 **
 ** Current maintainer: dongjunjie
 **
 ** Create date: 2010-08-22
 */

#ifndef MINIGUI_SPLITER_H
#define MINIGUI_SPLITER_H

#include "mglayout.h"

class MGSplitter : public MGVisual {
protected:
    unsigned int m_flags : 16;
    int m_pos : 16;
    int m_p1 : 16;
    int m_p2 : 16;
    MGVisual* m_parent;

public:
    enum {
        HORZ = 0,
        VERT,
        NEEDRESTORE = 0x2,
        FLAG_SHIFT = 3,
    };

    MGSplitter(MGVisual* parent, int direction)
    {
        m_parent = parent;
        SetDirection(direction);
    }
    ~MGSplitter() {}

    void SetDirection(int direction)
    {
        if (direction == HORZ)
            m_flags &= ~VERT;
        else
            m_flags |= VERT;
    }

    int GetDirection() { return m_flags & VERT; }

    MGVisual* GetParentOwner() { return m_parent; }
    HWND GetOwnerWindowHandle()
    {
        if (m_parent)
            return m_parent->GetOwnerWindowHandle();
        return 0;
    }

    BOOL IsSplitter() { return TRUE; }

    BOOL GetBound(PRECT prc);
    BOOL SetBound(int x, int y, int width, int height, BOOL bUpdate = TRUE);

    BOOL ProcessMouseMessage(int iMsg, int x, int y, DWORD flags, int* pret);
    void Paint(HDC hdc);

private:
    virtual BOOL AutoRestore() { return FALSE; }
    virtual void SetRestorePos(int pos) {}
    virtual BOOL IsInRestoreArea(int x, int y) { return FALSE; }
    virtual void UpdateRestoreArea() {}
    virtual void Restore(BOOL bRestore = TRUE) {}

    void updateCursor()
    {
        SetCursor(
            GetSystemCursor(GetDirection() == HORZ ? IDC_SIZENS : IDC_SIZEWE));
    }

private:
};

class MGSplitterContainer : public MGSplitter {
protected:
    MGVisual* m_visual;
    int m_restore;

    enum { POS_FLAG = (1 << (FLAG_SHIFT)) };

public:
    enum { POS_BEFORE = 0,
        POS_AFTER };

    MGSplitterContainer(MGVisual* parent, MGVisual* visual, int spliter_direction,
        int splitter_pos);

    ~MGSplitterContainer()
    {
        if (m_visual)
            delete m_visual;
    }

    void SetPos(int pos)
    {
        if (pos == POS_BEFORE)
            m_flags &= ~POS_FLAG;
        else
            m_flags |= POS_FLAG;
    }

    int GetPos() { return (m_flags & POS_FLAG) == 0 ? POS_BEFORE : POS_AFTER; }

    BOOL SetBound(int x, int y, int width, int height, BOOL bUpdate = TRUE);
    BOOL GetBound(PRECT prc);

    BOOL ProcessMouseMessage(int iMsg, int x, int y, DWORD flags, int* pret);
    void Paint(HDC hdc);

protected:
    BOOL AutoRestore() { return TRUE; }
    void SetRestorePos(int pos)
    {
        RECT rc;
        GetBound(&rc);
        if (GetDirection() == HORZ)
            m_restore = (GetPos() == POS_AFTER ? (pos - rc.top) : (rc.bottom - pos));
        else
            m_restore = (GetPos() == POS_AFTER ? (pos - rc.left) : (rc.right - pos));
    }
    BOOL IsInRestoreArea(int x, int y);
    void UpdateRestoreArea();
    void Restore(BOOL bRestore = TRUE);

    void GetRestoreArea(PRECT prc);
    void PaintRestoreArea(HDC hdc);
};

#endif
