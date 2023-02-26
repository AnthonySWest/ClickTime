//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"
#include "AboutForm.h"
#include "StringTool.h"
#include "MouseTool.h"
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
    ClickCount = 0;
    ShiftKeyDown = false;
    CtrlKeyDown = false;
    AltKeyDown = false;

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
    BtnAbout->Enabled = false;

    CBoxTimeFrame->Enabled = false;
    EditTimeValue->Enabled  = false;
}
//---------------------------------------------------------------------------
void TFrmMain::SetFormToProcessStopped()
{
    BtnStart->Enabled = true;
    BtnStop->Enabled = false;
    BtnAbout->Enabled = true;

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
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
            DWORD vkCode = key->vkCode;

            if (VK_LSHIFT == vkCode || VK_RSHIFT == vkCode)
                FrmMain->ShiftKeyDown = true;
            else if (VK_LCONTROL == vkCode || VK_RCONTROL == vkCode)
                FrmMain->CtrlKeyDown = true;
            else if (VK_LMENU == vkCode || VK_RMENU == vkCode)
                FrmMain->AltKeyDown = true;
                
            //can redirect - keeping here for knowledge sake
            //::keybd_event('A', 0, 0, 0);
            break;
        }
        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
            DWORD vkCode = key->vkCode;
            //bool altKeyDown = key->flags & LLKHF_ALTDOWN; //isn't always accurate for this use case

            if (VK_LSHIFT == vkCode || VK_RSHIFT == vkCode)
                FrmMain->ShiftKeyDown = false;
            else if (VK_LCONTROL == vkCode || VK_RCONTROL == vkCode)
                FrmMain->CtrlKeyDown = false;            
            else if (VK_LMENU == vkCode || VK_RMENU == vkCode)
                FrmMain->AltKeyDown = false;
            else if (vkCode == VK_F1 + FrmMain->CBox_HK_FKey->ItemIndex)
            {     
                DWORD shiftSettingsMask = GetShiftStateMask(FrmMain->CB_HK_Shift->Checked,
                    FrmMain->CB_HK_Ctrl->Checked, FrmMain->CB_HK_Alt->Checked);
                DWORD shiftStateMask = GetShiftStateMask(FrmMain->ShiftKeyDown, FrmMain->CtrlKeyDown, FrmMain->AltKeyDown);
                        
                if (shiftStateMask == shiftSettingsMask)
                {
                    if (FrmMain->BtnStart->Enabled)
                        FrmMain->BtnStartClick(nullptr);
                    else
                        FrmMain->BtnStopClick(nullptr);
                }
            }

            //finish redirect - keeping here for knowledge sake
            //::keybd_event('A', 0, KEYEVENTF_KEYUP, 0);
            break;
        }
    }

    //Note: return 1 instead of calling the next hook to eat the key event.
    return ::CallNextHookEx(NULL, nCode, wParam, lParam);
}
//---------------------------------------------------------------------------
DWORD TFrmMain::GetShiftStateMask(bool shift, bool ctrl, bool alt)
{
    DWORD shiftState = 0;

    if (shift)
        shiftState = 0x01;

    if (ctrl)
        shiftState |= 0x02;

    if (alt)
        shiftState |= 0x04;

    return shiftState;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::BtnStartClick(TObject *Sender)
{
    bool showInvalidValueMsg = false;

    try
    {
        ClickCount = 0;
        BtnStart->Enabled = false;
        double timeValue = EditTimeValue->Text.ToDouble();

        if (timeValue <= 0)
        {
            showInvalidValueMsg = true;
        }
        else
        {
            if (CBoxTimeFrame->ItemIndex == 0)
                TimerClick->Interval = timeValue;
            else if (CBoxTimeFrame->ItemIndex == 1)
                TimerClick->Interval = timeValue * 1000;
            else if (CBoxTimeFrame->ItemIndex == 2)
                TimerClick->Interval = timeValue * 60 * 1000;

            TimerClick->Enabled = true;
            SetFormToProcessStarted();
        }
    }
    catch(...)
    {
        showInvalidValueMsg = true;
    }

    if (showInvalidValueMsg)
    {
        MsgDlg("Please enter a time value greater than zero.", TFrmMain::AppFriendlyName, TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
        SetFormToProcessStopped();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::TimerClickTimer(TObject *Sender)
{
    int clickUpDelayMS = TMouseTool::Click_DefaultUpDelayMS;

    if (TimerClick->Interval < clickUpDelayMS)
        TimerClick->Interval = clickUpDelayMS;
    
    ClickCount++;

    if (RB_MouseLeft->Checked)
        TMouseTool::MouseLeftClick();
    else 
        TMouseTool::MouseRightClick();
        
    StatusBar1->Panels->Items[0]->Text = UnicodeString(L"Clicks: ") + IntToStr(ClickCount);
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
void __fastcall TFrmMain::BtnAboutClick(TObject *sender)
{
    FrmAbout->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::BtnExitClick(TObject *sender)
{
    if (FrmMain->BtnStop->Enabled)
        FrmMain->BtnStopClick(sender);

    Application->Terminate();    
}
//---------------------------------------------------------------------------

