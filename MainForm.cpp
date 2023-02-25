//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

using namespace Subroutines;

//---------------------------------------------------------------------------
// -A messagebox wrapper
int MsgDlg(const UnicodeString &msg, const UnicodeString &title, TMsgDlgType dlgType,
	TMsgDlgButtons buttons)
{
TForm *msgDlg = CreateMessageDialog(msg, dlgType, buttons);
TModalResult mRes;

	msgDlg->Color = clWindow;

	if (title.Length() > 0)
		msgDlg->Caption = title;
	else
		msgDlg->Caption = TFrmMain::AppFriendlyName;
//	msgDlg->GlassFrame->Bottom = 40;
//	msgDlg->GlassFrame->Enabled = true;
//	msgDlg->Font->Name = "Verdana";
//	msgDlg->Font->Size = 10;
//	msgDlg->Canvas->Font = msgDlg->Font;
//	msgDlg->Scaled = false;

	mRes = msgDlg->ShowModal();
	delete msgDlg;
	return mRes;
}

TFrmMain *FrmMain;

AnsiString TFrmMain::AppFriendlyName    = "ClickTime";
AnsiString TFrmMain::CompanyName        = "ASW Software";

UnicodeString TFrmMain::DirApp			= L"";

//UnicodeString TFrmMain::FileLog			= L"_CTGeneral.txt";
UnicodeString TFrmMain::FileMainEXE		= L""; //set from INI

//this name is the default name only - app checks first for an ini with same base name as app
UnicodeString TFrmMain::FileSettingsINI = L"ClickTime.ini";

Subroutines::TVersion TFrmMain::AppVersion;

unsigned short TFrmMain::AppStart_Day;
unsigned short TFrmMain::AppStart_Month;
unsigned short TFrmMain::AppStart_Year;

System::Word TFrmMain::AppStart_Hour;
System::Word TFrmMain::AppStart_Min;
System::Word TFrmMain::AppStart_Sec;
System::Word TFrmMain::AppStart_MSec;

#ifdef _DEBUG
	UnicodeString TFrmMain::LogID = L"- " + TFrmMain::AppFriendlyName + " - Debug Compile -";
#else
	UnicodeString TFrmMain::LogID = L"- " + TFrmMain::AppFriendlyName + " -";
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
__fastcall TFrmMain::TFrmMain(TComponent* Owner)
    : TForm(Owner)
{
    Caption = Caption + " - " + TFrmMain::CompanyName + " - " + AppVersion.ToStrVer().c_str();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    //don't remove this comment
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::CreateParams(Vcl::Controls::TCreateParams &params)
{
	TForm::CreateParams(params); // inherited
	//params.ExStyle = params.ExStyle | WS_EX_NOACTIVATE;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    TimerClick->Enabled = false;
}
//---------------------------------------------------------------------------
void TFrmMain::SetFormToProcessStarted()
{
    BtnStart->Enabled = false;
    BtnStop->Enabled = true;

    CBoxTimeFrame->Enabled = false;
    EditTimeValue->Enabled  = false;
}
//---------------------------------------------------------------------------
void TFrmMain::SetFormToProcessStopped()
{
    BtnStart->Enabled = true;
    BtnStop->Enabled = false;

    CBoxTimeFrame->Enabled = true;
    EditTimeValue->Enabled  = true;
}
//---------------------------------------------------------------------------
void TFrmMain::MouseLeftClick()
{
	INPUT mInput;

	mInput.type = INPUT_MOUSE;
	mInput.mi.mouseData = 0;
	mInput.mi.time = 0;
	mInput.mi.dwExtraInfo = 0;

	mInput.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN);
	::SendInput(1, &mInput, sizeof(mInput));

	Sleep(10);

	mInput.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
	::SendInput(1, &mInput, sizeof(INPUT));
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::BtnStartClick(TObject *Sender)
{
    try
    {
        BtnStart->Enabled = false;
        double timeValue = EditTimeValue->Text.ToDouble();

        if (CBoxTimeFrame->ItemIndex == 0)
            TimerClick->Interval = timeValue;
        else if (CBoxTimeFrame->ItemIndex == 1)
            TimerClick->Interval = timeValue * 1000;
        else if (CBoxTimeFrame->ItemIndex == 2)
            TimerClick->Interval = timeValue * 60 * 1000;

        TimerClick->Enabled = true;
        SetFormToProcessStarted();
    }
    catch(...)
    {
        MsgDlg("Please enter a valid time value.", TFrmMain::AppFriendlyName, TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
        BtnStart->Enabled = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::TimerClickTimer(TObject *Sender)
{
    MouseLeftClick();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::BtnStopClick(TObject *Sender)
{
    TimerClick->Enabled = false;
    SetFormToProcessStopped();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::CBoxTimeFrameChange(TObject *Sender)
{
    try
    {
        double timeValue = EditTimeValue->Text.ToDouble();

        if (CBoxTimeFrame->Tag == 0)
        {
            if (CBoxTimeFrame->ItemIndex == 1)
                timeValue /= 1000;
            else if (CBoxTimeFrame->ItemIndex == 2)
                timeValue /= (1000 * 60);
        }
        else if (CBoxTimeFrame->Tag == 1)
        {
            if (CBoxTimeFrame->ItemIndex == 0)
                timeValue *= 1000;
            else if (CBoxTimeFrame->ItemIndex == 2)
                timeValue /= 60;
        }
        else if (CBoxTimeFrame->Tag == 2)
        {
            if (CBoxTimeFrame->ItemIndex == 0)
                timeValue *= (60 * 1000);
            else if (CBoxTimeFrame->ItemIndex == 1)
                timeValue *= 60;
        }

        EditTimeValue->Text = FloatToStr(timeValue);
    }
    catch(...)
    {

    }

    CBoxTimeFrame->Tag = CBoxTimeFrame->ItemIndex;
}
//---------------------------------------------------------------------------

