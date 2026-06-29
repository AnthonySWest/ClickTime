/* **************************************************************************
MainForm.cpp
Author: Anthony S. West - ASW Software

See header for info.

Copyright 2023 Anthony S. West

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

************************************************************************** */

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "MainForm.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
#include "AboutForm.h"
#include "MouseTool.h"
#include "StringTool.h"
//---------------------------------------------------------------------------
using namespace Subroutines;
using namespace System::Sysutils;
//---------------------------------------------------------------------------

// MsgDlg
//
// A messagebox wrapper
int MsgDlg(const UnicodeString& msg, const UnicodeString& title, TMsgDlgType dlgType,
    TMsgDlgButtons buttons)
{
    TForm* msgDlg = CreateMessageDialog(msg, dlgType, buttons);
    TModalResult mRes;

    msgDlg->Color = clWindow;

    if (title.Length() > 0)
        msgDlg->Caption = title;
    else
        msgDlg->Caption = TFrmMain::AppFriendlyName;

//    msgDlg->GlassFrame->Bottom = 40;
//    msgDlg->GlassFrame->Enabled = true;
//    msgDlg->Font->Name = "Verdana";
//    msgDlg->Font->Size = 10;
//    msgDlg->Canvas->Font = msgDlg->Font;
//    msgDlg->Scaled = false;

    mRes = msgDlg->ShowModal();
    delete msgDlg;
    return mRes;
}

//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// TFrmMain
/////////////////////////////////////////////////////////////////////////////

TFrmMain* FrmMain;

AnsiString TFrmMain::AppFriendlyName    = "ClickTime";
AnsiString TFrmMain::CompanyName        = "ASW Software";

UnicodeString TFrmMain::DirApp            = L"";

//UnicodeString TFrmMain::FileLog            = L"_CTGeneral.txt";
UnicodeString TFrmMain::FileMainEXE        = L""; //set from INI

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
__fastcall TFrmMain::TFrmMain(TComponent* owner)
    : TForm(owner)
{
    WinKeyEventHook = nullptr;
    ClickCount = 0;
    ShiftKeyDown = false;
    CtrlKeyDown = false;
    AltKeyDown = false;

    Caption = Caption + " - " + TFrmMain::CompanyName + " - " + AppVersion.ToStrVer().c_str();

#ifdef _DEBUG
    Caption = Caption + " - Debug";
#endif
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::BtnAboutClick(TObject* /*sender*/)
{
    FrmAbout->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::BtnExitClick(TObject* sender)
{
    if (FrmMain->BtnStop->Enabled)
        FrmMain->BtnStopClick(sender);

    Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::BtnStartClick(TObject* /*sender*/)
{
    bool showInvalidValueMsg = false;
    DWORD holdIntervalMS = 0;

    BtnStart->Enabled = false;
    ClickCount = 0;
    UpdateStatusPanel_Clicks();

    try
    {
        holdIntervalMS = GetHoldClickIntervalMS();
        if (holdIntervalMS == 0)
            showInvalidValueMsg = true;
    }
    catch (...)
    {
        showInvalidValueMsg = true;
    }

    if (showInvalidValueMsg)
    {
        MsgDlg("Please enter a hold value greater than zero.",
            TFrmMain::AppFriendlyName, TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
        SetFormToProcessStopped();
        return;
    }

    try
    {
        DWORD clickIntervalMS = GetClickIntervalMS();
        if (clickIntervalMS == 0)
        {
            showInvalidValueMsg = true;
        }
        else
        {
            // Fix up the UI click interval value if less than the hold click value
            if (clickIntervalMS < holdIntervalMS)
            {
                clickIntervalMS = holdIntervalMS;
                EditTimeValue->Text = UIntToStr(static_cast<unsigned int>(clickIntervalMS));
            }

            TimerClick->Interval = clickIntervalMS;
            TimerClick->Enabled = true;
            SetFormToProcessStarted();
        }
    }
    catch (...)
    {
        showInvalidValueMsg = true;
    }

    if (showInvalidValueMsg)
    {
        MsgDlg("Please enter a time value greater than zero.",
            TFrmMain::AppFriendlyName, TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
        SetFormToProcessStopped();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::BtnStopClick(TObject* /*sender*/)
{
    DoStop();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::CB_MC_UnlimitedClick(TObject* /*sender*/)
{
    Edit_MaxClicks->Enabled = !CB_MC_Unlimited->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::CBoxHoldIntervalChange(TObject*/*sender*/)
{
    UpdateEditFromIntervalCBoxSelection(CBoxHoldInterval, EditHoldInterval);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::CBoxTimeFrameChange(TObject* /*sender*/)
{
    UpdateEditFromIntervalCBoxSelection(CBoxTimeFrame, EditTimeValue);
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::CreateParams(Vcl::Controls::TCreateParams& params)
{
    TForm::CreateParams(params); // inherited
    //params.ExStyle = params.ExStyle | WS_EX_NOACTIVATE;
}
//---------------------------------------------------------------------------
void TFrmMain::DoStop()
{
    TimerClick->Enabled = false;
    SetFormToProcessStopped();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormCloseQuery(TObject* /*sender*/, bool& /*canClose*/)
{
    TimerClick->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormCreate(TObject* /*sender*/)
{
    InitializeKeyEventHook();
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormDestroy(TObject* /*sender*/)
{
    ShutdownKeyEventHook();
}
//---------------------------------------------------------------------------
DWORD TFrmMain::GetClickIntervalMS()
{
    return GetIntervalMS(CBoxTimeFrame, EditTimeValue);
}
//---------------------------------------------------------------------------
DWORD TFrmMain::GetHoldClickIntervalMS()
{
    return GetIntervalMS(CBoxHoldInterval, EditHoldInterval);
}
//---------------------------------------------------------------------------
DWORD TFrmMain::GetIntervalMS(TComboBox* cBox, TEdit* edit)
{
    double timeValDbl = edit->Text.ToDouble();
    if (timeValDbl <= 0.0)
        return 0;

    DWORD intervalMS = 0;

    if (cBox->ItemIndex == 0)
        intervalMS = static_cast<DWORD>(timeValDbl);
    else if (cBox->ItemIndex == 1)
        intervalMS = static_cast<DWORD>(timeValDbl * 1000.0);
    else if (cBox->ItemIndex == 2)
        intervalMS = static_cast<DWORD>(timeValDbl * 60.0 * 1000.0);

    return intervalMS;
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
// see: https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms644985(v=vs.85)
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

    //Note: Can also return 1 instead of calling the next hook to eat the key event.
    return ::CallNextHookEx(NULL, nCode, wParam, lParam);
}
//---------------------------------------------------------------------------
void TFrmMain::SetFormToProcessStarted()
{
    BtnStart->Enabled = false;
    BtnStop->Enabled = true;
    BtnAbout->Enabled = false;

    CBoxTimeFrame->Enabled = false;
    EditTimeValue->Enabled  = false;

    CBoxHoldInterval->Enabled = false;
    EditHoldInterval->Enabled  = false;

    GBoxMouse->Enabled = false;
    GBoxMaxClicks->Enabled = false;
    GBoxHotKey->Enabled = false;
}
//---------------------------------------------------------------------------
void TFrmMain::SetFormToProcessStopped()
{
    BtnStart->Enabled = true;
    BtnStop->Enabled = false;
    BtnAbout->Enabled = true;

    CBoxTimeFrame->Enabled = true;
    EditTimeValue->Enabled  = true;

    CBoxHoldInterval->Enabled = true;
    EditHoldInterval->Enabled  = true;

    GBoxMouse->Enabled = true;
    GBoxMaxClicks->Enabled = true;
    GBoxHotKey->Enabled = true;
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
void __fastcall TFrmMain::TimerClickTimer(TObject* /*sender*/)
{
    bool doStop = false;
    DWORD clickUpDelayMS = GetHoldClickIntervalMS();
    long long maxClicks = static_cast<long long>(Edit_MaxClicks->Text.ToDouble());

    if (!CB_MC_Unlimited->Checked && ClickCount >= maxClicks)
    {   // Shouldn't get here, unless maybe the click intervals are REALLY short
        DoStop();
        return;
    }

    if (TimerClick->Interval < clickUpDelayMS)
        TimerClick->Interval = clickUpDelayMS;

    ClickCount++;

    if (!CB_MC_Unlimited->Checked && ClickCount == maxClicks)
    {
        TimerClick->Enabled = false;
        doStop = true;
    }

    if (RB_MouseLeft->Checked)
        TMouseTool::MouseLeftClick(clickUpDelayMS);
    else
        TMouseTool::MouseRightClick(clickUpDelayMS);

    UpdateStatusPanel_Clicks();

    if (doStop)
        DoStop();
}
//---------------------------------------------------------------------------
void TFrmMain::UpdateEditFromIntervalCBoxSelection(TComboBox* cBox, TEdit* edit)
{
    try
    {
        double timeValue = edit->Text.ToDouble();

        if (cBox->Tag == 0)
        {
            if (cBox->ItemIndex == 1)
                timeValue /= 1000.0;
            else if (cBox->ItemIndex == 2)
                timeValue /= (1000.0 * 60.0);
        }
        else if (cBox->Tag == 1)
        {
            if (cBox->ItemIndex == 0)
                timeValue *= 1000.0;
            else if (cBox->ItemIndex == 2)
                timeValue /= 60.0;
        }
        else if (cBox->Tag == 2)
        {
            if (cBox->ItemIndex == 0)
                timeValue *= (60.0 * 1000.0);
            else if (cBox->ItemIndex == 1)
                timeValue *= 60.0;
        }

        edit->Text = FloatToStr(timeValue);
    }
    catch (...)
    {

    }

    cBox->Tag = cBox->ItemIndex;
}
//---------------------------------------------------------------------------
void TFrmMain::UpdateStatusPanel_Clicks()
{
    StatusBar1->Panels->Items[0]->Text = String("Clicks: ") + IntToStr(ClickCount);
}
//---------------------------------------------------------------------------
