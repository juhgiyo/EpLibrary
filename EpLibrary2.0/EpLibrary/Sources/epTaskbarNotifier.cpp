/*! 
TaskbarNotifier for the EpLibrary

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

Copyright (c) 2002 John O'Byrne

Please refer to <http://www.codeproject.com/Articles/2562/Taskbar-Notification-dialog> for the license.
*/
#include "epTaskbarNotifier.h"

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

#define IDT_HIDDEN		0
#define IDT_APPEARING		1
#define IDT_WAITING		2
#define IDT_DISAPPEARING	3

#define TASKBAR_ON_TOP		1
#define TASKBAR_ON_LEFT		2
#define TASKBAR_ON_RIGHT	3
#define TASKBAR_ON_BOTTOM	4

// CTaskbarNotifier

IMPLEMENT_DYNAMIC(CTaskbarNotifier, CWnd)
CTaskbarNotifier::CTaskbarNotifier()
{
	m_strCaption="";
	m_pWndParent=NULL;
	m_bMouseIsOver=FALSE;
	m_hSkinRegion=NULL;
	m_hCursor=NULL;
	m_crNormalTextColor=RGB(133,146,181);
	m_crSelectedTextColor=RGB(10,36,106);
	m_nSkinHeight=0;
	m_nSkinWidth=0;
	
	m_dwTimeToShow=0;
	m_dwTimeToLive=0;
	m_dwTimeToHide=0;
	m_dwDelayBetweenShowEvents=0;
	m_dwDelayBetweenHideEvents=0;
	m_nStartPosX=0;
	m_nStartPosY=0;
	m_nCurrentPosX=0;
	m_nCurrentPosY=0;
	m_nIncrement=2;
	m_nTaskbarPlacement=0;
	m_nAnimStatus=IDT_HIDDEN;
	m_rcText.SetRect(0,0,0,0);
}

CTaskbarNotifier::~CTaskbarNotifier()
{
	// No need to delete the HRGN,  SetWindowRgn() owns it after being called
}

int CTaskbarNotifier::Create(CWnd *pWndParent)
{
	m_pWndParent=pWndParent;
	CString strWndClass=AfxRegisterWndClass(0,AfxGetApp()->LoadStandardCursor(IDC_ARROW),GetSysColorBrush(COLOR_WINDOW),NULL);
	return CreateEx(0,strWndClass,NULL,WS_POPUP,0,0,0,0,pWndParent->m_hWnd,NULL);
}

void CTaskbarNotifier::SetTextFont(const TCHAR * szFont,int nSize,int nNormalStyle,int nSelectedStyle)
{
	LOGFONT lf;
	m_myNormalFont.DeleteObject();
	m_myNormalFont.CreatePointFont(nSize,szFont);
	m_myNormalFont.GetLogFont(&lf);
	
	// We  set the Font of the unselected ITEM
	if (nNormalStyle & TN_TEXT_BOLD)
		lf.lfWeight = FW_BOLD;
	else
		lf.lfWeight = FW_NORMAL;
	
	if (nNormalStyle & TN_TEXT_ITALIC)
		lf.lfItalic=TRUE;
	else
		lf.lfItalic=FALSE;
	
	if (nNormalStyle & TN_TEXT_UNDERLINE)
		lf.lfUnderline=TRUE;
	else
		lf.lfUnderline=FALSE;

	m_myNormalFont.DeleteObject();
	m_myNormalFont.CreateFontIndirect(&lf);
	
	// We set the Font of the selected ITEM
	if (nSelectedStyle & TN_TEXT_BOLD)
		lf.lfWeight = FW_BOLD;
	else
		lf.lfWeight = FW_NORMAL;
	
	if (nSelectedStyle & TN_TEXT_ITALIC)
		lf.lfItalic=TRUE;
	else
		lf.lfItalic=FALSE;
	
	if (nSelectedStyle & TN_TEXT_UNDERLINE)
		lf.lfUnderline=TRUE;
	else
		lf.lfUnderline=FALSE;

	m_mySelectedFont.DeleteObject();
	m_mySelectedFont.CreateFontIndirect(&lf);
}

void CTaskbarNotifier::SetTextColor(COLORREF crNormalTextColor,COLORREF crSelectedTextColor)
{
	m_crNormalTextColor=crNormalTextColor;
	m_crSelectedTextColor=crSelectedTextColor;
	RedrawWindow();
}

void CTaskbarNotifier::SetTextRect(RECT rcText)
{
	m_rcText=rcText;
}

BOOL CTaskbarNotifier::SetSkin(UINT nBitmapID,short red,short green,short blue)
{
	BITMAP bm;
	
	m_biSkinBackground.DeleteObject();

	if (!m_biSkinBackground.LoadBitmap(nBitmapID))
		return FALSE;
	GetObject(m_biSkinBackground.GetSafeHandle(), sizeof(bm), &bm);
	m_nSkinWidth=bm.bmWidth;
	m_nSkinHeight=bm.bmHeight;
	m_rcText.SetRect(0,0,bm.bmWidth,bm.bmHeight);

	if (red!=-1 && green!=-1 && blue!=-1)
	{
		// No need to delete the HRGN,  SetWindowRgn() owns it after being called
		m_hSkinRegion=GenerateRegion((HBITMAP)m_biSkinBackground.GetSafeHandle(),(unsigned char) red,(unsigned char) green,(unsigned char) blue);
		SetWindowRgn(m_hSkinRegion, true);
	}

	return TRUE;
}

BOOL CTaskbarNotifier::SetSkin(const TCHAR * szFileName,short red,short green,short blue)
{
	BITMAP bm;
	HBITMAP hBmp;
	
	hBmp=(HBITMAP) ::LoadImage(AfxGetInstanceHandle(),szFileName,IMAGE_BITMAP,0,0, LR_LOADFROMFILE);
	if (!hBmp)
		return FALSE;

	m_biSkinBackground.DeleteObject();
	m_biSkinBackground.Attach(hBmp);
	GetObject(m_biSkinBackground.GetSafeHandle(), sizeof(bm), &bm);
	m_nSkinWidth=bm.bmWidth;
	m_nSkinHeight=bm.bmHeight;
	m_rcText.SetRect(0,0,bm.bmWidth,bm.bmHeight);

	if (red!=-1 && green!=-1 && blue!=-1)
	{
		// No need to delete the HRGN,  SetWindowRgn() owns it after being called
		m_hSkinRegion=GenerateRegion((HBITMAP)m_biSkinBackground.GetSafeHandle(),(unsigned char) red,(unsigned char) green,(unsigned char) blue);
		SetWindowRgn(m_hSkinRegion, true);
	}

	return TRUE;
}

void CTaskbarNotifier::Show(const TCHAR * szCaption,unsigned long dwTimeToShow,unsigned long dwTimeToLive,unsigned long dwTimeToHide,int nIncrement)
{
	unsigned int nDesktopHeight;
	unsigned int nDesktopWidth;
	unsigned int nScreenWidth;
	unsigned int nScreenHeight;
	CRect rcDesktop;

	m_strCaption=szCaption;
	m_dwTimeToShow=dwTimeToShow;
	m_dwTimeToLive=dwTimeToLive;
	m_dwTimeToHide=dwTimeToHide;

	::SystemParametersInfo(SPI_GETWORKAREA,0,&rcDesktop,0);
	nDesktopWidth=rcDesktop.right-rcDesktop.left;
	nDesktopHeight=rcDesktop.bottom-rcDesktop.top;
	nScreenWidth=::GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight=::GetSystemMetrics(SM_CYSCREEN);

	BOOL bTaskbarOnRight=nDesktopWidth<nScreenWidth && rcDesktop.left==0;
	BOOL bTaskbarOnLeft=nDesktopWidth<nScreenWidth && rcDesktop.left!=0;
	BOOL bTaskBarOnTop=nDesktopHeight<nScreenHeight && rcDesktop.top!=0;
	BOOL bTaskbarOnBottom=nDesktopHeight<nScreenHeight && rcDesktop.top==0;
	
	switch (m_nAnimStatus)
	{
		case IDT_HIDDEN:
			ShowWindow(SW_SHOW);
			if (bTaskbarOnRight)
			{
				m_dwDelayBetweenShowEvents=m_dwTimeToShow/(m_nSkinWidth/m_nIncrement);
				m_dwDelayBetweenHideEvents=m_dwTimeToHide/(m_nSkinWidth/m_nIncrement);
				m_nStartPosX=rcDesktop.right;
				m_nStartPosY=rcDesktop.bottom-m_nSkinHeight;
				m_nTaskbarPlacement=TASKBAR_ON_RIGHT;
			}
			else if (bTaskbarOnLeft)
			{
				m_dwDelayBetweenShowEvents=m_dwTimeToShow/(m_nSkinWidth/m_nIncrement);
				m_dwDelayBetweenHideEvents=m_dwTimeToHide/(m_nSkinWidth/m_nIncrement);
				m_nStartPosX=rcDesktop.left-m_nSkinWidth;
				m_nStartPosY=rcDesktop.bottom-m_nSkinHeight;
				m_nTaskbarPlacement=TASKBAR_ON_LEFT;
			}
			else if (bTaskBarOnTop)
			{
				m_dwDelayBetweenShowEvents=m_dwTimeToShow/(m_nSkinHeight/m_nIncrement);
				m_dwDelayBetweenHideEvents=m_dwTimeToHide/(m_nSkinHeight/m_nIncrement);
				m_nStartPosX=rcDesktop.right-m_nSkinWidth;
				m_nStartPosY=rcDesktop.top-m_nSkinHeight;
				m_nTaskbarPlacement=TASKBAR_ON_TOP;
			}
			else //if (bTaskbarOnBottom)
			{
				// Taskbar is on the bottom or Invisible
				m_dwDelayBetweenShowEvents=m_dwTimeToShow/(m_nSkinHeight/m_nIncrement);
				m_dwDelayBetweenHideEvents=m_dwTimeToHide/(m_nSkinHeight/m_nIncrement);
				m_nStartPosX=rcDesktop.right-m_nSkinWidth;
				m_nStartPosY=rcDesktop.bottom;
				m_nTaskbarPlacement=TASKBAR_ON_BOTTOM;
			}

			m_nCurrentPosX=m_nStartPosX;
			m_nCurrentPosY=m_nStartPosY;
	
			SetTimer(IDT_APPEARING,m_dwDelayBetweenShowEvents,NULL);
			break;

		case IDT_WAITING:
			RedrawWindow();
			KillTimer(IDT_WAITING);
			SetTimer(IDT_WAITING,m_dwTimeToLive,NULL);
			break;

		case IDT_APPEARING:
			RedrawWindow();
			break;

		case IDT_DISAPPEARING:
			KillTimer(IDT_DISAPPEARING);
			SetTimer(IDT_WAITING,m_dwTimeToLive,NULL);
			if (bTaskbarOnRight)
				m_nCurrentPosX=rcDesktop.right-m_nSkinWidth;
			else if (bTaskbarOnLeft)
				m_nCurrentPosX=rcDesktop.left;
			else if (bTaskBarOnTop)
				m_nCurrentPosY=rcDesktop.top;
			else //if (bTaskbarOnBottom)
				m_nCurrentPosY=rcDesktop.bottom-m_nSkinHeight;
			
			SetWindowPos(NULL,m_nCurrentPosX,m_nCurrentPosY,m_nSkinWidth,m_nSkinHeight,SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
			RedrawWindow();
			break;
	}
}

void CTaskbarNotifier::Hide()
{
	switch (m_nAnimStatus)
	{
		case IDT_APPEARING:
			KillTimer(IDT_APPEARING);
			break;
		case IDT_WAITING:
			KillTimer(IDT_WAITING);
			break;
		case IDT_DISAPPEARING:
			KillTimer(IDT_DISAPPEARING);
			break;
	}
	MoveWindow(0,0,0,0);
	ShowWindow(SW_HIDE);
	m_nAnimStatus=IDT_HIDDEN;
}

HRGN CTaskbarNotifier::GenerateRegion(HBITMAP hBitmap, unsigned char red, unsigned char green, unsigned char blue)
{
	WORD wBmpWidth,wBmpHeight;
	HRGN hRgn, hTmpRgn;

	// 24bit pixels from the bitmap
	unsigned char *pPixels = Get24BitPixels(hBitmap, &wBmpWidth, &wBmpHeight);
	if (!pPixels) return NULL;

	// create our working region
	hRgn = CreateRectRgn(0,0,wBmpWidth,wBmpHeight);
	if (!hRgn) { EP_DELETE pPixels; return NULL; }

	unsigned long p=0;
	for (WORD y=0; y<wBmpHeight; y++)
	{
		for (WORD x=0; x<wBmpWidth; x++)
		{
			unsigned char jRed   = pPixels[p+2];
			unsigned char jGreen = pPixels[p+1];
			unsigned char jBlue  = pPixels[p+0];

			if (jRed==red && jGreen==green && jBlue==blue)
			{
				// remove transparent color from region
				hTmpRgn = CreateRectRgn(x,y,x+1,y+1);
				CombineRgn(hRgn, hRgn, hTmpRgn, RGN_XOR);
				DeleteObject(hTmpRgn);
			}

			// next pixel
			p+=3;
		}
	}

	// release pixels
	EP_DELETE pPixels;

	// return the region
	return hRgn;
}

unsigned char* CTaskbarNotifier::Get24BitPixels(HBITMAP pBitmap, WORD *pwWidth, WORD *pwHeight)
{
	BITMAP bmpBmp;
	LPBITMAPINFO pbmiInfo;
	BITMAPINFO bmiInfo;
	WORD wBmpWidth, wBmpHeight;

	GetObject(pBitmap, sizeof(bmpBmp),&bmpBmp);
	pbmiInfo   = (LPBITMAPINFO)&bmpBmp;

	wBmpWidth  = (WORD)pbmiInfo->bmiHeader.biWidth;
	wBmpWidth -= (wBmpWidth%4);
	wBmpHeight = (WORD)pbmiInfo->bmiHeader.biHeight;

	*pwWidth  = wBmpWidth;
	*pwHeight = wBmpHeight;
	
	unsigned char *pPixels = EP_NEW unsigned char[wBmpWidth*wBmpHeight*3];
	if (!pPixels) return NULL;

	HDC hDC =::GetWindowDC(NULL);

	bmiInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiInfo.bmiHeader.biWidth = wBmpWidth;
	bmiInfo.bmiHeader.biHeight = -wBmpHeight;
	bmiInfo.bmiHeader.biPlanes = 1;
	bmiInfo.bmiHeader.biBitCount = 24;
	bmiInfo.bmiHeader.biCompression = BI_RGB;
	bmiInfo.bmiHeader.biSizeImage = wBmpWidth*wBmpHeight*3;
	bmiInfo.bmiHeader.biXPelsPerMeter = 0;
	bmiInfo.bmiHeader.biYPelsPerMeter = 0;
	bmiInfo.bmiHeader.biClrUsed = 0;
	bmiInfo.bmiHeader.biClrImportant = 0;

	// get pixels from the original bitmap converted to 24bits
	int iRes = GetDIBits(hDC,pBitmap,0,wBmpHeight,(LPVOID)pPixels,&bmiInfo,DIB_RGB_COLORS);

	// release the device context
	::ReleaseDC(NULL,hDC);

	// if failed, cancel the operation.
	if (!iRes)
	{
		EP_DELETE pPixels;
		return NULL;
	};

	// return the pixel array
	return pPixels;
}

BEGIN_MESSAGE_MAP(CTaskbarNotifier, CWnd)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTaskbarNotifier message handlers

int CTaskbarNotifier::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(32649));
	return 0;
}

void CTaskbarNotifier::OnDestroy()
{
	CWnd::OnDestroy();

}

void CTaskbarNotifier::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT t_MouseEvent;
	t_MouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
	t_MouseEvent.dwFlags     = TME_LEAVE | TME_HOVER;
	t_MouseEvent.hwndTrack   = m_hWnd;
	t_MouseEvent.dwHoverTime = 1;

	::_TrackMouseEvent(&t_MouseEvent);

	CWnd::OnMouseMove(nFlags, point);
}

void CTaskbarNotifier::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_pWndParent->PostMessage(WM_TASKBARNOTIFIERCLICKED,0,0);
	CWnd::OnLButtonUp(nFlags, point);
}

LRESULT CTaskbarNotifier::OnMouseHover(WPARAM w, LPARAM l)
{
	if (m_bMouseIsOver==FALSE)
	{
		m_bMouseIsOver=TRUE;
		RedrawWindow();
	}
	return 0;
}

LRESULT CTaskbarNotifier::OnMouseLeave(WPARAM w, LPARAM l)
{
	if (m_bMouseIsOver==TRUE)
	{
		m_bMouseIsOver=FALSE;
		RedrawWindow();
	}
	return 0;
}

BOOL CTaskbarNotifier::OnEraseBkgnd(CDC* pDC)
{
	CDC memDC;
	CBitmap *pOldBitmap;
	BITMAP bm;

	memDC.CreateCompatibleDC(pDC);
	GetObject(m_biSkinBackground.GetSafeHandle(), sizeof(bm), &bm);
	pOldBitmap=memDC.SelectObject(&m_biSkinBackground);

	pDC->BitBlt(0,0,bm.bmWidth,bm.bmHeight,&memDC,0,0,SRCCOPY);
	memDC.SelectObject(pOldBitmap);

	return TRUE;
}

void CTaskbarNotifier::OnPaint()
{
	CPaintDC dc(this);
	CRect rcClient;
	CFont *pOldFont;
	TCHAR *szBuffer;
		
	if (m_bMouseIsOver)
	{
		dc.SetTextColor(m_crSelectedTextColor);
		pOldFont=dc.SelectObject(&m_mySelectedFont);
	}
	else
	{
		dc.SetTextColor(m_crNormalTextColor);
		pOldFont=dc.SelectObject(&m_myNormalFont);
	}

	szBuffer=EP_NEW TCHAR[m_strCaption.GetLength()+10];
	_tcscpy(szBuffer,m_strCaption);

	dc.SetBkMode(TRANSPARENT); 
	rcClient.DeflateRect(10,20,10,20);
	dc.DrawText(szBuffer,-1,m_rcText,DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_END_ELLIPSIS);
	
	EP_DELETE[] szBuffer;
	dc.SelectObject(pOldFont);
}

BOOL CTaskbarNotifier::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CTaskbarNotifier::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
		case IDT_APPEARING:
			m_nAnimStatus=IDT_APPEARING;
			switch(m_nTaskbarPlacement)
			{
				case TASKBAR_ON_BOTTOM:
					if (m_nCurrentPosY>(m_nStartPosY-m_nSkinHeight))
						m_nCurrentPosY-=m_nIncrement;
					else
					{
						KillTimer(IDT_APPEARING);
						SetTimer(IDT_WAITING,m_dwTimeToLive,NULL);
						m_nAnimStatus=IDT_WAITING;
					}
					break;
				case TASKBAR_ON_TOP:
					if ((m_nCurrentPosY-m_nStartPosY)<m_nSkinHeight)
						m_nCurrentPosY+=m_nIncrement;
					else
					{
						KillTimer(IDT_APPEARING);
						SetTimer(IDT_WAITING,m_dwTimeToLive,NULL);
						m_nAnimStatus=IDT_WAITING;
					}
					break;
				case TASKBAR_ON_LEFT:
					if ((m_nCurrentPosX-m_nStartPosX)<m_nSkinWidth)
						m_nCurrentPosX+=m_nIncrement;
					else
					{
						KillTimer(IDT_APPEARING);
						SetTimer(IDT_WAITING,m_dwTimeToLive,NULL);
						m_nAnimStatus=IDT_WAITING;
					}
					break;
				case TASKBAR_ON_RIGHT:
					if (m_nCurrentPosX>(m_nStartPosX-m_nSkinWidth))
						m_nCurrentPosX-=m_nIncrement;
					else
					{
						KillTimer(IDT_APPEARING);
						SetTimer(IDT_WAITING,m_dwTimeToLive,NULL);
						m_nAnimStatus=IDT_WAITING;
					}
					break;
			}
			SetWindowPos(NULL,m_nCurrentPosX,m_nCurrentPosY,m_nSkinWidth,m_nSkinHeight,SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
			//RedrawWindow();
			break;

		case IDT_WAITING:
                  KillTimer(IDT_WAITING);
			SetTimer(IDT_DISAPPEARING,m_dwDelayBetweenHideEvents,NULL);
			break;

		case IDT_DISAPPEARING:
			m_nAnimStatus=IDT_DISAPPEARING;
			switch(m_nTaskbarPlacement)
			{
				case TASKBAR_ON_BOTTOM:
					if (m_nCurrentPosY<m_nStartPosY)
						m_nCurrentPosY+=m_nIncrement;
					else
					{
						KillTimer(IDT_DISAPPEARING);
						Hide();
					}
					break;
				case TASKBAR_ON_TOP:
					if (m_nCurrentPosY>m_nStartPosY)
						m_nCurrentPosY-=m_nIncrement;
					else
					{
						KillTimer(IDT_DISAPPEARING);
						Hide();
					}
					break;
				case TASKBAR_ON_LEFT:
					if (m_nCurrentPosX>m_nStartPosX)
						m_nCurrentPosX-=m_nIncrement;
					else
					{
						KillTimer(IDT_DISAPPEARING);
						Hide();
					}
					break;
				case TASKBAR_ON_RIGHT:
					if (m_nCurrentPosX<m_nStartPosX)
						m_nCurrentPosX+=m_nIncrement;
					else
					{
						KillTimer(IDT_DISAPPEARING);
						Hide();
					}
					break;
			}
			SetWindowPos(NULL,m_nCurrentPosX,m_nCurrentPosY,m_nSkinWidth,m_nSkinHeight,SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
			//RedrawWindow();
			break;
	}

	CWnd::OnTimer(nIDEvent);
}
