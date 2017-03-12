#if !defined(AFX_CHANGERADIUS_H__625510AB_89CE_4F1A_8B4B_9034267EA080__INCLUDED_)
#define AFX_CHANGERADIUS_H__625510AB_89CE_4F1A_8B4B_9034267EA080__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangeRadius.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChangeRadius dialog

class CChangeRadius : public CDialog
{
// Construction
public:
	CChangeRadius(CWnd* pParent = NULL);   // standard constructor
    double GetRadius();
// Dialog Data
	//{{AFX_DATA(CChangeRadius)
	enum { IDD = IDD_CHANGERADIUS };
	double	m_radius;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangeRadius)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangeRadius)
	afx_msg void OnSetfocusEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGERADIUS_H__625510AB_89CE_4F1A_8B4B_9034267EA080__INCLUDED_)
