//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "Version.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *BtnStart;
    TTimer *TimerClick;
    TBitBtn *BtnStop;
    TComboBox *CBoxTimeFrame;
    TLabel *Label1;
    TEdit *EditTimeValue;
    TBitBtn *BtnAbout;
    TBitBtn *BtnExit;
    TStatusBar *StatusBar1;
    TGroupBox *GBoxMouse;
    TRadioButton *RB_MouseLeft;
    TRadioButton *RB_MouseRight;
    TGroupBox *GroupBox1;
    TCheckBox *CB_HK_Shift;
    TCheckBox *CB_HK_Ctrl;
    TCheckBox *CB_HK_Alt;
    TComboBox *CBox_HK_FKey;
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall BtnStartClick(TObject *Sender);
    void __fastcall TimerClickTimer(TObject *Sender);
    void __fastcall BtnStopClick(TObject *Sender);
    void __fastcall CBoxTimeFrameChange(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall BtnAboutClick(TObject *Sender);
    void __fastcall BtnExitClick(TObject *Sender);
private:	// User declarations

    // /////////// Static vars go here ////////////////////////////

	// /////////// Non-static vars go here ////////////////////////////

    HHOOK WinKeyEventHook;

    bool ShiftKeyDown;
    bool CtrlKeyDown;
    bool AltKeyDown;

    long long ClickCount;

	// /////////// Functions go here ////////////////////////////

	virtual void __fastcall CreateParams(Vcl::Controls::TCreateParams &params);
	
    void InitializeKeyEventHook();
	void ShutdownKeyEventHook();


public:		// User declarations
    __fastcall TFrmMain(TComponent* Owner);

    // /////////// Static vars go here ////////////////////////////

    static AnsiString AppFriendlyName;
    static AnsiString CompanyName;

    static UnicodeString DirApp;

    static UnicodeString FileMainEXE;
    static UnicodeString FileSettingsINI;

	static UnicodeString LogID;
    static Subroutines::TVersion AppVersion;

    static unsigned short AppStart_Day, AppStart_Month, AppStart_Year;
	static System::Word AppStart_Hour, AppStart_Min, AppStart_Sec, AppStart_MSec;

    // /////////// Non-static vars go here ////////////////////////////


	// /////////// Functions go here ////////////////////////////
    void SetFormToProcessStarted();
    void SetFormToProcessStopped();

    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static DWORD GetShiftStateMask(bool shift, bool ctrl, bool alt);

	// /////////// Properties go here ////////////////////////////

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;

int MsgDlg(const UnicodeString &msg, const UnicodeString &title, TMsgDlgType dlgType, TMsgDlgButtons buttons);

//---------------------------------------------------------------------------
#endif
