/* **************************************************************************
MainForm.h
Author: Anthony S. West - ASW Software

Contains the UI for controlling the click configuration.

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

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
// END OF IDE INCLUDES
//---------------------------------------------------------------------------
#include "Version.h"
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// TFrmMain
/////////////////////////////////////////////////////////////////////////////
class TFrmMain : public TForm
{
__published: // IDE-managed Components
    TBitBtn* BtnStart;
    TTimer* TimerClick;
    TBitBtn* BtnStop;
    TComboBox* CBoxTimeFrame;
    TLabel* LblClickInterval;
    TEdit* EditTimeValue;
    TBitBtn* BtnAbout;
    TBitBtn* BtnExit;
    TStatusBar* StatusBar1;
    TGroupBox* GBoxMouse;
    TRadioButton* RB_MouseLeft;
    TRadioButton* RB_MouseRight;
    TGroupBox* GBoxHotKey;
    TCheckBox* CB_HK_Shift;
    TCheckBox* CB_HK_Ctrl;
    TCheckBox* CB_HK_Alt;
    TComboBox* CBox_HK_FKey;
    TLabel* LblHoldInterval;
    TComboBox* CBoxHoldInterval;
    TEdit* EditHoldInterval;
    TGroupBox* GBoxMaxClicks;
    TCheckBox* CB_MC_Unlimited;
    TEdit* Edit_MaxClicks;
    void __fastcall BtnAboutClick(TObject* Sender);
    void __fastcall BtnExitClick(TObject* Sender);
    void __fastcall BtnStartClick(TObject* Sender);
    void __fastcall BtnStopClick(TObject* Sender);
    void __fastcall CB_MC_UnlimitedClick(TObject* Sender);
    void __fastcall CBoxHoldIntervalChange(TObject* Sender);
    void __fastcall CBoxTimeFrameChange(TObject* Sender);
    void __fastcall FormCloseQuery(TObject* Sender, bool& CanClose);
    void __fastcall FormCreate(TObject* Sender);
    void __fastcall FormDestroy(TObject* Sender);
    void __fastcall TimerClickTimer(TObject* Sender);

private: // User declarations
    HHOOK WinKeyEventHook;

    bool AltKeyDown;
    bool CtrlKeyDown;
    bool ShiftKeyDown;

    long long ClickCount;

public:
    static AnsiString AppFriendlyName;
    static AnsiString CompanyName;

    static UnicodeString DirApp;

    static UnicodeString FileMainEXE;
    static UnicodeString FileSettingsINI;

    static UnicodeString LogID;
    static Subroutines::TVersion AppVersion;

    static unsigned short AppStart_Day, AppStart_Month, AppStart_Year;
    static System::Word AppStart_Hour, AppStart_Min, AppStart_Sec, AppStart_MSec;

public:
    static DWORD GetIntervalMS(TComboBox* cBox, TEdit* edit);
    static DWORD GetShiftStateMask(bool shift, bool ctrl, bool alt);
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static void UpdateEditFromIntervalCBoxSelection(TComboBox* cBox, TEdit* edit);

private:
    virtual void __fastcall CreateParams(Vcl::Controls::TCreateParams& params);

    DWORD GetClickIntervalMS();
    DWORD GetHoldClickIntervalMS();
    void InitializeKeyEventHook();
    void ShutdownKeyEventHook();
    void UpdateStatusPanel_Clicks();

public: // User declarations
    __fastcall TFrmMain(TComponent* owner);

    void DoStop();
    void SetFormToProcessStarted();
    void SetFormToProcessStopped();
};

extern PACKAGE TFrmMain* FrmMain;

//---------------------------------------------------------------------------

int MsgDlg(const UnicodeString& msg, const UnicodeString& title, TMsgDlgType dlgType, TMsgDlgButtons buttons);

//---------------------------------------------------------------------------
#endif // #ifndef MainFormH
