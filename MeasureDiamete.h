#if !defined(AFX_MEASUREDIAMETE_H__40560532_61F0_44DA_B45F_4EDF71E3E58F__INCLUDED_)
#define AFX_MEASUREDIAMETE_H__40560532_61F0_44DA_B45F_4EDF71E3E58F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasureDiamete.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMeasureDiamete dialog

class CMeasureDiamete : public CDialog
{
// Construction
public:
	CMeasureDiamete(CWnd* pParent = NULL);   // standard constructor




// Dialog Data
	//{{AFX_DATA(CMeasureDiamete)
	enum { IDD = IDD_MeasureDiameter };
	CComboBox	m_StopBits;
	CComboBox	m_Parity;
	CComboBox	m_DataBits;
	CComboBox	m_Speed;
	CComboBox	m_Com;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasureDiamete)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMeasureDiamete)
	afx_msg void OnEdgedect();
	afx_msg void OnSelendokComboComselect();
	afx_msg void OnSelendokComboDatabits();
	afx_msg void OnSelendokComboParity();
	afx_msg void OnSelendokComboSpeed();
	afx_msg void OnSelendokComboStopbits();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASUREDIAMETE_H__40560532_61F0_44DA_B45F_4EDF71E3E58F__INCLUDED_)
