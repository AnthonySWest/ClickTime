//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"
#include "StringTool.h"
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
    WinKeyEventHook = nullptr;

    Caption = Caption + " - " + TFrmMain::CompanyName + " - " + AppVersion.ToStrVer().c_str();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
    InitializeKeyEventHook();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
    ShutdownKeyEventHook();
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
// See: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowshookexw
// sets up the event hook.
void TFrmMain::InitializeKeyEventHook()
{
	//::CoInitialize(NULL);
	WinKeyEventHook = ::SetWindowsHookExW(
		WH_KEYBOARD_LL,     //type of hook procedure to be installed
		LowLevelKeyboardProc, // The callback.
		NULL,               // (HINSTANCE) Handle to DLL (if NULL, current process is used).
		0);                 // (DWORD) identifier of the thread with which the hook procedure is to be associated

    if (NULL == WinKeyEventHook)
    {
        DWORD lastErr = ::GetLastError();
        std::wstring lastErrMsg = L"Failed to set the key event hook for starting/stopping the keyboard from hot key.\n\nWin error: " +
            TStrTool::GetWindowsLastErrorCodeAsStringW(lastErr);
        MsgDlg(lastErrMsg.c_str(), TFrmMain::AppFriendlyName, TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
    }
}
//---------------------------------------------------------------------------
// See: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwineventhook?redirectedfrom=MSDN
// Unhooks the event
void TFrmMain::ShutdownKeyEventHook()
{
	::UnhookWindowsHookEx(WinKeyEventHook);
	WinKeyEventHook = nullptr;
	//::CoUninitialize();
}
//---------------------------------------------------------------------------
//see: https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms644985(v=vs.85)
// - nCode: Must return right away if less than zero. Otherwise, process
// - wParam can be: WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP
// - lParam is a pointer to a KBDLLHOOKSTRUCT structure.
LRESULT CALLBACK TFrmMain::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0) //per documentation, return without further processing ASAP
        return ::CallNextHookEx(NULL, nCode, wParam, lParam);
        
    //at this point, it should be HC_ACTION, but check anyway
    if (HC_ACTION != nCode || nullptr == FrmMain) 
        return ::CallNextHookEx(NULL, nCode, wParam, lParam);

    switch (wParam)
    {
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
            break;
        case WM_KEYDOWN:
        case WM_KEYUP:
            PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
            
            if (key->vkCode == VK_F6)
            {     
                if (WM_KEYDOWN == wParam)
                {
                    //can redirect - keeping here for knowledge sake
                    //::keybd_event('B', 0, 0, 0);
                }
                else if (WM_KEYUP == wParam) // Keyup
                {
                    if (FrmMain->BtnStart->Enabled)
                        FrmMain->BtnStartClick(nullptr);
                    else
                        FrmMain->BtnStopClick(nullptr);
                
                    //finish redirect - keeping here for knowledge sake
                    //::keybd_event('B', 0, KEYEVENTF_KEYUP, 0);
                }
                
            }
            break;
    }

    return ::CallNextHookEx(NULL, nCode, wParam, lParam);
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

