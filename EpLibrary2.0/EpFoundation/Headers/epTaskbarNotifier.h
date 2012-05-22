/*! 
@file epTaskbarNotifier.h
@author John O'Byrne
	edited by Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date October 01, 2011
@brief TaskbarNotifier Interface

@section LICENSE

Please refer to <http://www.codeproject.com/Articles/2562/Taskbar-Notification-dialog> for the license.

@section DESCRIPTION

An Interface for TaskbarNotifier Class.

*/

#ifndef __EP_TASKBAR_NOTIFIER_H__
#define __EP_TASKBAR_NOTIFIER_H__

#include "epFoundationLib.h"
#include "epSystem.h"
#include "epMemory.h"

#define WM_TASKBARNOTIFIERCLICKED	WM_USER+123
#define TN_TEXT_NORMAL			0x0000
#define TN_TEXT_BOLD			0x0001
#define TN_TEXT_ITALIC			0x0002
#define TN_TEXT_UNDERLINE		0x0004

namespace epl
{
	/*! 
	@class CTaskbarNotifier epTaskbarNotifier.h
	@brief A class for TaskbarNotifier.
	*/
	class EP_FOUNDATION CTaskbarNotifier : public CWnd
	{
		DECLARE_DYNAMIC(CTaskbarNotifier)

	public:
		CTaskbarNotifier();
		virtual ~CTaskbarNotifier();

		int Create(CWnd *pWndParent);
		void Show(const TCHAR * szCaption,unsigned long dwTimeToShow=500,unsigned long dwTimeToStay=3000,unsigned long dwTimeToHide=500,int nIncrement=1);
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
