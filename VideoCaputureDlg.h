// VideoCaputureDlg.h : header file
//
#pragma once
#include "ChangeRadius.h"
#include "MeasureDiamete.h"
#include "cv.h"
#include "highgui.h"
#include "MaskDialog.h"
#include "SkinPPWTL.h"

//extern int m_nBaud;       //波特率
//extern int m_nCom;         //串口号
//extern char m_cParity;    //校验
//extern int m_nDatabits;    //数据位
//extern int m_nStopbits;    //停止位

//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_VIDEOCAPUTUREDLG_H__93A37754_2A6C_4985_85B3_10145E8891EB__INCLUDED_)
#define AFX_VIDEOCAPUTUREDLG_H__93A37754_2A6C_4985_85B3_10145E8891EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVideoCaputureDlg dialog

class CVideoCaputureDlg : public CDialog
{
// Construction
public:
	CVideoCaputureDlg(CWnd* pParent = NULL);	// standard constructor
	HICON m_hIconRed;    //串口打开时的红灯图标句柄
	HICON m_hIconOff;    //串口关闭时的指示图标句柄

// Dialog Data
	//{{AFX_DATA(CVideoCaputureDlg)
	enum { IDD = IDD_VIDEOCAPUTURE_DIALOG };
	CStatic	m_ctrlIconOpenoff;
	CStatic	m_video;
	double	m_Diamet;
	double  m_DiametOld;
	CMSComm	m_comm;
	double	m_edit1;
	double	m_edit2;
	double	m_edit3;
//	double	m_edit4;
//	double	m_edit5;
//	double	m_edit7;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoCaputureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVideoCaputureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelectzone();
	afx_msg void OnBtnok();
	afx_msg void OnChangeradius();
	afx_msg void OnAutoMeasure();
	afx_msg void OnReadAVI();
	afx_msg void OnClose();
	afx_msg void OnDengjing();
	afx_msg void OnYinjing();
	afx_msg void OnFangjian();
	afx_msg void OnAutoMeasureDown();
	afx_msg void OnAutoMeasureReset();
	afx_msg void OnConfigport();
	afx_msg void OnOpencloseport();
	afx_msg void OnAutoChangeMode();
	afx_msg void OnBTNquanDefult();
	afx_msg void OnThreshold();
	afx_msg void OnReadConfig();
	afx_msg void OnSaveConfig();
	afx_msg void OnSetfocusQuanzhi1();
	afx_msg void OnSetfocusEdit2();
	afx_msg void OnSetfocusEdit3();
	afx_msg void OnSetfocusEdit4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void ThresholdTest(IplImage* threshould);
	double m_DiametSecondOld;
	int yinfangdengflag;
	BOOL autoChangeModeflag;//只用于手动/自动模式切换
	bool ComPortReadflag;
	int AutoChangeModeFlag;
	void OnComm();
	int nFrmNumCounter;
	BOOL FirstFrmFlag;
	CMeasureDiamete ROIError;
	void ROISelected();
//	BOOL ROISelectedFlag;
	BOOL AutoORManualFlag;
	int AutoMaxRadius;
//	int ScanLineMoveCounter;
	int MeasurePoint;
	void    GetSubMat(IplImage* src,IplImage* dst, CPoint start_point,CPoint end_point);
	BITMAPINFO *m_pBmpInfo;
	char	m_chBmpBuf[2048];
	BYTE*	m_lpbuff;
	HANDLE	m_handle;
	long    temp1;
	long	m_height;
	int     temp;
	long	m_width ;
	bool    m_bcode;
	int     mflags;
	int     brightness;
	bool    is_select;
	bool    is_LeftBtnDown;
	bool    is_show;
	bool    is_Equaldia;
	bool    is_Guidedia;
	bool	is_fangjiandia;
	int YJAve_w1;//引晶的最低点
	BOOL Thresholdflag;//measure二值化测试标志位

	int     numofregion;
	void    showpic();
	void    ShowRegion();
	void    Measure();


private:
	int m_DCom;
	int m_DStopbits;
	char m_DParity;
	int m_DDatabits;
	LONG m_DBaud;
	CString string2;
	CString string1;

	double softkeyboard_NUM;
	
	CMeasureDiamete cmeasure;
	BOOL SerialPortOpenCloseFlag;
	BOOL Thresholdonoffflag;
	BOOL m_cap;
	CRect m_lastRect;

	CMaskDialog m_mask;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOCAPUTUREDLG_H__93A37754_2A6C_4985_85B3_10145E8891EB__INCLUDED_)
