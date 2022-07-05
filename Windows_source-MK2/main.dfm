object frmMain: TfrmMain
  Left = 1637
  Top = 464
  Width = 912
  Height = 863
  Caption = 'CurveTracer'
  Color = clBtnFace
  Constraints.MinHeight = 625
  Constraints.MinWidth = 898
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Icon.Data = {
    0000010001002020100000000000E80200001600000028000000200000004000
    0000010004000000000080020000000000000000000000000000000000000000
    000000008000008000000080800080000000800080008080000080808000C0C0
    C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000666111166000000000000000000000611111111111160000000000000
    0000111111111111111E00000000000000611111111111111111160000000000
    0611111111111111111111600000000061111111111111111111111000000000
    1111111111111188811111110000000611111111111118F8F811111160000001
    1111111111111F111F811111100000611111111111118F1118F1111116000011
    111111111111F8111EF1111111000011111111111111F1111188111111000011
    111111111118F111118811111160061111111111111881111188111111600611
    1118811111188111118811111160001111188111111F11111111111111000011
    11188111111F111111111111110000111118F111118811111111111116000061
    1111FE1111F8111111111111160000011111F81111F111111111111110000006
    11118FE1188111111111111160000000111118F18F1111111111111100000000
    6111118FF811111111111110000000000611111E111111111111116000000000
    0061111111111111111116000000000000001111111111111111000000000000
    0000061111111111116000000000000000000006661111660000000000000000
    000000000000000000000000000000000000000000000000000000000000FFFF
    FFFFFFFFFFFFFFE00FFFFF8001FFFF0000FFFC00003FF800001FF000001FF000
    000FE0000007E0000007C0000003C0000003C0000003C0000001800000018000
    0001C0000003C0000003C0000003C0000003E0000007E0000007F000000FF000
    001FF800001FFC00003FFF0000FFFF8001FFFFE00FFFFFFFFFFFFFFFFFFF}
  OldCreateOrder = False
  OnClose = FormClose
  OnPaint = FormPaint
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 904
    Height = 817
    Align = alClient
    BevelOuter = bvNone
    BorderStyle = bsSingle
    Caption = ' '
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 0
    object Label1: TLabel
      Left = 592
      Top = 0
      Width = 5
      Height = 815
      Align = alRight
      AutoSize = False
      Caption = ' '
    end
    object Label3: TLabel
      Left = 897
      Top = 0
      Width = 5
      Height = 815
      Align = alRight
      AutoSize = False
      Caption = ' '
    end
    object Panel2: TPanel
      Left = 597
      Top = 0
      Width = 300
      Height = 815
      Align = alRight
      BevelOuter = bvNone
      Caption = ' '
      TabOrder = 0
      object Label4: TLabel
        Left = 0
        Top = 0
        Width = 300
        Height = 5
        Align = alTop
        AutoSize = False
        Caption = ' '
      end
      object Panel4: TPanel
        Left = 0
        Top = 5
        Width = 300
        Height = 556
        Align = alTop
        BevelOuter = bvNone
        BorderStyle = bsSingle
        Caption = ' '
        TabOrder = 0
        object lblProgress: TLabel
          Left = 16
          Top = 16
          Width = 10
          Height = 17
          Caption = '\'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -14
          Font.Name = 'Verdana'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label2: TLabel
          Left = 16
          Top = 48
          Width = 19
          Height = 13
          Caption = 'Port'
        end
        object cmdConnect: TSpeedButton
          Left = 16
          Top = 88
          Width = 89
          Height = 33
          Caption = 'Connect'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clGray
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          OnClick = cmdConnectClick
        end
        object cmdDisconnect: TSpeedButton
          Left = 192
          Top = 88
          Width = 89
          Height = 33
          Caption = 'Disconnect'
          OnClick = cmdDisconnectClick
        end
        object lblError: TLabel
          Left = 240
          Top = 19
          Width = 41
          Height = 13
          Alignment = taRightJustify
          Caption = 'lblError'
          Color = clBtnFace
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
        end
        object shpColourEven: TShape
          Left = 96
          Top = 216
          Width = 17
          Height = 17
          Brush.Color = 52224
        end
        object Label6: TLabel
          Left = 16
          Top = 193
          Width = 33
          Height = 13
          Caption = 'Colour:'
        end
        object Label8: TLabel
          Left = 16
          Top = 216
          Width = 69
          Height = 13
          Caption = 'Even samples:'
        end
        object Label9: TLabel
          Left = 24
          Top = 239
          Width = 64
          Height = 13
          Caption = 'Odd samples:'
        end
        object shpColourOdd: TShape
          Left = 96
          Top = 239
          Width = 17
          Height = 17
          Brush.Color = 16728128
        end
        object lblUpdateFreq: TLabel
          Left = 16
          Top = 385
          Width = 75
          Height = 13
          Caption = 'Update interval:'
        end
        object Label11: TLabel
          Left = 16
          Top = 433
          Width = 25
          Height = 13
          Caption = '50ms'
        end
        object Label12: TLabel
          Left = 244
          Top = 433
          Width = 37
          Height = 13
          Alignment = taRightJustify
          Caption = '1000ms'
        end
        object Label10: TLabel
          Left = 16
          Top = 466
          Width = 44
          Height = 13
          Caption = 'Line size:'
        end
        object Shape1: TShape
          Left = 16
          Top = 136
          Width = 265
          Height = 1
          Pen.Color = clMedGray
        end
        object Shape2: TShape
          Left = 16
          Top = 184
          Width = 265
          Height = 1
          Pen.Color = clMedGray
        end
        object Shape4: TShape
          Left = 16
          Top = 457
          Width = 265
          Height = 1
          Pen.Color = clMedGray
        end
        object Label5: TLabel
          Left = 24
          Top = 262
          Width = 66
          Height = 13
          Caption = 'Held samples:'
        end
        object shpColourHeld: TShape
          Left = 96
          Top = 262
          Width = 17
          Height = 17
        end
        object Shape5: TShape
          Left = 16
          Top = 492
          Width = 265
          Height = 1
          Pen.Color = clMedGray
        end
        object cmdHold: TSpeedButton
          Left = 16
          Top = 506
          Width = 89
          Height = 33
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'Hold samples'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          OnClick = cmdHoldClick
        end
        object cboPort: TComboBox
          Left = 16
          Top = 64
          Width = 265
          Height = 21
          ItemHeight = 13
          TabOrder = 0
          OnChange = cboPortChange
        end
        object chkGridVisible: TCheckBox
          Left = 16
          Top = 152
          Width = 81
          Height = 17
          Caption = 'Show grid'
          TabOrder = 1
          OnClick = chkGridVisibleClick
        end
        object chkRulerVisible: TCheckBox
          Left = 200
          Top = 152
          Width = 81
          Height = 17
          Caption = 'Show rulers'
          TabOrder = 2
          OnClick = chkRulerVisibleClick
        end
        object cmdColourEven: TButton
          Left = 120
          Top = 216
          Width = 25
          Height = 17
          Caption = '...'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Small Fonts'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = cmdColourEvenClick
        end
        object cmdColourOdd: TButton
          Left = 120
          Top = 239
          Width = 25
          Height = 17
          Caption = '...'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Small Fonts'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          OnClick = cmdColourOddClick
        end
        object tbFreq: TTrackBar
          Left = 8
          Top = 401
          Width = 281
          Height = 33
          Max = 20
          Min = 1
          Orientation = trHorizontal
          Frequency = 1
          Position = 4
          SelEnd = 0
          SelStart = 0
          TabOrder = 5
          TickMarks = tmBottomRight
          TickStyle = tsAuto
          OnChange = tbFreqChange
        end
        object optLineThick: TRadioButton
          Left = 160
          Top = 466
          Width = 49
          Height = 17
          Caption = 'Thick'
          TabOrder = 6
          OnClick = optLineThickClick
        end
        object optLineThin: TRadioButton
          Left = 232
          Top = 466
          Width = 49
          Height = 17
          Caption = 'Thin'
          TabOrder = 7
          OnClick = optLineThinClick
        end
        object optLineFat: TRadioButton
          Left = 88
          Top = 466
          Width = 41
          Height = 17
          Caption = 'Fat'
          TabOrder = 8
          OnClick = optLineFatClick
        end
        object cmdColourHeld: TButton
          Left = 120
          Top = 262
          Width = 25
          Height = 17
          Caption = '...'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Small Fonts'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
          OnClick = cmdColourHeldClick
        end
        object grpFreq: TGroupBox
          Left = 8
          Top = 296
          Width = 281
          Height = 81
          Caption = ' Frequency: '
          Color = clBtnFace
          ParentColor = False
          TabOrder = 10
          OnClick = grpFreqClick
          object opt1k: TRadioButton
            Left = 16
            Top = 48
            Width = 113
            Height = 17
            Caption = '1 kHz (1 ms)'
            TabOrder = 0
            OnClick = grpFreqClick
          end
          object opt200: TRadioButton
            Left = 144
            Top = 24
            Width = 113
            Height = 17
            Caption = '200 Hz (5 ms)'
            TabOrder = 1
            OnClick = grpFreqClick
          end
          object opt2k: TRadioButton
            Left = 144
            Top = 48
            Width = 113
            Height = 17
            Caption = '2 kHz (0.5 ms)'
            Enabled = False
            TabOrder = 2
            OnClick = grpFreqClick
          end
          object opt50: TRadioButton
            Left = 16
            Top = 24
            Width = 113
            Height = 17
            Caption = '50 Hz (20 ms)'
            Checked = True
            TabOrder = 3
            TabStop = True
            OnClick = grpFreqClick
          end
        end
      end
      object chkDebug: TCheckBox
        Left = 241
        Top = 577
        Width = 57
        Height = 17
        Caption = 'Debug'
        TabOrder = 1
        Visible = False
      end
      object Memo1: TMemo
        Left = 176
        Top = 8
        Width = 57
        Height = 33
        TabOrder = 2
        Visible = False
      end
      object Memo2: TMemo
        Left = 240
        Top = 8
        Width = 57
        Height = 33
        TabOrder = 3
        Visible = False
      end
    end
    object Panel3: TPanel
      Left = 0
      Top = 0
      Width = 592
      Height = 815
      Align = alClient
      BevelOuter = bvNone
      BorderStyle = bsSingle
      Caption = ' '
      Color = clBlack
      Constraints.MinHeight = 300
      Constraints.MinWidth = 300
      TabOrder = 1
      object img: TImage
        Left = 0
        Top = 0
        Width = 590
        Height = 813
        Align = alClient
      end
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 817
    Width = 904
    Height = 19
    Panels = <>
    SimplePanel = False
  end
  object Timer1: TTimer
    Interval = 10
    OnTimer = Timer1Timer
    Left = 704
    Top = 8
  end
  object ColorDialog1: TColorDialog
    Ctl3D = True
    Left = 736
    Top = 8
  end
end
