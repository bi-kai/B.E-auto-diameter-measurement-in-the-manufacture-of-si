#if !defined(AFX_MASKDIALOG_H__4C24FF6B_9D3D_4BF3_BCBB_EF6F54AD8C57__INCLUDED_)
#define AFX_MASKDIALOG_H__4C24FF6B_9D3D_4BF3_BCBB_EF6F54AD8C57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaskDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMaskDialog dialog

class CMaskDialog : public CDialog
{
// Construction
public:
	CMaskDialog(CWnd* pParent = NULL);   // standard constructor

	BOOL OnInitDialog();

	CPoint m_startPoint;
	CPoint m_endPoint;


// Dialog Data
	//{{AFX_DATA(CMaskDialog)
	enum { IDD = IDD_MASK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaskDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMaskDialog)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_cap;
	CRect m_lastRect;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MASKDIALOG_H__4C24FF6B_9D3D_4BF3_BCBB_EF6F54AD8C57__INCLUDED_)
