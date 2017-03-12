; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CVideoCaputureDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VideoCaputure.h"

ClassCount=7
Class1=CVideoCaputureApp
Class2=CVideoCaputureDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_CHANGERADIUS
Class4=CChangeRadius
Class5=CMaskDialog
Class6=CMeasureDiamete
Resource4=IDD_MeasureDiameter
Resource5=IDD_MASK
Class7=CSoftKeyboard
Resource6=IDD_VIDEOCAPUTURE_DIALOG
Resource7=IDD_SoftKeyboard

[CLS:CVideoCaputureApp]
Type=0
HeaderFile=VideoCaputure.h
ImplementationFile=VideoCaputure.cpp
Filter=N

[CLS:CVideoCaputureDlg]
Type=0
HeaderFile=VideoCaputureDlg.h
ImplementationFile=VideoCaputureDlg.cpp
Filter=D
LastObject=IDC_AutoChangeMode
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=VideoCaputureDlg.h
ImplementationFile=VideoCaputureDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_VIDEOCAPUTURE_DIALOG]
Type=1
Class=CVideoCaputureDlg
ControlCount=35
Control1=IDC_VIDEO,static,1342177284
Control2=IDC_START,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_Yinjing,button,1342242816
Control5=IDC_Fangjian,button,1342242816
Control6=IDC_Dengjing,button,1342242816
Control7=IDC_STATIC,button,1342177287
Control8=IDC_EDIT1,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT2,edit,1350631552
Control13=IDC_EDIT3,edit,1350631552
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,static,1342308352
Control16=IDC_EDIT6,edit,1350631552
Control17=IDC_CHANGERADIUS,button,1342242816
Control18=IDC_BTNOK,button,1342242816
Control19=IDC_SELECTZONE,button,1342242816
Control20=IDC_AutoMeasure,button,1342242816
Control21=IDC_ReadAVI,button,1342242816
Control22=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control23=IDC_AutoMeasureDown,button,1342242816
Control24=IDC_AutoMeasureReset,button,1342242816
Control25=IDC_STATIC,button,1342177287
Control26=IDC_OPENCLOSEPORT,button,1342242816
Control27=IDC_CONFIGPORT,button,1342242816
Control28=IDC_STATIC_OPENOFF,static,1342177795
Control29=IDC_AutoChangeMode,button,1342242816
Control30=IDC_STATIC,button,1342177287
Control31=IDC_Threshold,button,1342242816
Control32=IDC_BTNquanDefult,button,1342242816
Control33=IDC_STATIC,button,1342177287
Control34=IDC_Threshold2,button,1342242816
Control35=IDC_Threshold3,button,1342242816

[DLG:IDD_CHANGERADIUS]
Type=1
Class=CChangeRadius
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT1,edit,1350631552

[CLS:CChangeRadius]
Type=0
HeaderFile=ChangeRadius.h
ImplementationFile=ChangeRadius.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT1

[DLG:IDD_MeasureDiameter]
Type=1
Class=CMeasureDiamete
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_COMBO_COMSELECT,combobox,1344274434
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO_SPEED,combobox,1344339970
Control9=IDC_COMBO_PARITY,combobox,1344339970
Control10=IDC_COMBO_STOPBITS,combobox,1344340226
Control11=IDC_COMBO_DATABITS,combobox,1344339970
Control12=IDCANCEL,button,1342242817

[CLS:CMeasureDiamete]
Type=0
HeaderFile=MeasureDiamete.h
ImplementationFile=MeasureDiamete.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMBO_COMSELECT
VirtualFilter=dWC

[DLG:IDD_MASK]
Type=1
Class=CMaskDialog
ControlCount=0

[CLS:CMaskDialog]
Type=0
HeaderFile=MaskDialog.h
ImplementationFile=MaskDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CMaskDialog
VirtualFilter=dWC

[CLS:CSoftKeyboard]
Type=0
HeaderFile=SoftKeyboard.h
ImplementationFile=SoftKeyboard.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSoftKeyboard

[DLG:IDD_SoftKeyboard]
Type=1
Class=CSoftKeyboard
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SOFTKEYBOARDEDITNUM,edit,1350631554
Control4=IDC_SOFTKEYBOARDBTN1,button,1342242816
Control5=IDC_SOFTKEYBOARDBTN4,button,1342242816
Control6=IDC_SOFTKEYBOARDBTN2,button,1342242816
Control7=IDC_SOFTKEYBOARDBTN5,button,1342242816
Control8=IDC_SOFTKEYBOARDBTN3,button,1342242816
Control9=IDC_SOFTKEYBOARDBTN8,button,1342242816
Control10=IDC_SOFTKEYBOARDBTN9,button,1342242816
Control11=IDC_SOFTKEYBOARDBTN6,button,1342242816
Control12=IDC_SOFTKEYBOARDBTN7,button,1342242816
Control13=IDC_SOFTKEYBOARDBTNBACK,button,1342242816
Control14=IDC_SOFTKEYBOARDBTNDIAN,button,1342242816
Control15=IDC_SOFTKEYBOARDBTN0,button,1342242816

