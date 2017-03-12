#if !defined(AFX_SOFTKEYBOARD_H__6D5E58A7_6E06_4E4B_926D_9EE0FECFB1B6__INCLUDED_)
#define AFX_SOFTKEYBOARD_H__6D5E58A7_6E06_4E4B_926D_9EE0FECFB1B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoftKeyboard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSoftKeyboard dialog

class CSoftKeyboard : public CDialog
{
// Construction
public:
	CSoftKeyboard(CWnd* pParent = NULL);   // standard constructor
	bool onePointFlag;
	bool firstNumFlag;
	int counter;
	int pointPosition;
// Dialog Data
	//{{AFX_DATA(CSoftKeyboard)
	enum { IDD = IDD_SoftKeyboard };
	CString	m_editNUM;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoftKeyboard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSoftKeyboard)
	afx_msg void OnSoftkeyboardbtn1();
	afx_msg void OnSoftkeyboardbtn2();
	afx_msg void OnSoftkeyboardbtn3();
	afx_msg void OnSoftkeyboardbtn4();
	afx_msg void OnSoftkeyboardbtn5();
	afx_msg void OnSoftkeyboardbtn6();
	afx_msg void OnSoftkeyboardbtn7();
	afx_msg void OnSoftkeyboardbtn8();
	afx_msg void OnSoftkeyboardbtn9();
	afx_msg void OnSoftkeyboardbtn0();
	afx_msg void OnSoftkeyboardbtndian();
	afx_msg void OnSoftkeyboardbtnback();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOFTKEYBOARD_H__6D5E58A7_6E06_4E4B_926D_9EE0FECFB1B6__INCLUDED_)
