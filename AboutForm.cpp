//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AboutForm.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmAbout *FrmAbout;
//---------------------------------------------------------------------------
__fastcall TFrmAbout::TFrmAbout(TComponent* Owner)
    : TForm(Owner)
{
    LblVersion->Caption = UnicodeString(L"Version: ") + FrmMain->AppVersion.ToStrVerW().c_str();
}
//---------------------------------------------------------------------------
