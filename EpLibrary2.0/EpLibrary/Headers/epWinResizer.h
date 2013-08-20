/*
@file epWinResizer.h
@author Mizan Rahman - 25 Jan 2013
    edited by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com> - June 07, 2013
    <http://github.com/juhgiyo/eplibrary>
@date Jan 25, 2013
@brief WinResizer Interface

@section LICENSE

The MIT License (MIT)

Copyright (c) 2012-2013 Woong Gyu La <juhgiyo@gmail.com>

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

-------------------------------------------------------------------------------

@section USAGE

WinResizer m_resizer;
...
m_resizer.Hook(this);

m_resizer.SetAnchor(IDC_BTN_LOAD_PRE_TEXT,ANCHOR_LEFT|ANCHOR_TOP);
m_resizer.SetAnchor(IDC_BTN_DELETE,ANCHOR_LEFT|ANCHOR_TOP);
m_resizer.SetAnchor(IDC_TREE1,ANCHOR_RIGHT|ANCHOR_TOP|ANCHOR_LEFT|ANCHOR_BOTTOM);

BOOL bOk = FALSE;
m_resizer.SetShowResizeGrip(TRUE);

bOk = m_resizer.InvokeOnResized();
ASSERT( bOk);

*/

#ifndef __EP_WIN_RESIZER_H__
#define __EP_WIN_RESIZER_H__

#include "epLib.h"
#include "uxtheme.h"


#define ANCHOR_LEFT                   1
#define ANCHOR_TOP                    2
#define ANCHOR_RIGHT                  4
#define ANCHOR_BOTTOM                 8
#define ANCHOR_HORIZONTALLY_CENTERED  16
#define ANCHOR_VERTICALLY_CENTERED    32
#define ANCHOR_PRIORITY_RIGHT         64   // by defualt, left has higher priority
#define ANCHOR_PRIORITY_BOTTOM        128  // by defualt, top has higher priority
#define ANCHOR_VERTICALLY             (ANCHOR_TOP | ANCHOR_BOTTOM)
#define ANCHOR_HORIZONTALLY           (ANCHOR_LEFT | ANCHOR_RIGHT)
#define ANCHOR_ALL                    (ANCHOR_VERTICALLY | ANCHOR_HORIZONTALLY)

#define DOCK_NONE   0
#define DOCK_LEFT   1
#define DOCK_TOP    2
#define DOCK_RIGHT  3
#define DOCK_BOTTOM 4
#define DOCK_FILL   5

namespace epl
{
	/*! 
	@class WinResizer epWinResizer.h
	@brief A class for WinResizer.
	*/
	class EP_LIBRARY WinResizer 
	{
	public:
	  WinResizer(void);
	  ~WinResizer(void);
	private:

	  class CPanel;
	  typedef CList<CPanel *, CPanel *> CPanelList;

	  typedef enum tagSplitterOrientation 
	  {
		// a horizontal splitter container panel
		SPLIT_CONTAINER_H = 1,
		// a vertical splitter contianer panel
		SPLIT_CONTAINER_V = 2, 

	  } SplitterOrientation;


	  typedef enum tagFlowDirection 
	  {
		// left to right flow direction
		LEFT_TO_RIGHT = 3,
		// top to bottom flow direction
		TOP_TO_BOTTOM = 4, 

	  } FlowDirection;

	  class EP_LIBRARY CPanel : public CRect
	  {
	  public:
		CPanel();
		CPanel(const CRect * prc);
		virtual ~CPanel();
		UINT Anchor;
		UINT Dock;
		CString Name;
		CPanel * Parent;
		CPanelList Children;
		int LeftOffset;
		int TopOffset;
		int RightOffset;
		int BottomOffset; 
		CSize MinSize;
		CSize MaxSize;
		virtual void OnResized();
		virtual BOOL AddChild(CPanel * prc);
		virtual BOOL RemoveChild(CPanel * prc);
		virtual BOOL SetAnchor(UINT anchor);
		virtual BOOL SetMinSize(CSize & size);
		virtual BOOL SetMaxSize(CSize & size);
		virtual CString ToString();
		virtual CString GetTypeName();
		virtual CWnd * GetHookedWnd();
	  private:
		void Init();
	  };

	  class EP_LIBRARY CRootPanel : public CPanel
	  {
	  public:
		CRootPanel();
		~CRootPanel();
		virtual CWnd * GetHookedWnd();
		CWnd * m_pHookWnd;
		virtual CString GetTypeName();
	  }; 

	  class EP_LIBRARY CUIPanel : public CPanel
	  {
	  public:
		CUIPanel(const CRect * prc, UINT uID);
		~CUIPanel();
		UINT m_uID;   // could be a resource ID or a HWND
		BOOL m_bOle;  // TRUE= m_uID is an ID of an ActiveX or OLE control, FALSE=regular windows control
		virtual CString GetTypeName();
	  };


	  class EP_LIBRARY CVisualPanel : public CPanel
	  {
	  public:
		CVisualPanel();
		CVisualPanel(const CRect * prc);
		~CVisualPanel();
		virtual void Draw(CDC * pDC);
		BOOL m_bVisible;
		virtual CString GetTypeName();
		virtual void OnResized();
	  private:
		CRect m_rcPrev;
	  };

	  class EP_LIBRARY CGripperPanel : public CVisualPanel
	  {
	  public:
		CGripperPanel();
		CGripperPanel(const CRect * prc);
		~CGripperPanel();
		virtual void Draw(CDC * pDC);
		virtual CString GetTypeName();
	   private:
		HTHEME m_hTheme;
		int m_iPartId;
		int m_iStateId;
		CString m_sClassName;
	  };

	  class EP_LIBRARY CSplitterGripperPanel : public CVisualPanel
	  {
	  public:
		CSplitterGripperPanel(WinResizer::SplitterOrientation type);
		~CSplitterGripperPanel();
		virtual void Draw(CDC * pDC);
		virtual CString GetTypeName();
	  private:
		WinResizer::SplitterOrientation m_OrienType;
	  };

	  class EP_LIBRARY CSplitPanel : public CPanel
	  {
	  public:
		CSplitPanel(CPanel * pPanel);
		~CSplitPanel();
		virtual BOOL SetAnchor(UINT anchor);
		virtual BOOL AddChild(CPanel * prc);
		virtual BOOL RemoveChild(CPanel * prc);
		virtual CString GetTypeName();
	  private:
		CPanel * m_pOriginalPanel;
	  };


	  class EP_LIBRARY CSpitterPanel : public CPanel
	  {
	  public:
		CSpitterPanel(WinResizer::SplitterOrientation type);
		CSpitterPanel(const CRect * prc, WinResizer::SplitterOrientation type);
		~CSpitterPanel();
		virtual CString GetTypeName();
		CSplitterGripperPanel * m_pGrippePanel;
	  private:
		WinResizer::SplitterOrientation m_OrienType;
	  };


	  class EP_LIBRARY CSplitContainer : public CPanel
	  {
	  public:
		CSplitContainer(CSplitPanel * pPanelA, CSplitPanel * pPanelB, WinResizer::SplitterOrientation type);
		~CSplitContainer();
		virtual void OnResized();
		virtual BOOL AddChild(CPanel * prc);
		virtual BOOL RemoveChild(CPanel * prc);
		virtual CString GetTypeName();
		void SetSplitterPosition(int leftOfSpliter /* or topOfSpliter if vertical */);
		int GetSplitterPosition();
		void SetFixedPanel(short nFixedPanel /* 1=left or top; 2=right or bototm; other=no fixed panel */);
		short GetFixedPanel();
		void SetIsSplitterFixed(BOOL bFixed);
		BOOL GetIsSplitterFixed();
		void SetShowSplitterGrip(BOOL bShow);
		BOOL GetShowSplitterGrip();

		static WinResizer::CSplitContainer * Create(CPanel * pPanelA, CPanel * pPanelB);
	    
		WinResizer::SplitterOrientation m_Orientation;
	  private:
		BOOL m_IsSplitterFixed;
		short m_FixedPanel; // 1=left or top panel; 2=right or bottom panel, otherwise no fixed panel

		double m_nRatio;
		int m_nSplitterSize; // for horizontal, it is the splitter width, otherwise it is the splitter height
		void GetSplitArea(CRect * pSplitterPanel);
		int GetSplitterSize(CPanel * pLeftPanel, CPanel * pRightPanel);
		void UpdateRatio();

		CSplitPanel * m_pPanelA;
		CSplitPanel * m_pPanelB;
		CSpitterPanel * m_pSplitter;
	  };


	  class EP_LIBRARY CFlowLayoutPanel : public CPanel
	  {
	  public:
		CFlowLayoutPanel();
		CFlowLayoutPanel(const CRect * prc);
		~CFlowLayoutPanel();
		virtual void OnResized();
		virtual CString GetTypeName();
		void SetFlowDirection(WinResizer::FlowDirection direction);
		WinResizer::FlowDirection GetFlowDirection();
		void SetItemSpacingX(int nSpace);
		int GetItemSpacingX();
		void SetItemSpacingY(int nSpace);
		int GetItemSpacingY();

	  private:
		int m_nItemSpacingX;
		int m_nItemSpacingY;
		WinResizer::FlowDirection m_nFlowDirection;
	  };

	public:
	  BOOL SetAnchor(LPCTSTR panelName, UINT anchor);
	  BOOL SetAnchor(UINT panelID, UINT anchor);
	  BOOL GetAnchor(LPCTSTR panelName, UINT & anchor);
	  BOOL GetAnchor(UINT panelID, UINT & anchor);


	  BOOL SetDock(LPCTSTR panelName, UINT anchor);
	  BOOL SetDock(UINT panelID, UINT anchor);
	  BOOL GetDock(LPCTSTR panelName, UINT & anchor);
	  BOOL GetDock(UINT panelID, UINT & anchor);

	  BOOL SetFixedPanel(LPCTSTR splitContainerName, short panel);
	  BOOL GetFixedPanel(LPCTSTR splitContainerName, short & panel);

	  BOOL SetIsSplitterFixed(LPCTSTR splitContainerName, BOOL fixed);
	  BOOL GetIsSplitterFixed(LPCTSTR splitContainerName, BOOL &fixed);

	  BOOL SetFlowDirection(LPCTSTR flowPanelName, short direction);
	  BOOL GetFlowDirection(LPCTSTR flowPanelName, short &direction);

	  BOOL SetFlowItemSpacingX(LPCTSTR flowPanelName, int nSpacing);
	  BOOL GetFlowItemSpacingX(LPCTSTR flowPanelName, int &nSpacing);

	  BOOL SetFlowItemSpacingY(LPCTSTR flowPanelName, int nSpacing);
	  BOOL GetFlowItemSpacingY(LPCTSTR flowPanelName, int &nSpacing);


	  BOOL SetShowSplitterGrip(LPCTSTR splitContainerName, BOOL bShow);
	  BOOL GetShowSplitterGrip(LPCTSTR splitContainerName, BOOL & bShow);

	  BOOL SetParent(LPCTSTR panelName, LPCTSTR parentName);
	  BOOL SetParent(UINT panelID, LPCTSTR parentName);
	  BOOL SetParent(LPCTSTR panelName, UINT parentID);
	  BOOL SetParent(UINT panelID, UINT parentID);
	  BOOL GetParent(LPCTSTR panelName, CString & parentName);
	  BOOL GetParent(UINT panelID, CString & parentName);

	  BOOL SetMinimumSize(LPCTSTR panelName, CSize & size);
	  BOOL SetMinimumSize(UINT panelID, CSize & size);
	  BOOL GetMinimumSize(LPCTSTR panelName, CSize & size) ;
	  BOOL GetMinimumSize(UINT panelID, CSize & size) ;

	  BOOL SetMaximumSize(LPCTSTR panelName, CSize & size);
	  BOOL SetMaximumSize(UINT panelID, CSize & size);
	  BOOL GetMaximumSize(LPCTSTR panelName, CSize & size) ;
	  BOOL GetMaximumSize(UINT panelID, CSize & size) ;

	  void SetShowResizeGrip(BOOL show = TRUE);
	  BOOL GetShowResizeGrip();
	  BOOL Hook(CWnd * pWnd);
	  BOOL Hook(CWnd * pWnd, CSize & size);
	  BOOL Unhook();
	  
	  BOOL CreateFlowLayoutPanel(LPCTSTR panelName, const CRect * prcPanel);
	  BOOL CreateFlowLayoutPanel(LPCTSTR panelName, const CUIntArray * parrID, BOOL setAsChildren = FALSE);

	  BOOL CreatePanel(LPCTSTR panelName, const CRect * prcPanel);
	  BOOL CreatePanel(LPCTSTR panelName, const CUIntArray * parrID, BOOL setAsChildren = FALSE);

	  BOOL CreatePanel(UINT uID);

	  BOOL CreateSplitContainer(LPCTSTR splitContainerName, LPCTSTR panelNameA, LPCTSTR panelNameB);    
	  BOOL CreateSplitContainer(LPCTSTR splitContainerName, LPCTSTR panelNameA, UINT panelIDB);  
	  BOOL CreateSplitContainer(LPCTSTR splitContainerName, UINT panelIDA, UINT panelIDB);  
	  BOOL CreateSplitContainer(LPCTSTR splitContainerName, UINT panelIDA, LPCTSTR panelNameB); 

	  // useful for persisting UI layout
	  BOOL SetSplitterPosition(LPCTSTR splitContainerName, UINT position);  
	  BOOL GetSplitterPosition(LPCTSTR splitContainerName, UINT & position);  

	  BOOL InvokeOnResized();
	  
	  CString GetDebugInfo();
	  
	private:
	  
	  CWnd * m_pHookedWnd;
	  CRootPanel root;
	  CPanel * m_pCaptured;
	  HCURSOR m_hOldCursor;
	  int m_splitterOffset;  

	  void OnPaint();
	  void OnSize(UINT nType, int cx, int cy);
	  void OnSizing(UINT fwSide, LPRECT pRect);
	  void OnScroll();
	  void OnDestroy();
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnLButtonUp(UINT nFlags, CPoint point);

	  WinResizer::CPanel * FindPanelByName(WinResizer::CPanel * pRoot, LPCTSTR name);
	  WinResizer::CPanel * FindSplitterFromPoint(WinResizer::CPanel * pRoot, CPoint point);
	  void GetUIPanels(WinResizer::CPanel * pRoot, CPanelList * pList, BOOL bOle);
	  void GetVisualPanels(WinResizer::CPanel * pRoot, CPanelList * pList);

	  void ResizeUI(WinResizer::CPanel * pRoot);

	  CString IdToName(UINT uID);
	  WinResizer::CUIPanel * CreateUIPanel(UINT uID);
	  WinResizer::CUIPanel * GetUIPanel(UINT uID);

	  void UpdateSplitterOffset(CPoint ptCurr);
	  void GetDebugInfo(CPanel * pRoot, CString & info, CString  indent);
	  void GetTrueClientRect(CWnd * pWnd, CRect * prc);
	  void EnsureRootMinMax();

	  WNDPROC m_pfnWndProc;
	  static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	};
}

#endif //__EP_WIN_RESIZER_H__
