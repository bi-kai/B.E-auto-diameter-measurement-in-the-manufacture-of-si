// SoftKeyboard.cpp : implementation file
//
#pragma once
#include "stdafx.h"
#include "VideoCaputure.h"
#include "SoftKeyboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoftKeyboard dialog


CSoftKeyboard::CSoftKeyboard(CWnd* pParent /*=NULL*/)
	: CDialog(CSoftKeyboard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoftKeyboard)
	m_editNUM = _T("");
	//}}AFX_DATA_INIT
	onePointFlag=false;//小数点只能用一次的标志位
	firstNumFlag=false;//第一个输入的数标志位，第一个不能是零

}


void CSoftKeyboard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoftKeyboard)
	DDX_Text(pDX, IDC_SOFTKEYBOARDEDITNUM, m_editNUM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSoftKeyboard, CDialog)
	//{{AFX_MSG_MAP(CSoftKeyboard)
	ON_BN_CLICKED(IDC_SOFTKEYBOARDBTN1, OnSoftkeyboardbtn1)
	ON_BN_CLICKED(IDC_SOFTKEYBOARDBTN2, OnSoftkeyboardbtn2)
	ON_BN_CLICKED(IDC_SOFTKEYBOARDBTN3, OnSoftkeyboardbtn3)
	ON_BN_CLICKED(IDC_SOFTKEYBOARDBTN4, OnSoftkeyboardbtn4)
	ON_BN_CLICKED(IDC_SOFTKEYBOARDBTN5, OnSoftkeyboardbtn5)
	ON_BN_CLICKED(IDC_SOFTKEYBOARDBTN6, OnSoftkeyboardbtn6)
	ON_BN_CLICKED(IDC_SOFTKEYBOARDBTN7, OnSoftkeyboardbtn7)
	ON_BN_CLICKED(IDC_SOFTKEYBOARDBTN8, OnSoftkeyboardbtn8)
	ON_BN_CLICKED(IDC_SOFTKEYBOARDBTN9, OnSoftkeyboardbtn9)
	ON_BN_CLICKED(IDC_SOFTKEYBOARDBTN0, OnSoftkeyboardbtn0)
	ON_BN_CLICKED(IDC_SOFTKEYBOARDBTNDIAN, OnSoftkeyboardbtndian)
	ON_BN_CLICKED(IDC_SOFTKEYBOARDBTNBACK, OnSoftkeyboardbtnback)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoftKeyboard message handlers

void CSoftKeyboard::OnSoftkeyboardbtn1() 
{
	// TODO: Add your control notification handler code here
	m_editNUM+="1";
	counter++;
	UpdateData(0);
	if (firstNumFlag==false)
	{
		firstNumFlag=true;
		GetDlgItem(IDC_SOFTKEYBOARDBTN0)->EnableWindow(1);
	}
}

void CSoftKeyboard::OnSoftkeyboardbtn2() 
{
	// TODO: Add your control notification handler code here
	m_editNUM+="2";
	counter++;
	UpdateData(0);
	if (firstNumFlag==false)
	{
		firstNumFlag=true;
		GetDlgItem(IDC_SOFTKEYBOARDBTN0)->EnableWindow(1);
	}
}

void CSoftKeyboard::OnSoftkeyboardbtn3() 
{
	// TODO: Add your control notification handler code here
	m_editNUM+="3";
	counter++;
	UpdateData(0);
	if (firstNumFlag==false)
	{
		firstNumFlag=true;
		GetDlgItem(IDC_SOFTKEYBOARDBTN0)->EnableWindow(1);
	}
}

void CSoftKeyboard::OnSoftkeyboardbtn4() 
{
	// TODO: Add your control notification handler code here
	m_editNUM+="4";
	counter++;
	UpdateData(0);
	if (firstNumFlag==false)
	{
		firstNumFlag=true;
		GetDlgItem(IDC_SOFTKEYBOARDBTN0)->EnableWindow(1);
	}
}

void CSoftKeyboard::OnSoftkeyboardbtn5() 
{
	// TODO: Add your control notification handler code here
	m_editNUM+="5";
	counter++;
	UpdateData(0);
	if (firstNumFlag==false)
	{
		firstNumFlag=true;
		GetDlgItem(IDC_SOFTKEYBOARDBTN0)->EnableWindow(1);
	}
}

void CSoftKeyboard::OnSoftkeyboardbtn6() 
{
	// TODO: Add your control notification handler code here
	m_editNUM+="6";
	counter++;
	UpdateData(0);
	if (firstNumFlag==false)
	{
		firstNumFlag=true;
		GetDlgItem(IDC_SOFTKEYBOARDBTN0)->EnableWindow(1);
	}
}

void CSoftKeyboard::OnSoftkeyboardbtn7() 
{
	// TODO: Add your control notification handler code here
	m_editNUM+="7";
	counter++;
	UpdateData(0);
	if (firstNumFlag==false)
	{
		firstNumFlag=true;
		GetDlgItem(IDC_SOFTKEYBOARDBTN0)->EnableWindow(1);
	}
}

void CSoftKeyboard::OnSoftkeyboardbtn8() 
{
	// TODO: Add your control notification handler code here
	m_editNUM+="8";
	counter++;
	UpdateData(0);
	if (firstNumFlag==false)
	{
		firstNumFlag=true;
		GetDlgItem(IDC_SOFTKEYBOARDBTN0)->EnableWindow(1);
	}
}

void CSoftKeyboard::OnSoftkeyboardbtn9() 
{
	// TODO: Add your control notification handler code here
	m_editNUM+="9";
	counter++;
	UpdateData(0);
	if (firstNumFlag==false)
	{
		firstNumFlag=true;
		GetDlgItem(IDC_SOFTKEYBOARDBTN0)->EnableWindow(1);
	}
}

void CSoftKeyboard::OnSoftkeyboardbtn0() 
{
	// TODO: Add your control notification handler code here
	m_editNUM+="0";
	counter++;
	UpdateData(0);

}

void CSoftKeyboard::OnSoftkeyboardbtndian() 
{
	// TODO: Add your control notification handler code here
	if (firstNumFlag==false)
	{
		firstNumFlag=true;
		m_editNUM+="0";
		counter++;
		GetDlgItem(IDC_SOFTKEYBOARDBTN0)->EnableWindow(1);
	}
	if(onePointFlag==false)
	{
		onePointFlag=true;
		m_editNUM+=".";
		counter++;	
		pointPosition=counter;
		GetDlgItem(IDC_SOFTKEYBOARDBTNDIAN)->EnableWindow(0);
		UpdateData(0);
	}
}

void CSoftKeyboard::OnSoftkeyboardbtnback() 
{
	// TODO: Add your control notification handler code here
	if(counter>=0)
	{ 
		if (onePointFlag==true)
		{
			if (counter==pointPosition)
			{
				onePointFlag=false;
				GetDlgItem(IDC_SOFTKEYBOARDBTNDIAN)->EnableWindow(1);
			}		
		}

		m_editNUM.Delete(counter,1);
			counter--;
		UpdateData(0);
	}
	if(counter<0)
	{
		GetDlgItem(IDC_SOFTKEYBOARDBTN0)->EnableWindow(0);
		firstNumFlag=false;
	}
}

BOOL CSoftKeyboard::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_SOFTKEYBOARDBTN0)->EnableWindow(0);
	counter=-1;
	pointPosition=0;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoftKeyboard::OnOK() 
{
	// TODO: Add extra validation here
	CVideoCaputureApp *app = (CVideoCaputureApp *)AfxGetApp(); //生成指向应用程序类的指针
	app->softkeyboard_value=atof(m_editNUM);
//	m_editNUM.Format("%lf",app->softkeyboard_value);
//	AfxMessageBox(m_editNUM,MB_OK,0);
	CDialog::OnOK();
}
