object FrmMain: TFrmMain
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'ClickTime'
  ClientHeight = 261
  ClientWidth = 384
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  TextHeight = 15
  object LblClickInterval: TLabel
    Left = 8
    Top = 11
    Width = 74
    Height = 15
    Caption = 'Click interval: '
  end
  object LblHoldInterval: TLabel
    Left = 8
    Top = 42
    Width = 74
    Height = 15
    Caption = 'Hold interval: '
  end
  object BtnStart: TBitBtn
    Left = 8
    Top = 210
    Width = 75
    Height = 25
    Caption = '&Start'
    TabOrder = 6
    OnClick = BtnStartClick
  end
  object BtnStop: TBitBtn
    Left = 89
    Top = 210
    Width = 75
    Height = 25
    Caption = '&Stop'
    TabOrder = 7
    OnClick = BtnStopClick
  end
  object CBoxTimeFrame: TComboBox
    Left = 88
    Top = 8
    Width = 132
    Height = 23
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 0
    Text = 'Milliseconds'
    OnChange = CBoxTimeFrameChange
    Items.Strings = (
      'Milliseconds'
      'Seconds'
      'Minutes')
  end
  object EditTimeValue: TEdit
    Left = 240
    Top = 8
    Width = 136
    Height = 23
    TabOrder = 1
    Text = '1000'
  end
  object BtnAbout: TBitBtn
    Left = 240
    Top = 210
    Width = 70
    Height = 25
    Caption = 'A&bout'
    TabOrder = 8
    OnClick = BtnAboutClick
  end
  object BtnExit: TBitBtn
    Left = 316
    Top = 210
    Width = 60
    Height = 25
    Caption = 'E&xit'
    TabOrder = 9
    OnClick = BtnExitClick
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 241
    Width = 384
    Height = 20
    Panels = <
      item
        Text = 'Clicks: 0'
        Width = 100
      end>
  end
  object GBoxMouse: TGroupBox
    Left = 8
    Top = 74
    Width = 140
    Height = 55
    Caption = 'Mouse Button '
    TabOrder = 4
    object RB_MouseLeft: TRadioButton
      Left = 12
      Top = 24
      Width = 51
      Height = 17
      Caption = 'Left'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object RB_MouseRight: TRadioButton
      Left = 69
      Top = 24
      Width = 51
      Height = 17
      Caption = 'Right'
      TabOrder = 1
    end
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 143
    Width = 366
    Height = 54
    Caption = 'Hot Key Start/Stop '
    TabOrder = 5
    object CB_HK_Shift: TCheckBox
      Left = 24
      Top = 24
      Width = 57
      Height = 17
      Caption = 'Shift'
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object CB_HK_Ctrl: TCheckBox
      Left = 103
      Top = 24
      Width = 46
      Height = 17
      Caption = 'Ctrl'
      Checked = True
      State = cbChecked
      TabOrder = 1
    end
    object CB_HK_Alt: TCheckBox
      Left = 171
      Top = 24
      Width = 57
      Height = 17
      Caption = 'Alt'
      TabOrder = 2
    end
    object CBox_HK_FKey: TComboBox
      Left = 248
      Top = 21
      Width = 86
      Height = 23
      Style = csDropDownList
      ItemIndex = 5
      TabOrder = 3
      Text = 'F6'
      Items.Strings = (
        'F1'
        'F2'
        'F3'
        'F4'
        'F5'
        'F6'
        'F7'
        'F8'
        'F9'
        'F10'
        'F11'
        'F12')
    end
  end
  object CBoxHoldInterval: TComboBox
    Left = 88
    Top = 39
    Width = 132
    Height = 23
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 2
    Text = 'Milliseconds'
    OnChange = CBoxHoldIntervalChange
    Items.Strings = (
      'Milliseconds'
      'Seconds'
      'Minutes')
  end
  object EditHoldInterval: TEdit
    Left = 240
    Top = 39
    Width = 136
    Height = 23
    TabOrder = 3
    Text = '10'
  end
  object TimerClick: TTimer
    Enabled = False
    OnTimer = TimerClickTimer
    Left = 328
    Top = 40
  end
end
