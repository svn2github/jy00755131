object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 632
  ClientWidth = 877
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object RzPageControl1: TRzPageControl
    Left = 32
    Top = 24
    Width = 793
    Height = 593
    ActivePage = TabSheet5
    TabIndex = 4
    TabOrder = 0
    FixedDimension = 19
    object TabSheet1: TRzTabSheet
      Caption = #25968#25454#37319#38598
      object StaticText1: TStaticText
        Left = 16
        Top = 16
        Width = 52
        Height = 17
        Alignment = taCenter
        BorderStyle = sbsSunken
        Caption = #21457#36865#31471#21475
        TabOrder = 0
      end
      object RzEdit1: TRzEdit
        Left = 16
        Top = 39
        Width = 121
        Height = 21
        Text = '0x8202'
        Alignment = taCenter
        TabOrder = 1
      end
      object StaticText2: TStaticText
        Left = 16
        Top = 80
        Width = 100
        Height = 17
        Alignment = taCenter
        BorderStyle = sbsSunken
        Caption = #37319#26679#25968#25454#23384#20648#24103#25968
        TabOrder = 2
      end
      object RzEdit2: TRzEdit
        Left = 16
        Top = 112
        Width = 121
        Height = 21
        Text = '406'
        Alignment = taCenter
        TabOrder = 3
      end
      object RzCheckBox1: TRzCheckBox
        Left = 16
        Top = 152
        Width = 67
        Height = 15
        Caption = #27979#32593#24314#31435
        State = cbUnchecked
        TabOrder = 4
      end
      object RzCheckBox2: TRzCheckBox
        Left = 89
        Top = 152
        Width = 43
        Height = 15
        Caption = #24515#36339
        State = cbUnchecked
        TabOrder = 5
      end
      object RzButton1: TRzButton
        Left = 16
        Top = 184
        Width = 121
        Height = 33
        Caption = #25163#21160#23545#26102
        Enabled = False
        TabOrder = 6
      end
      object RzButton2: TRzButton
        Left = 16
        Top = 232
        Width = 121
        Height = 33
        Caption = 'ADC'#21442#25968#35774#32622
        Enabled = False
        TabOrder = 7
      end
      object RzButton3: TRzButton
        Left = 16
        Top = 280
        Width = 121
        Height = 33
        Caption = #25163#21160#35774#32622
        Enabled = False
        TabOrder = 8
      end
      object RzButton4: TRzButton
        Left = 16
        Top = 328
        Width = 121
        Height = 33
        Caption = #24320#22987#37319#26679
        Enabled = False
        TabOrder = 9
      end
      object RzButton5: TRzButton
        Left = 16
        Top = 376
        Width = 121
        Height = 33
        Caption = #20572#27490#37319#26679
        Enabled = False
        TabOrder = 10
      end
      object RzButton6: TRzButton
        Left = 16
        Top = 424
        Width = 121
        Height = 33
        Caption = #25991#20214#23384#20648#36335#24452
        TabOrder = 11
      end
      object RzButton7: TRzButton
        Left = 16
        Top = 472
        Width = 121
        Height = 33
        Caption = #37325#32622
        TabOrder = 12
      end
      object RzButton8: TRzButton
        Left = 16
        Top = 520
        Width = 121
        Height = 33
        Caption = #32472#22270#31243#24207
        TabOrder = 13
      end
    end
    object TabSheet2: TRzTabSheet
      Caption = 'ADC'#21442#25968#35774#32622
      object RzGroupBox1: TRzGroupBox
        Left = 56
        Top = 40
        Width = 345
        Height = 481
        Caption = 'ADC'#25511#21046
        TabOrder = 0
        object StaticText3: TStaticText
          Left = 24
          Top = 32
          Width = 28
          Height = 17
          Caption = #21516#27493
          TabOrder = 0
        end
        object StaticText4: TStaticText
          Left = 24
          Top = 71
          Width = 28
          Height = 17
          Caption = #27169#24335
          TabOrder = 1
        end
        object StaticText5: TStaticText
          Left = 24
          Top = 110
          Width = 40
          Height = 17
          Caption = #37319#26679#29575
          TabOrder = 2
        end
        object StaticText6: TStaticText
          Left = 24
          Top = 150
          Width = 45
          Height = 17
          Caption = 'FIR'#30456#20301
          TabOrder = 3
        end
        object StaticText7: TStaticText
          Left = 24
          Top = 190
          Width = 64
          Height = 17
          Caption = #25968#23383#28388#27874#22120
          TabOrder = 4
        end
        object StaticText8: TStaticText
          Left = 24
          Top = 230
          Width = 40
          Height = 17
          Caption = #22810#36335#22120
          TabOrder = 5
        end
        object StaticText9: TStaticText
          Left = 24
          Top = 270
          Width = 32
          Height = 17
          Caption = 'CHOP'
          TabOrder = 6
        end
        object StaticText10: TStaticText
          Left = 24
          Top = 310
          Width = 48
          Height = 17
          Caption = 'PGA'#22686#30410
          TabOrder = 7
        end
        object RzComboBox1: TRzComboBox
          Left = 160
          Top = 32
          Width = 129
          Height = 21
          TabOrder = 8
          Text = #33033#20914#21516#27493#27169#24335
          Items.Strings = (
            #33033#20914#21516#27493#27169#24335
            #36830#32493#21516#27493#27169#24335)
        end
        object RzComboBox2: TRzComboBox
          Left = 160
          Top = 71
          Width = 129
          Height = 21
          TabOrder = 9
          Text = #39640#31934#24230#27169#24335
          Items.Strings = (
            #20302#21151#32791#27169#24335
            #39640#31934#24230#27169#24335)
        end
        object RzComboBox3: TRzComboBox
          Left = 160
          Top = 110
          Width = 129
          Height = 21
          TabOrder = 10
          Text = '500'
          Items.Strings = (
            '4000'
            '2000'
            '1000'
            '500'
            '250')
        end
        object RzComboBox4: TRzComboBox
          Left = 160
          Top = 150
          Width = 129
          Height = 21
          TabOrder = 11
          Text = #32447#24615#30456#20301
          Items.Strings = (
            #32447#24615#30456#20301
            #26368#23567#30456#20301)
        end
        object RzComboBox5: TRzComboBox
          Left = 160
          Top = 190
          Width = 129
          Height = 21
          TabOrder = 12
          Text = 'Sinc+LPF'#28388#27874#22120
          Items.Strings = (
            #35843#21046#22120#36755#20986#27169#24335#65292#21487#24573#30053#29255#19978#28388#27874#22120
            'Sinc'#28388#27874#22120
            'Sinc+LPF'#28388#27874#22120
            'Sinc+LPF+HPF'#28388#27874#22120)
        end
        object RzComboBox6: TRzComboBox
          Left = 160
          Top = 231
          Width = 129
          Height = 21
          TabOrder = 13
          Text = 'AINP1+AINN1'
          Items.Strings = (
            'AINP1+AINN1'
            'Internal short via 400'#937
            'AINP1 + AINP2'#36830#25509'AINP2 + AINN2'
            'External short to AINN2')
        end
        object RzComboBox7: TRzComboBox
          Left = 160
          Top = 270
          Width = 129
          Height = 21
          TabOrder = 14
          Text = 'PGA chopping enabled'
          Items.Strings = (
            'PGA chopping disabled'
            'PGA chopping enabled')
        end
        object RzComboBox8: TRzComboBox
          Left = 160
          Top = 310
          Width = 129
          Height = 21
          TabOrder = 15
          Text = 'G = 1'
          Items.Strings = (
            'G = 1'
            'G = 2'
            'G = 4'
            'G = 8'
            'G = 16'
            'G = 32')
        end
      end
      object RzGroupBox2: TRzGroupBox
        Left = 424
        Top = 43
        Width = 321
        Height = 128
        Caption = #39640#36890#28388#27874#22120#25296#35282#39057#29575#65288'HPF'#65289
        TabOrder = 1
        object StaticText11: TStaticText
          Left = 24
          Top = 32
          Width = 28
          Height = 17
          Caption = #20302#20301
          TabOrder = 0
        end
        object StaticText12: TStaticText
          Left = 24
          Top = 83
          Width = 28
          Height = 17
          Caption = #39640#20301
          TabOrder = 1
        end
        object RzEdit3: TRzEdit
          Left = 120
          Top = 29
          Width = 97
          Height = 21
          Text = '50'
          Alignment = taCenter
          TabOrder = 2
        end
        object RzEdit4: TRzEdit
          Left = 120
          Top = 83
          Width = 97
          Height = 21
          Text = '3'
          Alignment = taCenter
          TabOrder = 3
        end
      end
    end
    object TabSheet3: TRzTabSheet
      Caption = 'ADC'#25968#25454#26174#31034
      object RzListBox1: TRzListBox
        Left = 17
        Top = 14
        Width = 753
        Height = 545
        ItemHeight = 13
        MultiSelect = True
        TabOrder = 0
      end
    end
    object TabSheet4: TRzTabSheet
      Caption = #20018#21475#36716'UDP'
      object StaticText13: TStaticText
        Left = 152
        Top = 160
        Width = 52
        Height = 17
        Caption = #31471#21475#35774#32622
        TabOrder = 0
      end
      object RzComboBox9: TRzComboBox
        Left = 248
        Top = 156
        Width = 129
        Height = 21
        TabOrder = 1
        Text = 'COM9'
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4'
          'COM5'
          'COM6'
          'COM7'
          'COM8'
          'COM9'
          'COM10')
      end
      object StaticText14: TStaticText
        Left = 152
        Top = 216
        Width = 64
        Height = 17
        Caption = #27874#29305#29575#35774#32622
        TabOrder = 2
      end
      object RzComboBox10: TRzComboBox
        Left = 248
        Top = 212
        Width = 129
        Height = 21
        TabOrder = 3
        Text = '115200'
        Items.Strings = (
          '300'
          '600'
          '1200'
          '2400'
          '4800'
          '9600'
          '19200'
          '38400'
          '57600'
          '115200')
      end
      object StaticText15: TStaticText
        Left = 422
        Top = 160
        Width = 62
        Height = 17
        Caption = #30446#26631'IP'#22320#22336
        TabOrder = 4
      end
      object StaticText16: TStaticText
        Left = 422
        Top = 216
        Width = 52
        Height = 17
        Caption = #30446#26631#31471#21475
        TabOrder = 5
      end
      object StaticText17: TStaticText
        Left = 422
        Top = 272
        Width = 76
        Height = 17
        Caption = #26412#26426#25509#25910#31471#21475
        TabOrder = 6
      end
      object RzButton9: TRzButton
        Left = 168
        Top = 344
        Width = 137
        Height = 41
        Caption = #25171#24320#20018#21475
        TabOrder = 7
      end
      object RzButton10: TRzButton
        Left = 422
        Top = 344
        Width = 137
        Height = 41
        Caption = #20851#38381#20018#21475
        TabOrder = 8
      end
      object RzEdit5: TRzEdit
        Left = 536
        Top = 212
        Width = 89
        Height = 21
        Text = '0x9001'
        Alignment = taCenter
        TabOrder = 9
      end
      object RzEdit6: TRzEdit
        Left = 536
        Top = 268
        Width = 89
        Height = 21
        Text = '0x9002'
        Alignment = taCenter
        TabOrder = 10
      end
      object RzEdit7: TRzEdit
        Left = 536
        Top = 156
        Width = 89
        Height = 21
        Text = '127.0.0.1'
        TabOrder = 11
      end
    end
    object TabSheet5: TRzTabSheet
      Caption = #31471#21475#30417#35270
    end
  end
end
