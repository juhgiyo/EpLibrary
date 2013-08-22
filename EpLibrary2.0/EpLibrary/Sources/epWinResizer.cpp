/*
/*! 
WinResizer for the EpLibrary

The MIT License (MIT)

Copyright (c) 2008-2013 Woong Gyu La <juhgiyo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

------------------------------------------------------------------------------

Part of this software is

DISCLAIMER

Auther: Mizan Rahman
Original publication location: http://www.codeproject.com/KB/dialog/WndResizer.aspx

This work is provided under the terms and condition described in The Code Project Open License (CPOL)
(http://www.codeproject.com/info/cpol10.aspx)

This disclaimer should not be removed and should exist in any reproduction of this work.
*/


#include "epWinResizer.h"
#pragma comment(lib,"UxTheme.lib")

using namespace epl;

static CMap<HWND, HWND, WinResizer *, WinResizer *> WndResizerData;


/////////////////////////////  WinResizer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WinResizer::WinResizer(void)
{
  m_pHookedWnd = NULL;
  m_pfnWndProc = NULL;
  root.Name = _T("_root");
  m_pCaptured = NULL;
  m_hOldCursor = NULL;
  m_splitterOffset = 0;

}

WinResizer::~WinResizer(void)
{

}
void WinResizer::GetTrueClientRect(CWnd * pWnd, CRect * prc)
{  
  int nMin = 0;
  int nMax = 0;
  int nCur = 0;

  pWnd->GetClientRect(prc);

  if ( (pWnd->GetStyle() & WS_HSCROLL) > 0)
  {
    pWnd->GetScrollRange(SB_HORZ, &nMin, &nMax);
    nCur = pWnd->GetScrollPos(SB_HORZ);
    prc->right = prc->left + nMax;
    prc->OffsetRect( -nCur , 0);
  }

  if ( (pWnd->GetStyle() & WS_VSCROLL) > 0)
  {
    pWnd->GetScrollRange(SB_VERT, &nMin, &nMax);
    nCur = pWnd->GetScrollPos(SB_VERT);
    prc->bottom = prc->top + nMax;
    prc->OffsetRect(0, -nCur);
  }  
}
void WinResizer::EnsureRootMinMax()
{
  if (root.Width() < root.MinSize.cx)
  {
     root.right = root.left + root.MinSize.cx;
  }
  if (root.Width() > root.MaxSize.cx)
  {
     root.right = root.left + root.MaxSize.cx;
  }

  if (root.Height() < root.MinSize.cy)
  {
     root.bottom = root.top + root.MinSize.cy;
  }
  if (root.Height() > root.MaxSize.cy)
  {
     root.bottom = root.top + root.MaxSize.cy;
  }
}

void WinResizer::OnSize(UINT nType, int cx, int cy)
{
  GetTrueClientRect(m_pHookedWnd, &root);
  EnsureRootMinMax();
  root.OnResized();
  ResizeUI(&root);
}
void WinResizer::OnScroll()
{
  GetTrueClientRect(m_pHookedWnd, &root);
  EnsureRootMinMax();
  root.OnResized();
  ResizeUI(&root);
}
BOOL WinResizer::Hook(CWnd * pParent)
{
  ASSERT( m_pHookedWnd == NULL );

  m_pHookedWnd = pParent;

  GetTrueClientRect(m_pHookedWnd, &root);
  
  root.m_pHookWnd = m_pHookedWnd;

  // create the resize gripper panel
  CRect rcResziGrip(&root);
  int cx = ::GetSystemMetrics(SM_CXHSCROLL);
  int cy = ::GetSystemMetrics(SM_CYVSCROLL);
  rcResziGrip.DeflateRect(root.Width() - cx, root.Height() - cy, 0, 0);
  CGripperPanel * pResizeGripper = new CGripperPanel(&rcResziGrip);
  pResizeGripper->SetAnchor( ANCHOR_RIGHT | ANCHOR_BOTTOM );
  pResizeGripper->Name = _T("_resizeGrip");
  root.AddChild( pResizeGripper );

  WndResizerData.SetAt(m_pHookedWnd->m_hWnd, this);

  m_pfnWndProc = (WNDPROC)::SetWindowLongPtr(m_pHookedWnd->m_hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
  return TRUE;
}
BOOL WinResizer::Hook(CWnd * pParent, CSize &  size)
{
  ASSERT( m_pHookedWnd == NULL );

  m_pHookedWnd = pParent;

  GetTrueClientRect(m_pHookedWnd, &root);
  root.right = root.left + size.cx;
  root.bottom = root.top + size.cy;
  root.m_pHookWnd = m_pHookedWnd;

  // create the resize gripper panel
  CRect rcResziGrip(&root);
  int cx = ::GetSystemMetrics(SM_CXHSCROLL);
  int cy = ::GetSystemMetrics(SM_CYVSCROLL);
  rcResziGrip.DeflateRect(root.Width() - cx, root.Height() - cy, 0, 0);
  CGripperPanel * pResizeGripper = new CGripperPanel(&rcResziGrip);
  pResizeGripper->SetAnchor( ANCHOR_RIGHT | ANCHOR_BOTTOM );
  pResizeGripper->Name = _T("_resizeGrip");
  root.AddChild( pResizeGripper );

  WndResizerData.SetAt(m_pHookedWnd->m_hWnd, this);

  m_pfnWndProc = (WNDPROC)::SetWindowLongPtr(m_pHookedWnd->m_hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
  return TRUE;
}

void WinResizer::ResizeUI(WinResizer::CPanel * pRoot)
{
  CPanelList panels;
  GetUIPanels(pRoot, &panels, FALSE);

  POSITION pos = NULL;

  if (panels.GetCount() > 0)
  {
    HDWP hDWP = ::BeginDeferWindowPos(panels.GetCount());
    ASSERT( hDWP != NULL);
    pos = panels.GetHeadPosition();
    while (pos != NULL)
    {
      CUIPanel * pPanel = (CUIPanel *) panels.GetNext(pos);

        ::DeferWindowPos(hDWP, m_pHookedWnd->GetDlgItem(pPanel->m_uID)->m_hWnd, NULL,
                      pPanel->left , pPanel->top , pPanel->Width(), pPanel->Height(),
                      SWP_NOACTIVATE | SWP_NOZORDER );

    }
    BOOL bOk = ::EndDeferWindowPos(hDWP);
     ASSERT( bOk );
    m_pHookedWnd->InvalidateRect(pRoot, FALSE);
  }

  panels.RemoveAll();
  GetUIPanels(pRoot, &panels, TRUE);
  pos = panels.GetHeadPosition();
  while (pos != NULL)
  {
    CUIPanel * pPanel = (CUIPanel *) panels.GetNext(pos);
    m_pHookedWnd->GetDlgItem(pPanel->m_uID)->MoveWindow(pPanel);
  }
}


void WinResizer::OnLButtonDown(UINT nFlags, CPoint point)
{
  UpdateSplitterOffset(point);
}
void WinResizer::OnMouseMove(UINT nFlags, CPoint point)
{
  if (m_pCaptured != NULL )
  {
    if ((nFlags & MK_LBUTTON) <= 0)
    {
      CPoint ptScreen = point;
      m_pHookedWnd->ClientToScreen(&ptScreen);
      HWND hWndFromPoint = ::WindowFromPoint(ptScreen);
      if (m_pCaptured->PtInRect( point ) == FALSE || hWndFromPoint != m_pHookedWnd->m_hWnd)
      {
        ::ReleaseCapture();      
        m_pCaptured = NULL;
        HCURSOR hCur = ::SetCursor(m_hOldCursor);
        ::DestroyCursor( hCur );
        m_hOldCursor = NULL;
      }
    }
  }

  if (m_pCaptured == NULL )
  {
    m_pCaptured = FindSplitterFromPoint(&root, point);
    if (m_pCaptured != NULL)
    {
      m_pHookedWnd->SetCapture();
      LPCTSTR cursor = NULL;
      CSplitContainer * pSplitContainer = (CSplitContainer *)m_pCaptured->Parent;
      if (pSplitContainer->m_Orientation == WinResizer::SPLIT_CONTAINER_H)
      {
        cursor = IDC_SIZEWE;
      }
      else
      {
        cursor = IDC_SIZENS;
      }
      HCURSOR hCur = AfxGetApp()->LoadStandardCursor(cursor);
      HCURSOR m_hOldCursor = ::SetCursor(hCur);
    }
  }

  if (m_pCaptured != NULL && (nFlags & MK_LBUTTON) > 0)
  {
    CSplitContainer * pSplitterContainer = (CSplitContainer *) m_pCaptured->Parent;
    
    if (pSplitterContainer->m_Orientation == WinResizer::SPLIT_CONTAINER_H)
    {
      pSplitterContainer->SetSplitterPosition( point.x - m_splitterOffset);
    }
    else
    {
      pSplitterContainer->SetSplitterPosition( point.y - m_splitterOffset);
    }
    UpdateSplitterOffset(point);

    ResizeUI(pSplitterContainer);

  }
}
void WinResizer::OnLButtonUp(UINT nFlags, CPoint point)
{
  OnMouseMove(nFlags, point);
}
void WinResizer::UpdateSplitterOffset(CPoint ptCurr)
{
  if (m_pCaptured == NULL )
  {
    return;
  }
  if (((CSplitContainer *)m_pCaptured->Parent)->m_Orientation == WinResizer::SPLIT_CONTAINER_H)
  {
    if ( ptCurr.x  < m_pCaptured->left)
    {
      m_splitterOffset = 0;
    }
    else if ( ptCurr.x > m_pCaptured->right)
    {
      m_splitterOffset = m_pCaptured->Width();
    }
    else
    {
      m_splitterOffset = ptCurr.x - m_pCaptured->left;
    }
  }
  else
  {
    if ( ptCurr.y  < m_pCaptured->top)
    {
      m_splitterOffset = 0;
    }
    else if ( ptCurr.y > m_pCaptured->bottom)
    {
      m_splitterOffset = m_pCaptured->Height();
    }
    else
    {
      m_splitterOffset = ptCurr.y - m_pCaptured->top;
    }    
  }
}
void WinResizer::OnSizing(UINT fwSide, LPRECT pRect)
{

  CRect * prc = (CRect *) pRect;

  CRect rcMin(0, 0, root.MinSize.cx, root.MinSize.cy);
  CRect rcMax(0, 0, root.MaxSize.cx, root.MaxSize.cy);

  LONG_PTR style = GetWindowLongPtr(m_pHookedWnd->m_hWnd , GWL_STYLE);
  LONG_PTR styleEx = GetWindowLongPtr(m_pHookedWnd->m_hWnd, GWL_EXSTYLE);

  ::AdjustWindowRectEx(&rcMin, style, (m_pHookedWnd->GetMenu() != NULL), styleEx);
  ::AdjustWindowRectEx(&rcMax, style, (m_pHookedWnd->GetMenu() != NULL), styleEx);

  switch (fwSide)
  {
  case WMSZ_BOTTOM:
    if (prc->Height() < rcMin.Height() )
    {
      prc->bottom = prc->top + rcMin.Height();
    }
    if (prc->Height() > rcMax.Height() )
    {
      prc->bottom = prc->top + rcMax.Height();
    }    
    break;
  case WMSZ_BOTTOMLEFT:
    if (prc->Height() < rcMin.Height() )
    {
      prc->bottom = prc->top + rcMin.Height();
    }
    if (prc->Width() < rcMin.Width() )
    {
      prc->left = prc->right - rcMin.Width();
    }

    if (prc->Height() > rcMax.Height() )
    {
      prc->bottom = prc->top + rcMax.Height();
    }
    if (prc->Width() > rcMax.Width() )
    {
      prc->left = prc->right - rcMax.Width();
    }
    break;
  case WMSZ_BOTTOMRIGHT:
    if (prc->Height() < rcMin.Height() )
    {
      prc->bottom = prc->top + rcMin.Height();
    }
    if (prc->Width() < rcMin.Width() )
    {
      prc->right = prc->left + rcMin.Width();
    }

    if (prc->Height() > rcMax.Height() )
    {
      prc->bottom = prc->top + rcMax.Height();
    }
    if (prc->Width() > rcMax.Width() )
    {
      prc->right = prc->left + rcMax.Width();
    }

    break;
  case WMSZ_LEFT:
    if (prc->Width() < rcMin.Width() )
    {
      prc->left = prc->right - rcMin.Width();
    }

    if (prc->Width() > rcMax.Width() )
    {
      prc->left = prc->right - rcMax.Width();
    }
    break;
  case WMSZ_RIGHT:
    if (prc->Width() < rcMin.Width() )
    {
      prc->right = prc->left + rcMin.Width();
    }

    if (prc->Width() > rcMax.Width() )
    {
      prc->right = prc->left + rcMax.Width();
    }
    break;
  case WMSZ_TOP:
    if (prc->Height() < rcMin.Height() )
    {
      prc->top = prc->bottom - rcMin.Height();
    }

    if (prc->Height() > rcMax.Height() )
    {
      prc->top = prc->bottom - rcMax.Height();
    }
    break;
  case WMSZ_TOPLEFT:
    if (prc->Height() < rcMin.Height() )
    {
      prc->top = prc->bottom - rcMin.Height();
    }
    if (prc->Width() < rcMin.Width() )
    {
      prc->left = prc->right - rcMin.Width();
    }


    if (prc->Height() > rcMax.Height() )
    {
      prc->top = prc->bottom - rcMax.Height();
    }
    if (prc->Width() > rcMax.Width() )
    {
      prc->left = prc->right - rcMax.Width();
    }
    break;
  case WMSZ_TOPRIGHT:
    if (prc->Height() < rcMin.Height() )
    {
      prc->top = prc->bottom - rcMin.Height();
    }
    if (prc->Width() < rcMin.Width() )
    {
      prc->right = prc->left + rcMin.Width();
    }

    if (prc->Height() > rcMax.Height() )
    {
      prc->top = prc->bottom - rcMax.Height();
    }
    if (prc->Width() > rcMax.Width() )
    {
      prc->right = prc->left + rcMax.Width();
    }
    break;
  }

}

void WinResizer::OnPaint()
{
  if (m_pHookedWnd == NULL)
  {
    return;
  }
  CPaintDC dc(m_pHookedWnd); // device context for painting

  CPanelList panelList;
  GetVisualPanels(&root, &panelList);
  POSITION pos = panelList.GetHeadPosition();
  while (pos != NULL)
  {
    CVisualPanel * pPanel = (CVisualPanel *) panelList.GetNext(pos);
    if (pPanel->m_bVisible)
    {
      pPanel->Draw(&dc);
    }
  }

}


BOOL WinResizer::SetAnchor(LPCTSTR panelName, UINT uAnchor)
{
  // container must already exist
  CPanel * pPanel = NULL;
  if ( (pPanel = FindPanelByName(&root, panelName)) == NULL)  
  {
    return FALSE;
  }

  return pPanel->SetAnchor(uAnchor);
}

BOOL WinResizer::SetAnchor(UINT uID, UINT uAnchor)
{
  ASSERT(m_pHookedWnd != NULL);

  CUIPanel * pPanel = GetUIPanel(uID);

  if (pPanel == NULL)
  {
    return FALSE;
  }
  pPanel->SetAnchor(uAnchor);  
  
  return TRUE;
}
BOOL WinResizer::GetAnchor(LPCTSTR panelName, UINT & anchor)
{
  CPanel * pPanel = NULL;
  if ( (pPanel = FindPanelByName(&root, panelName)) == NULL)  // name of parent must already exist
  {
    return FALSE;
  }

  anchor = pPanel->Anchor;
  return TRUE;
}
BOOL WinResizer::GetAnchor(UINT uID, UINT & anchor)
{
  CPanel * pPanel = NULL;
  if ( (pPanel = FindPanelByName(&root, IdToName(uID))) == NULL)  // name of parent must already exist
  {
    return FALSE;
  }

  anchor = pPanel->Anchor;
  return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL WinResizer::SetDock(LPCTSTR panelName, UINT uDock)
{
  // container must already exist
  CPanel * pPanel = NULL;
  if ( (pPanel = FindPanelByName(&root, panelName)) == NULL)  
  {
    return FALSE;
  }

  pPanel->Dock = uDock ;
  return TRUE;
}

BOOL WinResizer::SetDock(UINT uID, UINT uDock)
{
  ASSERT(m_pHookedWnd != NULL);

  CUIPanel * pPanel = GetUIPanel(uID);

  if (pPanel == NULL)
  {
    return FALSE;
  }
  pPanel->Dock = uDock;  
  
  return TRUE;
}
BOOL WinResizer::GetDock(LPCTSTR panelName, UINT & uDock)
{
  CPanel * pPanel = NULL;
  if ( (pPanel = FindPanelByName(&root, panelName)) == NULL)  // name of parent must already exist
  {
    return FALSE;
  }

  uDock = pPanel->Dock;
  return TRUE;
}
BOOL WinResizer::GetDock(UINT uID, UINT & uDock)
{
  CPanel * pPanel = NULL;
  if ( (pPanel = FindPanelByName(&root, IdToName(uID))) == NULL)  // name of parent must already exist
  {
    return FALSE;
  }

  uDock = pPanel->Dock;
  return TRUE;
}


//////////////////////
BOOL WinResizer::SetParent(LPCTSTR panelName, LPCTSTR parentName)
{
  ASSERT(m_pHookedWnd != NULL);


  // now make sure parentName is OK
  CPanel * pParent = NULL;
  if ( (pParent = FindPanelByName(&root, parentName)) == NULL)  // name of parent must already exist
  {
    return FALSE;
  }

  // make sure panelName exist
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, panelName)) == NULL)
  {
    return FALSE;
  }

  return pParent->AddChild(pPanel);
}
BOOL WinResizer::SetParent(UINT uID, LPCTSTR parentName)
{
  ASSERT(m_pHookedWnd != NULL);

  // now make sure parentName is OK
  CPanel * pParent = NULL;
  if ( (pParent = FindPanelByName(&root, parentName)) == NULL)  // name of parent must already exist
  {
    return FALSE;
  }
  CPanel * pPanel = NULL;

  // first see if it is already defined
  if ((pPanel = FindPanelByName(&root, IdToName(uID))) == NULL)
  {
    if ((pPanel = CreateUIPanel(uID)) == NULL)
    {
      return FALSE;
    }
  }

  return pParent->AddChild(pPanel);

}

BOOL WinResizer::SetParent(LPCTSTR panelName, UINT uParentID)
{
  ASSERT(m_pHookedWnd != NULL);

  // now make sure parentName is OK
  CPanel * pParent = GetUIPanel(uParentID);
  if ( pParent == NULL)  // name of parent must already exist
  {
    return FALSE;
  }

  // make sure panelName exist
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, panelName)) == NULL)
  {
    return FALSE;
  }

  return pParent->AddChild(pPanel);

}
BOOL WinResizer::SetParent(UINT uID, UINT uParentID)
{
  CPanel * pParent = GetUIPanel(uParentID);
  if (pParent == NULL)
  {
    return FALSE;
  }
  CPanel * pPanel = GetUIPanel(uID);;
  if (pParent == NULL)
  {
    return FALSE;
  }

  return pParent->AddChild( pPanel );

}


BOOL WinResizer::GetParent(LPCTSTR panelName, CString & parentName)
{
  CPanel * pPanel = NULL;
  if ( (pPanel = FindPanelByName(&root, panelName)) == NULL) 
  {
    return FALSE;
  }

  parentName = pPanel->Parent->Name;
  return TRUE;
}
BOOL WinResizer::GetParent(UINT uID, CString & parentName)
{
  CPanel * pPanel = NULL;
  if ( (pPanel = FindPanelByName(&root, IdToName(uID))) == NULL)  // name of parent must already exist
  {
    return FALSE;
  }

  parentName = pPanel->Parent->Name;
  return TRUE;
}

BOOL WinResizer::SetFixedPanel(LPCTSTR splitContainerName, short panel)
{
  CPanel * pContainer = FindPanelByName(&root, splitContainerName);
  if (pContainer == NULL)
  {
    return FALSE;
  }
  CSplitContainer * pSplitContainer =  dynamic_cast<CSplitContainer *>( pContainer );
  if (pSplitContainer == NULL)
  {
    return FALSE;
  }

  pSplitContainer->SetFixedPanel(panel);
  return TRUE;
}
BOOL WinResizer::GetFixedPanel(LPCTSTR splitContainerName, short & panel)
{
  CPanel * pContainer = FindPanelByName(&root, splitContainerName);
  if (pContainer == NULL)
  {
    return FALSE;
  }
  CSplitContainer * pSplitContainer =  dynamic_cast<CSplitContainer *> (pContainer);
  if (pSplitContainer == NULL)
  {
    return FALSE;
  }
  panel = pSplitContainer->GetFixedPanel();
  return TRUE;

}

BOOL WinResizer::SetIsSplitterFixed(LPCTSTR splitContainerName, BOOL fixed)
{
  CPanel * pContainer = FindPanelByName(&root, splitContainerName);
  if (pContainer == NULL)
  {
    return FALSE;
  }
  CSplitContainer * pSplitContainer =  dynamic_cast<CSplitContainer *> (pContainer);
  if (pSplitContainer == NULL)
  {
    return FALSE;
  }
  pSplitContainer->SetIsSplitterFixed( fixed );
  return TRUE;
}
BOOL WinResizer::GetIsSplitterFixed(LPCTSTR splitContainerName, BOOL &fixed)
{
  CPanel * pContainer = FindPanelByName(&root, splitContainerName);
  if (pContainer == NULL)
  {
    return FALSE;
  }
  CSplitContainer * pSplitContainer =  dynamic_cast<CSplitContainer *> (pContainer);
  if (pSplitContainer == NULL)
  {
    return FALSE;
  }
  fixed = pSplitContainer->GetIsSplitterFixed();
  return TRUE;
}



BOOL WinResizer::SetShowSplitterGrip(LPCTSTR splitContainerName, BOOL bShow)
{
  CPanel * pContainer = FindPanelByName(&root, splitContainerName);
  if (pContainer == NULL)
  {
    return FALSE;
  }
  CSplitContainer * pSplitContainer =  dynamic_cast<CSplitContainer *> (pContainer);
  if (pSplitContainer == NULL)
  {
    return FALSE;
  }
  pSplitContainer->SetShowSplitterGrip( bShow );
  return TRUE;
}
BOOL WinResizer::GetShowSplitterGrip(LPCTSTR splitContainerName, BOOL &bShow)
{
  CPanel * pContainer = FindPanelByName(&root, splitContainerName);
  if (pContainer == NULL)
  {
    return FALSE;
  }
  CSplitContainer * pSplitContainer =  dynamic_cast<CSplitContainer *> (pContainer);
  if (pSplitContainer == NULL)
  {
    return FALSE;
  }
  bShow = pSplitContainer->GetShowSplitterGrip();
  return TRUE;
}




/////////////////////////
BOOL WinResizer::SetMinimumSize( LPCTSTR panelName, CSize & size)
{
  CPanel * pPanel = NULL;

  if ( (pPanel = FindPanelByName(&root, panelName)) == NULL)  
  {
    return FALSE;
  }
  return pPanel->SetMinSize( size );
}
BOOL WinResizer::SetMinimumSize(UINT uID, CSize & size)
{
  CPanel * pPanel = NULL;

  if ( (pPanel = FindPanelByName(&root, IdToName(uID))) == NULL) 
  {
    if ((pPanel = CreateUIPanel(uID)) == NULL)
    {
      return FALSE;
    }
  }

  return pPanel->SetMinSize( size );
}

BOOL WinResizer::GetMinimumSize(LPCTSTR panelName, CSize & size) 
{
  CPanel * pPanel = NULL;

  if ( (pPanel = FindPanelByName(&root, panelName)) == NULL) 
  {
    return FALSE;
  }

  size = pPanel->MinSize;
  return TRUE;
}
BOOL WinResizer::GetMinimumSize(UINT uID, CSize & size) 
{
  const CPanel * pPanel = NULL;

  if ( (pPanel = FindPanelByName(&root, IdToName(uID))) == NULL) 
  {
    return FALSE;
  }

  size = pPanel->MinSize;
  return TRUE;
}
BOOL WinResizer::SetMaximumSize(UINT uID, CSize & size)
{
  CPanel * pPanel = NULL;

  if ( (pPanel = FindPanelByName(&root, IdToName(uID))) == NULL)  
  {
    return FALSE;
  }
  
  return pPanel->SetMaxSize(size);
}

BOOL WinResizer::SetMaximumSize( LPCTSTR panelName, CSize & size)
{
  CPanel * pPanel = NULL;

  if ( (pPanel = FindPanelByName(&root, panelName)) == NULL) 
  {
    return FALSE;
  }

  return pPanel->SetMaxSize(size);

}
BOOL WinResizer::GetMaximumSize(  LPCTSTR panelName, CSize & size) 
{
  CPanel * pPanel = NULL;
  if ( (pPanel = FindPanelByName(&root, panelName)) == NULL)  
  {
    return FALSE;
  }

  size = pPanel->MaxSize;
  return TRUE;
}
BOOL WinResizer::GetMaximumSize(  UINT uID, CSize & size) 
{
  CPanel * pPanel = NULL;
  if ( (pPanel = FindPanelByName(&root, IdToName(uID))) == NULL)  
  {
    return FALSE;
  }

  size = pPanel->MaxSize;
  return TRUE;
}
void WinResizer::SetShowResizeGrip(BOOL show)
{  
  CGripperPanel * pPanel = (CGripperPanel *)FindPanelByName(&root, _T("_resizeGrip"));
  ASSERT(pPanel != NULL);
  if ( pPanel->m_bVisible != show )
  {
    pPanel->m_bVisible = show;
    m_pHookedWnd->Invalidate(TRUE);
  }
}
BOOL WinResizer::GetShowResizeGrip()
{
  CGripperPanel * pPanel = (CGripperPanel *)FindPanelByName(&root, _T("_resizeGrip"));
  ASSERT(pPanel != NULL);
  return pPanel->m_bVisible ;

} 
void WinResizer::OnDestroy()
{
  if (m_pHookedWnd != NULL)
  {
    Unhook();
  }
}

BOOL WinResizer::Unhook()
{
  ASSERT( m_pHookedWnd != NULL );
  if (m_pHookedWnd == NULL )
  {
    return FALSE; //hasent been hooked
  }

  WNDPROC pWndProc = (WNDPROC)::SetWindowLongPtr(m_pHookedWnd->m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_pfnWndProc);
  WndResizerData.RemoveKey(m_pHookedWnd->m_hWnd); 
  root.m_pHookWnd = NULL;
  m_pHookedWnd = NULL;

  // destroy all chilldren
  while(root.Children.GetCount() > 0 )
  {
    CPanel * pChild = root.Children.RemoveHead();
    delete pChild;
  }
  return TRUE;
}


LRESULT CALLBACK WinResizer::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  WinResizer * pResizer = NULL;
  WndResizerData.Lookup(hWnd, pResizer);
  ASSERT( pResizer != NULL);


	switch (uMsg)
	{
		case WM_SIZE:
      {
	      int cx = 0;
	      int cy = 0;
        cx = LOWORD(lParam);    
        cy = HIWORD(lParam);    
        pResizer->OnSize((UINT) wParam, cx, cy);
      }
			break;
    case WM_SIZING:
      pResizer->OnSizing((UINT) wParam, (LPRECT)lParam);      
      break;
    case WM_DESTROY:
      pResizer->OnDestroy();
      break;
    case WM_MOUSEMOVE:
      pResizer->OnMouseMove((UINT) wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
      break;
    case WM_LBUTTONDOWN:
      pResizer->OnLButtonDown((UINT) wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
      break;
    case WM_LBUTTONUP:
      pResizer->OnLButtonUp((UINT) wParam, CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
      break;
    case WM_PAINT:
      pResizer->OnPaint();     
      break;
    case WM_HSCROLL:
    case WM_VSCROLL:
      pResizer->OnScroll();
      break;

    //case WM_ERASEBKGND:
    //  return FALSE;
    //  break;
		default:
			break;
	}

	return ::CallWindowProc(pResizer->m_pfnWndProc, hWnd, uMsg, wParam, lParam);


}



BOOL WinResizer::CreateSplitContainer(LPCTSTR panelName, LPCTSTR panelNameA, LPCTSTR panelNameB)
{
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, panelName)) != NULL)  
  {
    return FALSE;
  }

  CPanel * pPanelA = NULL;
  if ((pPanelA = FindPanelByName(&root, panelNameA)) == NULL)  
  {
    return FALSE;
  }


  CPanel * pPanelB = NULL;
  if ((pPanelB = FindPanelByName(&root, panelNameB)) == NULL)  
  {
    return FALSE;
  }
  

  if (pPanelA == pPanelB) // two panel cannot be same
  {
    return FALSE;
  }


  CPanel * pSplitterContainer = CSplitContainer::Create(pPanelA, pPanelB);
  if (pSplitterContainer == NULL)
  {
    return FALSE;
  }
  pSplitterContainer->Name = panelName;

  return root.AddChild(pSplitterContainer);
}



BOOL WinResizer::CreateSplitContainer(LPCTSTR panelName, LPCTSTR panelNameA, UINT panelIDB)
{
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, panelName)) != NULL)  
  {
    return FALSE;
  }

  CPanel * pPanelA = NULL;
  if ((pPanelA = FindPanelByName(&root, panelNameA)) == NULL)  
  {
    return FALSE;
  }

  CPanel * pPanelB = GetUIPanel(panelIDB);
  if (pPanelB == NULL )
  {
    return FALSE;
  }
  

  if (pPanelA == pPanelB) // two panel cannot be same
  {
    return FALSE;
  }

  // first lets make sure the two CRect are properly set

  CPanel * pSplitterContainer = CSplitContainer::Create(pPanelA, pPanelB);
  if (pSplitterContainer == NULL)
  {
    return FALSE;
  }
  pSplitterContainer->Name = panelName;

  return root.AddChild(pSplitterContainer);

}


BOOL WinResizer::CreateSplitContainer(LPCTSTR panelName, UINT panelIDA, LPCTSTR panelNameB)
{
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, panelName)) != NULL)  
  {
    return FALSE;
  }

  CPanel * pPanelA = GetUIPanel(panelIDA);
  if (pPanelA == NULL )
  {
    return FALSE;
  }


  CPanel * pPanelB = NULL;
  if ((pPanelB = FindPanelByName(&root, panelNameB)) == NULL)  
  {
    return FALSE;
  }

  if (pPanelA == pPanelB) // two panel cannot be same
  {
    return FALSE;
  }
  
  CPanel * pSplitterContainer = CSplitContainer::Create(pPanelA, pPanelB);
  if (pSplitterContainer == NULL)
  {
    return FALSE;
  }
  pSplitterContainer->Name.Append(panelName);

  return root.AddChild(pSplitterContainer);

}



BOOL WinResizer::CreateSplitContainer(LPCTSTR panelName, UINT panelIDA, UINT panelIDB)
{
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, panelName)) != NULL)  
  {
    return FALSE;
  }

  CPanel * pPanelA = GetUIPanel(panelIDA);
  if (pPanelA == NULL )
  {
    return FALSE;
  }


  CPanel * pPanelB = GetUIPanel(panelIDB);
  if (pPanelB == NULL )
  {
    return FALSE;
  }
  
  if (pPanelA == pPanelB) // two panel cannot be same
  {
    return FALSE;
  }
 
  CPanel * pSplitterContainer = CSplitContainer::Create(pPanelA, pPanelB);
  if (pSplitterContainer == NULL)
  {
    return FALSE;
  }
  pSplitterContainer->Name = panelName;

  return root.AddChild(pSplitterContainer);

}

BOOL WinResizer::SetSplitterPosition(LPCTSTR splitContainerName, UINT position)
{
  ASSERT(m_pHookedWnd != NULL);
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, splitContainerName)) == NULL)
  {
    return FALSE;
  }
  CSplitContainer * pContainer = dynamic_cast<CSplitContainer *> ( pPanel);
  if (pContainer == NULL)
  {
    return FALSE;
  }
  pContainer->SetSplitterPosition( (int) position);
  ResizeUI( pContainer );
  return TRUE;
}
BOOL WinResizer::GetSplitterPosition(LPCTSTR splitContainerName, UINT & position)
{
  ASSERT(m_pHookedWnd != NULL);
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, splitContainerName)) == NULL)
  {
    return FALSE;
  }
  CSplitContainer * pContainer = dynamic_cast<CSplitContainer *> ( pPanel);
  if (pContainer == NULL)
  {
    return FALSE;
  }
  position = (UINT) pContainer->GetSplitterPosition();
  return TRUE;
}

BOOL WinResizer::CreatePanel(UINT uID)
{
  ASSERT(m_pHookedWnd != NULL);
  if (FindPanelByName(&root, IdToName(uID)) != NULL)
  {
    return FALSE;
  }
  CUIPanel * pPanel = GetUIPanel(uID);
  ASSERT(pPanel != NULL);

  pPanel->m_bOle = TRUE;

  return TRUE;
}

BOOL WinResizer::SetFlowDirection(LPCTSTR flowPanelName, short direction)
{
  ASSERT(m_pHookedWnd != NULL);
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, flowPanelName)) == NULL)
  {
    return FALSE;
  }
  CFlowLayoutPanel * pFlowLayout = dynamic_cast<CFlowLayoutPanel *> ( pPanel);
  if (pFlowLayout == NULL)
  {
    return FALSE;
  }
  pFlowLayout->SetFlowDirection( direction == 1 ? WinResizer::LEFT_TO_RIGHT : WinResizer::TOP_TO_BOTTOM);
  return TRUE;
}
BOOL WinResizer::GetFlowDirection(LPCTSTR flowPanelName, short &direction)
{
  ASSERT(m_pHookedWnd != NULL);
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, flowPanelName)) == NULL)
  {
    return FALSE;
  }
  CFlowLayoutPanel * pFlowLayout = dynamic_cast<CFlowLayoutPanel *> ( pPanel);
  if (pFlowLayout == NULL)
  {
    return FALSE;
  }
  direction = (pFlowLayout->GetFlowDirection() == WinResizer::LEFT_TO_RIGHT ? 1 : 2);
  return TRUE;
}

BOOL WinResizer::SetFlowItemSpacingX(LPCTSTR flowPanelName, int nSpacing)
{
  ASSERT(m_pHookedWnd != NULL);
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, flowPanelName)) == NULL)
  {
    return FALSE;
  }
  CFlowLayoutPanel * pFlowLayout = dynamic_cast<CFlowLayoutPanel *> ( pPanel);
  if (pFlowLayout == NULL)
  {
    return FALSE;
  }
  pFlowLayout->SetItemSpacingX(nSpacing);
  return TRUE;
}
BOOL WinResizer::GetFlowItemSpacingX(LPCTSTR flowPanelName, int &nSpacing)
{
  ASSERT(m_pHookedWnd != NULL);
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, flowPanelName)) == NULL)
  {
    return FALSE;
  }
  CFlowLayoutPanel * pFlowLayout = dynamic_cast<CFlowLayoutPanel *> ( pPanel);
  if (pFlowLayout == NULL)
  {
    return FALSE;
  }
  nSpacing = pFlowLayout->GetItemSpacingX();
  return TRUE;
}

BOOL WinResizer::SetFlowItemSpacingY(LPCTSTR flowPanelName, int nSpacing)
{
  ASSERT(m_pHookedWnd != NULL);
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, flowPanelName)) == NULL)
  {
    return FALSE;
  }
  CFlowLayoutPanel * pFlowLayout = dynamic_cast<CFlowLayoutPanel *> ( pPanel);
  if (pFlowLayout == NULL)
  {
    return FALSE;
  }
  pFlowLayout->SetItemSpacingY(nSpacing);
  return TRUE;

}

BOOL WinResizer::GetFlowItemSpacingY(LPCTSTR flowPanelName, int &nSpacing)
{
  ASSERT(m_pHookedWnd != NULL);
  CPanel * pPanel = NULL;
  if ((pPanel = FindPanelByName(&root, flowPanelName)) == NULL)
  {
    return FALSE;
  }
  CFlowLayoutPanel * pFlowLayout = dynamic_cast<CFlowLayoutPanel *> ( pPanel);
  if (pFlowLayout == NULL)
  {
    return FALSE;
  }
  nSpacing = pFlowLayout->GetItemSpacingY();
  return TRUE;

}


BOOL WinResizer::CreateFlowLayoutPanel(LPCTSTR panelName, const CRect * prcPanel)
{
  if (FindPanelByName(&root, panelName) != NULL)  
  {
    return FALSE;
  }

  CPanel * pPanel = new CFlowLayoutPanel(prcPanel);
  pPanel->Name = panelName;
  return root.AddChild(pPanel);
 
}
BOOL WinResizer::CreateFlowLayoutPanel(LPCTSTR panelName, const CUIntArray * parrID, BOOL setAsChildren)
{
  ASSERT(m_pHookedWnd != NULL);

  CRect rcFinal(0, 0, 0, 0);
  for(int i = 0; i < parrID->GetCount(); i++)
  {
    CRect rc(0, 0, 0, 0);
    m_pHookedWnd->GetDlgItem(parrID->GetAt(i))->GetWindowRect(&rc);
    m_pHookedWnd->ScreenToClient(&rc);
    rcFinal.UnionRect(&rcFinal, &rc);
  }

  BOOL bOk = CreateFlowLayoutPanel(panelName, &rcFinal);
  if (bOk == FALSE)
  {
    return FALSE;
  }

  if ( setAsChildren )
  {
    CPanel * pPanel = FindPanelByName(&root, panelName);
    for(int i = 0; i < parrID->GetCount(); i++)
    {
      if (FindPanelByName(&root, IdToName(parrID->GetAt(i))) != NULL)  
      {
        bOk = root.RemoveChild(pPanel);
         ASSERT( bOk );
        delete pPanel;
        return FALSE;
      }
      CUIPanel * pUIPanel = GetUIPanel(parrID->GetAt(i));
      ASSERT( pUIPanel != NULL);
      bOk = pPanel->AddChild( pUIPanel );
       ASSERT( bOk );
    }
  }
  return TRUE;
}

BOOL WinResizer::CreatePanel(LPCTSTR panelName, const CRect * prcPanel)
{
  if (FindPanelByName(&root, panelName) != NULL)  
  {
    return FALSE;
  }

  CPanel * pPanel = new CPanel(prcPanel);
  pPanel->Name = panelName;
  return root.AddChild(pPanel);
 
}
BOOL WinResizer::CreatePanel(LPCTSTR panelName, const CUIntArray * parrID, BOOL setAsChildren)
{
  ASSERT(m_pHookedWnd != NULL);

  CRect rcFinal(0, 0, 0, 0);
  for(int i = 0; i < parrID->GetCount(); i++)
  {
    CRect rc(0, 0, 0, 0);
    m_pHookedWnd->GetDlgItem(parrID->GetAt(i))->GetWindowRect(&rc);
    m_pHookedWnd->ScreenToClient(&rc);
    rcFinal.UnionRect(&rcFinal, &rc);
  }

  BOOL bOk = CreatePanel(panelName, &rcFinal);
  if (bOk == FALSE)
  {
    return FALSE;
  }

  if ( setAsChildren )
  {
    CPanel * pPanel = FindPanelByName(&root, panelName);
    for(int i = 0; i < parrID->GetCount(); i++)
    {
      if (FindPanelByName(&root, IdToName(parrID->GetAt(i))) != NULL)  
      {
        bOk = root.RemoveChild(pPanel);
         ASSERT( bOk );
        delete pPanel;
        return FALSE;
      }
      CUIPanel * pUIPanel = GetUIPanel(parrID->GetAt(i));
      ASSERT( pUIPanel != NULL);
      bOk = pPanel->AddChild( pUIPanel );
       ASSERT( bOk );
    }
  }
  return TRUE;
}

WinResizer::CPanel * WinResizer::FindPanelByName(WinResizer::CPanel * pRoot, LPCTSTR name)
{
  if (CString(name).GetLength() == 0)
  {
    return NULL;
  }

  if (pRoot == NULL )
  {
    return NULL;
  }

  if (pRoot->Name.CompareNoCase(name) == 0 )
  {
    return pRoot;
  }
  else
  {
    POSITION pos = pRoot->Children.GetHeadPosition();
    while(pos != NULL)
    {
      CPanel * pChild = pRoot->Children.GetNext(pos);
      CPanel * pFound = FindPanelByName(pChild, name);
      if (pFound != NULL )
      {
        return pFound;
      }
    }
  }
  
  return NULL;
}
void WinResizer::GetUIPanels(WinResizer::CPanel * pRoot, CPanelList * pList, BOOL bOle)
{
  if (pRoot == NULL )
  {
    return ;
  }

  CUIPanel * pUIPanel = dynamic_cast<CUIPanel *> ( pRoot);

  if (pUIPanel != NULL && pUIPanel->m_bOle == bOle)
  {
    pList->AddTail( pRoot );
  }

  // try the childreen
  POSITION pos = pRoot->Children.GetHeadPosition();
  while(pos != NULL)
  {
    CPanel * pChild = pRoot->Children.GetNext(pos);
    GetUIPanels(pChild, pList, bOle);

  }
  
}

void WinResizer::GetVisualPanels(WinResizer::CPanel * pRoot, CPanelList * pList)
{
  if (pRoot == NULL )
  {
    return ;
  }

  CVisualPanel * pUIPanel = dynamic_cast<CVisualPanel *> ( pRoot);

  if (pUIPanel != NULL)
  {
    pList->AddTail( pRoot );
  }

  // try the childreen
  POSITION pos = pRoot->Children.GetHeadPosition();
  while(pos != NULL)
  {
    CPanel * pChild = pRoot->Children.GetNext(pos);
    GetVisualPanels(pChild, pList);
  }  
}


WinResizer::CPanel * WinResizer::FindSplitterFromPoint(WinResizer::CPanel * pRoot, CPoint point)
{  
  if (pRoot == NULL )
  {
    return NULL;
  }

  CSpitterPanel * pSpitterPanel = dynamic_cast<CSpitterPanel *>(pRoot);

  if (pSpitterPanel != NULL && pRoot->PtInRect(point) == TRUE )
  {
    CSplitContainer * pContainer = (CSplitContainer *)pRoot->Parent;
    if (!pContainer->GetIsSplitterFixed())
    {
      CPoint ptScreen = point;
      m_pHookedWnd->ClientToScreen(&ptScreen);
      HWND hWndFromPoint = ::WindowFromPoint(ptScreen);
      if (m_pHookedWnd->m_hWnd == hWndFromPoint)
      {
        return pRoot;
      }
    }
  }

  // try the childreen
  POSITION pos = pRoot->Children.GetHeadPosition();
  while(pos != NULL)
  {
    CPanel * pChild = pRoot->Children.GetNext(pos);
    CPanel * pFound = FindSplitterFromPoint(pChild, point);
    if (pFound != NULL )
    {
      return pFound;
    }
  }
  
  
  return NULL;
}

CString WinResizer::IdToName(UINT uID)
{
  CString sName;
  sName.Format(_T("%d"), uID);
  return sName;
}

WinResizer::CUIPanel * WinResizer::CreateUIPanel(UINT uID)
{
  ASSERT(m_pHookedWnd != NULL);
  CWnd * pWnd = m_pHookedWnd->GetDlgItem(uID);

  if ( pWnd == NULL )
  {
    return NULL;
  }
  CRect rc(0, 0, 0, 0);
  pWnd->GetWindowRect( &rc );  
  m_pHookedWnd->ScreenToClient( &rc );

  CUIPanel * pPanel = new CUIPanel(&rc, uID);
  pPanel->Name = IdToName(uID);
  return pPanel ;
}

WinResizer::CUIPanel * WinResizer::GetUIPanel(UINT uID)
{
  CUIPanel * pPanel = NULL;
  if ((pPanel = (CUIPanel *)FindPanelByName(&root, IdToName(uID))) == NULL)  
  {
    pPanel = CreateUIPanel(uID);
    if (pPanel != NULL)
    {
      root.AddChild( pPanel );
    }
  }
  return pPanel;
}

BOOL WinResizer::InvokeOnResized()
{
  ASSERT(m_pHookedWnd != NULL);

  OnSize(0, 0, 0);
  return TRUE;
}


CString WinResizer::GetDebugInfo()
{
  ASSERT(m_pHookedWnd != NULL);

  CString sInfo;
  CString sIndent;

  GetDebugInfo(&root, sInfo, sIndent);

  return sInfo;
}
void WinResizer::GetDebugInfo(CPanel * pRoot, CString & info, CString indent)
{
  if (pRoot == NULL )
  {
    return ;
  }

  info.Append(_T("\n"));
  info.Append(indent);
  info.Append(pRoot->ToString() );

  indent.Append(_T("  "));
  for(int i = 0; i < pRoot->Children.GetCount(); i++)
  {
    CPanel * pChild = pRoot->Children.GetAt(pRoot->Children.FindIndex(i));
    GetDebugInfo(pChild, info, indent);
  }
  
}


/////////////////////////////  CPanel
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WinResizer::CPanel::CPanel() : CRect(0, 0, 0, 0)
{
  Init();
}
WinResizer::CPanel::CPanel(const CRect * prc) : CRect(prc)
{
  Init();
}
void WinResizer::CPanel::Init()
{
  Parent = NULL;
  LeftOffset = 0;
  TopOffset = 0;
  RightOffset = 0;
  BottomOffset = 0; 
  MinSize.SetSize(10, 10);
  MaxSize.SetSize(100000, 100000);
  Anchor =(ANCHOR_LEFT | ANCHOR_TOP);  
  Dock = DOCK_NONE;
}

WinResizer::CPanel::~CPanel()
{
  while(Children.GetCount() > 0 )
  {
    delete Children.RemoveHead();
  }
}
void WinResizer::CPanel::OnResized()
{
  BOOL bOk = FALSE;
  CRect rcEmpty(this);  // available area for docking.  ininitally it is the entire area

  POSITION pos = Children.GetHeadPosition();
  while(pos != NULL)
  {
    CPanel * pChild = Children.GetNext(pos);    
    
    if (pChild->Dock != DOCK_NONE)
    {
      switch(pChild->Dock)
      {
      case DOCK_LEFT:
        pChild->SetRect(rcEmpty.left, rcEmpty.top, rcEmpty.left + pChild->Width(), rcEmpty.bottom);
        bOk = rcEmpty.SubtractRect(&rcEmpty, pChild);
        // ASSERT( bOk );
        break;
      case DOCK_TOP:
        pChild->SetRect(rcEmpty.left, rcEmpty.top, rcEmpty.right, rcEmpty.top + pChild->Height());
        bOk = rcEmpty.SubtractRect(&rcEmpty, pChild);
        // ASSERT( bOk );
        break;
      case DOCK_RIGHT:
        pChild->SetRect(rcEmpty.right - pChild->Width(), rcEmpty.top, rcEmpty.right, rcEmpty.bottom);
        bOk = rcEmpty.SubtractRect(&rcEmpty, pChild);
        // ASSERT( bOk );
        break;
      case DOCK_BOTTOM:
        pChild->SetRect(rcEmpty.left, rcEmpty.bottom - pChild->Height(), rcEmpty.right, rcEmpty.bottom);
        bOk = rcEmpty.SubtractRect(&rcEmpty, pChild);
        // ASSERT( bOk );
        break;
      case DOCK_FILL:
        pChild->SetRect(rcEmpty.left, rcEmpty.top, rcEmpty.right, rcEmpty.bottom);
        break;
      }
      pChild->OnResized();

      // if docking is in action, then we igonre anchor, therefore we continue
      continue;
    }

    CRect rc(0, 0, 0, 0);
    if ((pChild->Anchor & ANCHOR_HORIZONTALLY_CENTERED) ==  ANCHOR_HORIZONTALLY_CENTERED )
    {
      rc.left = this->left + ((int)( (this->Width() - pChild->Width()) /  2));
      rc.right = rc.left + pChild->Width();

      BOOL bReposition = FALSE;
      if (pChild->MinSize.cx > rc.Width() )
      {
        bReposition = TRUE;
        rc.right = rc.left + pChild->MinSize.cx;
      }
      if (pChild->MaxSize.cx < rc.Width() )
      {
        bReposition = TRUE;
        rc.right = rc.left + pChild->MaxSize.cx;
      }

      if (bReposition)
      {
        int nWidth = rc.Width();
        rc.left = (int)( (this->Width() - nWidth) /  2) ;
        rc.right = rc.left + nWidth;
      }
    }
    else if ((pChild->Anchor & ANCHOR_HORIZONTALLY) ==  ANCHOR_HORIZONTALLY )
    {
      rc.left = this->left + pChild->LeftOffset;
      rc.right = this->right - pChild->RightOffset;

      // we will be left anchor if minsize or maxsize does not match
      // (giving ANCHOR_LEFT priority over ANCHOR_RIGHT)
      if ((pChild->Anchor & ANCHOR_PRIORITY_RIGHT) == ANCHOR_PRIORITY_RIGHT)
      {
        if (pChild->MinSize.cx > rc.Width() )
        {
          rc.left = rc.right - pChild->MinSize.cx;
        }
        if (pChild->MaxSize.cx < rc.Width() )
        {
          rc.left = rc.right - pChild->MaxSize.cx;
        }

      }
      else
      {
        if (pChild->MinSize.cx > rc.Width() )
        {
          rc.right = rc.left + pChild->MinSize.cx;
        }
        if (pChild->MaxSize.cx < rc.Width() )
        {
          rc.right = rc.left + pChild->MaxSize.cx;
        }
      }
    }
    else if ((pChild->Anchor & ANCHOR_RIGHT) ==  ANCHOR_RIGHT )
    {
      rc.right = this->right - pChild->RightOffset;     
      rc.left = rc.right - pChild->Width();

      if (pChild->MinSize.cx > rc.Width() )
      {
        rc.left = rc.right - pChild->MinSize.cx;
      }
      if (pChild->MaxSize.cx < rc.Width() )
      {
        rc.left = rc.right - pChild->MaxSize.cx;
      }
    }
    else if ((pChild->Anchor & ANCHOR_LEFT) == ANCHOR_LEFT )
    {
      rc.left = this->left + pChild->LeftOffset;
      rc.right = rc.left + pChild->Width();   

      if (pChild->MinSize.cx > rc.Width() )
      {
        rc.right = rc.left + pChild->MinSize.cx;
      }
      if (pChild->MaxSize.cx < rc.Width() )
      {
        rc.right = rc.left + pChild->MaxSize.cx;
      }
    }
    else
    {
      // it should never be here
      ASSERT( FALSE );
    }


    if ((pChild->Anchor & ANCHOR_VERTICALLY_CENTERED) ==  ANCHOR_VERTICALLY_CENTERED )
    {
      rc.top = this->top + ((int)( (this->Height() - pChild->Height()) /  2));
      rc.bottom = rc.top + pChild->Height();

      BOOL bReposition = FALSE;
      if (pChild->MinSize.cy > rc.Height() )
      {
        bReposition = TRUE;
        rc.bottom = rc.top + pChild->MinSize.cy;
      }
      if (pChild->MaxSize.cy < rc.Height() )
      {
        bReposition = TRUE;
        rc.bottom = rc.top + pChild->MaxSize.cy;
      }

      if (bReposition)
      {
        int nHeight = rc.Height();
        rc.top = (int)( (this->Height() - nHeight) /  2);
        rc.bottom = rc.top + nHeight;
      }
    }
    else if ((pChild->Anchor & ANCHOR_VERTICALLY ) == ANCHOR_VERTICALLY )
    {
      rc.top = this->top + pChild->TopOffset;
      rc.bottom = this->bottom - pChild->BottomOffset;

      if ((pChild->Anchor & ANCHOR_PRIORITY_BOTTOM) == ANCHOR_PRIORITY_BOTTOM)
      {
        if (pChild->MinSize.cy > rc.Height() )
        {
          rc.top = rc.bottom - pChild->MinSize.cy;
        }
        if (pChild->MaxSize.cy < rc.Height() )
        {
          rc.top = rc.bottom - pChild->MaxSize.cy;
        }
      }
      else
      {
        if (pChild->MinSize.cy > rc.Height() )
        {
          rc.bottom = rc.top + pChild->MinSize.cy;
        }
        if (pChild->MaxSize.cy < rc.Height() )
        {
          rc.bottom = rc.top + pChild->MaxSize.cy;
        }
      }
    }
    else if ((pChild->Anchor & ANCHOR_BOTTOM) == ANCHOR_BOTTOM )
    {
      rc.bottom = this->bottom - pChild->BottomOffset;     
      rc.top = rc.bottom - pChild->Height();

      if (pChild->MinSize.cy > rc.Height() )
      {
        rc.top = rc.bottom - pChild->MinSize.cy;
      }
      if (pChild->MaxSize.cy < rc.Height() )
      {
        rc.top = rc.bottom - pChild->MaxSize.cy;
      }
    }
    else if ((pChild->Anchor & ANCHOR_TOP) == ANCHOR_TOP )
    {
      rc.top = this->top + pChild->TopOffset;
      rc.bottom = rc.top + pChild->Height();   

      if (pChild->MinSize.cy > rc.Height() )
      {
        rc.bottom = rc.top + pChild->MinSize.cy;
      }
      if (pChild->MaxSize.cy < rc.Height() )
      {
        rc.bottom = rc.top + pChild->MaxSize.cy;
      }
    }
    else
    {
      // it should never be here
      ASSERT( FALSE );
    }
    pChild->SetRect(rc.TopLeft(), rc.BottomRight());

    pChild->OnResized();

  }
}
BOOL WinResizer::CPanel::AddChild(CPanel * pChild)
{
  if (pChild->Parent != NULL)
  {
    BOOL bOk = pChild->Parent->RemoveChild(pChild);
    if (bOk == FALSE)
    {
      return FALSE;
    }
  }
  pChild->LeftOffset = pChild->left - this->left;
  pChild->TopOffset = pChild->top - this->top;
  pChild->RightOffset = this->right - pChild->right;
  pChild->BottomOffset = this->bottom - pChild->bottom;

  pChild->Parent = this;

  Children.AddTail( pChild );
  return TRUE;
}

BOOL WinResizer::CPanel::RemoveChild(CPanel * pChild)
{
  POSITION pos = Children.Find(pChild);
  if (pos == NULL)
  {
    return FALSE;
  }
  Children.RemoveAt(pos);
  return TRUE;
  
}

BOOL WinResizer::CPanel::SetMinSize(CSize & size)
{  
  if (MaxSize.cx < size.cx)  
  {
    return FALSE;
  }
  if (MaxSize.cy < size.cy)
  {
    return FALSE;
  }

  MinSize = size;
  return TRUE;
}
BOOL WinResizer::CPanel::SetMaxSize(CSize & size)
{
  if (MinSize.cx > size.cx)  
  {
    return FALSE;
  }
  if (MinSize.cy > size.cy)
  {
    return FALSE;
  }

  MaxSize = size;
  return TRUE;
}

BOOL WinResizer::CPanel::SetAnchor(UINT anchor)
{
  if ((anchor & ANCHOR_VERTICALLY_CENTERED) <= 0 )
  {
    if ((anchor & ANCHOR_TOP) <= 0 )
    {
      if ((anchor & ANCHOR_BOTTOM) <= 0 )
      {
        anchor |= ANCHOR_TOP;  // default
      }
    }
  }

  if ((anchor & ANCHOR_HORIZONTALLY_CENTERED) <= 0 )
  {
    if ((anchor & ANCHOR_LEFT) <= 0 )
    {
      if ((anchor & ANCHOR_RIGHT) <= 0 )
      {
        anchor |= ANCHOR_LEFT; // default
      }
    }
  }
  Anchor = anchor;

  return TRUE;
}
CString WinResizer::CPanel::ToString()
{
  CString sFormat(_T("Name(%s), Type(%s), Anchor(%d), Size(w:%d, h:%d), Area(l:%d, t:%d, r:%d, b:%d), MinSize(w:%d, h:%d), MaxSize(w:%d, h:%d), Parent(%s), ChildrenCount(%d)"));

  CString sTo;
  sTo.Format(sFormat, Name, GetTypeName(), Anchor, Width(), Height(), left, top, right, bottom, MinSize.cx, MinSize.cy, MaxSize.cx, MaxSize.cy, (Parent == NULL? _T("NULL") : Parent->Name), Children.GetCount());
  return sTo;
}
CString WinResizer::CPanel::GetTypeName()
{
  return _T("CPanel");
}

CWnd * WinResizer::CPanel::GetHookedWnd()
{
  if (Parent != NULL)
  {
    return Parent->GetHookedWnd();
  }
  return NULL;
}

/////////////////////////////  CSplitContainer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WinResizer::CSplitContainer::CSplitContainer(CSplitPanel * pPanelA, CSplitPanel * pPanelB, WinResizer::SplitterOrientation type) : CPanel()
{
  m_IsSplitterFixed = FALSE;
  m_FixedPanel = 0;

  m_pPanelA = NULL;
  m_pPanelB = NULL;
  m_pSplitter = NULL;

  m_Orientation = type;
  m_pPanelA = pPanelA;
  m_pPanelB = pPanelB;
  UnionRect(m_pPanelA, m_pPanelB);  

  CRect rc(0, 0, 0, 0);  
  GetSplitArea(&rc);
  m_pSplitter = new CSpitterPanel(&rc, type);
  m_pSplitter->m_pGrippePanel->m_bVisible = FALSE;


  if (m_Orientation == WinResizer::SPLIT_CONTAINER_H)
  {
    m_pPanelA->Anchor = (ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_BOTTOM);
    m_pPanelB->Anchor = (ANCHOR_RIGHT | ANCHOR_TOP | ANCHOR_BOTTOM);
  }
  else
  {
    m_pPanelA->Anchor = (ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_RIGHT);
    m_pPanelB->Anchor = (ANCHOR_LEFT | ANCHOR_BOTTOM | ANCHOR_RIGHT);
  }

  m_nSplitterSize = GetSplitterSize(m_pPanelA, m_pPanelB);

  BOOL bOk = AddChild(m_pPanelA);
  ASSERT( bOk);

  bOk = AddChild(m_pSplitter);
  ASSERT( bOk);

  bOk = AddChild(m_pPanelB);
  ASSERT( bOk);

  UpdateRatio();
}

WinResizer::CSplitContainer::~CSplitContainer()
{

}

void WinResizer::CSplitContainer::OnResized()
{
  CPanel::OnResized();
 
  if (m_Orientation == WinResizer::SPLIT_CONTAINER_H)
  {
    if (Width() < MinSize.cx)
    {
      return;
    }

    if (m_FixedPanel == 1 ) // left panel is fixed
    {
      m_pPanelB->left = m_pPanelA->right + m_nSplitterSize;

      if (m_pPanelB->MinSize.cx > m_pPanelB->Width() )
      {
        m_pPanelB->left = m_pPanelB->right - m_pPanelB->MinSize.cx;
        m_pPanelA->right = m_pPanelB->left - m_nSplitterSize;
      }
    }
    else if (m_FixedPanel == 2 )  // right panel is fixed
    {
      m_pPanelA->right = m_pPanelB->left - m_nSplitterSize;

      if (m_pPanelA->MinSize.cx > m_pPanelA->Width() )
      {
        m_pPanelA->right = m_pPanelA->left + m_pPanelA->MinSize.cx;
        m_pPanelB->left = m_pPanelA->right + m_nSplitterSize;
      }
    }
    else
    {
      m_pPanelA->right = (LONG) ((double)m_pPanelA->left + ((double)this->Width() * m_nRatio));

      if (m_pPanelA->MinSize.cx > m_pPanelA->Width() )
      {
        m_pPanelA->right = m_pPanelA->left + m_pPanelA->MinSize.cx;
      }

      m_pPanelB->left = m_pPanelA->right + m_nSplitterSize;

      if (m_pPanelB->MinSize.cx > m_pPanelB->Width() )
      {
        m_pPanelB->left = m_pPanelB->right - m_pPanelB->MinSize.cx;
        m_pPanelA->right = m_pPanelB->left - m_nSplitterSize;
      }
    }

  }
  else /*if (m_Orientation == WinResizer::SPLIT_CONTAINER_V)*/
  {
    if (Height() < MinSize.cy)
    {
      return;
    }

    if (m_FixedPanel == 1 ) // top panel is fixed
    {
      m_pPanelB->top = m_pPanelA->bottom + m_nSplitterSize;

      if (m_pPanelB->MinSize.cy > m_pPanelB->Height() )
      {
        m_pPanelB->top = m_pPanelB->bottom - m_pPanelB->MinSize.cy;
        m_pPanelA->bottom = m_pPanelB->top - m_nSplitterSize;
      }
    }
    else if (m_FixedPanel == 2 )  // bottom panel is fixed
    {
      m_pPanelA->bottom = m_pPanelB->top - m_nSplitterSize;

      if (m_pPanelA->MinSize.cy > m_pPanelA->Height() )
      {
        m_pPanelA->bottom = m_pPanelA->top + m_pPanelA->MinSize.cy;
        m_pPanelB->top = m_pPanelA->bottom + m_nSplitterSize;
      }
    }
    else
    {
      m_pPanelA->bottom = (LONG) ((double)m_pPanelA->top + ((double)this->Height() * m_nRatio));

      if (m_pPanelA->MinSize.cy > m_pPanelA->Height() )
      {
        m_pPanelA->bottom = m_pPanelA->top + m_pPanelA->MinSize.cy;
      }

      m_pPanelB->top = m_pPanelA->bottom + m_nSplitterSize;

      if (m_pPanelB->MinSize.cy > m_pPanelB->Height() )
      {
        m_pPanelB->top = m_pPanelB->bottom - m_pPanelB->MinSize.cy;
        m_pPanelA->bottom = m_pPanelB->top - m_nSplitterSize;
      }
    }


  }

  GetSplitArea(m_pSplitter);
  m_pPanelA->OnResized();
  m_pPanelB->OnResized();
  m_pSplitter->OnResized();

}
void WinResizer::CSplitContainer::SetSplitterPosition(int leftOfSplitter)
{ 

  short nFixedPanel = m_FixedPanel;
  m_FixedPanel = 0;
  if (m_Orientation == WinResizer::SPLIT_CONTAINER_H )
  {    
    m_pPanelA->right = leftOfSplitter;
    m_pPanelB->left = m_pPanelA->right + m_nSplitterSize;
  }
  else
  {
    m_pPanelA->bottom = leftOfSplitter;
    m_pPanelB->top = m_pPanelA->bottom + m_nSplitterSize;
  }
  UpdateRatio();

  OnResized();

  UpdateRatio();
  m_FixedPanel = nFixedPanel;

}
int WinResizer::CSplitContainer::GetSplitterPosition()
{ 
  if (m_Orientation == WinResizer::SPLIT_CONTAINER_H )
  {    
    return m_pPanelA->right;
  }
  else
  {
    return m_pPanelA->bottom;
  }
}
BOOL WinResizer::CSplitContainer::AddChild(CPanel * prc)
{
  if (Children.GetCount() == 3)
  {
    return FALSE;
  }
  return CPanel::AddChild(prc);

}
BOOL WinResizer::CSplitContainer::RemoveChild(CPanel * prc)
{
  return FALSE; // cannot remove child from split container
}

void WinResizer::CSplitContainer::GetSplitArea(CRect * pSplitterPanel)
{

  if (m_Orientation == WinResizer::SPLIT_CONTAINER_H)
  {
    pSplitterPanel->left = m_pPanelA->right;
    pSplitterPanel->top = this->top;
    pSplitterPanel->right = m_pPanelB->left;
    pSplitterPanel->bottom = this->bottom;
  }
  else // vertical
  {
    pSplitterPanel->left = this->left;
    pSplitterPanel->top = m_pPanelA->bottom;
    pSplitterPanel->right = this->right;
    pSplitterPanel->bottom = m_pPanelB->top;
  }  
}
int WinResizer::CSplitContainer::GetSplitterSize(CPanel * m_pPanelA, CPanel * m_pPanelB)
{
  if (m_Orientation == WinResizer::SPLIT_CONTAINER_H)
  {
    int nWidth = m_pPanelB->left - m_pPanelA->right;
    return nWidth;
  }
  else // vertical
  {
    int nHeight = m_pPanelB->top - m_pPanelA->bottom;
    return nHeight;
  }  
}




void WinResizer::CSplitContainer::UpdateRatio()
{

  if (m_Orientation == WinResizer::SPLIT_CONTAINER_H )
  {    
    m_nRatio = (double)m_pPanelA->Width() / (double)this->Width();
  }
  else
  {
    m_nRatio = (double)m_pPanelA->Height() / (double)this->Height();
  }

}

WinResizer::CSplitContainer *  WinResizer::CSplitContainer::Create(CPanel * pPanelA, CPanel * pPanelB)
{
  CSplitPanel * pSplitPanelA =  dynamic_cast<CSplitPanel *>( pPanelA );
  if (pSplitPanelA != NULL)
  {
    return NULL;  // already a part of a CSplitContainer
  }

  CSplitPanel * pSplitPanelB =  dynamic_cast<CSplitPanel *>( pPanelB );
  if (pSplitPanelB != NULL)
  {
    return NULL; // already a part of a CSplitContainer
  }

  CRect rcDest(0, 0, 0, 0);
  WinResizer::SplitterOrientation orien = WinResizer::SPLIT_CONTAINER_H;

  if (::IntersectRect(&rcDest, pPanelA, pPanelB) == TRUE) // invalid spliter container, a spliter continer's two panel cannot intersect each other
  {
    return NULL;
  } 


  if (pPanelA->right < pPanelB->left)
  {
      orien = WinResizer::SPLIT_CONTAINER_H;    
  }
  else if (pPanelA->bottom < pPanelB->top)
  {
      orien =  WinResizer::SPLIT_CONTAINER_V;    
  }
  else
  {
    return NULL;
  }
  if (pPanelA->Parent != NULL)
  {
    if (pPanelA->Parent->RemoveChild( pPanelA ) == FALSE)
    {
      return NULL;
    }
  }
  if (pPanelB->Parent != NULL)
  {
    if (pPanelB->Parent->RemoveChild( pPanelB ) == FALSE)
    {
      return NULL;
    }
  }

  pSplitPanelA = new CSplitPanel( pPanelA );  
  pSplitPanelB = new CSplitPanel( pPanelB );  


  CSplitContainer * pSpliter = new CSplitContainer(pSplitPanelA, pSplitPanelB, orien);
  return pSpliter;

}

CString WinResizer::CSplitContainer::GetTypeName()
{
  return _T("CSplitContainer");
}

void WinResizer::CSplitContainer::SetFixedPanel(short nFixedPanel /* 1=left/top; 2=right/bototm; other=no fixed panel */)
{
  m_FixedPanel = nFixedPanel;
}
short WinResizer::CSplitContainer::GetFixedPanel()
{
  return m_FixedPanel;
}
void WinResizer::CSplitContainer::SetIsSplitterFixed(BOOL bFixed)
{
  m_IsSplitterFixed = bFixed;
}
BOOL WinResizer::CSplitContainer::GetIsSplitterFixed()
{
  return m_IsSplitterFixed;
}
void WinResizer::CSplitContainer::SetShowSplitterGrip(BOOL bShow)
{
  m_pSplitter->m_pGrippePanel->m_bVisible = bShow;
}
BOOL WinResizer::CSplitContainer::GetShowSplitterGrip()
{
  return m_pSplitter->m_pGrippePanel->m_bVisible;
}


/////////////////////////////  CRootPanel
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WinResizer::CRootPanel::CRootPanel() : CPanel()
{
  m_pHookWnd = NULL;
}
WinResizer::CRootPanel::~CRootPanel()
{

}
CWnd * WinResizer::CRootPanel::GetHookedWnd()
{
  return m_pHookWnd;
}
CString WinResizer::CRootPanel::GetTypeName()
{
  return _T("CRootPanel");
}
/////////////////////////////  CUIPanel
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WinResizer::CUIPanel::CUIPanel(const CRect * prc, UINT uID) : CPanel(prc)
{
  m_uID = uID;
  m_bOle = FALSE;
}
WinResizer::CUIPanel::~CUIPanel()
{

}
CString WinResizer::CUIPanel::GetTypeName()
{
  return _T("CUIPanel");
}

/////////////////////////////  CVisualPanel
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WinResizer::CVisualPanel::CVisualPanel() : CPanel()
{
  m_bVisible = FALSE;
  m_rcPrev.SetRect(0, 0, 0, 0);
}
WinResizer::CVisualPanel::CVisualPanel(const CRect * prc) : CPanel(prc)
{
  m_bVisible = FALSE;
  m_rcPrev.SetRect(this->left, this->top, this->right, this->bottom);
}
WinResizer::CVisualPanel::~CVisualPanel()
{


}
void WinResizer::CVisualPanel::Draw(CDC * pDC)
{

}
CString WinResizer::CVisualPanel::GetTypeName()
{
  return _T("CVisualPanel");
}
void WinResizer::CVisualPanel::OnResized()
{
  CWnd * pWnd = NULL;
  if ((pWnd = GetHookedWnd()) != NULL)
  {
    pWnd->InvalidateRect( &m_rcPrev, FALSE );
    pWnd->InvalidateRect( this, FALSE );
  }
  m_rcPrev.SetRect(this->left, this->top, this->right, this->bottom);
}

/////////////////////////////  CGripperPanel
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WinResizer::CGripperPanel::CGripperPanel() : CVisualPanel()
{
  m_hTheme = NULL;
  m_iPartId = SBP_SIZEBOX;
  m_iStateId = 5; //SZB_HALFBOTTOMRIGHTALIGN;
  m_sClassName = _T("SCROLLBAR");
}
WinResizer::CGripperPanel::CGripperPanel(const CRect * prc) : CVisualPanel(prc)
{
  m_hTheme = NULL;
  m_iPartId = SBP_SIZEBOX;
  m_iStateId = 5; //SZB_HALFBOTTOMRIGHTALIGN;
  m_sClassName = _T("SCROLLBAR");
}
WinResizer::CGripperPanel::~CGripperPanel()
{
  if (m_hTheme != NULL)
  {
    HRESULT lres = ::CloseThemeData(m_hTheme);
    ASSERT(SUCCEEDED(lres) == TRUE);  
    m_hTheme = NULL;
  }
}
void WinResizer::CGripperPanel::Draw(CDC * pDC)
{
  if (m_hTheme == NULL)
  {
    CWnd * pHookedWnd = GetHookedWnd();
    if (pHookedWnd == NULL)
    {
      return;
    }
    m_hTheme = ::OpenThemeData(pHookedWnd->m_hWnd, m_sClassName.AllocSysString());
  }

  if (m_hTheme == NULL)
  {
    BOOL bOk = pDC->DrawFrameControl(this, DFC_SCROLL, DFCS_SCROLLSIZEGRIP );
     ASSERT( bOk );
  }
  else
  {
    HRESULT lres = ::DrawThemeBackground(m_hTheme, pDC->m_hDC, m_iPartId, m_iStateId, this, this);
    ASSERT(SUCCEEDED(lres) == TRUE);
  }
}

CString WinResizer::CGripperPanel::GetTypeName()
{
  return _T("CGripperPanel");
}

/////////////////////////////  CSplitterGripperPanel
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WinResizer::CSplitterGripperPanel::CSplitterGripperPanel(WinResizer::SplitterOrientation type) : CVisualPanel()
{
  m_OrienType = type;
}
WinResizer::CSplitterGripperPanel::~CSplitterGripperPanel()
{

}
void WinResizer::CSplitterGripperPanel::Draw(CDC * pDC)
{
  CPen penDark(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
  CPen penWhite(PS_SOLID, 1, RGB(255, 255, 255));

  if (m_OrienType == WinResizer::SPLIT_CONTAINER_H )
  {
    CPen * pOrigPen = pDC->SelectObject(&penWhite);    

    CRect rc(0, 0, 0, 0);
    rc.SetRect(left + 1, top + 1, left + 3, top + 3); 
    while(rc.bottom <= bottom)
    {
      pDC->Rectangle(&rc);
      rc.OffsetRect(0, 4);
   }

    pDC->SelectObject(&penDark);
    rc.SetRect(left, top, left + 2, top + 2);
    while(rc.bottom <= bottom)
    {
      pDC->Rectangle(&rc);
     rc.OffsetRect(0, 4);
   }

    pDC->SelectObject(pOrigPen);

  }
  else
  {
    CPen * pOrigPen = pDC->SelectObject(&penWhite);    

    CRect rc(0, 0, 0, 0);
    rc.SetRect(left + 1, top + 1, left + 3, top + 3); 
    while(rc.right <= right)
    {
      pDC->Rectangle(&rc);
      rc.OffsetRect(4, 0);
    }

    pDC->SelectObject(&penDark);
    rc.SetRect(left, top, left + 2, top + 2);
    while(rc.right <= right)
    {
      pDC->Rectangle(&rc);
      rc.OffsetRect(4, 0);
    }

    pDC->SelectObject(pOrigPen);
  } 
}

CString WinResizer::CSplitterGripperPanel::GetTypeName()
{
  return _T("CSplitterGripperPanel");
}


/////////////////////////////  CSpitterPanel
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WinResizer::CSpitterPanel::CSpitterPanel(const CRect * prc, WinResizer::SplitterOrientation type) : CPanel(prc)
{
  m_OrienType = type;
  m_pGrippePanel = NULL;

  if (m_pGrippePanel == NULL)
  {
    m_pGrippePanel = new CSplitterGripperPanel(type);
    if (m_OrienType == WinResizer::SPLIT_CONTAINER_H )
    {
      m_pGrippePanel->SetRect(0, 0, 3, 12);
    }
    else
    {
      m_pGrippePanel->SetRect(0, 0, 12, 3);
    }
    m_pGrippePanel->SetAnchor ( ANCHOR_HORIZONTALLY_CENTERED | ANCHOR_VERTICALLY_CENTERED );
    m_pGrippePanel->SetMinSize(CSize(2,2));
    BOOL bOk = AddChild(m_pGrippePanel);
    ASSERT( bOk);
  }
}
WinResizer::CSpitterPanel::CSpitterPanel(WinResizer::SplitterOrientation type) : CPanel()
{
  m_OrienType = type;
  m_pGrippePanel = NULL;

}

WinResizer::CSpitterPanel::~CSpitterPanel()
{

}


CString WinResizer::CSpitterPanel::GetTypeName()
{
  return _T("CSpitterPanel");
}
/////////////////////////////  CSpitPanel
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WinResizer::CSplitPanel::CSplitPanel(CPanel * pPanel) : CPanel(pPanel)
{
  pPanel->LeftOffset = 0;
  pPanel->TopOffset = 0;
  pPanel->RightOffset = 0;
  pPanel->BottomOffset = 0;
  pPanel->Anchor = ANCHOR_ALL;
  Name = pPanel->Name;
  pPanel->Name = _T("");
  m_pOriginalPanel = pPanel;
  MaxSize = pPanel->MaxSize;
  MinSize = pPanel->MinSize;
  Children.AddTail( pPanel );
}

WinResizer::CSplitPanel::~CSplitPanel()
{

}

BOOL WinResizer::CSplitPanel::SetAnchor(UINT anchor)
{
  return FALSE;
}

BOOL WinResizer::CSplitPanel::AddChild(CPanel * prc)
{
  return m_pOriginalPanel->AddChild(prc);
}
BOOL WinResizer::CSplitPanel::RemoveChild(CPanel * prc)
{
  return m_pOriginalPanel->RemoveChild(prc);
}


CString WinResizer::CSplitPanel::GetTypeName()
{
  return _T("CSplitPanel");
}

/////////////////////////////  CFlowLayoutPanel
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WinResizer::CFlowLayoutPanel::CFlowLayoutPanel() : CPanel()
{
  m_nItemSpacingX = 0;
  m_nItemSpacingY = 0;
  m_nFlowDirection = WinResizer::LEFT_TO_RIGHT;
}
WinResizer::CFlowLayoutPanel::CFlowLayoutPanel(const CRect * prc) : CPanel(prc)
{
  m_nItemSpacingX = 0;
  m_nItemSpacingY = 0;
  m_nFlowDirection = WinResizer::LEFT_TO_RIGHT;
}
WinResizer::CFlowLayoutPanel::~CFlowLayoutPanel()
{

}
void WinResizer::CFlowLayoutPanel::OnResized()
{
  int max = 0; // maximimum height of a item in the row in case of left-to-right, otherwise maximum width of a item
  int x = left;
  int y = top;
  POSITION pos = Children.GetHeadPosition();

  // first one will be at the top-left corner, no matter what
  if (pos != NULL)
  {
    CPanel * pPanel = Children.GetNext(pos);
    pPanel->MoveToXY(x, y);
    pPanel->OnResized();

    if (m_nFlowDirection == WinResizer::LEFT_TO_RIGHT)
    {
      x += pPanel->Width() + m_nItemSpacingX;
      max = (pPanel->Height() > max ? pPanel->Height() : max);
    }
    else
    {
      y += pPanel->Height() + m_nItemSpacingY;
      max = (pPanel->Width() > max ? pPanel->Width() : max);
    }
  }


  if (m_nFlowDirection == WinResizer::LEFT_TO_RIGHT)
  {
    while(pos != NULL)
    {
      CPanel * pPanel = Children.GetNext(pos);
      // check to see if it is to wrap
      if (x + pPanel->Width() > right)
      {
        x = left;
        y += (max + m_nItemSpacingY);
        max = 0;
      }

      pPanel->MoveToXY(x, y);
      pPanel->OnResized();

      x += pPanel->Width() + m_nItemSpacingX;
      max = (pPanel->Height() > max ? pPanel->Height() : max);
    }
  }
  else
  {
    while(pos != NULL)
    {
      CPanel * pPanel = Children.GetNext(pos);
      // check to see if it is to wrap
      if (y + pPanel->Height() > bottom)
      {
        x += (max + m_nItemSpacingX);
        y = top;
        max = 0;
      }

      pPanel->MoveToXY(x, y);
      pPanel->OnResized();

      y += pPanel->Height() + m_nItemSpacingY;
      max = (pPanel->Width() > max ? pPanel->Width() : max);
    }

  }
}
CString WinResizer::CFlowLayoutPanel::GetTypeName()
{
  return _T("CFlowLayoutPanel");

}
void WinResizer::CFlowLayoutPanel::SetFlowDirection(WinResizer::FlowDirection direction)
{
  m_nFlowDirection = direction;
}
WinResizer::FlowDirection WinResizer::CFlowLayoutPanel::GetFlowDirection()
{
  return m_nFlowDirection;
}
void WinResizer::CFlowLayoutPanel::SetItemSpacingX(int nSpace)
{
  m_nItemSpacingX = nSpace;
}
int WinResizer::CFlowLayoutPanel::GetItemSpacingX()
{
  return m_nItemSpacingX;
}
void WinResizer::CFlowLayoutPanel::SetItemSpacingY(int nSpace)
{
  m_nItemSpacingY = nSpace;
}
int WinResizer::CFlowLayoutPanel::GetItemSpacingY()
{
  return m_nItemSpacingY;
}

