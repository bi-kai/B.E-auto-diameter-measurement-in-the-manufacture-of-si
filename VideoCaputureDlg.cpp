// VideoCaputureDlg.cpp : implementation file
//
#pragma once
#include "stdafx.h"
#include "VideoCaputure.h"
#include "VideoCaputureDlg.h"
#include "cv.h"
#include "highgui.h"
#include "IplShell.h"
#include "cxcore.h"
#include "SoftKeyboard.h"

#include "mvds32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


#define quanzhier 3
#define PI 3.1415926
double multiple = 0.0;     // 初始化放大倍数为0.0
float  theta = 27;         //镜头偏转角度
double Diameter;
double Real_Diameter;

int Framenum = 0;

double Avediameter = 0;

CPoint  StartPoint;
CPoint  EndPoint;          
int     width;
int     height;    
CvRect  rect;            // 需要二值化的区域

DWORD	m_dwStart=0;
DWORD	m_dwEnd=0;
DWORD	m_dwCount=0;
double	fps=0;

BOOL factorChangeFlag=TRUE;//修改权值时不允许updatedata；

double a,b,r,*z=NULL;
double x[1000] = {0};
double y[1000] = {0}; 
int xl[720];
int yl[580];
int xr[720];
int yr[580];
double xmid[500];
double R_Ave[50];
double Dia[125];
double Ave_realdiameter[50];
int    DLR[1000];
double diameter;
double real_diameter;

double m_edit1save_y=1;//引晶权值，保存
double m_edit2save_y=quanzhier;
double m_edit3save_y=0;
//double m_edit4save_y=1;
double m_edit1save_d=1;//等径权值，保存
double m_edit2save_d=quanzhier;
double m_edit3save_d=0;
//double m_edit4save_d=1;
double m_edit1save_f=1;//放肩权值，保存
double m_edit2save_f=1;
double m_edit3save_f=0;
//double m_edit4save_f=1;

double m_edit5save_y=180;
double m_edit7save_y=255;
double m_edit5save_f=120;
double m_edit7save_f=255;
double m_edit5save_d=230;
double m_edit7save_d=255;
BOOL useChangedyuzhiflag=FALSE;//使用改变后的阈值标志位，为FALSE默认是用设定好的阈值

BOOL softkeyboardFlag=FALSE;

CString strForFactor;//用于模式切换时，权值向编辑框的显示(废弃)


void WINAPI streamcallback(LPBYTE lpBuf,long len,PVOID m_pContext)
{
	CVideoCaputureDlg *pDlg=(CVideoCaputureDlg*)m_pContext;
	memcpy(pDlg->m_lpbuff,lpBuf,len);
	
    if( pDlg->is_show == true && pDlg->numofregion % 2 == 0)
	{
		pDlg->ShowRegion();
	}
	if(pDlg->m_bcode) MV_AVIWrite(pDlg->m_handle,lpBuf);

	if ((pDlg->is_Equaldia)||(pDlg->is_Guidedia))
	{
		pDlg->Measure();
	}

	pDlg->showpic();

	if(m_dwEnd-m_dwStart>1000)
	{
		fps = 1000.0 * m_dwCount / (m_dwEnd-m_dwStart);
		m_dwCount=0;
		m_dwEnd=m_dwStart=::GetTickCount();
	}
	else
	{
		m_dwEnd=::GetTickCount();
		m_dwCount++;
	}
}



double Average(double A[],int n)
{
	int i;
	double tmp=0;
	for (i=0;i<n;i++)tmp+=A[i];
	tmp=tmp/(double)n;
	return tmp;
}

char HexChar(char c)
{
	if((c>='0')&&(c<='9'))
		return c-0x30;
	else if((c>='A')&&(c<='F'))
		return c-'A'+10;
	else if((c>='a')&&(c<='f'))
		return c-'a'+10;
	else 
		return c;
}

int strHex(CString str, char * data)
{
	int t,t1;
	int rlen=0,len=str.GetLength();
	
	for(int i=0;i<len;)
	{
		unsigned char l,h=str.GetAt(i);
		if ((0<h)&&(h<128))                                //判断此字符是否为汉字
		{
			i++;
			if(i>=len)
				break;
			l=str[i];
			t=HexChar(h);
			t1=HexChar(l);
			t=t*16+t1;
			i++;
			data[rlen]=(char)t;
			rlen++;
		}
		else
		{
			data[rlen]=h;                   //
			rlen++;
			i++;
		}
	}
	return rlen;
}




double R_Average(double A[],int n)
{
	int i = 0;
	double tmp=0;
	double temp = 0;
	
	for(i = 0;i < n;i++){
		for(int j = i + 1;j < n;j++){
			if(A[i] > A[j]){
				temp = A[j];
				A[j] = A[i];
				A[i]  = temp;
			}
		}
	}
//    tmp=0;
	for (i=15;i<n-15;i++)tmp+=A[i];
	tmp=tmp/(double)(n-30);
	return tmp;
}

void CircleSim1(double x[],double y[],int n)
{
    int i=0;
    double X1,Y1,X2,Y2,Z1,X3,Y3,Z2,Z3;
    double p;
   	double *tp=NULL;
	tp= (double*)malloc((unsigned)(n*sizeof(double)));
    X1=Average(x,n);
	Y1=Average(y,n);
	
	for(i=0;i<n;i++)tp[i]=y[i]*y[i];	
	Y2=Average(tp,n);
	
    for(i=0;i<n;i++)tp[i]=x[i]*x[i];
	X2=Average(tp,n);
	
    for(i=0;i<n;i++)tp[i]=x[i]*y[i];
	Z1=Average(tp,n);
	
	for(i=0;i<n;i++)tp[i]=x[i]*x[i]*y[i];
	Z2=Average(tp,n);
    
    for(i=0;i<n;i++)tp[i]=x[i]*x[i]*x[i];
	X3=Average(tp,n);
    
	for(i=0;i<n;i++)tp[i]=y[i]*y[i]*y[i];
	Y3=Average(tp,n);
	
	for(i=0;i<n;i++)tp[i]=y[i]*y[i]*x[i];
	Z3=Average(tp,n);
	
	p=(double)2*(X1*X1-X2)*(Y1*Y1-Y2)-(double)2*(X1*Y1-Z1)*(X1*Y1-Z1);
	
	a=((X2*X1+X1*Y2-X3-Z3)*(Y1*Y1-Y2)-(X2*Y1+Y1*Y2-Z2-Y3)*(X1*Y1-Z1))/p;
	
	b=((X2*Y1+Y1*Y2-Z2-Y3)*(X1*X1-X2)-(X2*X1+X1*Y2-X3-Z3)*(X1*Y1-Z1))/p;	
    r=sqrt(a*a-(double)2*X1*a+b*b-(double)2*Y1*b+X2+Y2);
    
	free(tp);
	}

double * MatrixProduct(double A[],double B[],int l,int m,int n)
{
	int i,j,k;
	double *C=NULL;
	C=(double *)malloc(m*m*sizeof(double));
	for(i=0;i<l;i++)
        for(j=0;j<n;j++)C[i*n+j]=0;
		for(i=0;i<l;i++)
			for(j=0;j<n;j++)
				for(k=0;k<m;k++) C[i*n+j]+=A[i*m+k]*B[k*n+j];  
				return C;
}

double Surplus(double A[],int m,int n) //求矩阵行列式
{
	int i,j,k,p,r;
	double X,temp=1.0,temp1=1.0,s=0,s1=0;
	
	if(n==2)
	{
		for(i=0;i<m;i++)
			for(j=0;j<n;j++)
				if((i+j)%2) temp1*=A[i*n+j];
				else temp*=A[i*n+j];
				X=temp-temp1;
	}
	else
	{
		for(k=0;k<n;k++)
		{
			for(i=0,j=k;i<m,j<n;i++,j++)temp*=A[i*n+j];
			if(m>i)
			{
				for(p=m-i,r=m-1;p>0;p--,r--)temp*=A[r*n+p-1];
			}
			s+=temp;
			temp=(double)1;
		}
		for(k=n-1;k>=0;k--)
		{
			for(i=0,j=k;i<m,j>=0;i++,j--)temp1*=A[i*n+j];
            if(m>i)
			{
				for(p=m-1,r=i;r<m;p--,r++)temp1*=A[r*n+p];
			}
			s1+=temp1;
			temp1=(double)1;
		}
		X=s-s1;
	}
	return X;
}

double * MatrixInver(double A[],int m,int n) //矩阵转置
{
	int i,j;
	double *B=NULL;
	B=(double *)malloc(m*n*sizeof(double));
	
	for(i=0;i<n;i++)
		for(j=0;j<m;j++)
			B[i*m+j]=A[j*n+i];
		
		return B;
		
}

double * MatrixOpp(double A[],int m,int n) //矩阵求逆
{
	int i,j,x,y,k;
	double *SP=NULL,*AB=NULL,*B=NULL,X,*C;
	SP=(double *)malloc(m*n*sizeof(double));
	AB=(double *)malloc(m*n*sizeof(double));
	B=(double *)malloc(m*n*sizeof(double));
	
	X=Surplus(A,m,n);
	//printf("|F|=%e  \n ",X);
	X=(double)1/X;
	
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
		{
			for(k=0;k<m*n;k++)B[k]=A[k];
			{
				for(x=0;x<n;x++)B[i*n+x]=0;
				for(y=0;y<m;y++)B[m*y+j]=0;
				B[i*n+j]=1;
				SP[i*n+j]=Surplus(B,m,n);
				AB[i*n+j]=X*SP[i*n+j];
			}
		}
		C=MatrixInver(AB,m,n);
		
		return C;
 }


void CircleParaInit(double X[],double Y[], int N) //求圆参数初值
{
	int i;
	double tmp=0;
	for(i=0;i<N;i++)tmp+=X[i];
	
	a=tmp/(double)N;
	
	tmp=0;
	for(i=0;i<N;i++)tmp+=Y[i];
	b=tmp/(double)N;
	tmp=0;
	for(i=0;i<N;i++)tmp+=sqrt(pow(X[i]-a,2.0)+pow(Y[i]-b,2.0));
	r=tmp/(double)N;
	z= (double*)malloc((unsigned)(N*sizeof(double)));
	for(i=0;i<N;i++)z[i]=X[i]*X[i]+Y[i]*Y[i];
	
}


double * CircleSim(double X[],double Y[],double Z[],double a,double b,double r,int N)
{
	int j,m=3;
	double *C,*D,*E,*F,*P,*T,*H=NULL;
	T= (double*)malloc((unsigned)(N*sizeof(double)));
	// Z(j)=X[j]*X[j]+Y[j]*Y[j];  f(Xj,Yj;a,b,r)=2ax+2by+r*r-a*a-b*b; T[j]=Z(j)-f(Xj,Yj;a,b,r);
	
	for(j=0;j<N;j++)
		T[j]=Z[j]-((double)2*a*X[j]+(double)2*b*Y[j]+r*r-a*a-b*b);
	
	if( (C = ( double*)malloc((unsigned)(m*N*sizeof(  double)))) == NULL )exit( 1 );
	if( (D = ( double*)malloc((unsigned)(m*N*sizeof(  double)))) == NULL )exit( 1 );
	
	if( (E = ( double*)malloc((unsigned)(m*m*sizeof(  double)))) == NULL )exit( 1 );
	if( (F = ( double*)malloc((unsigned)(m*m*sizeof(  double)))) == NULL )exit( 1 );
	if( (P = ( double*)malloc((unsigned)(m*sizeof(  double)))) == NULL )exit( 1 );
	if( (H = ( double*)malloc((unsigned)(m*N*sizeof(  double)))) == NULL )exit( 1 );
	
	//C是偏导矩阵
	for(j=0;j<N;j++)C[j*m]=(double)2*(X[j]-a);
	for(j=0;j<N;j++)C[j*m+1]=(double)2*(Y[j]-b);
	for(j=0;j<N;j++)C[j*m+2]=(double)2*r;
	//D是C的转置
	
	D=MatrixInver(C,N,m);//D是C的转置
	E=MatrixProduct(D,C,m,N,m);// E=DC
	F=MatrixOpp(E,m,m);         // F是E的逆
	H=MatrixProduct(D,T,m,N,1); // H=DT
	
	P=MatrixProduct(F,H,m,m,1);
	//for(j=0;j<m;j++)printf("P*=%E; ",P[j]);
	free(C);
	free(D);
	free(E);
	free(F);
	free(H);
	return P;
}



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoCaputureDlg dialog

CVideoCaputureDlg::CVideoCaputureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoCaputureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVideoCaputureDlg)
	m_Diamet = 0.0;
	m_edit1 = 1;
	if((is_Guidedia==TRUE)||(is_Equaldia==true))
	m_edit2 = quanzhier;
	else m_edit2 =1;
	m_edit3 = 0;
//	m_edit4 = 1;
//	m_edit5 = 0.0;
//	m_edit7 = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
//	m_width =720;//改动前的原版值
//	m_height=576;//
	m_width =720;//662;
	m_height=576;//530;

	rect.x = 0;
	rect.y = 0;
	rect.width = 720;
	rect.height = 576;
//	ScanLineMoveCounter=12;
	AutoMaxRadius= 0;
	MeasurePoint=0;
	AutoORManualFlag=FALSE;
//	ROISelectedFlag=FALSE;
	SerialPortOpenCloseFlag=FALSE;
	Thresholdonoffflag=FALSE;
}

void CVideoCaputureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideoCaputureDlg)
	DDX_Control(pDX, IDC_STATIC_OPENOFF, m_ctrlIconOpenoff);
	DDX_Control(pDX, IDC_VIDEO, m_video);
	DDX_Control(pDX, IDC_MSCOMM1, m_comm);
	DDX_Text(pDX, IDC_EDIT1, m_edit1);
	DDV_MinMaxDouble(pDX, m_edit1, 0., 255.);
	DDX_Text(pDX, IDC_EDIT2, m_edit2);
	DDV_MinMaxDouble(pDX, m_edit2, 0., 255.);
	DDX_Text(pDX, IDC_EDIT3, m_edit3);
	DDV_MinMaxDouble(pDX, m_edit3, 0., 255.);
//	DDX_Text(pDX, IDC_EDIT4, m_edit4);
//	DDV_MinMaxDouble(pDX, m_edit4, 0., 255.);
//	DDX_Text(pDX, IDC_EDIT5, m_edit5);
//	DDV_MinMaxDouble(pDX, m_edit5, 0., 255.);
//	DDX_Text(pDX, IDC_EDIT7, m_edit7);
//	DDV_MinMaxDouble(pDX, m_edit7, 0., 255.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVideoCaputureDlg, CDialog)
	//{{AFX_MSG_MAP(CVideoCaputureDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SELECTZONE, OnSelectzone)
	ON_BN_CLICKED(IDC_BTNOK, OnBtnok)
	ON_BN_CLICKED(IDC_CHANGERADIUS, OnChangeradius)
	ON_BN_CLICKED(IDC_AutoMeasure, OnAutoMeasure)
	ON_BN_CLICKED(IDC_ReadAVI, OnReadAVI)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_Dengjing, OnDengjing)
	ON_BN_CLICKED(IDC_Yinjing, OnYinjing)
	ON_BN_CLICKED(IDC_Fangjian, OnFangjian)
	ON_BN_CLICKED(IDC_AutoMeasureDown, OnAutoMeasureDown)
	ON_BN_CLICKED(IDC_AutoMeasureReset, OnAutoMeasureReset)
	ON_BN_CLICKED(IDC_CONFIGPORT, OnConfigport)
	ON_BN_CLICKED(IDC_OPENCLOSEPORT, OnOpencloseport)
	ON_BN_CLICKED(IDC_AutoChangeMode, OnAutoChangeMode)
	ON_BN_CLICKED(IDC_BTNquanDefult, OnBTNquanDefult)
	ON_BN_CLICKED(IDC_Threshold, OnThreshold)
	ON_BN_CLICKED(IDC_Threshold3, OnReadConfig)
	ON_BN_CLICKED(IDC_Threshold2, OnSaveConfig)
	ON_EN_SETFOCUS(IDC_EDIT1, OnSetfocusQuanzhi1)
	ON_EN_SETFOCUS(IDC_EDIT2, OnSetfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, OnSetfocusEdit3)
//	ON_EN_SETFOCUS(IDC_EDIT4, OnSetfocusEdit4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoCaputureDlg message handlers

BOOL CVideoCaputureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	m_handle= MV_Opencard(0);
	m_bcode	= false;

	m_pBmpInfo	= (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth			= 720;
	m_pBmpInfo->bmiHeader.biHeight			= 576;
	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 24;
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;

	for (int i = 0; i < 256; i++){
		m_pBmpInfo->bmiColors[i].rgbBlue		= (BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbGreen		= (BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbRed			= (BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbReserved	= 0;	
	}

	m_lpbuff	=new BYTE[720*576*3];

	mflags = 0;         // 初始化为0
	brightness = 128;  // 初始化亮度值为128
	is_select = false; // 初始化为false
	is_LeftBtnDown = false;  // 初始化为false
	is_show = false;   // 初始化为false;
	is_Equaldia = FALSE;
	is_Guidedia = FALSE;
	is_fangjiandia=FALSE;
	FirstFrmFlag = FALSE;
	Thresholdflag=FALSE;
	nFrmNumCounter=0;
	real_diameter=0.0;//平均直径，用于计算比例因子

	m_hIconRed  = AfxGetApp()->LoadIcon(IDI_ICON_RED);
	m_hIconOff	= AfxGetApp()->LoadIcon(IDI_ICON_OFF);
	
	numofregion = 0;
	GetDlgItem(IDC_BTNOK)->EnableWindow(0);
	GetDlgItem(IDC_ReadAVI)->EnableWindow(0);
//	GetDlgItem(IDC_STOP)->EnableWindow(0);
//	GetDlgItem(IDC_SAVEVIDEO)->EnableWindow(0);
//	GetDlgItem(IDC_SAVEIMAGES)->EnableWindow(0);
	GetDlgItem(IDC_Yinjing)->EnableWindow(0);
	GetDlgItem(IDC_Fangjian)->EnableWindow(0);
	GetDlgItem(IDC_Dengjing)->EnableWindow(0);
//	GetDlgItem(IDC_ADDBRIGHT)->EnableWindow(0);
//	GetDlgItem(IDC_REDUCEBRIGHT)->EnableWindow(0);
//	GetDlgItem(IDC_BRIGHTDEFAULT)->EnableWindow(0);
	GetDlgItem(IDC_SELECTZONE)->EnableWindow(0);
	GetDlgItem(IDC_CHANGERADIUS)->EnableWindow(0);
	GetDlgItem(IDC_AutoMeasure)->EnableWindow(0);
	GetDlgItem(IDC_AutoMeasureDown)->EnableWindow(0);
	GetDlgItem(IDC_AutoMeasureReset)->EnableWindow(0);
	GetDlgItem(IDC_OPENCLOSEPORT)->EnableWindow(0);
	GetDlgItem(IDC_CONFIGPORT)->EnableWindow(0);
	GetDlgItem(IDC_AutoChangeMode)->EnableWindow(0);

	GetDlgItem(IDC_Threshold)->EnableWindow(0);
	GetDlgItem(IDC_Threshold2)->EnableWindow(0);
	GetDlgItem(IDC_Threshold3)->EnableWindow(0);
//	GetDlgItem(IDC_BUTTON2)->EnableWindow(0);
//	GetDlgItem(IDC_BUTTON3)->EnableWindow(0);
//	GetDlgItem(IDC_BUTTON4)->EnableWindow(0);
	GetDlgItem(IDC_BTNquanDefult)->EnableWindow(0);

	GetDlgItem(IDC_EDIT1)->EnableWindow(0);//关闭权值输入框
	GetDlgItem(IDC_EDIT2)->EnableWindow(0);
	GetDlgItem(IDC_EDIT3)->EnableWindow(0);
//	GetDlgItem(IDC_EDIT4)->EnableWindow(0);

//	GetDlgItem(IDC_EDIT5)->EnableWindow(0);//关闭阈值输入框
//	GetDlgItem(IDC_EDIT7)->EnableWindow(0);

	string1="";
	string2="";

	::DeleteFile("deng.txt");
	::DeleteFile("yin.txt");
	::DeleteFile("fang.txt");
	::DeleteFile("Diamet.txt");

	m_DCom=1;
	m_DStopbits=1;
	m_DParity='N';
	m_DDatabits=8;
	 m_DBaud=115200;
	 m_DiametSecondOld=0.0;

	 softkeyboard_NUM=0;

	 m_DiametOld=0.0;
	 YJAve_w1=20;
	 AutoChangeModeFlag=0;//自动改变测量模式的标志
	 ComPortReadflag=0;//串口是否读取数据的标志位
	 autoChangeModeflag=FALSE;//自动选择测量模式，手动选择测量模式标志位
	 yinfangdengflag=0;


//    if(m_comm.GetPortOpen())
//     m_comm.SetPortOpen(FALSE);

	 m_comm.SetCommPort(1); //选择com1
	 m_comm.SetInputMode(1); //输入方式为二进制方式
	 m_comm.SetInBufferSize(1024); //设置输入缓冲区大小
     m_comm.SetOutBufferSize(512); //设置输出缓冲区大小
	 m_comm.SetSettings("115200,n,8,1"); //波特率115200，无校验，8个数据位，1个停止位	 
	 m_comm.SetRThreshold(1); //参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件
	 m_comm.SetInputLen(0); //设置当前接收区数据长度为0
//	 m_comm.GetInput();    //先预读缓冲区以清除残留数据
	 if(!m_comm.GetPortOpen())
	 {		 
//		m_comm.SetPortOpen(TRUE);//打开串口(此处不必打开，后边用“打开串口”按钮实现)
	 }
	 else
		 MessageBox("can not open serial port");
 //    m_comm.GetInput();//先预读缓冲区以清除残留数据

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVideoCaputureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoCaputureDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVideoCaputureDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVideoCaputureDlg::OnStart() 
{
	// TODO: Add your control notification handler code here
	// 初始化指定设备，申请资源
	if(!m_handle)
	{
		MessageBox("非法的采集设备!");
//		return;
	}
	GetDlgItem(IDC_ReadAVI)->EnableWindow(1);
	GetDlgItem(IDC_START)->EnableWindow(0);
//	GetDlgItem(IDC_STOP)->EnableWindow(1);
//	GetDlgItem(IDC_SAVEVIDEO)->EnableWindow(1);
//	GetDlgItem(IDC_SAVEIMAGES)->EnableWindow(1);
	GetDlgItem(IDC_Yinjing)->EnableWindow(1);
	GetDlgItem(IDC_Fangjian)->EnableWindow(1);
	GetDlgItem(IDC_Dengjing)->EnableWindow(1);
//	GetDlgItem(IDC_ADDBRIGHT)->EnableWindow(1);
//	GetDlgItem(IDC_REDUCEBRIGHT)->EnableWindow(1);
//	GetDlgItem(IDC_BRIGHTDEFAULT)->EnableWindow(1);
	GetDlgItem(IDC_SELECTZONE)->EnableWindow(1);
	GetDlgItem(IDC_CHANGERADIUS)->EnableWindow(0);
	GetDlgItem(IDC_AutoMeasure)->EnableWindow(1);
	GetDlgItem(IDC_AutoMeasureDown)->EnableWindow(1);
	GetDlgItem(IDC_AutoMeasureReset)->EnableWindow(1);
	GetDlgItem(IDC_OPENCLOSEPORT)->EnableWindow(1);
	GetDlgItem(IDC_CONFIGPORT)->EnableWindow(1);
	GetDlgItem(IDC_AutoChangeMode)->EnableWindow(1);
	GetDlgItem(IDC_Threshold)->EnableWindow(1);
	GetDlgItem(IDC_Threshold2)->EnableWindow(1);
	GetDlgItem(IDC_Threshold3)->EnableWindow(1);
//	GetDlgItem(IDC_BUTTON2)->EnableWindow(1);
//	GetDlgItem(IDC_BUTTON3)->EnableWindow(1);
//	GetDlgItem(IDC_BUTTON4)->EnableWindow(0);
	GetDlgItem(IDC_BTNquanDefult)->EnableWindow(1);

	MV_SetOutSize(m_handle,0);
	MV_SetCallBack(m_handle,streamcallback,this);
	MV_StartCap(m_handle,0);
	m_dwEnd=m_dwStart = GetTickCount();
//	SetTimer(0,1000,0);
}

void CVideoCaputureDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	if(nIDEvent==1)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);

			if(factorChangeFlag==TRUE)
			UpdateData();
/*			m_Diamet = diameter*multiple;
			m_Diamet=((m_Diamet/10)+(m_DiametOld*3/10))+(m_DiametSecondOld*6/10);//求个均值，减小测量误差
			m_DiametSecondOld=m_DiametOld;
			m_DiametOld=m_Diamet;
*/			

//			str.Format(_T("%.2lf"),m_Diamet);
//			SetDlgItemText(IDC_EDIT6,str);
//	  	    UpdateData(0);

			long senddata=(int)(m_Diamet*100);

			//senddata = 1000;

			CString data,buf;
				
			
			char hexdata[6];
			data.Format("%4x",senddata);
			
			int len=strHex(data,hexdata); 
//			buf.Format("%d",len);	
//			AfxMessageBox(buf,MB_OK,0);//这里data已经是十六进制的字符串了

			CByteArray Array;
			Array.RemoveAll();
            Array.SetSize(len);
			for (int i=0; i<len; i++)
			{
				Array.SetAt(i,hexdata[i]);
			}

			
			if((m_comm.GetPortOpen())&&(SerialPortOpenCloseFlag==TRUE))
			{
				//m_comm.SetOutput(COleVariant(data));//发送数据
				m_comm.SetOutput(COleVariant(Array));//发送数据
			}
		
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CVideoCaputureDlg::showpic()
{
	CDC* pDC =m_video.GetDC();
	SetStretchBltMode(pDC->m_hDC,3);
	StretchDIBits(pDC->m_hDC,0,0,588,470,0,0,m_width,m_height,m_lpbuff,m_pBmpInfo,DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(pDC);
}

void CVideoCaputureDlg::ShowRegion( )
{
	CDC* pDC =m_video.GetDC();
	CPen pen;
	pen.CreatePen(PS_SOLID,4,RGB(0,0,255));
	::SelectObject(pDC->GetSafeHdc(),pen);
	pDC->MoveTo(StartPoint);
	CPoint temp;
	temp.x = StartPoint.x;
	temp.y = EndPoint.y;
	pDC->LineTo(temp);
	pDC->LineTo(EndPoint);
	temp.x = EndPoint.x;
	temp.y = StartPoint.y;
	pDC->LineTo(temp);
	pDC->LineTo(StartPoint);
	ReleaseDC(pDC);
}

//DEL void CVideoCaputureDlg::OnStop() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	GetDlgItem(IDC_START)->EnableWindow(1);
//DEL 	MV_StopCap(m_handle);
//DEL }

//DEL void CVideoCaputureDlg::OnSavevideo() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	CButton* m_button = (CButton*)GetDlgItem(IDC_SAVEVIDEO);
//DEL 	if(mflags == 0)
//DEL 	{
//DEL 		CString szFile;
//DEL 		CFileDialog dlg(false,NULL,NULL,OFN_OVERWRITEPROMPT,"*.*");
//DEL 		dlg.m_ofn.lpstrTitle="选择存盘路径";
//DEL 		if(dlg.DoModal()==IDOK)
//DEL 			szFile=dlg.GetPathName();
//DEL 		else
//DEL 			return;
//DEL 		int n=szFile.ReverseFind('.');
//DEL 		if(n==-1) szFile+=".avi";
//DEL 		MV_AVICreate(m_handle,szFile.GetBuffer(0),m_width,m_height,m_pBmpInfo->bmiHeader.biBitCount,25.0);
//DEL 		m_bcode	=true;
//DEL 		m_button->SetWindowText("停止录像");
//DEL 		mflags = 1;
//DEL 	}
//DEL 	else
//DEL 	{
//DEL         m_button->SetWindowText("开始录像");
//DEL 		m_bcode	=false;
//DEL 		Sleep(500);
//DEL 		mflags = 0;
//DEL 		MV_AVIClose(m_handle);
//DEL 
//DEL 	}
//DEL 	
//DEL }

//DEL void CVideoCaputureDlg::OnSaveimages() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	bool bcolor =false;
//DEL 	if(m_pBmpInfo->bmiHeader.biBitCount==24)
//DEL 		bcolor =true;
//DEL 	UpdateData();
//DEL 	
//DEL 	CString szFile,szFileB;
//DEL 	CFileDialog dlg(false,NULL,NULL,OFN_OVERWRITEPROMPT,"*.*");
//DEL 	dlg.m_ofn.lpstrTitle="选择存盘路径";
//DEL 	if(dlg.DoModal()==IDOK)
//DEL 		szFile=dlg.GetPathName();
//DEL 	else
//DEL 		return;
//DEL 	int n=szFile.ReverseFind('.');
//DEL 	if(n==-1)
//DEL 	{
//DEL 		szFile+=".jpg";
//DEL 	}
//DEL 	MV_SaveJPEGFile(szFile.GetBuffer(0),m_lpbuff,m_width,m_height,bcolor,75);
//DEL }

//DEL void CVideoCaputureDlg::OnAddbright() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	if( brightness <= 250 )
//DEL 	{
//DEL 		brightness = brightness + 5;
//DEL 		MV_SetPropery(m_handle,_BRIGHTNESS,brightness);
//DEL 	}
//DEL }

//DEL void CVideoCaputureDlg::OnReducebright() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	if( brightness >= 5 )
//DEL 	{
//DEL 		brightness = brightness - 5;
//DEL 		MV_SetPropery(m_handle,_BRIGHTNESS,brightness);
//DEL 	}
//DEL }

//DEL void CVideoCaputureDlg::OnBrightdefault() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	brightness = 128;
//DEL 	MV_SetPropery(m_handle,_BRIGHTNESS,brightness);
//DEL }

void CVideoCaputureDlg::OnSelectzone() 
{
	// TODO: Add your control notification handler code here
	is_select = true;
	factorChangeFlag=FALSE;
	//KillTimer(1);
//	GetDlgItem(IDC_CHANGERADIUS)->EnableWindow(1);
//  GetDlgItem(IDC_MeasureDiamete)->EnableWindow(1);
//	GetDlgItem(IDC_AutoMeasure)->EnableWindow(1);
	GetDlgItem(IDC_SELECTZONE)->EnableWindow(0);
	GetDlgItem(IDC_BTNOK)->EnableWindow(1);

//	GetDlgItem(IDC_STOP)->EnableWindow(0);
//	GetDlgItem(IDC_SAVEVIDEO)->EnableWindow(0);
//	GetDlgItem(IDC_SAVEIMAGES)->EnableWindow(0);
//	GetDlgItem(IDC_Yinjing)->EnableWindow(0);
//	GetDlgItem(IDC_Fangjian)->EnableWindow(0);
//	GetDlgItem(IDC_Dengjing)->EnableWindow(0);
//	GetDlgItem(IDC_ADDBRIGHT)->EnableWindow(0);
//	GetDlgItem(IDC_REDUCEBRIGHT)->EnableWindow(0);
//	GetDlgItem(IDC_BRIGHTDEFAULT)->EnableWindow(0);
//	GetDlgItem(IDC_CHANGERADIUS)->EnableWindow(0);
//	GetDlgItem(IDC_AutoMeasure)->EnableWindow(0);
//	GetDlgItem(IDC_AutoMeasureDown)->EnableWindow(0);
//	GetDlgItem(IDC_AutoMeasureReset)->EnableWindow(0);
//	GetDlgItem(IDC_AutoChangeMode)->EnableWindow(0);
//	GetDlgItem(IDC_OPENCLOSEPORT)->EnableWindow(0);
//	GetDlgItem(IDC_CONFIGPORT)->EnableWindow(0);
//	GetDlgItem(IDC_Threshold2)->EnableWindow(0);
//	GetDlgItem(IDC_Threshold3)->EnableWindow(0);
	
//	m_mask.Create(IDD_MASK);
//	m_mask.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT1)->EnableWindow(1);//权值输入框打开
	GetDlgItem(IDC_EDIT2)->EnableWindow(1);
	GetDlgItem(IDC_EDIT3)->EnableWindow(1);
//	GetDlgItem(IDC_EDIT4)->EnableWindow(1);
//	UpdateData(TRUE);



}

void CVideoCaputureDlg::OnBtnok() 
{
//	m_mask.Invalidate();
//	m_mask.DestroyWindow();
	
	is_show = false;
	is_select = false;
//	ROISelectedFlag=TRUE;
	numofregion=0;
	factorChangeFlag=TRUE;
//	UpdateData();
//	 m_bottom= m_mask.m_startPoint.y;
//	 m_top= m_mask.m_endPoint.y;
//	m_left = m_mask.m_startPoint.x;
//	m_right = m_mask.m_endPoint.x;
//	UpdateData(0);
	GetDlgItem(IDC_SELECTZONE)->EnableWindow(1);
	GetDlgItem(IDC_BTNOK)->EnableWindow(0);

	GetDlgItem(IDC_START)->EnableWindow(0);
//	GetDlgItem(IDC_STOP)->EnableWindow(1);
//	GetDlgItem(IDC_SAVEVIDEO)->EnableWindow(1);
//	GetDlgItem(IDC_SAVEIMAGES)->EnableWindow(1);
	GetDlgItem(IDC_Yinjing)->EnableWindow(1);
	GetDlgItem(IDC_Fangjian)->EnableWindow(1);
	GetDlgItem(IDC_Dengjing)->EnableWindow(1);
//	GetDlgItem(IDC_ADDBRIGHT)->EnableWindow(1);
//	GetDlgItem(IDC_REDUCEBRIGHT)->EnableWindow(1);
//	GetDlgItem(IDC_BRIGHTDEFAULT)->EnableWindow(1);
	GetDlgItem(IDC_CHANGERADIUS)->EnableWindow(1);
	GetDlgItem(IDC_AutoMeasure)->EnableWindow(1);
	GetDlgItem(IDC_AutoMeasureDown)->EnableWindow(1);
	GetDlgItem(IDC_AutoMeasureReset)->EnableWindow(1);
	GetDlgItem(IDC_OPENCLOSEPORT)->EnableWindow(1);
	GetDlgItem(IDC_CONFIGPORT)->EnableWindow(1);
	GetDlgItem(IDC_Threshold2)->EnableWindow(1);
	GetDlgItem(IDC_Threshold3)->EnableWindow(1);
	GetDlgItem(IDC_AutoChangeMode)->EnableWindow(1);
	

	GetDlgItem(IDC_EDIT1)->EnableWindow(0);//关闭权值输入框
	GetDlgItem(IDC_EDIT2)->EnableWindow(0);
	GetDlgItem(IDC_EDIT3)->EnableWindow(0);
//	GetDlgItem(IDC_EDIT4)->EnableWindow(0);
//	if(factorChangeFlag==TRUE)
    	UpdateData();
	if (is_Guidedia==TRUE)//把从编辑框中读入的数据，分别装进三种模式的buf中
	{
		m_edit1save_y=m_edit1;
		m_edit2save_y=m_edit2;
		m_edit3save_y=m_edit3;
//		m_edit4save_y=m_edit4;
	} 
	else if(is_fangjiandia==TRUE)
	{
		m_edit1save_f=m_edit1;
		m_edit2save_f=m_edit2;
		m_edit3save_f=m_edit3;
//		m_edit4save_f=m_edit4;

	}
	else if (is_Equaldia==TRUE)
	{
		m_edit1save_d=m_edit1;
		m_edit2save_d=m_edit2;
		m_edit3save_d=m_edit3;
//		m_edit4save_d=m_edit4;
	}
	
	
}


void CVideoCaputureDlg::OnChangeradius() 
{
	// TODO: Add your control notification handler code here
	CChangeRadius changeRadius;
	if(factorChangeFlag==TRUE)
		UpdateData();
	if(changeRadius.DoModal() == IDOK)
	{	
		Real_Diameter = changeRadius.GetRadius();
		m_DiametOld=Real_Diameter;
		m_DiametSecondOld=Real_Diameter;
		if (diameter!=0)
		{
			multiple = Real_Diameter/diameter;//在此处计算比例因子
			if(factorChangeFlag==TRUE)
			UpdateData(0);
			SetTimer(1,1000,NULL);//一秒钟触发一次定时器			
		}
		else 
		{
			AfxMessageBox("预处理，请稍等......",MB_OK,0);
			GetDlgItem(IDC_CHANGERADIUS)->EnableWindow(0);
		}
		
	}
	
}



void CVideoCaputureDlg::OnAutoMeasure() 
{
	// TODO: Add your control notification handler code here
    
 //   m_top = m_top +5;
	AutoORManualFlag=TRUE;
	if(is_Equaldia==true)
	{
	MeasurePoint+=5;
	}
	else if (is_Guidedia==true)
	{
		MeasurePoint+=1;
	}
	else if (is_fangjiandia==true)
	{
		MeasurePoint+=5;//YJAve_w1
	}
/*	CString   tmp;
	tmp.Format( "%d ",MeasurePoint);
  MessageBox( "AutoMaxRadius="+tmp );
*/
}

void CVideoCaputureDlg::OnReadAVI() 
{
	// TODO: Add your control notification handler code here
	CvCapture* pCapture = NULL;
	IplImage* pFrame = NULL; //原始帧
	IplImage* pThreshImg = NULL; //二值化图像
    IplImage* pImg = NULL;//灰度图像 
    IplImage* pback = NULL;
	int nFrmNum = 0;//帧数
	int real_nFrmNum=0;//实际有用的好帧数
//	char FrmName[25];
//	char Frmbmp[25];
	char Diadata[25];
	int  Top = 518;
    int  Bottom = 576-518;
	int  m_Left = 56;
	int  m_Right = 656;
//	double Avediameter = 0;
	int i,i1,j,j1;
	CvPoint Bottom_point;
	int flags,first,last;
	int Point_Num = 0;

	BOOL flagAutoMaxRadius=0;//有左右直径的第一帧标志位
	BOOL flagAutoBottomPoint_d=false;//等径有最低点的第一帧标志位
	BOOL flagAutoBottomPoint_f=false;//放肩有最低点的第一帧标志位
	BOOL flagAutofirstPoint=0;//记录第一次起点标志位
	BOOL flagAutolastPoint=0;//纪律第一次终点标志位
	BOOL flagblock=0;//回显直径求均值标志位
	BOOL flagReal_diameter=0;//最开始计算比例因子提供实际直径标志位
    int Real_diameterCounter=0;//平均测量值的积累统计
//	CChangeRadius changeRadius1;//获取输入的人工调整直径值
	int Ave_w=0;//固定最低点
	double Scale = 1;//以下是字体设置，显示像素数
	int Thickness = 1;
	CvFont Font1=cvFont(Scale,Thickness);
	int Ave_r=0;//固定左右最边缘点的平均纵坐标
	int first_start=0;//记录第一次起点
	int first_last=0;//记录第一次终点
	int YJAve_w=0;//跟踪最低点


	int LeftMaxValue=606;//自动全局扫描边缘最左端的点的X轴对应的值
	int LeftPointX=0;//自动全局扫描边缘最左端的点的Y轴对应的值

	int RightMaxValue=0;//自动全局扫描边缘最右端的点的X轴对应的值
	int RightPointX=0;//自动全局扫描边缘最右端的点的Y轴对应的值

	unsigned long ihist[256]={0};// 图像直方图，256个点
	unsigned char pixel;
	unsigned long Pixelcounter=0;//像素点数统计
	
	CString strCDit6;
	unsigned long PixelThresholdValue=0;//各个模式二值化阈值之上像素点个数
	
	//创建窗口
      cvNamedWindow("canny", 1);
      cvNamedWindow("Threshold",1);
	  cvNamedWindow("bright circle",1);

	  FILE *fpint;
	  FILE *fpdouble;
//	  ::DeleteFile("deng.txt");
//	  ::DeleteFile("yin.txt");
//	  ::DeleteFile("fang.txt");
//	  ::DeleteFile("Diamet.txt");
	  int junzhi=0;
	  int pingjun[15]={0};
	  int MeasureValue=0;//测量点处中心像素值
	  CvPoint writePoint;//用于在屏幕上显示像素值的位置
	  
      if( !(pCapture = cvCaptureFromFile("deng1.avi")))
	  {
		  MessageBox("error");
	       return;
	  }
	  

	  while(pFrame = cvQueryFrame( pCapture ))
	  {
		  nFrmNum++;
//		  nFrmNumCounter++;
		  Pixelcounter=0;
		  memset(ihist, 0, sizeof(ihist));
		  
		  //如果是第一帧，需要申请内存，并初始化
		  if(nFrmNum == 1)
		  { 
			  pThreshImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
			  pThreshImg -> origin = 1;

			  pImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
			  pImg->origin = 1;

			  pback = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
			  pback -> origin = 1;
		  }

		  else
		  {
//			  UpdateData();
			 /*m_Diamet = diameter*multiple;
			 m_Diamet=((m_Diamet/10)+(m_DiametOld*3/10))+(m_DiametSecondOld*6/10);//求个均值，减小测量误差
			 m_DiametSecondOld=m_DiametOld;
			 m_DiametOld=m_Diamet;
			  */
			  if(m_Diamet!=0)//初始化为零时，不显示
			  {
			  strCDit6.Format(_T("%.2lf"),m_Diamet);
			  SetDlgItemText(IDC_EDIT6,strCDit6);
			  if(factorChangeFlag==TRUE)
	  			 UpdateData(0);
			  }

			    //转化成单通道图像再处理
			  cvCvtColor(pFrame, pImg, CV_BGR2GRAY);
			  pFrame -> origin = 1;
			  if(useChangedyuzhiflag==FALSE)
		{

			  if(is_Guidedia>0)
			  {
//			  cvThreshold(pImg, pThreshImg, 200, 255, CV_THRESH_BINARY);//200用于引晶
				  PixelThresholdValue=1364;//0.5%
			  }
			  else if(is_fangjiandia>0)
			  {
//			  cvThreshold(pImg, pThreshImg, 120, 255, CV_THRESH_BINARY);//120用于放肩
				  PixelThresholdValue=53176;//17%
			  }
			  else
			  {
//			  cvThreshold(pImg, pThreshImg, 90, 255, CV_THRESH_BINARY);//230//原始参数，未动//用于等径
				  PixelThresholdValue=12512;//4%

			  }

				GrayIplShell Bin_Image(pImg);
				for(  i1 = 0; i1 <570; i1++)//遍历图像，统计亮度值
				{
					for(  j1 = 0; j1 <710; j1++)
					{
						pixel = Bin_Image[i1][j1];
						ihist[ pixel]++;
					}
					
				}

				for (i1=255;((Pixelcounter<PixelThresholdValue)&&(i1>=0));i1--)//找到17%最亮像素点的亮度下限
				{
					Pixelcounter+=ihist[i1];
				}
				cvThreshold(pImg,pThreshImg,i1,255,CV_THRESH_BINARY);//threshold
		   }
			  else
			  {
//				  cvThreshold(pImg, pThreshImg, m_edit5, m_edit7, CV_THRESH_BINARY);//公用阈值处理
			  }

			  cvShowImage("Threshold", pThreshImg);
			  			  
	//		  cvCanny(pThreshImg, pThreshImg, 80, 100, 3);//原值是80，100
	//			cvShowImage("canny",pThreshImg);

				ThresholdTest(pThreshImg);

				if((is_Equaldia==0)&&(is_Guidedia==0)&&(is_fangjiandia==0))
				{
					memcpy(m_lpbuff,(BYTE*)(pFrame->imageData),(pFrame->width)*(pFrame->height)*3);
					showpic();
				}
				else//按下引晶或者等径,或者放肩
				{
					GrayIplShell Bin_Image(pThreshImg);	//共用部分			
				for ( i = Bottom; i < Top; i++)//寻找基准点，光圈的最低处
				{
					for ( j = m_Left; j < m_Right; j++ )
					{
						if (Bin_Image[i][j] != 0)
						{
							break;
						}
						
					}
					
					if (Bin_Image[i][j] != 0)
					{
						Bottom_point.x = j;
						Bottom_point.y = i;
						break;
						
					}
					
				}
			cvLine(pFrame,cvPoint(j-5,i),cvPoint(j+5,i),CV_RGB(255,0,0),1,8,0);

			cvSetZero(pback);			
			GrayIplShell Back_Image(pback);//公共部分结束			  
///////////////////////////等径部分/////////////////////////////////////////////////////////////////
			if((is_Equaldia>0)||(AutoChangeModeFlag==1))
			{	//当等径按钮后才开始画线、测量
				AutoChangeModeFlag=1;//使模式切换具有记忆功能
				
				if (is_Equaldia>0)
				{
					yinfangdengflag=1;
				}
			
				if(factorChangeFlag==TRUE)
					UpdateData(0);//为了更新阈值文本框，而又不影响factorChangeFlag对update的控制，故只更新一次
				

			  if ((flagAutoBottomPoint_d==false)&&(Bottom_point.y!=0))//找到第一次最低点，保存
			  {
				  flagAutoBottomPoint_d=true;
				  Ave_w=Bottom_point.y;
				  
			  }

	 if(AutoORManualFlag==FALSE) {//扫描线自动调整
			  for(  i = (Bottom_point.y); i <Top; i++)//寻找左边缘轮廓
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Left; j < Bottom_point.x; j++)
				  {
					  
					  
					  if( Bin_Image[i][j] != 0)
					  {
						 Back_Image[i][j] = 255;
						 if (j<LeftMaxValue)
						 {
							 LeftMaxValue=j;
							 LeftPointX=i;//是纵坐标
						 }
						 break;
					  }
				  }
				  
			  }

			  
			  for(  i = (Bottom_point.y); i <Top; i++)//寻找右边缘轮廓
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Right; j > Bottom_point.x; j--)
				  {
					  
					  
					  if( Bin_Image[i][j] != 0)
					  {
						  Back_Image[i][j] = 255;
						  if (j>RightMaxValue)
						  {
							  RightMaxValue=j;
							  RightPointX=i;//是纵坐标
						  }
						  break;
					  }
				  }
				  
			  }
		MeasurePoint=(int)((LeftPointX+RightPointX)/2);//自动检测的测量点的最佳位置
		if ((flagAutoMaxRadius==0)&&(MeasurePoint!=0))//找到了第一次直径处的纵坐标，保存
		{
			flagAutoMaxRadius=1;
			Ave_r=MeasurePoint;
		}
		if ((flagAutoMaxRadius==1)&&(flagAutoBottomPoint_d==1))
		{
			MeasurePoint=(int)((Ave_r-Ave_w)/2)+Ave_w+35;
		}

	}
	 else //扫描线手动调节
	 {
		 for(  i = (Bottom_point.y); i <MeasurePoint+41; i++)//寻找左边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Left; j < Bottom_point.x; j++)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 
					 break;
				 }
			 }
			 
		 }
		 
		 
		 for(  i = (Bottom_point.y); i <MeasurePoint+41; i++)//寻找右边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Right; j > Bottom_point.x; j--)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 
					 break;
				 }
			 }
			 
			  }

	 }
		cvShowImage("bright circle",pback);

		for(junzhi=0;junzhi<15;junzhi++)//清零
			pingjun[junzhi]=0;
		junzhi=0;//清零
		MeasureValue=0;//测量点处中心像素值清零
		writePoint.x=0;//清零
		writePoint.y=0;//清零
       
			  for(  i = (MeasurePoint-60);i<(MeasurePoint+41); i+=10)//标注测量点
			  {
				  flags = 0;
				  first = 0;
				  last =  0;			  

				  for(  j = m_Left; j < m_Right; j++)
				  {
					  
					  
					  if( Back_Image[i][j] != 0)
					  {
						  if( flags == 0)
						  {   
							  first = j;
							  flags = 1;
							  last = j;
						  }

						  last = j;
					  }

					}
				  if (i==MeasurePoint)
				  {
					  MeasureValue=last-first;
				  }
				  
				  if ((last-first)!=0)
				  {
					  pingjun[junzhi]=last-first;
					  junzhi++;
					  
				  }
			//	  cvLine(pFrame,cvPoint(first,i),cvPoint(last,i),CV_RGB(0,0,255),1,8,0);

				  if(i==MeasurePoint)
				  {
				  cvLine(pFrame,cvPoint(first-7,i),cvPoint(first+7,i),CV_RGB(0,255,0),1,8,0);//起点横线
			      cvLine(pFrame,cvPoint(last-7,i),cvPoint(last+7,i),CV_RGB(0,255,0),1,8,0);//终点横线
				  cvLine(pFrame,cvPoint(first,i+7),cvPoint(first,i-7),CV_RGB(0,255,0),1,8,0);//起点竖线
			      cvLine(pFrame,cvPoint(last,i+7),cvPoint(last,i-7),CV_RGB(0,255,0),1,8,0);//终点竖线
  				  cvLine(pFrame,cvPoint(first-65,i+8),cvPoint(last+30,i+8),CV_RGB(255,0,0),1,8,0);
  			      cvLine(pFrame,cvPoint(first-65,i-8),cvPoint(last+30,i-8),CV_RGB(255,0,0),1,8,0);
  			      cvLine(pFrame,cvPoint(first-65,i+8),cvPoint(first-65,i-8),CV_RGB(255,0,0),1,8,0);
			      cvLine(pFrame,cvPoint(last+30,i+8),cvPoint(last+30,i-8),CV_RGB(255,0,0),1,8,0);
					//下面是加粗扫描线
				  cvLine(pFrame,cvPoint(first-7,i+1),cvPoint(first+7,i+1),CV_RGB(0,255,0),1,8,0);//起点横线
				  cvLine(pFrame,cvPoint(last-7,i+1),cvPoint(last+7,i+1),CV_RGB(0,255,0),1,8,0);//终点横线
				  cvLine(pFrame,cvPoint(first+1,i+7),cvPoint(first+1,i-7),CV_RGB(0,255,0),1,8,0);//起点竖线
				  cvLine(pFrame,cvPoint(last+1,i+7),cvPoint(last+1,i-7),CV_RGB(0,255,0),1,8,0);//终点竖线
				  cvLine(pFrame,cvPoint(first-65,i+9),cvPoint(last+30,i+9),CV_RGB(255,0,0),1,8,0);
				  cvLine(pFrame,cvPoint(first-65,i-7),cvPoint(last+30,i-7),CV_RGB(255,0,0),1,8,0);
				  cvLine(pFrame,cvPoint(first-64,i+8),cvPoint(first-64,i-8),CV_RGB(255,0,0),1,8,0);
			      cvLine(pFrame,cvPoint(last+31,i+8),cvPoint(last+31,i-8),CV_RGB(255,0,0),1,8,0);
					
				  writePoint.x=first-50;
				  writePoint.y=i-7;
				  }
			  }
			  if(junzhi==0)//模式切换，避免除数为零
				continue;
			  else  junzhi=(pingjun[0]+pingjun[1]+pingjun[2]+pingjun[3]+pingjun[4]+pingjun[5]+pingjun[6]+pingjun[7]+pingjun[8]+pingjun[9]+pingjun[10]+pingjun[11]+pingjun[12])/junzhi;

		//	itoa(MeasureValue,Diadata,10);//把一整数转换为字符串,十进制方式
			itoa(junzhi,Diadata,10);
			cvPutText( pFrame, Diadata,writePoint, &Font1, CV_RGB(255,0,0));//cvPoint(first-50,i-7)				 
			memcpy(m_lpbuff,(BYTE*)(pFrame->imageData),(pFrame->width)*(pFrame->height)*3);
	        showpic();

			fpint=fopen("deng.txt","aw+");
			fprintf(fpint,"%d\n",junzhi);
			fclose(fpint);
		  }
///////////////////////////////等径部分结束//////////////////////////////////////////////////
///////////////////////////////引晶开始///////////////////////////////////////////////////////
		  if ((is_Guidedia>0)||(AutoChangeModeFlag==2))//当引晶按钮后才开始画线、测量
		  {
			  AutoChangeModeFlag=2;
			  if (is_Guidedia>0)
			  {
				  yinfangdengflag=2;
			  }

			  if(factorChangeFlag==TRUE)
				  UpdateData(0);//为了更新阈值文本框，而又不影响factorChangeFlag对update的控制，故只更新一次
			  
			  YJAve_w=Bottom_point.y;
	 if(AutoORManualFlag==FALSE) {//扫描线自动调整

		 for(  i = (Bottom_point.y); i <Top; i++)//寻找左边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Left; j < Bottom_point.x; j++)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 if (j<LeftMaxValue)
					 {
						 LeftMaxValue=j;
						 LeftPointX=i;
					 }
					 break;
				 }
			 }
			 
		 }
		 
		 
		 for(  i = (Bottom_point.y); i <Top; i++)//寻找右边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Right; j > Bottom_point.x; j--)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 if (j>RightMaxValue)
					 {
						 RightMaxValue=j;
						 RightPointX=i;
					 }
					 break;
				 }
			 }
			 
			  }
		MeasurePoint=YJAve_w+YJAve_w1;//找到距离底边距离20个像素的相对固定位置

	}
	 else //扫描线手动调节
	 {
		 for(  i = (Bottom_point.y); i <MeasurePoint; i++)//寻找左边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Left; j < Bottom_point.x; j++)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 
					 break;
				 }
			 }
			 
		 }
		 
		 
		 for(  i = (Bottom_point.y); i <MeasurePoint; i++)//寻找右边缘轮廓
		 { 
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Right; j > Bottom_point.x; j--)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 
					 break;
				 }
			 }
			 
		 }

	 }
	 cvShowImage("bright circle",pback);

	 pingjun[0]=pingjun[1]=pingjun[2]=pingjun[3]=pingjun[4]=0;
	 junzhi=0;//清零

			  for(  i = MeasurePoint-2; i < MeasurePoint+3; i++)//标注测量点
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  
				  for(  j = m_Left; j < m_Right; j++)
				  {
					  
					  
					  if( Back_Image[i][j] != 0)
					  {
						  if( flags == 0)
						  {   
							  first = j;
							  flags = 1;
							  last = j;
						  }

						  last = j;
					  }
				  }
				  if ((last-first)!=0)
				  {
					  pingjun[junzhi]=last-first;
					  junzhi++;
					  
				  }

				  if(i==MeasurePoint)
				  {
				  cvLine(pFrame,cvPoint(first-5,i),cvPoint(first+5,i),CV_RGB(0,255,0),1,8,0);//起点横线
			      cvLine(pFrame,cvPoint(last-5,i),cvPoint(last+5,i),CV_RGB(0,255,0),1,8,0);//终点横线
				  cvLine(pFrame,cvPoint(first,i+5),cvPoint(first,i-5),CV_RGB(0,255,0),1,8,0);//起点竖线
			      cvLine(pFrame,cvPoint(last,i+5),cvPoint(last,i-5),CV_RGB(0,255,0),1,8,0);//终点竖线
				  //下面加粗
				  cvLine(pFrame,cvPoint(first-4,i),cvPoint(first+6,i),CV_RGB(0,255,0),1,8,0);//起点横线
				  cvLine(pFrame,cvPoint(last-4,i),cvPoint(last+6,i),CV_RGB(0,255,0),1,8,0);//终点横线
				  cvLine(pFrame,cvPoint(first,i+6),cvPoint(first,i-4),CV_RGB(0,255,0),1,8,0);//起点竖线
			      cvLine(pFrame,cvPoint(last,i+6),cvPoint(last,i-4),CV_RGB(0,255,0),1,8,0);//终点竖线
				  }
				  
			  } 
				if(junzhi==0)//模式切换，避免除数为零
					continue;
				else  junzhi=(pingjun[0]+pingjun[1]+pingjun[2]+pingjun[3]+pingjun[4])/junzhi;
				  itoa(junzhi,Diadata,10);//把一整数转换为字符串,十进制方式
				  cvPutText( pFrame, Diadata, cvPoint(first-50,i-7), &Font1, CV_RGB(255,0,0));				 
				  memcpy(m_lpbuff,(BYTE*)(pFrame->imageData),(pFrame->width)*(pFrame->height)*3);
				  showpic();
				  
				  fpint=fopen("yin.txt","aw+");
				  fprintf(fpint,"%d\n",junzhi);
				  fclose(fpint);
		}
//////////////////////////////引晶结束////////////////////////////////////////////////////////
///////////////////////////////放肩部分///////////////////////////////////////////////////////
		if ((is_fangjiandia>0)||(AutoChangeModeFlag==3))//放肩部分
		{

//			m_edit1 = 1;
//			m_edit2 = 1;
//			m_edit3 = 0;
//			m_edit4 = 1;
			if(factorChangeFlag==TRUE)
		    UpdateData(0);//为了更新阈值文本框，而又不影响factorChangeFlag对update的控制，故只更新一次


			AutoChangeModeFlag=3;
			if (is_fangjiandia>0)
			{
				yinfangdengflag=3;
			}
			if ((flagAutoBottomPoint_f==0)&&(Bottom_point.y!=0))//找到第一次最低点，保存
			  {
				  flagAutoBottomPoint_f=1;
				  Ave_w=Bottom_point.y;
			  }

	 if(AutoORManualFlag==FALSE) {//扫描线自动调整
			  for(  i = (Bottom_point.y); i <Top; i++)//寻找左边缘轮廓
			  {
				  for(  j = m_Left; j < Bottom_point.x; j++)
				  {
					  
					  
					  if( Bin_Image[i][j] != 0)
					  {
						 Back_Image[i][j] = 255;
						 if (j<LeftMaxValue)
						 {
							 LeftMaxValue=j;
							 LeftPointX=i;
						 }
						 break;
					  }
				  }
				  
			  }

			  
			  for(  i = (Bottom_point.y); i <Top; i++)//寻找右边缘轮廓
			  {
				  for(  j = m_Right; j > Bottom_point.x; j--)
				  {
					  
					  
					  if( Bin_Image[i][j] != 0)
					  {
						  Back_Image[i][j] = 255;
						  if (j>RightMaxValue)
						  {
							  RightMaxValue=j;
							  RightPointX=i;
						  }
						  break;
					  }
				  }
				  
			  }
		MeasurePoint=(int)((LeftPointX+RightPointX)/2);//自动检测的测量点的最佳位置
		if ((flagAutoMaxRadius==0)&&(MeasurePoint!=0))//找到了第一次直径处的纵坐标，保存
		{
			flagAutoMaxRadius=1;
			Ave_r=MeasurePoint;
		}
		if ((flagAutoMaxRadius==1)&&(flagAutoBottomPoint_f==1))
		{
			MeasurePoint=(int)((Ave_r-Ave_w)/2)+Ave_w+35;
		}

	}
	 else //扫描线手动调节
	 {
		 for(  i = (Bottom_point.y); i <MeasurePoint+41; i++)//寻找左边缘轮廓
		 {
			 for(  j = m_Left; j < Bottom_point.x; j++)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 
					 break;
				 }
			 }
			 
		 }
		 
		 
		 for(  i = (Bottom_point.y); i <MeasurePoint+41; i++)//寻找右边缘轮廓
		 {
			 for(  j = m_Right; j > Bottom_point.x; j--)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 
					 break;
				 }
			 }
			 
			  }

	 }
		cvShowImage("bright circle",pback);

		for(junzhi=0;junzhi<15;junzhi++)//清零
			pingjun[junzhi]=0;
		junzhi=0;//清零
		MeasureValue=0;//清零
		writePoint.x=0;//清零
		writePoint.y=0;//清零
		
			  for(  i = (MeasurePoint-60);i<(MeasurePoint+41); i+=10)//标注测量点
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Left; j < m_Right; j++)
				  {
					  
					  
					  if( Back_Image[i][j] != 0)
					  {
						  if( flags == 0)
						  {   
							  first = j;
							  flags = 1;
							  last = j;
						  }

						  last = j;
					  }
				  }

				  if (i==MeasurePoint)
				  {
					  MeasureValue=last-first;
				  }

				  if ((last-first)!=0)
				  {
					  pingjun[junzhi]=last-first;
					  junzhi++;					  
				  }

			//	  cvLine(pFrame,cvPoint(first,i),cvPoint(last,i),CV_RGB(0,0,255),1,8,0);

				  if(i ==MeasurePoint)
				  {
			  cvLine(pFrame,cvPoint(first-7,i),cvPoint(first+7,i),CV_RGB(0,255,0),1,8,0);//起点横线
			  cvLine(pFrame,cvPoint(last-7,i),cvPoint(last+7,i),CV_RGB(0,255,0),1,8,0);//终点横线
			  cvLine(pFrame,cvPoint(first,i+7),cvPoint(first,i-7),CV_RGB(0,255,0),1,8,0);//起点竖线
			  cvLine(pFrame,cvPoint(last,i+7),cvPoint(last,i-7),CV_RGB(0,255,0),1,8,0);//终点竖线
  			  cvLine(pFrame,cvPoint(first-65,i+8),cvPoint(last+30,i+8),CV_RGB(255,0,0),1,8,0);
  			  cvLine(pFrame,cvPoint(first-65,i-8),cvPoint(last+30,i-8),CV_RGB(255,0,0),1,8,0);
  			  cvLine(pFrame,cvPoint(first-65,i+8),cvPoint(first-65,i-8),CV_RGB(255,0,0),1,8,0);
			  cvLine(pFrame,cvPoint(last+30,i+8),cvPoint(last+30,i-8),CV_RGB(255,0,0),1,8,0);

			  //下面是加粗扫描线
			  cvLine(pFrame,cvPoint(first-7,i+1),cvPoint(first+7,i+1),CV_RGB(0,255,0),1,8,0);//起点横线
			  cvLine(pFrame,cvPoint(last-7,i+1),cvPoint(last+7,i+1),CV_RGB(0,255,0),1,8,0);//终点横线
			  cvLine(pFrame,cvPoint(first+1,i+7),cvPoint(first+1,i-7),CV_RGB(0,255,0),1,8,0);//起点竖线
			  cvLine(pFrame,cvPoint(last+1,i+7),cvPoint(last+1,i-7),CV_RGB(0,255,0),1,8,0);//终点竖线
			  cvLine(pFrame,cvPoint(first-65,i+9),cvPoint(last+30,i+9),CV_RGB(255,0,0),1,8,0);
			  cvLine(pFrame,cvPoint(first-65,i-7),cvPoint(last+30,i-7),CV_RGB(255,0,0),1,8,0);
			  cvLine(pFrame,cvPoint(first-64,i+8),cvPoint(first-64,i-8),CV_RGB(255,0,0),1,8,0);
			  cvLine(pFrame,cvPoint(last+31,i+8),cvPoint(last+31,i-8),CV_RGB(255,0,0),1,8,0);
			  
			  writePoint.x=first-50;
				  writePoint.y=i-7;
				  }
				  
			  }		
			  if(junzhi==0)//模式切换，避免除数为零
				  continue;
			  else junzhi=(pingjun[0]+pingjun[1]+pingjun[2]+pingjun[3]+pingjun[4]+pingjun[5]+pingjun[6]+pingjun[7]+pingjun[8]+pingjun[9]+pingjun[10]+pingjun[11]+pingjun[12])/junzhi;
			itoa(MeasureValue,Diadata,10);//把一整数转换为字符串,十进制方式
			cvPutText( pFrame, Diadata, writePoint, &Font1, CV_RGB(255,0,0));				 
			memcpy(m_lpbuff,(BYTE*)(pFrame->imageData),(pFrame->width)*(pFrame->height)*3);
		    showpic();

			fpint=fopen("fang.txt","aw+");
			fprintf(fpint,"%d\n",junzhi);
			fclose(fpint);
		}

/*
		if ((is_fangjiandia>0)||(AutoChangeModeFlag==3))//放肩部分
		{
			AutoChangeModeFlag=3;
			if (is_fangjiandia>0)
			{
				yinfangdengflag=3;
			}
			if ((flagAutoBottomPoint==0)&&(Bottom_point.y!=0))//找到第一次最低点，保存
			  {
				  flagAutoBottomPoint=1;
				  Ave_w=Bottom_point.y;
			  }

	 if(AutoORManualFlag==FALSE) {//扫描线自动调整
			  for(  i = (Bottom_point.y); i <Top; i++)//寻找左边缘轮廓
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Left; j < Bottom_point.x; j++)
				  {
					  
					  
					  if( Bin_Image[i][j] != 0)
					  {
						 Back_Image[i][j] = 255;
						 if (j<LeftMaxValue)
						 {
							 LeftMaxValue=j;
							 LeftPointX=i;
						 }
						 break;
					  }
				  }
				  
			  }

			  
			  for(  i = (Bottom_point.y); i <Top; i++)//寻找右边缘轮廓
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Right; j > Bottom_point.x; j--)
				  {
					  
					  
					  if( Bin_Image[i][j] != 0)
					  {
						  Back_Image[i][j] = 255;
						  if (j>RightMaxValue)
						  {
							  RightMaxValue=j;
							  RightPointX=i;
						  }
						  break;
					  }
				  }
				  
			  }
		AutoMaxRadius=(int)((LeftPointX+RightPointX)/2);//自动检测的测量点的最佳位置
		MeasurePoint=AutoMaxRadius;
		if ((flagAutoMaxRadius==0)&&(MeasurePoint!=0))//找到了第一次直径处的纵坐标，保存
		{
			flagAutoMaxRadius=1;
			Ave_r=MeasurePoint;
		}
		if ((flagAutoMaxRadius==1)&&(flagAutoBottomPoint==1))
		{
			MeasurePoint=(int)((Ave_r-Ave_w)/2)+Ave_w+40;
		}

	}
	 else //扫描线手动调节
	 {
		 for(  i = (Bottom_point.y); i <MeasurePoint; i++)//寻找左边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Left; j < Bottom_point.x; j++)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 
					 break;
				 }
			 }
			 
		 }
		 
		 
		 for(  i = (Bottom_point.y); i <MeasurePoint; i++)//寻找右边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Right; j > Bottom_point.x; j--)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 
					 break;
				 }
			 }
			 
			  }

	 }
		cvShowImage("bright circle",pback);
       
			  for(  i = MeasurePoint-2; i < MeasurePoint; i++)//标注测量点
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Left; j < m_Right; j++)
				  {
					  
					  
					  if( Back_Image[i][j] != 0)
					  {
						  if( flags == 0)
						  {   
							  first = j;
							  flags = 1;
							  last = j;
						  }

						  last = j;
					  }
				  }
				  cvLine(pFrame,cvPoint(first-5,i),cvPoint(first+5,i),CV_RGB(0,255,0),1,8,0);//起点横线
			      cvLine(pFrame,cvPoint(last-5,i),cvPoint(last+5,i),CV_RGB(0,255,0),1,8,0);//终点横线
				  cvLine(pFrame,cvPoint(first,i+5),cvPoint(first,i-5),CV_RGB(0,255,0),1,8,0);//起点竖线
			      cvLine(pFrame,cvPoint(last,i+5),cvPoint(last,i-5),CV_RGB(0,255,0),1,8,0);//终点竖线
  			  cvLine(pFrame,cvPoint(first-65,i+8),cvPoint(last+30,i+8),CV_RGB(255,0,0),1,8,0);
  			  cvLine(pFrame,cvPoint(first-65,i-8),cvPoint(last+30,i-8),CV_RGB(255,0,0),1,8,0);
  			  cvLine(pFrame,cvPoint(first-65,i+8),cvPoint(first-65,i-8),CV_RGB(255,0,0),1,8,0);
			  cvLine(pFrame,cvPoint(last+30,i+8),cvPoint(last+30,i-8),CV_RGB(255,0,0),1,8,0);

				  
			  }				  
			itoa(last-first,Diadata,10);//把一整数转换为字符串,十进制方式
			cvPutText( pFrame, Diadata, cvPoint(first-50,i-7), &Font1, CV_RGB(255,0,0));				 
			memcpy(m_lpbuff,(BYTE*)(pFrame->imageData),(pFrame->width)*(pFrame->height)*3);
	        showpic();

		}
*/
////////////////////////////////放肩部分，结束/////////////////////////////////////////////////
////////////////////////////////比例因子，权值修改部分////////////////////////////////////////
		if (is_Guidedia==TRUE)//三种模式切换时，保存各自的权值，当模式切换时换回原来是用的权值
		{
			m_edit1=m_edit1save_y;
			m_edit2=m_edit2save_y;
			m_edit3=m_edit3save_y;
//			m_edit4=m_edit4save_y;
//			m_edit5=m_edit5save_y;
//			m_edit7=m_edit7save_y;
		} 
		else if(is_fangjiandia==TRUE)
		{
			m_edit1=m_edit1save_f;
			m_edit2=m_edit2save_f;
			m_edit3=m_edit3save_f;
//			m_edit4=m_edit4save_f;
//			m_edit5=m_edit5save_f;
//			m_edit7=m_edit7save_f;
		}
		else if(is_Equaldia==TRUE)
		{
			m_edit1=m_edit1save_d;
			m_edit2=m_edit2save_d;
			m_edit3=m_edit3save_d;
//			m_edit4=m_edit4save_d;
//			m_edit5=m_edit5save_d;
//			m_edit7=m_edit7save_d;
		}
//		strForFactor.Format(_T("%lf"),m_edit1);
//		GetDlgItem(IDC_EDIT1)->SetWindowText(_T(strForFactor));
//		strForFactor.Format(_T("%lf"),m_edit2);
//		GetDlgItem(IDC_EDIT2)->SetWindowText(_T(strForFactor));
//		strForFactor.Format(_T("%lf"),m_edit3);
//		GetDlgItem(IDC_EDIT3)->SetWindowText(_T(strForFactor));
//		strForFactor.Format(_T("%lf"),m_edit4);
//		GetDlgItem(IDC_EDIT4)->SetWindowText(_T(strForFactor));
////////////////////////////////权值修改部分，结束/////////////////////////////////////////////
		  real_nFrmNum++;//第二部分，公共部分
		  


		  				  if (real_nFrmNum % 50 != 0)
						  {
							  R_Ave[nFrmNum%50] = r;//用于测试，向文件中写入
							  Dia[real_nFrmNum%50] =junzhi;//last - first
						  }
						  
						  
						  if (real_nFrmNum % 50 == 0)
						  {
							  
							  diameter = R_Average(Dia,50);//半径求平均值
							  if(factorChangeFlag==TRUE)
							  UpdateData();
							  m_Diamet = diameter*multiple;
							  
							  m_Diamet=((m_Diamet*m_edit1/(m_edit1+m_edit2+m_edit3))+(m_DiametOld*m_edit2/(m_edit1+m_edit2+m_edit3)))+(m_DiametSecondOld*m_edit3/(m_edit1+m_edit2+m_edit3));//求个均值，减小测量误差
							  m_DiametSecondOld=m_DiametOld;
							  m_DiametOld=m_Diamet;
							  
							  
							  strCDit6.Format(_T("%.2f"),m_Diamet);
							  SetDlgItemText(IDC_EDIT6,strCDit6);
							  if(factorChangeFlag==TRUE)
							  UpdateData(0);
								
							  if(m_Diamet!=0)//零是没有意义的
							  {
							  fpdouble=fopen("Diamet.txt","aw+");//保存直径值
							  fprintf(fpdouble,"%f\n",m_Diamet);
							  fclose(fpdouble);
							  }
							  
						  }//第二部分公共部分，完毕

						  if (diameter>0)//读取50帧之前，不允许输入数据
						  {
							  GetDlgItem(IDC_CHANGERADIUS)->EnableWindow(1);
						  } 
						  else
						  {
							  GetDlgItem(IDC_CHANGERADIUS)->EnableWindow(0);
						  }

	}//end of 按下引晶或者等径,或者放肩

	  }//end of 不是第一帧

		  //如果有按键事件，则跳出循环
		  //此等待也为cvShowImage函数提供时间完成显示
			  //等待时间可以根据CPU速度调整

		  if(cvWaitKey(32)>=0)
				  break;
		  
	  }

/*	  FILE *fpint; 
	  
	  //保存半径数据文件 
	  if((fpint=fopen("a.txt","w+"))!=NULL) 
	  { 
		  for(i=0; i<1000; i++) 
		  { 
			  fprintf(fpint,"%f\n",Dia[i]); 
		  } 
		  fclose(fpint); 
	  } 
	  else 
	  { 
		  MessageBox("写文件 b.txt 失败！", "错误");
	  } 
*/

	  cvDestroyAllWindows();	  
	  //释放图像和矩阵
	  //cvReleaseImage(&pFrame);
	  cvReleaseImage(&pThreshImg);
      cvReleaseImage(&pImg);	  
	  cvReleaseCapture(&pCapture);	
}





void CVideoCaputureDlg::OnClose() 
{
	if(m_comm.GetPortOpen()){
		m_comm.SetPortOpen(FALSE);
	}
	
	CDialog::OnClose();
}

void CVideoCaputureDlg::OnDengjing() 
{
	// TODO: Add your control notification handler code here

	is_Equaldia = true;
	is_Guidedia = false;
	is_fangjiandia=false;
}

void CVideoCaputureDlg::Measure() 
{
	// TODO: Add your control notification handler code here
	IplImage* pFrame = cvCreateImageHeader(cvSize(720,576),IPL_DEPTH_8U,3);//NULL; //原始帧
	IplImage* pThreshImg = NULL; //二值化图像
    IplImage* pImg = NULL;//灰度图像 
    IplImage* pback = NULL;
//	char FrmName[25];
//	char Frmbmp[25];
	char Diadata[25];
	int  Top = 518;
    int  Bottom = 576-518;
	int  m_Left = 56;
	int  m_Right = 656;
//	double Avediameter = 0;
	int i,j,i1,j1;
	CvPoint Bottom_point;
	int flags,first,last;
	int Point_Num = 0;
	int YJAve_w=0;//引晶的最低点

	double Scale = 1;
	int Thickness = 1;
	CvFont Font1=cvFont(Scale,Thickness);

//	int real_nFrmNum=0;//实际有用的好帧数
	BOOL flagAutoMaxRadius=0;//有左右直径的第一帧标志位
	BOOL flagAutoBottomPoint_d=false;//等径有最低点的第一帧标志位
	BOOL flagAutoBottomPoint_f=false;//放肩有最低点的第一帧标志位
	BOOL flagAutofirstPoint=0;//记录第一次起点标志位
	BOOL flagAutolastPoint=0;//纪律第一次终点标志位
	BOOL flagblock=0;//边框初始化标志位，仅允许初始化一次
	BOOL flagReal_diameter=0;//最开始计算比例因子提供实际直径标志位
    int Real_diameterCounter=0;//平均测量值的积累统计
	//	CChangeRadius changeRadius1;//获取输入的人工调整直径值
	BOOL Allowradiusflag=0;//允许测量直径标志位
	CString strCDit6;
	
	int Ave_w=0;//固定最低点
	int Ave_r=0;//固定左右最边缘点的平均纵坐标
	int first_start=0;//记录第一次起点
	int first_last=0;//记录第一次终点


	int LeftMaxValue=606;//自动全局扫描边缘最左端的点的X轴对应的值
	int LeftPointX=0;//自动全局扫描边缘最左端的点的Y轴对应的值
	
	int RightMaxValue=0;//自动全局扫描边缘最右端的点的X轴对应的值
	int RightPointX=0;//自动全局扫描边缘最右端的点的Y轴对应的值	

	unsigned long ihist[256]={0};// 图像直方图，256个点
	unsigned char pixel;
	unsigned long Pixelcounter=0;//像素点数统计,每次使用都要归零
	unsigned long PixelThresholdValue=0;//各个模式二值化阈值之上像素点个数
	FILE *fpint;//像素数据观测
	FILE *fpdouble;//直径数据观测
	int junzhi=0;
	int pingjun[15]={0};
	int MeasureValue=0;//测量点处中心像素值
	CvPoint writePoint;//用于在屏幕上显示像素值的位置

	          cvSetData(pFrame,m_lpbuff,720*3);
		      memset(ihist, 0, sizeof(ihist));
			  pFrame -> origin = 1;
			  
			  pThreshImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
			  pThreshImg -> origin = 1;

			  pImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
			  pImg->origin = 1;

			  pback = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
			  pback -> origin = 1;

/*			  UpdateData();
			  m_Diamet = diameter*multiple;
			  m_Diamet=((m_Diamet/10)+(m_DiametOld*3/10))+(m_DiametSecondOld*6/10);//求个均值，减小测量误差
			  m_DiametSecondOld=m_DiametOld;
			  m_DiametOld=m_Diamet;
*/
			  strCDit6.Format(_T("%.2lf"),m_Diamet);
			  SetDlgItemText(IDC_EDIT6,strCDit6);
			  if(factorChangeFlag==TRUE)
	  	      UpdateData(0);
			               
			 

			    //转化成单通道图像再处理
			  cvCvtColor(pFrame, pImg, CV_BGR2GRAY);
			  if(useChangedyuzhiflag==FALSE)
			{
			  if(is_Guidedia>0)
			  {
				 // cvThreshold(pImg, pThreshImg, 180, 255, CV_THRESH_BINARY);//用于引晶
				PixelThresholdValue=1364;
			  }
			  else if(is_fangjiandia>0)
			  {
				//  cvThreshold(pImg, pThreshImg, 120, 255, CV_THRESH_BINARY);//用于放肩
			  PixelThresholdValue=53176;
			  }
			  else
			  {
				PixelThresholdValue=12512;
				  //cvThreshold(pImg, pThreshImg, 230, 255, CV_THRESH_BINARY);//canny的时候是165//原始参数，未动//用于等径			  			  
			  }

			  GrayIplShell Bin_Image(pImg);
			  for(  i1 = 0; i1 <570; i1++)//遍历图像，统计亮度值
			  {
				  for(  j1 = 0; j1 <710; j1++)
				  {
					  pixel = Bin_Image[i1][j1];
					  ihist[ pixel]++;
				  }
				  
			  }
			  
			  for (i1=255;((Pixelcounter<PixelThresholdValue)&&(i1>=0));i1--)//找到17%最亮像素点的亮度下限
			  {
				  Pixelcounter+=ihist[i1];
			  }
			  cvThreshold(pImg,pThreshImg,i1,255,CV_THRESH_BINARY);//threshold
		  }
			  else
			  {
//				  cvThreshold(pImg, pThreshImg, m_edit5, m_edit7, CV_THRESH_BINARY);//公用阈值处理
			  }

//			  if (Thresholdflag==TRUE)//用于观看二值化结果，以便选择阈值
//			  {
//				  cvNamedWindow("Threshold_text",1);//如果已经存在这个名字的窗口，这个函数将不做任何事情。//需要的就是这条重要的特性
//				  cvShowImage("Threshold", pThreshImg);
//			  }
//			  else cvDestroyAllWindows();
			  ThresholdTest(pThreshImg);

	//		  cvCanny(pThreshImg, pThreshImg, 80, 100, 3);//原值是80，100

			  if((is_Equaldia==0)&&(is_Guidedia==0)&&(is_fangjiandia==0))
			  {
				  memcpy(m_lpbuff,(BYTE*)(pFrame->imageData),(pFrame->width)*(pFrame->height)*3);
				  showpic();
			  }
			  else 
			  { 
				  GrayIplShell Bin_Image(pThreshImg);//共用部分
				  for ( i = Bottom; i < Top; i++)//寻找基准点，光圈的最低处
				  {
					  for ( j = m_Left; j < m_Right; j++ )
					  {
						  if (Bin_Image[i][j] != 0)
						  {
							  break;
						  }
						  
					  }
					  
					  if (Bin_Image[i][j] != 0)
					  {
						  Bottom_point.x = j;
						  Bottom_point.y = i;
						  break;
						  
					  }
					  
				  }
				  cvLine(pFrame,cvPoint(j-5,i),cvPoint(j+5,i),CV_RGB(255,0,0),1,8,0);
				  
				  cvSetZero(pback);				  
			  GrayIplShell Back_Image(pback);//共用部分结束
///////////////////////////////////等径部分////////////////////////////////////////////////
				  if((is_Equaldia>0)||(AutoChangeModeFlag==1)){	//当等径按钮后才开始画线、测量
					  AutoChangeModeFlag=1;
					  if (is_Equaldia>0)
					  {
						  yinfangdengflag=1;
					  }
					  if(factorChangeFlag==TRUE)
						  UpdateData(0);//为了更新阈值文本框，而又不影响factorChangeFlag对update的控制，故只更新一次
				
			  if ((flagAutoBottomPoint_d==0)&&(Bottom_point.y!=0))//找到第一次最低点，保存
			  {
				  flagAutoBottomPoint_d=true;
				  Ave_w=Bottom_point.y;
			  }

              
		if(AutoORManualFlag==FALSE) {//扫描线自动调整
				  for(  i = (Bottom_point.y); i <Top; i++)//寻找左边缘轮廓
				  {
					  flags = 0;
					  first = 0;
					  last =  0;
					  for(  j = m_Left; j < Bottom_point.x; j++)
					  {
						  
						  
						  if( Bin_Image[i][j] != 0)
						  {
							  Back_Image[i][j] = 255;
							  if (j<LeftMaxValue)
							  {
								  LeftMaxValue=j;
								  LeftPointX=i;
							  }
							  break;
						  }
					  }
					  
				  }
				  
				  
				  for(  i = (Bottom_point.y); i <Top; i++)//寻找右边缘轮廓
				  {
					  flags = 0;
					  first = 0;
					  last =  0;
					  for(  j = m_Right; j > Bottom_point.x; j--)
					  {
						  
						  
						  if( Bin_Image[i][j] != 0)
						  {
							  Back_Image[i][j] = 255;
							  if (j>RightMaxValue)
							  {
								  RightMaxValue=j;
								  RightPointX=i;
							  }
							  break;
						  }
					  }
					  
				  }
				  AutoMaxRadius=(int)((LeftPointX+RightPointX)/2);//自动检测的测量点的最佳位置
				  MeasurePoint=AutoMaxRadius;
				  if ((flagAutoMaxRadius==0)&&(MeasurePoint!=0))//找到了第一次直径处的纵坐标，保存
				  {
					  flagAutoMaxRadius=1;
					  Ave_r=MeasurePoint;
				  }
				  if ((flagAutoMaxRadius==1)&&(flagAutoBottomPoint_d==1))
				  {
					  MeasurePoint=(int)((Ave_r-Ave_w)/2)+Ave_w+35;
				  }
				  
		}
			else//扫描线手动调节
			{
			  for(  i = (Bottom_point.y); i <MeasurePoint; i++)//寻找左边缘轮廓
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Left; j < Bottom_point.x; j++)
				  {
					  
					  
					  if( Bin_Image[i][j] != 0)
					  {
						 Back_Image[i][j] = 255;

						 break;
					  }
				  }
				  
			  }
		  
			  for(  i = (Bottom_point.y); i <MeasurePoint; i++)//寻找右边缘轮廓
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Right; j > Bottom_point.x; j--)
				  {
					  
					  
					  if( Bin_Image[i][j] != 0)
					  {
						  Back_Image[i][j] = 255;
						  
						  break;
					  }
				  }
				  
			  }
			} 
			
			for(junzhi=0;junzhi<15;junzhi++)//清零
				pingjun[junzhi]=0;
			junzhi=0;//清零
			MeasureValue=0;//测量点处中心像素值清零
			writePoint.x=0;//清零
			writePoint.y=0;//清零
			
			  for(  i = MeasurePoint-60; i < MeasurePoint+41; i+=10)//标注测量点
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Left; j < m_Right; j++)
				  {
					  
					  
					  if( Bin_Image[i][j] != 0)
					  {
						  if( flags == 0)
						  {   
							  first = j;
							  flags = 1;
							  last = j;
						  }

						  last = j;
					  }
				  }

				  if (i==MeasurePoint)
				  {
					  MeasureValue=last-first;
				  }
				  
				  if ((last-first)!=0)
				  {
					  pingjun[junzhi]=last-first;
					  junzhi++;
					  
				  }

				 if(i==MeasurePoint)
				  {
					 cvLine(pFrame,cvPoint(first-7,i),cvPoint(first+7,i),CV_RGB(0,255,0),1,8,0);//起点横线
					 cvLine(pFrame,cvPoint(last-7,i),cvPoint(last+7,i),CV_RGB(0,255,0),1,8,0);//终点横线
					 cvLine(pFrame,cvPoint(first,i+7),cvPoint(first,i-7),CV_RGB(0,255,0),1,8,0);//起点竖线
					 cvLine(pFrame,cvPoint(last,i+7),cvPoint(last,i-7),CV_RGB(0,255,0),1,8,0);//终点竖线
					 cvLine(pFrame,cvPoint(first-65,i+8),cvPoint(last+30,i+8),CV_RGB(255,0,0),1,8,0);
					 cvLine(pFrame,cvPoint(first-65,i-8),cvPoint(last+30,i-8),CV_RGB(255,0,0),1,8,0);
					 cvLine(pFrame,cvPoint(first-65,i+8),cvPoint(first-65,i-8),CV_RGB(255,0,0),1,8,0);
					 cvLine(pFrame,cvPoint(last+30,i+8),cvPoint(last+30,i-8),CV_RGB(255,0,0),1,8,0);
					 //下面是加粗扫描线
					 cvLine(pFrame,cvPoint(first-7,i+1),cvPoint(first+7,i+1),CV_RGB(0,255,0),1,8,0);//起点横线
					 cvLine(pFrame,cvPoint(last-7,i+1),cvPoint(last+7,i+1),CV_RGB(0,255,0),1,8,0);//终点横线
					 cvLine(pFrame,cvPoint(first+1,i+7),cvPoint(first+1,i-7),CV_RGB(0,255,0),1,8,0);//起点竖线
					 cvLine(pFrame,cvPoint(last+1,i+7),cvPoint(last+1,i-7),CV_RGB(0,255,0),1,8,0);//终点竖线
					 cvLine(pFrame,cvPoint(first-65,i+9),cvPoint(last+30,i+9),CV_RGB(255,0,0),1,8,0);
					 cvLine(pFrame,cvPoint(first-65,i-7),cvPoint(last+30,i-7),CV_RGB(255,0,0),1,8,0);
					 cvLine(pFrame,cvPoint(first-64,i+8),cvPoint(first-64,i-8),CV_RGB(255,0,0),1,8,0);
					 cvLine(pFrame,cvPoint(last+31,i+8),cvPoint(last+31,i-8),CV_RGB(255,0,0),1,8,0);
					 
					 writePoint.x=first-50;
				  writePoint.y=i-7;
				 }  			  
			  }
			  if(junzhi!=0)
			  {
			  junzhi=(pingjun[0]+pingjun[1]+pingjun[2]+pingjun[3]+pingjun[4]+pingjun[5]+pingjun[6]+pingjun[7]+pingjun[8]+pingjun[9]+pingjun[10]+pingjun[11]+pingjun[12])/junzhi;
			  itoa(last-first,Diadata,10);
			  cvPutText( pFrame, Diadata,writePoint, &Font1, CV_RGB(255,0,0));//cvPoint(first-50,i-7)				 
			  memcpy(m_lpbuff,(BYTE*)(pFrame->imageData),(pFrame->width)*(pFrame->height)*3);
			  showpic();
			  }
			  
			  fpint=fopen("deng.txt","aw+");
			  fprintf(fpint,"%d\n",junzhi);
			  fclose(fpint);

		}
////////////////////////////////等径部分，结束////////////////////////////////////////////////
///////////////////////////////引晶开始///////////////////////////////////////////////////////
if ((is_Guidedia>0)||(AutoChangeModeFlag==2))//当引晶按钮后才开始画线、测量
{
	AutoChangeModeFlag=2;
	if (is_Guidedia>0)
	{
		yinfangdengflag=2;
	}
	if(factorChangeFlag==TRUE)
		UpdateData(0);//为了更新阈值文本框，而又不影响factorChangeFlag对update的控制，故只更新一次
			  
			  YJAve_w=Bottom_point.y;
	 if(AutoORManualFlag==FALSE) {//扫描线自动调整

		 for(  i = (Bottom_point.y); i <Top; i++)//寻找左边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Left; j < Bottom_point.x; j++)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 if (j<LeftMaxValue)
					 {
						 LeftMaxValue=j;
						 LeftPointX=i;
					 }
					 break;
				 }
			 }
			 
		 }
		 
		 
		 for(  i = (Bottom_point.y); i <Top; i++)//寻找右边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Right; j > Bottom_point.x; j--)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 if (j>RightMaxValue)
					 {
						 RightMaxValue=j;
						 RightPointX=i;
					 }
					 break;
				 }
			 }
			 
		 }
		MeasurePoint=YJAve_w+YJAve_w1;//找到距离底边距离20个像素的相对固定位置

	}
	 else //扫描线手动调节
	 {
		 for(  i = (Bottom_point.y); i <MeasurePoint; i++)//寻找左边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Left; j < Bottom_point.x; j++)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 
					 break;
				 }
			 }
			 
		 }
		 
		 
		 for(  i = (Bottom_point.y); i <MeasurePoint; i++)//寻找右边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Right; j > Bottom_point.x; j--)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 
					 break;
				 }
			 }
			 
			  }

	 }

	 pingjun[0]=pingjun[1]=pingjun[2]=pingjun[3]=pingjun[4]=0;
	 junzhi=0;//清零

			  for(  i = MeasurePoint-2; i < MeasurePoint+3; i++)//标注测量点
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Left; j < m_Right; j++)
				  {
					  
					  
					  if( Back_Image[i][j] != 0)
					  {
						  if( flags == 0)
						  {   
							  first = j;
							  flags = 1;
							  last = j;
						  }

						  last = j;
					  }
				  }

				  if ((last-first)!=0)
				  {
					  pingjun[junzhi]=last-first;
					  junzhi++;
					  
				  }

				  if(i==MeasurePoint)
				  {
					  cvLine(pFrame,cvPoint(first-5,i),cvPoint(first+5,i),CV_RGB(0,255,0),1,8,0);//起点横线
					  cvLine(pFrame,cvPoint(last-5,i),cvPoint(last+5,i),CV_RGB(0,255,0),1,8,0);//终点横线
					  cvLine(pFrame,cvPoint(first,i+5),cvPoint(first,i-5),CV_RGB(0,255,0),1,8,0);//起点竖线
					  cvLine(pFrame,cvPoint(last,i+5),cvPoint(last,i-5),CV_RGB(0,255,0),1,8,0);//终点竖线
					  //下面加粗
					  cvLine(pFrame,cvPoint(first-4,i),cvPoint(first+6,i),CV_RGB(0,255,0),1,8,0);//起点横线
					  cvLine(pFrame,cvPoint(last-4,i),cvPoint(last+6,i),CV_RGB(0,255,0),1,8,0);//终点横线
					  cvLine(pFrame,cvPoint(first,i+6),cvPoint(first,i-4),CV_RGB(0,255,0),1,8,0);//起点竖线
					  cvLine(pFrame,cvPoint(last,i+6),cvPoint(last,i-4),CV_RGB(0,255,0),1,8,0);//终点竖线
				  }
				  
			  } 
			  if(junzhi!=0)
			  {
			  junzhi=(pingjun[0]+pingjun[1]+pingjun[2]+pingjun[3]+pingjun[4])/junzhi;
			  itoa(junzhi,Diadata,10);//把一整数转换为字符串,十进制方式
			  cvPutText( pFrame, Diadata, cvPoint(first-50,i-7), &Font1, CV_RGB(255,0,0));				 
			  memcpy(m_lpbuff,(BYTE*)(pFrame->imageData),(pFrame->width)*(pFrame->height)*3);
			  showpic();
			  }
			  
			  fpint=fopen("yin.txt","aw+");
			  fprintf(fpint,"%d\n",junzhi);
			  fclose(fpint);
}
//////////////////////////////引晶结束////////////////////////////////////////////////////////
//////////////////////////////放肩部分////////////////////////////////////////////////////////
	if ((is_fangjiandia>0)||(AutoChangeModeFlag==3))//放肩部分
	{  
//		m_edit1 = 1;
//		m_edit2 = 1;
//		m_edit3 = 0;
//		m_edit4 = 1;
		if(factorChangeFlag==TRUE)
			UpdateData(0);//为了更新阈值文本框，而又不影响factorChangeFlag对update的控制，故只更新一次

			AutoChangeModeFlag=3;
			if (is_fangjiandia>0)
			{
				yinfangdengflag=3;
			}
			if ((flagAutoBottomPoint_f==0)&&(Bottom_point.y!=0))//找到第一次最低点，保存
			  {
				  flagAutoBottomPoint_f=1;
				  Ave_w=Bottom_point.y;
			  }

	 if(AutoORManualFlag==FALSE) {//扫描线自动调整
			  for(  i = (Bottom_point.y); i <Top; i++)//寻找左边缘轮廓
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Left; j < Bottom_point.x; j++)
				  {
					  
					  
					  if( Bin_Image[i][j] != 0)
					  {
						 Back_Image[i][j] = 255;
						 if (j<LeftMaxValue)
						 {
							 LeftMaxValue=j;
							 LeftPointX=i;
						 }
						 break;
					  }
				  }
				  
			  }

			  
			  for(  i = (Bottom_point.y); i <Top; i++)//寻找右边缘轮廓
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Right; j > Bottom_point.x; j--)
				  {
					  
					  
					  if( Bin_Image[i][j] != 0)
					  {
						  Back_Image[i][j] = 255;
						  if (j>RightMaxValue)
						  {
							  RightMaxValue=j;
							  RightPointX=i;
						  }
						  break;
					  }
				  }
				  
			  }
		AutoMaxRadius=(int)((LeftPointX+RightPointX)/2);//自动检测的测量点的最佳位置
		MeasurePoint=AutoMaxRadius;
		if ((flagAutoMaxRadius==0)&&(MeasurePoint!=0))//找到了第一次直径处的纵坐标，保存
		{
			flagAutoMaxRadius=1;
			Ave_r=MeasurePoint;
		}
		if ((flagAutoMaxRadius==1)&&(flagAutoBottomPoint_f==1))
		{
			MeasurePoint=(int)((Ave_r-Ave_w)/2)+Ave_w+35;
		}

	}
	 else //扫描线手动调节
	 {
		 for(  i = (Bottom_point.y); i <MeasurePoint; i++)//寻找左边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Left; j < Bottom_point.x; j++)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 
					 break;
				 }
			 }
			 
		 }
		 
		 
		 for(  i = (Bottom_point.y); i <MeasurePoint; i++)//寻找右边缘轮廓
		 {
			 flags = 0;
			 first = 0;
			 last =  0;
			 for(  j = m_Right; j > Bottom_point.x; j--)
			 {
				 
				 
				 if( Bin_Image[i][j] != 0)
				 {
					 Back_Image[i][j] = 255;
					 
					 break;
				 }
			 }
			 
			  }

	 }

	 for(junzhi=0;junzhi<15;junzhi++)//清零
		 pingjun[junzhi]=0;
	 junzhi=0;//清零
	 MeasureValue=0;//清零
	 writePoint.x=0;//清零
		writePoint.y=0;//清零
       
			  for(  i = MeasurePoint-60; i < MeasurePoint+41; i+=10)//标注测量点
			  {
				  flags = 0;
				  first = 0;
				  last =  0;
				  for(  j = m_Left; j < m_Right; j++)
				  {
					  
					  
					  if( Back_Image[i][j] != 0)
					  {
						  if( flags == 0)
						  {   
							  first = j;
							  flags = 1;
							  last = j;
						  }

						  last = j;
					  }
				  }
				  if (i==MeasurePoint)
				  {
					  MeasureValue=last-first;
				  }
				  
				  if ((last-first)!=0)
				  {
					  pingjun[junzhi]=last-first;
					  junzhi++;					  
				  }
				  
				  //	  cvLine(pFrame,cvPoint(first,i),cvPoint(last,i),CV_RGB(0,0,255),1,8,0);
				  
				  if(i ==MeasurePoint)
				  {
					  cvLine(pFrame,cvPoint(first-7,i),cvPoint(first+7,i),CV_RGB(0,255,0),1,8,0);//起点横线
					  cvLine(pFrame,cvPoint(last-7,i),cvPoint(last+7,i),CV_RGB(0,255,0),1,8,0);//终点横线
					  cvLine(pFrame,cvPoint(first,i+7),cvPoint(first,i-7),CV_RGB(0,255,0),1,8,0);//起点竖线
					  cvLine(pFrame,cvPoint(last,i+7),cvPoint(last,i-7),CV_RGB(0,255,0),1,8,0);//终点竖线
					  cvLine(pFrame,cvPoint(first-65,i+8),cvPoint(last+30,i+8),CV_RGB(255,0,0),1,8,0);
					  cvLine(pFrame,cvPoint(first-65,i-8),cvPoint(last+30,i-8),CV_RGB(255,0,0),1,8,0);
					  cvLine(pFrame,cvPoint(first-65,i+8),cvPoint(first-65,i-8),CV_RGB(255,0,0),1,8,0);
					  cvLine(pFrame,cvPoint(last+30,i+8),cvPoint(last+30,i-8),CV_RGB(255,0,0),1,8,0);
					  
					  //下面是加粗扫描线
					  cvLine(pFrame,cvPoint(first-7,i+1),cvPoint(first+7,i+1),CV_RGB(0,255,0),1,8,0);//起点横线
					  cvLine(pFrame,cvPoint(last-7,i+1),cvPoint(last+7,i+1),CV_RGB(0,255,0),1,8,0);//终点横线
					  cvLine(pFrame,cvPoint(first+1,i+7),cvPoint(first+1,i-7),CV_RGB(0,255,0),1,8,0);//起点竖线
					  cvLine(pFrame,cvPoint(last+1,i+7),cvPoint(last+1,i-7),CV_RGB(0,255,0),1,8,0);//终点竖线
					  cvLine(pFrame,cvPoint(first-65,i+9),cvPoint(last+30,i+9),CV_RGB(255,0,0),1,8,0);
					  cvLine(pFrame,cvPoint(first-65,i-7),cvPoint(last+30,i-7),CV_RGB(255,0,0),1,8,0);
					  cvLine(pFrame,cvPoint(first-64,i+8),cvPoint(first-64,i-8),CV_RGB(255,0,0),1,8,0);
					  cvLine(pFrame,cvPoint(last+31,i+8),cvPoint(last+31,i-8),CV_RGB(255,0,0),1,8,0);
					  
					  writePoint.x=first-50;
				      writePoint.y=i-7;
				  }
				  
			  }				  

			  if(junzhi!=0)
			  {
			  junzhi=(pingjun[0]+pingjun[1]+pingjun[2]+pingjun[3]+pingjun[4]+pingjun[5]+pingjun[6]+pingjun[7]+pingjun[8]+pingjun[9]+pingjun[10]+pingjun[11]+pingjun[12])/junzhi;
			  itoa(MeasureValue,Diadata,10);//把一整数转换为字符串,十进制方式
			  cvPutText( pFrame, Diadata, writePoint, &Font1, CV_RGB(255,0,0));				 
			  memcpy(m_lpbuff,(BYTE*)(pFrame->imageData),(pFrame->width)*(pFrame->height)*3);
			  showpic();
			  }
			  
			  fpint=fopen("fang.txt","aw+");
			  fprintf(fpint,"%d\n",junzhi);
			fclose(fpint);
		}
//////////////////////////////放肩部分结束//////////////////////////////////////////////////////
////////////////////////////////比例因子，权值修改部分////////////////////////////////////////
if (is_Guidedia==TRUE)//三种模式切换时，保存各自的权值，当模式切换时换回原来是用的权值
{
	m_edit1=m_edit1save_y;
	m_edit2=m_edit2save_y;
	m_edit3=m_edit3save_y;
//	m_edit4=m_edit4save_y;
//	m_edit5=m_edit5save_y;
//	m_edit7=m_edit7save_y;
} 
else if(is_fangjiandia==TRUE)
{
	m_edit1=m_edit1save_f;
	m_edit2=m_edit2save_f;
	m_edit3=m_edit3save_f;
//	m_edit4=m_edit4save_f;
//	m_edit5=m_edit5save_f;
//	m_edit7=m_edit7save_f;
}
else if(is_Equaldia==TRUE)
{
	m_edit1=m_edit1save_d;
	m_edit2=m_edit2save_d;
	m_edit3=m_edit3save_d;
//	m_edit4=m_edit4save_d;
//	m_edit5=m_edit5save_d;
//	m_edit7=m_edit7save_d;
}

//		strForFactor.Format(_T("%lf"),m_edit1);
//		GetDlgItem(IDC_EDIT1)->SetWindowText(_T(strForFactor));
//		strForFactor.Format(_T("%lf"),m_edit2);
//		GetDlgItem(IDC_EDIT2)->SetWindowText(_T(strForFactor));
//		strForFactor.Format(_T("%lf"),m_edit3);
//		GetDlgItem(IDC_EDIT3)->SetWindowText(_T(strForFactor));
//		strForFactor.Format(_T("%lf"),m_edit4);
//		GetDlgItem(IDC_EDIT4)->SetWindowText(_T(strForFactor));
////////////////////////////////全职修改部分，结束/////////////////////////////////////////////
//					  real_nFrmNum++;

					  if (m_dwCount % 50 != 0)
					  {
						  Dia[m_dwCount%50] = junzhi;					  
					  }
					  
					  
					  if (m_dwCount % 50 == 0)
					  {
						  
						  diameter = R_Average(Dia,50);//半径求平均值
						  if(factorChangeFlag==TRUE)
						  UpdateData();
						  m_Diamet = diameter*multiple;
						  
						  m_Diamet=((m_Diamet*m_edit1/(m_edit1+m_edit2+m_edit3))+(m_DiametOld*m_edit2/(m_edit1+m_edit2+m_edit3)))+(m_DiametSecondOld*m_edit3/(m_edit1+m_edit2+m_edit3));//求个均值，减小测量误差
						  m_DiametSecondOld=m_DiametOld;
						  m_DiametOld=m_Diamet;
						  
						  
						  strCDit6.Format(_T("%.2f"),m_Diamet);
						  SetDlgItemText(IDC_EDIT6,strCDit6);
						  if(factorChangeFlag==TRUE)
						  UpdateData(0);

						  if(m_Diamet!=0)//零是没有意义的
						  {
							  fpdouble=fopen("Diamet.txt","aw+");//保存直径值
							  fprintf(fpdouble,"%f\n",m_Diamet);
							  fclose(fpdouble);
						  }						  
					  }//第二部分公共部分，完毕
					  if (diameter>0)//读取50帧之前，不允许输入数据
					  { 
						  GetDlgItem(IDC_CHANGERADIUS)->EnableWindow(1);
					  } 
					  else
					  {
						  GetDlgItem(IDC_CHANGERADIUS)->EnableWindow(0);
					  }
	}//end of 按下引晶或者等径,或者放肩

	

	    cvReleaseImage(&pThreshImg);
        cvReleaseImage(&pImg); 
        cvReleaseImage(&pback);

	
}



void CVideoCaputureDlg::OnYinjing() 
{
	// TODO: Add your control notification handler code here
	is_Equaldia = false;
	is_Guidedia = true;
	is_fangjiandia=false;
	
}

void CVideoCaputureDlg::OnFangjian() 
{
	// TODO: Add your control notification handler code here
	is_Equaldia = false;
	is_Guidedia = false;
	is_fangjiandia=true;
	
}

void CVideoCaputureDlg::OnAutoMeasureDown() 
{
	// TODO: Add your control notification handler code here
	AutoORManualFlag=TRUE;
	if(is_Equaldia==true)
	{	
	MeasurePoint-=5;
	}
	else if(is_Guidedia==true)
	{
	MeasurePoint-=1;
	}
	else if(is_fangjiandia==true)
	{
		MeasurePoint-=5;//YJAve_w1

	}
}

void CVideoCaputureDlg::OnAutoMeasureReset() 
{
	// TODO: Add your control notification handler code here
	AutoORManualFlag=FALSE;
}

void CVideoCaputureDlg::OnConfigport() 
{
	// TODO: Add your control notification handler code here
	CMeasureDiamete PortConfigaration;
	UpdateData();
	CVideoCaputureApp *app = (CVideoCaputureApp *)AfxGetApp(); //生成指向应用程序类的指针
	if(PortConfigaration.DoModal() == IDOK)
	{	
		m_DCom=app->m_nCom;
		m_DStopbits=app->m_nStopbits;
		m_DParity=app->m_cParity;
		m_DDatabits=app->m_nDatabits;
	    m_DBaud=app->m_nBaud;
		UpdateData(TRUE);
	}
	
}

void CVideoCaputureDlg::OnOpencloseport() 
{
	// TODO: Add your control notification handler code here

	char buff[2];
	buff[1]='\0';
	buff[0]=m_DParity;
	string1.Format(_T("%d"),m_DBaud);
	string1+=",";
	string2=buff;
	string1+=string2;
	string1+=",";
	string2.Format(_T("%d"),m_DDatabits); 
	string1+=string2;
	string1+=",";
	string2.Format(_T("%d"),m_DStopbits);
	string1+=string2;
/*
	CString   tmp;
	tmp.Format( "%d ",string1);
	MessageBox( "config:"+string1);
*/
	if(SerialPortOpenCloseFlag==FALSE)
	{
		SerialPortOpenCloseFlag=TRUE;

		//以下是串口的初始化配置
		if(m_comm.GetPortOpen())//打开端口前的检测，先关，再开
			MessageBox("can not open serial port");
//			m_comm.SetPortOpen(FALSE);	//	
		m_comm.SetCommPort(m_DCom); //选择端口，默认是com1
		m_comm.SetSettings((LPSTR)(LPCTSTR)string1); //波特率9600，无校验，8个数据位，1个停止位
		if(!m_comm.GetPortOpen())
		{			
			m_comm.SetPortOpen(TRUE);//打开串口
			GetDlgItem(IDC_OPENCLOSEPORT)->SetWindowText("关闭串口");
			m_ctrlIconOpenoff.SetIcon(m_hIconRed);
		}
		else
			MessageBox("can not open serial port");	 
	}
	else
	{
		SerialPortOpenCloseFlag=FALSE;
		GetDlgItem(IDC_OPENCLOSEPORT)->SetWindowText("打开串口");
			m_ctrlIconOpenoff.SetIcon(m_hIconOff);
		m_comm.SetPortOpen(FALSE);//关闭串口
	}
}

void CVideoCaputureDlg::OnComm()
{
	// TODO: Add your control notification handler code here
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len,k;
	BYTE rxdata[10]; //设置BYTE数组
	if((m_comm.GetCommEvent()==2)&&(ComPortReadflag==1)) //事件值为2表示接收缓冲区内有字符
	{//ComPortReadflag==1,是判断在自动调整模式下
		variant_inp=m_comm.GetInput(); //读缓冲区
		safearray_inp=variant_inp;  //VARIANT型变量转换为ColeSafeArray型变量
		len=safearray_inp.GetOneDimSize(); //得到有效数据长度
		if(len==1)
		{
			k=0;
			safearray_inp.GetElement(&k,rxdata+k);//转换为BYTE型数组
            
			//下面将数组转换为Cstring型变量			
				BYTE bt=*(char*)(rxdata+0);    //字符型
				k=bt;
				if (k==1)
				{
					AutoChangeModeFlag=1;
				} 
				else if(k==2)
				{
					AutoChangeModeFlag=2;
				}
				else if(k==3)
				{
					AutoChangeModeFlag=3;
				}
				else AutoChangeModeFlag=0;

		}
//				strtemp.Format("%c",bt);    //将字符送入临时变量strtemp存放
//				m_strEditRXData+=strtemp;  //加入接收编辑框对应字符串    
		else 
		{
			AfxMessageBox("从串口得到的数据不正确!",MB_OK,0);
			AutoChangeModeFlag=0;
		}
	}
	if(factorChangeFlag==TRUE)
	UpdateData(FALSE);  //更新编辑框内容
}

void CVideoCaputureDlg::OnAutoChangeMode() 
{
	// TODO: Add your control notification handler code here
	if(autoChangeModeflag==FALSE)
	{
		autoChangeModeflag=TRUE;
	GetDlgItem(IDC_Yinjing)->EnableWindow(0);
	GetDlgItem(IDC_Fangjian)->EnableWindow(0);
	GetDlgItem(IDC_Dengjing)->EnableWindow(0);
//	yinfangdengflag=true;
	is_Equaldia=FALSE;//关闭三个模式的全部使能
	is_fangjiandia=FALSE;
	is_Guidedia=FALSE;
	ComPortReadflag=1;
	GetDlgItem(IDC_AutoChangeMode)->SetWindowText("切换手动模式");
	}
	else
	{
		autoChangeModeflag=FALSE;
		
		GetDlgItem(IDC_Yinjing)->EnableWindow(1);
		GetDlgItem(IDC_Fangjian)->EnableWindow(1);
		GetDlgItem(IDC_Dengjing)->EnableWindow(1);
	    ComPortReadflag=0;//不在从串口读入数据
		GetDlgItem(IDC_AutoChangeMode)->SetWindowText("切换自动模式");
		//下面是还原原来的测量方式
		if (AutoChangeModeFlag==1)
		{
			is_Equaldia=TRUE;
			is_fangjiandia=FALSE;
			is_Guidedia=FALSE;
		} 
		else if(AutoChangeModeFlag==2)
		{
			is_Equaldia=FALSE;
			is_fangjiandia=FALSE;
			is_Guidedia=TRUE;
		}
		else if (AutoChangeModeFlag==3)
		{
			is_Equaldia=FALSE;
			is_fangjiandia=TRUE;
			is_Guidedia=FALSE;
		}


		else AutoChangeModeFlag=0;//不允许自动调整测量方式
	}
}

void CVideoCaputureDlg::OnBTNquanDefult() 
{
	// TODO: Add your control notification handler code here
	

	if (is_Guidedia==TRUE)//保存最新更改的权值
	{
		m_edit1=1;
		m_edit2=quanzhier;
		m_edit3=0;
//		m_edit4=1;
		GetDlgItem(IDC_EDIT1)->SetWindowText(_T("1"));
		GetDlgItem(IDC_EDIT2)->SetWindowText(_T("3"));
		GetDlgItem(IDC_EDIT3)->SetWindowText(_T("0"));
//	GetDlgItem(IDC_EDIT4)->SetWindowText(_T("1"));
		m_edit1save_y=m_edit1;
		m_edit2save_y=m_edit2;
		m_edit3save_y=m_edit3;
//		m_edit4save_y=m_edit4;
	} 
	else if(is_fangjiandia==TRUE)
	{
		m_edit1=1;
		m_edit2=1;
		m_edit3=0;
//		m_edit4=1;
		GetDlgItem(IDC_EDIT1)->SetWindowText(_T("1"));
		GetDlgItem(IDC_EDIT2)->SetWindowText(_T("1"));
		GetDlgItem(IDC_EDIT3)->SetWindowText(_T("0"));
//	GetDlgItem(IDC_EDIT4)->SetWindowText(_T("1"));

		m_edit1save_f=m_edit1;
		m_edit2save_f=m_edit2;
		m_edit3save_f=m_edit3;
//		m_edit4save_f=m_edit4;
		
	}
	else if (is_Equaldia==TRUE)
	{
		m_edit1=1;
		m_edit2=quanzhier;
		m_edit3=0;
//		m_edit4=1;
		GetDlgItem(IDC_EDIT1)->SetWindowText(_T("1"));
		GetDlgItem(IDC_EDIT2)->SetWindowText(_T("3"));
		GetDlgItem(IDC_EDIT3)->SetWindowText(_T("0"));
//	GetDlgItem(IDC_EDIT4)->SetWindowText(_T("1"));
		m_edit1save_d=m_edit1;
		m_edit2save_d=m_edit2;
		m_edit3save_d=m_edit3;
//		m_edit4save_d=m_edit4;
	}

	//	UpdateData();
}

void CVideoCaputureDlg::OnThreshold() 
{
	// TODO: Add your control notification handler code here
	
	if(Thresholdonoffflag==FALSE)
	{
		Thresholdflag=TRUE;//measure中开始显示二值化结果
		Thresholdonoffflag=TRUE;
		GetDlgItem(IDC_Threshold)->SetWindowText("停止测试");
	}
	else
	{
		Thresholdflag=FALSE;//measure中停止显示二值化结果
		Thresholdonoffflag=FALSE;
		GetDlgItem(IDC_Threshold)->SetWindowText("二值化测试");
	}
}

//DEL void CVideoCaputureDlg::OnButton2() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	factorChangeFlag=FALSE;//停止刷新编辑框
//DEL 	GetDlgItem(IDC_EDIT5)->EnableWindow(1);//权值输入框打开
//DEL 	GetDlgItem(IDC_EDIT7)->EnableWindow(1);
//DEL 	GetDlgItem(IDC_BUTTON4)->EnableWindow(1);
//DEL 	
//DEL }

//DEL void CVideoCaputureDlg::OnButton4() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	factorChangeFlag=TRUE;
//DEL 	useChangedyuzhiflag=TRUE;
//DEL 	GetDlgItem(IDC_EDIT5)->EnableWindow(0);//关闭阈值输入编辑框
//DEL 	GetDlgItem(IDC_EDIT7)->EnableWindow(0);
//DEL 	GetDlgItem(IDC_BUTTON4)->EnableWindow(0);
//DEL //	if(factorChangeFlag==TRUE)
//DEL 		UpdateData();
//DEL 		UpdateData(FALSE);
//DEL 		if (m_edit5>=m_edit7)
//DEL 		{
//DEL 			AfxMessageBox("无效的输入!\n第一个阈值要小于第二个阈值。",MB_OK,0);
//DEL 			if (is_Guidedia==TRUE)
//DEL 			{
//DEL 				m_edit5=m_edit5save_y;
//DEL 				m_edit7=m_edit7save_y;
//DEL 			} 
//DEL 			else if(is_fangjiandia==TRUE)
//DEL 			{
//DEL 				m_edit5=m_edit5save_f;
//DEL 				m_edit7=m_edit7save_f;		
//DEL 			}
//DEL 			else if (is_Equaldia==TRUE)
//DEL 			{
//DEL 				m_edit5=m_edit5save_d;
//DEL 				m_edit7=m_edit7save_d;
//DEL 			}
//DEL 			
//DEL 			strForFactor.Format(_T("%lf"),m_edit5);
//DEL 			GetDlgItem(IDC_EDIT5)->SetWindowText(strForFactor);
//DEL 			strForFactor.Format(_T("%lf"),m_edit7);
//DEL 			GetDlgItem(IDC_EDIT7)->SetWindowText(strForFactor);
//DEL 		}
//DEL 		else 
//DEL 		{			
//DEL 			if (is_Guidedia==TRUE)//把编辑框中正确的数据，保存起来
//DEL 			{
//DEL 				m_edit5save_y=m_edit5;
//DEL 				m_edit7save_y=m_edit7;
//DEL 			} 
//DEL 			else if(is_fangjiandia==TRUE)
//DEL 			{
//DEL 				m_edit5save_f=m_edit5;
//DEL 				m_edit7save_f=m_edit7;		
//DEL 			}
//DEL 			else if (is_Equaldia==TRUE)
//DEL 			{
//DEL 				m_edit5save_d=m_edit5;
//DEL 				m_edit7save_d=m_edit7;
//DEL 			}
//DEL 		}
//DEL }

//DEL void CVideoCaputureDlg::OnButton3() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	if (is_Guidedia==TRUE)//把编辑框中正确的数据，保存起来
//DEL 	{
//DEL 		m_edit5save_y=180;
//DEL 		m_edit7save_y=255;
//DEL 		GetDlgItem(IDC_EDIT5)->SetWindowText(_T("180"));
//DEL 	    GetDlgItem(IDC_EDIT7)->SetWindowText(_T("255"));
//DEL 	} 
//DEL 	else if(is_fangjiandia==TRUE)
//DEL 	{
//DEL 		m_edit5save_f=120;
//DEL 		m_edit7save_f=255;
//DEL 		GetDlgItem(IDC_EDIT5)->SetWindowText(_T("120"));
//DEL 	    GetDlgItem(IDC_EDIT7)->SetWindowText(_T("255"));
//DEL 	}
//DEL 	else if (is_Equaldia==TRUE)
//DEL 	{
//DEL 		m_edit5save_d=230;
//DEL 		m_edit7save_d=255;
//DEL 		GetDlgItem(IDC_EDIT5)->SetWindowText(_T("230"));
//DEL 	    GetDlgItem(IDC_EDIT7)->SetWindowText(_T("255"));
//DEL 	}
//DEL }

void CVideoCaputureDlg::OnReadConfig() 
{
	// TODO: Add your control notification handler code here
	CString strBufferReadConfig,strtmpReadConfig;
	
	GetPrivateProfileString("ConfigInfo","m_edit1","1",strBufferReadConfig.GetBuffer(MAX_PATH),MAX_PATH,".\\Config.ini");
	strBufferReadConfig.ReleaseBuffer();
	strtmpReadConfig+=strBufferReadConfig;
	m_edit1= (float)atof((char *)(LPTSTR)(LPCTSTR)strBufferReadConfig);

	GetPrivateProfileString("ConfigInfo","m_edit2","3",strBufferReadConfig.GetBuffer(MAX_PATH),MAX_PATH,".\\Config.ini");
	strBufferReadConfig.ReleaseBuffer();
	strtmpReadConfig+=","+strBufferReadConfig;
	m_edit2= (float)atof((char *)(LPTSTR)(LPCTSTR)strBufferReadConfig);

	GetPrivateProfileString("ConfigInfo","m_edit3","0",strBufferReadConfig.GetBuffer(MAX_PATH),MAX_PATH,".\\Config.ini");
	strBufferReadConfig.ReleaseBuffer();
	strtmpReadConfig+=","+strBufferReadConfig;
	m_edit3= (float)atof((char *)(LPTSTR)(LPCTSTR)strBufferReadConfig);

//	GetPrivateProfileString("ConfigInfo","m_edit4","1",strBufferReadConfig.GetBuffer(MAX_PATH),MAX_PATH,".\\Config.ini");
//	strBufferReadConfig.ReleaseBuffer();
//	strtmpReadConfig+=","+strBufferReadConfig;
//	m_edit4= (float)atof((char *)(LPTSTR)(LPCTSTR)strBufferReadConfig);

//	GetPrivateProfileString("ConfigInfo","m_edit5","230",strBufferReadConfig.GetBuffer(MAX_PATH),MAX_PATH,".\\Config.ini");
//	strBufferReadConfig.ReleaseBuffer();
//	strtmpReadConfig+=","+strBufferReadConfig;
//	m_edit5= (float)atof((char *)(LPTSTR)(LPCTSTR)strBufferReadConfig);
//	GetPrivateProfileString("ConfigInfo","m_edit7","255",strBufferReadConfig.GetBuffer(MAX_PATH),MAX_PATH,".\\Config.ini");
//	strBufferReadConfig.ReleaseBuffer();
//	strtmpReadConfig+=","+strBufferReadConfig;
//	m_edit7= (float)atof((char *)(LPTSTR)(LPCTSTR)strBufferReadConfig);

	AfxMessageBox("配置参数为：\n"+strtmpReadConfig,MB_OK,0);
}

void CVideoCaputureDlg::OnSaveConfig() 
{
	// TODO: Add your control notification handler code here
	CString StrDestFile=".\\Config.ini";//保存配置之前先要删除之前的ini文件，貌似不像txt那样可以清除之前的内容
	if(::DeleteFile(StrDestFile))
		AfxMessageBox("配置保存成功",MB_OK,0);//如果可以删除，那么一定可以成功保存配置
	CString strTemp;
	
	strTemp.Format(_T("%.0lf"),m_edit1);
	::WritePrivateProfileString("ConfigInfo","m_edit1",strTemp,".\\Config.ini");
	strTemp.Format(_T("%.0lf"),m_edit2);
	::WritePrivateProfileString("ConfigInfo","m_edit2",strTemp,".\\Config.ini");
	strTemp.Format(_T("%.0lf"),m_edit3);
	::WritePrivateProfileString("ConfigInfo","m_edit3",strTemp,".\\Config.ini");
//	strTemp.Format(_T("%.0lf"),m_edit4);
//	::WritePrivateProfileString("ConfigInfo","m_edit4",strTemp,".\\Config.ini");
//	strTemp.Format(_T("%.0lf"),m_edit5);
//	::WritePrivateProfileString("ConfigInfo","m_edit5",strTemp,".\\Config.ini");
//	strTemp.Format(_T("%.0lf"),m_edit7);
//	::WritePrivateProfileString("ConfigInfo","m_edit7",strTemp,".\\Config.ini");
}

//DEL void CVideoCaputureDlg::OnSoftKeyboard() //软键盘
//DEL {
//DEL 	HWND hWnd1;
//DEL 	// TODO: Add your control notification handler code here
//DEL 	if (softkeyboardFlag==FALSE)
//DEL 	{
//DEL 		softkeyboardFlag=TRUE;
//DEL 		GetDlgItem(IDC_SoftKeyboard)->SetWindowText("关闭软键盘");
//DEL 
//DEL 		ShellExecute(GetSafeHwnd(), NULL, _T("osk.exe"), NULL, NULL,SW_NORMAL);
//DEL 		hWnd1 = ::FindWindow(NULL, _T("屏幕键盘"));
//DEL //		CWnd *pWnd1;
//DEL //		pWnd1->Attach(hWnd1);
//DEL //		pWnd1->MoveWindow( CRect(0,0,100,100) );
//DEL 	//	::SetWindowPos(hWnd,HWND_TOPMOST,0,0,200,100,SWP_NOSIZE);
//DEL 	} 
//DEL 	else
//DEL 	{
//DEL 		softkeyboardFlag=FALSE;
//DEL 		GetDlgItem(IDC_SoftKeyboard)->SetWindowText("打开软键盘");
//DEL 
//DEL 			 hWnd1 = ::FindWindow(NULL, _T("屏幕键盘"));
//DEL 			if (NULL != hWnd1) 
//DEL 			{
//DEL 				::SendMessage(hWnd1, WM_CLOSE, 0, 0);
//DEL 			}
//DEL 	}
//DEL }

void CVideoCaputureDlg::ThresholdTest(IplImage* threshould)
{
  if (Thresholdflag==TRUE)//用于观看二值化结果，以便选择阈值
  {
	  cvNamedWindow("canny_text",1);//如果已经存在这个名字的窗口，这个函数将不做任何事情。//需要的就是这条重要的特性
	  cvShowImage("canny_text", threshould);
  }
  else if(Thresholdflag==FALSE)
  {
	  //cvDestroyAllWindows();
  }
}

void CVideoCaputureDlg::OnSetfocusQuanzhi1() 
{
	// TODO: Add your control notification handler code here
//	CString  buf;
	CSoftKeyboard softkeyboard;
	if(factorChangeFlag==TRUE)
		UpdateData();
	CVideoCaputureApp *app = (CVideoCaputureApp *)AfxGetApp(); //生成指向应用程序类的指针
	if(softkeyboard.DoModal() == IDOK)
	{
		softkeyboard_NUM=app->softkeyboard_value;
		m_edit1=softkeyboard_NUM;

//		buf.Format("%lf",app->softkeyboard_value);
//		AfxMessageBox(buf,MB_OK,0);

//		if(factorChangeFlag==TRUE)
		UpdateData(0);
	}
}

void CVideoCaputureDlg::OnSetfocusEdit2() 
{
	// TODO: Add your control notification handler code here
	CSoftKeyboard softkeyboard;
	if(factorChangeFlag==TRUE)
		UpdateData();
	CVideoCaputureApp *app = (CVideoCaputureApp *)AfxGetApp(); //生成指向应用程序类的指针
	if(softkeyboard.DoModal() == IDOK)
	{
		softkeyboard_NUM=app->softkeyboard_value;
		m_edit2=softkeyboard_NUM;
		UpdateData(0);
	}
}

void CVideoCaputureDlg::OnSetfocusEdit3() 
{
	// TODO: Add your control notification handler code here
	CSoftKeyboard softkeyboard;
	if(factorChangeFlag==TRUE)
		UpdateData();
	CVideoCaputureApp *app = (CVideoCaputureApp *)AfxGetApp(); //生成指向应用程序类的指针
	if(softkeyboard.DoModal() == IDOK)
	{
		softkeyboard_NUM=app->softkeyboard_value;
		m_edit3=softkeyboard_NUM;
		UpdateData(0);
	}
}

void CVideoCaputureDlg::OnSetfocusEdit4() 
{
	// TODO: Add your control notification handler code here
	CSoftKeyboard softkeyboard;
	if(factorChangeFlag==TRUE)
		UpdateData();
	CVideoCaputureApp *app = (CVideoCaputureApp *)AfxGetApp(); //生成指向应用程序类的指针
	if(softkeyboard.DoModal() == IDOK)
	{
		softkeyboard_NUM=app->softkeyboard_value;
		m_edit1=softkeyboard_NUM;
		UpdateData(0);	
	}
}
