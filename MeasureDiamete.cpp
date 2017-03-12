// MeasureDiamete.cpp : implementation file
//
#pragma once
#include "stdafx.h"
#include "VideoCaputure.h"
#include "MeasureDiamete.h"
#include "cv.h"
#include "highgui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeasureDiamete dialog


CMeasureDiamete::CMeasureDiamete(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasureDiamete::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeasureDiamete)
	//}}AFX_DATA_INIT
}


void CMeasureDiamete::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasureDiamete)
	DDX_Control(pDX, IDC_COMBO_STOPBITS, m_StopBits);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_Parity);
	DDX_Control(pDX, IDC_COMBO_DATABITS, m_DataBits);
	DDX_Control(pDX, IDC_COMBO_SPEED, m_Speed);
	DDX_Control(pDX, IDC_COMBO_COMSELECT, m_Com);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasureDiamete, CDialog)
	//{{AFX_MSG_MAP(CMeasureDiamete)
	ON_BN_CLICKED(IDC_Edgedect, OnEdgedect)
	ON_CBN_SELENDOK(IDC_COMBO_COMSELECT, OnSelendokComboComselect)
	ON_CBN_SELENDOK(IDC_COMBO_DATABITS, OnSelendokComboDatabits)
	ON_CBN_SELENDOK(IDC_COMBO_PARITY, OnSelendokComboParity)
	ON_CBN_SELENDOK(IDC_COMBO_SPEED, OnSelendokComboSpeed)
	ON_CBN_SELENDOK(IDC_COMBO_STOPBITS, OnSelendokComboStopbits)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasureDiamete message handlers

void CMeasureDiamete::OnEdgedect() 
{
	// TODO: Add your control notification handler code here
	
}

void CMeasureDiamete::OnSelendokComboComselect() 
{
	// TODO: Add your control notification handler code here
	CVideoCaputureApp *app = (CVideoCaputureApp *)AfxGetApp(); //生成指向应用程序类的指针
	app->m_nCom=m_Com.GetCurSel()+1;	
	UpdateData();
}

void CMeasureDiamete::OnSelendokComboDatabits() 
{
	// TODO: Add your control notification handler code here
	int i=m_DataBits.GetCurSel();
	switch(i)
	{
	case 0:
		i=8;
		break;
	case 1:
		i=7;
		break;
	case 2:
		i=6;
		break;
	}
	CVideoCaputureApp *app = (CVideoCaputureApp *)AfxGetApp(); //生成指向应用程序类的指针
	app->m_nDatabits=i;
	UpdateData();
}

void CMeasureDiamete::OnSelendokComboParity() 
{
	// TODO: Add your control notification handler code here
	char temp;
	int i=m_Parity.GetCurSel();
	switch(i)
	{
	case 0:
		temp='N';
		break;
	case 1:
		temp='O';
		break;
	case 2:
		temp='E';
		break;
	}
	CVideoCaputureApp *app = (CVideoCaputureApp *)AfxGetApp(); //生成指向应用程序类的指针
	app->m_cParity=temp;
	UpdateData();
}

void CMeasureDiamete::OnSelendokComboSpeed() 
{
	// TODO: Add your control notification handler code here
	int i=m_Speed.GetCurSel();
	switch(i)
	{
	case 0:
		i=300;
		break;
	case 1:
		i=600;
		break;
	case 2:
		i=1200;
		break;
	case 3:
		i=2400;
		break;
	case 4:
		i=4800;
		break;
	case 5:
		i=9600;
		break;
	case 6:
		i=19200;
		break;
	case 7:
		i=38400;
		break;
	case 8:
		i=43000;
		break;
	case 9:
		i=56000;
		break;
	case 10:
		i=57600;
		break;
	case 11:
		i=115200;
		break;
	default:
		break;
		
	}
	CVideoCaputureApp *app = (CVideoCaputureApp *)AfxGetApp(); //生成指向应用程序类的指针
	app->m_nBaud=i;
	UpdateData();
}

void CMeasureDiamete::OnSelendokComboStopbits() 
{
	// TODO: Add your control notification handler code here
	int i=m_StopBits.GetCurSel();
	switch(i)
	{
	case 0:
		i=1;
		break;
	case 1:
		i=2;
		break;
	}
	CVideoCaputureApp *app = (CVideoCaputureApp *)AfxGetApp(); //生成指向应用程序类的指针
	app->m_nStopbits=i;
	UpdateData();	
}


BOOL CMeasureDiamete::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Com.SetCurSel(0);
	m_Speed.SetCurSel(11);
	m_Parity.SetCurSel(0);
	m_DataBits.SetCurSel(0);
	m_StopBits.SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
