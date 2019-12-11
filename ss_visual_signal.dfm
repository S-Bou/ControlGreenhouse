object F_signaled: TF_signaled
  Left = 748
  Top = 368
  Width = 201
  Height = 94
  Caption = 'Signal editor'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lbSignalStr: TLabel
    Left = 8
    Top = 8
    Width = 50
    Height = 13
    Caption = 'lbSignalStr'
  end
  object sbAnalogValue: TScrollBar
    Left = 8
    Top = 32
    Width = 177
    Height = 9
    Max = 10000
    PageSize = 0
    TabOrder = 0
    OnChange = sbAnalogValueChange
  end
  object btHigh: TButton
    Left = 40
    Top = 24
    Width = 57
    Height = 17
    Caption = 'High'
    TabOrder = 1
    OnClick = btHighClick
  end
  object btLow: TButton
    Left = 40
    Top = 40
    Width = 57
    Height = 17
    Caption = 'Low'
    TabOrder = 2
    OnClick = btLowClick
  end
end
