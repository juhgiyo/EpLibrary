/*! 
@file epTaskbarNotifier.h
@author John O'Byrne - July 05, 2002
	edited by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com> - October 01, 2011
			  <http://github.com/juhgiyo/eplibrary>
@date July 05, 2002
@brief TaskbarNotifier Interface

@section LICENSE

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

-------------------------------------------------------------------------------

@section USAGE

CTaskbarNotifier m_notifyWin;
...
m_notifyWin.Create(this);
m_notifyWin.SetSkin(IDB_BITMAP1);
m_notifyWin.SetTextFont(_T("Arial"),90,TN_TEXT_NORMAL,TN_TEXT_UNDERLINE);
m_notifyWin.SetTextColor(RGB(0,0,0),RGB(0,0,200));
m_notifyWin.SetTextRect(CRect(10,40,m_notifyWin.m_nSkinWidth-10,m_notifyWin.m_nSkinHeight-25));
...
m_notifyWin.Show(_T("Message."),TN_POP_UP_TIME_TO_SHOW,TN_POP_UP_TIME_TO_STAY,TN_POP_UP_TIME_TO_HIDE);

@section DESCRIPTION

An Interface for TaskbarNotifier Class.

*/
#ifndef __EP_TASKBAR_NOTIFIER_H__
#define __EP_TASKBAR_NOTIFIER_H__

#include "epLib.h"
#include "epSystem.h"
#include "epMemory.h"

#define WM_TASKBARNOTIFIERCLICKED	WM_USER+123
#define TN_TEXT_NORMAL			0x0000
#define TN_TEXT_BOLD			0x0001
#define TN_TEXT_ITALIC			0x0002
#define TN_TEXT_UNDERLINE		0x0004

#define TN_POP_UP_TIME_TO_SHOW 500
#define TN_POP_UP_TIME_TO_STAY 3000
#define TN_POP_UP_TIME_TO_HIDE 500

namespace epl
{
	/*! 
	@class CTaskbarNotifier epTaskbarNotifier.h
	@brief A class for TaskbarNotifier.
	*/
	class EP_LIBRARY CTaskbarNotifier : public CWnd
	{
		DECLARE_DYNAMIC(CTaskbarNotifier)

	public:
		CTaskbarNotifier();
		virtual ~CTaskbarNotifier();

		int Create(CWnd *pWndParent);
		void Show(const TCHAR * szCaption,unsigned long dwTimeToShow=TN_POP_UP_TIME_TO_SHOW,unsigned long dwTimeToStay=TN_POP_UP_TIME_TO_STAY,unsigned long dwTimeToHide=TN_POP_UP_TIME_TO_HIDE,int nIncrement=1);
		void Hide();
		BOOL SetSkin(UINT nBitmapID,short red=-1,short green=-1,short blue=-1);
		BOOL SetSkin(const TCHAR * szFileName,short red=-1,short green=-1,short blue=-1);
		void SetTextFont(const TCHAR * szFont,int nSize,int nNormalStyle,int nSelectedStyle);
		void SetTextColor(COLORREF crNormalTextColor,COLORREF crSelectedTextColor);
		void SetTextRect(RECT rcText);
		
		CWnd *m_pWndParent;
		
		CFont m_myNormalFont;
		CFont m_mySelectedFont;
		COLORREF m_crNormalTextColor;
		COLORREF m_crSelectedTextColor;
		HCURSOR m_hCursor;
		
		CBitmap m_biSkinBackground;
		HRGN m_hSkinRegion;
		CRect m_rcText;
		int m_nSkinWidth;
		int m_nSkinHeight;

		CString m_strCaption;
		BOOL m_bMouseIsOver;
		int m_nAnimStatus;

 		unsigned long m_dwTimeToShow;
		unsigned long m_dwTimeToLive;
		unsigned long m_dwTimeToHide;
		unsigned long m_dwDelayBetweenShowEvents;
		unsigned long m_dwDelayBetweenHideEvents;
		int m_nStartPosX;
		int m_nStartPosY;
		int m_nCurrentPosX;
		int m_nCurrentPosY;
		int m_nTaskbarPlacement;
		int m_nIncrement;
		
	protected:
		unsigned char* Get24BitPixels(HBITMAP pBitmap, WORD *pwWidth, WORD *pwHeight);
		HRGN GenerateRegion(HBITMAP hBitmap, unsigned char red, unsigned char green, unsigned char blue);

	protected:
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnDestroy();
		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg LRESULT OnMouseHover(WPARAM w, LPARAM l);
		afx_msg LRESULT OnMouseLeave(WPARAM w, LPARAM l);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnPaint();
		afx_msg void OnTimer(UINT nIDEvent);
	};

}
#endif //__EP_TASKBAR_NOTIFIER_H__
