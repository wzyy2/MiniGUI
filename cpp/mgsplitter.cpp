/*
 ** $Id$
 **
 ** mgsplitter.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-08-25 
 */

#include "mgsplitter.h"

#define PROP_LEFT_OBJ "left"
#define PROP_TOP_OBJ "top"
#define PROP_RIGHT_OBJ "right"
#define PROP_BOTTOM_OBJ "bottom"
#define PROP_RANGE "range"
#define PROP_POS "pos"
#define PROP_ISHORZ "horz"

//define the special info of spliter
struct MGSplitterInfo {
    MGVisual* obj_before;
    MGVisual* obj_after;
    RECT rc_limit;
    int cur_pos;
    HDC hdc;
    HWND hwnd;
    int old_x, old_y;
    int is_horz;

    MGSplitterInfo()
    {
        Clear();
    }

    void Clear()
    {
        obj_before = obj_after = NULL;
        memset(&rc_limit, 0, sizeof(rc_limit));
        cur_pos = 0;
        hdc = 0;
        hwnd = 0;
        old_x = old_y = -1;
        is_horz = 0;
    }

    void Start(HWND hwnd, int x, int y)
    {
        this->hwnd = hwnd;
        SetCapture(hwnd);
        hdc = GetClientDC(hwnd);
        SetRasterOperation(hdc, ROP_XOR);
        old_x = x;
        old_y = y;
        drawSplitter(x, y);
    }
    void Stop()
    {
        drawSplitter(old_x, old_y);
        ReleaseDC(hdc);
        ReleaseCapture();
        Clear();
    }

    void Update(int x, int y)
    {
        int pos = posFromPoint(x, y);
        if (pos == cur_pos)
            return;
        drawSplitter(old_x, old_y);
        drawSplitter(x, y);
        old_x = x;
        old_y = y;
        cur_pos = pos;
    }

    void* Get(const char* strPropName)
    {
        if (!strPropName)
            return NULL;
        MGQUERY_GET_PROP_MEMBER(strPropName, PROP_LEFT_OBJ, obj_before);
        MGQUERY_GET_PROP_MEMBER(strPropName, PROP_TOP_OBJ, obj_before);
        MGQUERY_GET_PROP_MEMBER(strPropName, PROP_RIGHT_OBJ, obj_after);
        MGQUERY_GET_PROP_MEMBER(strPropName, PROP_BOTTOM_OBJ, obj_after);
        MGQUERY_GET_PROP_MEMBER_PTR(strPropName, PROP_RANGE, rc_limit);
        MGQUERY_GET_PROP_MEMBER(strPropName, PROP_POS, cur_pos);
        MGQUERY_GET_PROP_MEMBER(strPropName, PROP_ISHORZ, is_horz);
        return NULL;
    }

    void Set(const char* strPropName, void* value)
    {
        if (!strPropName)
            return;
        MGQUERY_SET_PROP_MEMBER(strPropName, PROP_LEFT_OBJ, obj_before, MGVisual*, value);
        MGQUERY_SET_PROP_MEMBER(strPropName, PROP_TOP_OBJ, obj_before, MGVisual*, value);
        MGQUERY_SET_PROP_MEMBER(strPropName, PROP_RIGHT_OBJ, obj_after, MGVisual*, value);
        MGQUERY_SET_PROP_MEMBER(strPropName, PROP_BOTTOM_OBJ, obj_after, MGVisual*, value);
        MGQUERY_SET_PROP_MEMBER_PTR(strPropName, PROP_RANGE, rc_limit, RECT, value);
        MGQUERY_SET_PROP_MEMBER(strPropName, PROP_POS, cur_pos, int, (long)value);
    }

    BOOL posInEdage();
    int GetMinPos() { return is_horz ? rc_limit.top : rc_limit.left; }
    int GetMaxPos() { return is_horz ? rc_limit.bottom : rc_limit.right; }
private:
    void drawSplitter(int x, int y);
    int posFromPoint(int x, int y);
};

#define SPLITER_SIZE 4

void MGSplitterInfo::drawSplitter(int x, int y)
{
    int bx, by, bw, bh;
    if (is_horz) {
        bx = rc_limit.left;
        by = posFromPoint(x, y) - SPLITER_SIZE / 2;
        bw = RECTW(rc_limit);
        bh = SPLITER_SIZE;
    } else {
        bx = posFromPoint(x, y) - SPLITER_SIZE / 2;
        by = rc_limit.top;
        bw = SPLITER_SIZE;
        bh = RECTH(rc_limit);
    }
    FillBox(hdc, bx, by, bw, bh);
}

int MGSplitterInfo::posFromPoint(int x, int y)
{
    int min = is_horz ? rc_limit.top : rc_limit.left;
    int max = is_horz ? rc_limit.bottom : rc_limit.right;
    int pos = is_horz ? y : x;

    if (pos < min)
        pos = min;
    else if (pos > max)
        pos = max;
    return pos;
}

BOOL MGSplitterInfo::posInEdage()
{
    int min = is_horz ? rc_limit.top : rc_limit.left;
    int max = is_horz ? rc_limit.bottom : rc_limit.right;

    return (cur_pos == min || cur_pos == max);
}

class MGSplitterQuery : public MGQuery {
    MGSplitterInfo* m_splitterInfo;
    int m_type;

public:
    MGSplitterQuery()
    {
        m_splitterInfo = NULL;
        m_type = -1;
    }

    enum {
        RANGE = 0,
        CHANGE
    };

    MGSplitterQuery(MGSplitter* sender, MGSplitterInfo* info, int type)
        : MGQuery(sender)
        , m_splitterInfo(info)
        , m_type(type)
    {
    }

    void SetType(int type) { m_type = type; }

    void Set(const char* str, void* value)
    {
        if (m_splitterInfo)
            m_splitterInfo->Set(str, value);
    }

    void* Get(const char* str)
    {
        if (m_splitterInfo)
            return m_splitterInfo->Get(str);
        return NULL;
    }

    const char* GetQueryName()
    {
        switch (m_type) {
        case RANGE:
            return "MGSplitter.Range";
        case CHANGE:
            return "MGSplitter.Change";
        }
        return NULL;
    }
};

void MGSplitter::Paint(HDC hdc)
{
    RECT rc;
    HWND hwnd = GetOwnerWindowHandle();
    DWORD color = GetWindowElementAttr(hwnd, WE_MAINC_THREED_BODY);
    const WINDOWINFO* win_info = GetWindowInfo(hwnd);
    const WINDOW_ELEMENT_RENDERER* rdr = win_info->we_rdr;
    if (!rdr)
        rdr = GetWindowRendererFromName("classic");
    MGSplitter::GetBound(&rc);

    SetBrushColor(hdc, DWORD2PIXEL(hdc, color));
    FillBox(hdc, rc.left, rc.top, RECTW(rc), RECTH(rc));
}

BOOL MGSplitter::ProcessMouseMessage(int iMsg, int x, int y, DWORD flags, int* pret)
{
    static MGSplitterInfo _splitter_info;
    static MGSplitter* _current_splitter = NULL;

    if (_current_splitter == NULL) {
        RECT rc;
        MGSplitter::GetBound(&rc);
        if (!PtInRect(&rc, x, y))
            return FALSE;
    } else if (_current_splitter != this)
        return FALSE;

    switch (iMsg) {
    case MSG_LBUTTONDOWN:
        if (AutoRestore() && IsInRestoreArea(x, y)) {
            Restore(m_flags & NEEDRESTORE);
        } else {
            //get the splitter_info
            _splitter_info.Clear();
            _splitter_info.is_horz = GetDirection() == HORZ;
            MGSplitterQuery query(this, &_splitter_info, MGSplitterQuery::RANGE);
            if (QueryParent(&query)) {
                _splitter_info.Start(GetOwnerWindowHandle(), x, y);
                updateCursor();
                _current_splitter = this;
            }
        }
        break;
    case MSG_LBUTTONUP:
        if (_current_splitter == this) {
            MGSplitterQuery query(this, &_splitter_info, MGSplitterQuery::CHANGE);
            QueryParent(&query);
            SetCursor(GetSystemCursor(IDC_ARROW));
            if (AutoRestore()) {
                if (!_splitter_info.posInEdage()) {
                    if (m_flags & NEEDRESTORE) {
                        m_flags &= ~NEEDRESTORE;
                        UpdateRestoreArea();
                    }
                    SetRestorePos(_splitter_info.cur_pos);
                } else {
                    m_flags |= NEEDRESTORE;
                    UpdateRestoreArea();
                }
            }
            _splitter_info.Stop();
            _current_splitter = NULL;
            break;
        }
    case MSG_MOUSEMOVE:
        if (_current_splitter == this) {
            updateCursor();
            ScreenToClient(_splitter_info.hwnd, &x, &y);
            _splitter_info.Update(x, y);
        } else {
            if (AutoRestore() && IsInRestoreArea(x, y)) {
                break;
            }
            updateCursor();
        }
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

BOOL MGSplitter::GetBound(PRECT prc)
{
    if (!prc)
        return FALSE;

    if (GetDirection() == HORZ) {
        prc->left = m_p1;
        prc->right = m_p2;
        prc->top = m_pos - SPLITER_SIZE / 2;
        prc->bottom = prc->top + SPLITER_SIZE;
    } else {
        prc->top = m_p1;
        prc->bottom = m_p2;
        prc->left = m_pos - SPLITER_SIZE / 2;
        prc->right = prc->left + SPLITER_SIZE;
    }
    return TRUE;
}

BOOL MGSplitter::SetBound(int x, int y, int width, int height, BOOL bUpdate)
{

    if (GetDirection() == HORZ) {
        m_p1 = x;
        m_p2 = x + width;
        m_pos = y + height / 2;
    } else {
        m_p1 = y;
        m_p2 = y + height;
        m_pos = x + width / 2;
    }

    if (bUpdate) {
        RECT rc;
        MGSplitter::GetBound(&rc);
        InvalidateRect(GetOwnerWindowHandle(), &rc, TRUE);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////
MGSplitterContainer::MGSplitterContainer(MGVisual* parent, MGVisual* visual, int spliter_direction, int splitter_pos)
    : MGSplitter(parent, spliter_direction)
    , m_visual(visual)
{
    SetPos(splitter_pos);

    if (m_visual) {
        RECT rc;
        m_visual->GetBound(&rc);
        if (GetDirection() == HORZ) {
            if (GetPos() == POS_BEFORE) {
                rc.bottom = rc.top + SPLITER_SIZE;
            } else {
                rc.top = rc.bottom - SPLITER_SIZE;
            }
        } else {
            if (GetPos() == POS_BEFORE) {
                rc.right = rc.left + SPLITER_SIZE;
            } else {
                rc.left = rc.right - SPLITER_SIZE;
            }
        }
        MGSplitter::SetBound(rc.left, rc.top, RECTW(rc), RECTH(rc));
    }

    m_restore = m_pos;
}

BOOL MGSplitterContainer::SetBound(int x, int y, int width, int height, BOOL bUpdate)
{
    int direction = GetDirection();
    int splitter_pos = GetPos();
    BOOL bRestore = FALSE;

    if (direction == HORZ) {
        if (height < SPLITER_SIZE)
            height = SPLITER_SIZE;
        if (splitter_pos == POS_BEFORE) {
            MGSplitter::SetBound(x, y, width, SPLITER_SIZE, bUpdate);
            if (m_visual)
                m_visual->SetBound(x, y + SPLITER_SIZE, width, height - SPLITER_SIZE, bUpdate);
        } else {
            MGSplitter::SetBound(x, y + height - SPLITER_SIZE, width, SPLITER_SIZE, bUpdate);
            if (m_visual)
                m_visual->SetBound(x, y, width, height - SPLITER_SIZE, bUpdate);
        }

        bRestore = (height == SPLITER_SIZE);

    } else {
        if (width < SPLITER_SIZE)
            width = SPLITER_SIZE;
        if (splitter_pos == POS_BEFORE) {
            MGSplitter::SetBound(x, y, SPLITER_SIZE, height, bUpdate);
            if (m_visual)
                m_visual->SetBound(x + SPLITER_SIZE, y, width - SPLITER_SIZE, height, bUpdate);
        } else {
            MGSplitter::SetBound(x + width - SPLITER_SIZE, y, SPLITER_SIZE, height, bUpdate);
            if (m_visual)
                m_visual->SetBound(x, y, width - SPLITER_SIZE, height, bUpdate);
        }

        bRestore = (width == SPLITER_SIZE);
    }

    if (bRestore) {
        m_flags |= NEEDRESTORE;
    } else {
        m_flags &= ~NEEDRESTORE;
        SetRestorePos(m_pos);
    }

    return TRUE;
}

BOOL MGSplitterContainer::GetBound(PRECT prc)
{
    RECT rcVisual;
    RECT rcSpliter;
    int direction = GetDirection();
    int splitter_pos = GetPos();

    MGSplitter::GetBound(&rcSpliter);
    if (m_visual)
        m_visual->GetBound(&rcVisual);
    else
        rcVisual = rcSpliter;

    if (direction == HORZ) {
        prc->left = rcSpliter.left;
        prc->right = rcSpliter.right;
        if (splitter_pos == POS_BEFORE) {
            prc->top = rcSpliter.top;
            prc->bottom = rcVisual.bottom;
        } else {
            prc->top = rcVisual.top;
            prc->bottom = rcSpliter.bottom;
        }
    } else {
        prc->top = rcSpliter.top;
        prc->bottom = rcSpliter.bottom;
        if (splitter_pos == POS_BEFORE) {
            prc->left = rcSpliter.left;
            prc->right = rcVisual.right;
        } else {
            prc->left = rcVisual.left;
            prc->right = rcSpliter.right;
        }
    }

    return TRUE;
}

BOOL MGSplitterContainer::ProcessMouseMessage(int iMsg, int x, int y, DWORD flags, int* pret)
{
    if (MGSplitter::ProcessMouseMessage(iMsg, x, y, flags, pret))
        return TRUE;
    if (m_visual)
        return m_visual->ProcessMouseMessage(iMsg, x, y, flags, pret);
    return FALSE;
}

void MGSplitterContainer::Paint(HDC hdc)
{
    MGSplitter::Paint(hdc);
    PaintRestoreArea(hdc);
    if (m_visual)
        m_visual->Paint(hdc);
}

void MGSplitterContainer::Restore(BOOL bRestore)
{
    MGSplitterInfo splitterInfo;
    splitterInfo.Clear();
    splitterInfo.is_horz = GetDirection() == HORZ;
    MGSplitterQuery query(this, &splitterInfo, MGSplitterQuery::RANGE);
    if (QueryParent(&query)) {
        if (bRestore) {
            RECT rc;
            GetBound(&rc);
            if (GetDirection() == HORZ)
                splitterInfo.cur_pos = (GetPos() == POS_AFTER ? (rc.top + m_restore) : (rc.bottom - m_restore));
            else
                splitterInfo.cur_pos = (GetPos() == POS_AFTER ? (rc.left + m_restore) : (rc.right - m_restore));

        } else
            splitterInfo.cur_pos = (GetPos() == POS_AFTER ? splitterInfo.GetMinPos() : splitterInfo.GetMaxPos());

        query.SetType(MGSplitterQuery::CHANGE);
        QueryParent(&query);
    }
}

void MGSplitterContainer::GetRestoreArea(PRECT prc)
{
    MGSplitter::GetBound(prc);

    if (GetDirection() == HORZ) {
        prc->left = (prc->left + prc->right) / 2 - SPLITER_SIZE;
        prc->right = prc->left + SPLITER_SIZE * 2;
    } else {
        prc->top = (prc->top + prc->bottom) / 2 - SPLITER_SIZE;
        prc->bottom = prc->top + SPLITER_SIZE * 2;
    }
}

BOOL MGSplitterContainer::IsInRestoreArea(int x, int y)
{
    RECT rc;
    GetRestoreArea(&rc);

    return PtInRect(&rc, x, y);
}

void MGSplitterContainer::UpdateRestoreArea()
{
    RECT rc;
    GetRestoreArea(&rc);
    InvalidateRect(GetOwnerWindowHandle(), &rc, TRUE);
}

void MGSplitterContainer::PaintRestoreArea(HDC hdc)
{
    RECT rc;
    GetRestoreArea(&rc);
    int direction = GetDirection();
    int pos_type = GetPos();
    BOOL bNeedRestore = m_flags & NEEDRESTORE;
    HWND hwnd = GetOwnerWindowHandle();
    DWORD status = 0;

    if (direction == HORZ) {
        if ((pos_type == POS_BEFORE && bNeedRestore)
            || (pos_type == POS_AFTER && !bNeedRestore))
            status = LFRDR_ARROW_LEFT;
        else
            status = LFRDR_ARROW_RIGHT;
    } else {
        if ((pos_type == POS_BEFORE && bNeedRestore)
            || (pos_type == POS_AFTER && !bNeedRestore))
            status = LFRDR_ARROW_UP;
        else
            status = LFRDR_ARROW_DOWN;
    }

    DWORD color = GetWindowElementAttr(hwnd, WE_FGC_WINDOW);
    const WINDOWINFO* win_info = GetWindowInfo(hwnd);
    const WINDOW_ELEMENT_RENDERER* rdr = win_info->we_rdr;
    if (!rdr)
        rdr = GetWindowRendererFromName("classic");
    rdr->draw_arrow(hwnd, hdc, &rc, color, status);
}
