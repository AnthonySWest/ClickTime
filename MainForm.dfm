object FrmMain: TFrmMain
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'ClickTime'
  ClientHeight = 211
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
  object Label1: TLabel
    Left = 8
    Top = 24
    Width = 63
    Height = 15
    Caption = 'Timeframe: '
  end
  object BtnStart: TBitBtn
    Left = 104
    Top = 152
    Width = 75
    Height = 25
    Caption = 'Start'
    TabOrder = 2
    OnClick = BtnStartClick
  end
  object BtnStop: TBitBtn
    Left = 200
    Top = 152
    Width = 75
    Height = 25
    Caption = 'Stop'
    TabOrder = 3
    OnClick = BtnStopClick
  end
  object CBoxTimeFrame: TComboBox
    Left = 88
    Top = 21
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
    Top = 21
    Width = 136
    Height = 23
    TabOrder = 1
    Text = '1000'
  end
  object TimerClick: TTimer
    Enabled = False
    OnTimer = TimerClickTimer
    Left = 32
    Top = 56
  end
end
