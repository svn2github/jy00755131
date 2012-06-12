; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "E2PROM_TOOL.h"
LastPage=0

ClassCount=5
Class1=CE2PROM_TOOLApp
Class2=CE2PROM_TOOLDoc
Class3=CE2PROM_TOOLView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource7=IDD_E2PROM_TOOL_FORM

[CLS:CE2PROM_TOOLApp]
Type=0
HeaderFile=E2PROM_TOOL.h
ImplementationFile=E2PROM_TOOL.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CE2PROM_TOOLApp

[CLS:CE2PROM_TOOLDoc]
Type=0
HeaderFile=E2PROM_TOOLDoc.h
ImplementationFile=E2PROM_TOOLDoc.cpp
Filter=N

[CLS:CE2PROM_TOOLView]
Type=0
HeaderFile=E2PROM_TOOLView.h
ImplementationFile=E2PROM_TOOLView.cpp
Filter=D
BaseClass=CFormView
VirtualFilter=VWC
LastObject=IDC_STATIC_RANGE


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_FILE_OPEN




[CLS:CAboutDlg]
Type=0
HeaderFile=E2PROM_TOOL.cpp
ImplementationFile=E2PROM_TOOL.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE
Command3=ID_FILE_MRU_FILE1
Command4=ID_APP_EXIT
Command5=ID_APP_ABOUT
CommandCount=5

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[DLG:IDD_E2PROM_TOOL_FORM]
Type=1
Class=CE2PROM_TOOLView
ControlCount=9
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_PROGRESS1,msctls_progress32,1350565888
Control3=IDC_STATIC_RANGE,static,1342308352
Control4=IDC_STATIC,static,1342308353
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_STATIC,static,1342308353
Control7=IDC_EDIT2,edit,1350631552
Control8=IDC_STATIC,static,1342308353
Control9=IDC_EDIT3,edit,1350631552

