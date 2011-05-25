object Form1: TForm1
  Left = 1022
  Top = 152
  Width = 789
  Height = 470
  Caption = #20018#21475#36890#35759
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object MSComm1: TMSComm
    Left = 368
    Top = 112
    Width = 32
    Height = 32
    OnComm = MSComm1Comm
    ControlData = {
      2143341208000000ED030000ED03000001568A64000006000000030000040000
      0004010000C201000000080000000000000000003F00000004000000}
  end
  object Button1: TButton
    Left = 200
    Top = 120
    Width = 75
    Height = 25
    Caption = 'OPEN'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 200
    Top = 176
    Width = 75
    Height = 25
    Caption = 'CLOSE'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Edit1: TEdit
    Left = 352
    Top = 184
    Width = 121
    Height = 21
    TabOrder = 3
    Text = 'Edit1'
  end
  object nmdp_send: TNMUDP
    RemoteHost = '127.0.0.1'
    RemotePort = 8001
    LocalPort = 8000
    ReportLevel = 1
    Left = 464
    Top = 112
  end
  object nmdp_rec: TNMUDP
    RemotePort = 8000
    LocalPort = 8001
    ReportLevel = 1
    OnDataReceived = nmdp_recDataReceived
    Left = 528
    Top = 112
  end
end
