// VideoCaputure.h : main header file for the VIDEOCAPUTURE application
//

#if !defined(AFX_VIDEOCAPUTURE_H__E2E75DB3_07CB_4357_B8C1_9D4E1126E284__INCLUDED_)
#define AFX_VIDEOCAPUTURE_H__E2E75DB3_07CB_4357_B8C1_9D4E1126E284__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#define SKINSPACE _T("/SPATH:") ////  ע�⣺������������#include�����棡����

/////////////////////////////////////////////////////////////////////////////
// CVideoCaputureApp:
// See VideoCaputure.cpp for the implementation of this class
//

class CVideoCaputureApp : public CWinApp
{
public:
	CVideoCaputureApp();
//	int ExitInstance();
	int m_nBaud;       //������
	int m_nCom;         //���ں�
	char m_cParity;    //У��
	int m_nDatabits;    //����λ
	int m_nStopbits;    //ֹͣλ

	double softkeyboard_value;//�洢����������ֵ

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoCaputureApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVideoCaputureApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOCAPUTURE_H__E2E75DB3_07CB_4357_B8C1_9D4E1126E284__INCLUDED_)
