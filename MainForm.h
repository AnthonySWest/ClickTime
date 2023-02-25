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
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall BtnStartClick(TObject *Sender);
    void __fastcall TimerClickTimer(TObject *Sender);
    void __fastcall BtnStopClick(TObject *Sender);
    void __fastcall CBoxTimeFrameChange(TObject *Sender);
private:	// User declarations

    // /////////// Static vars go here ////////////////////////////

	// /////////// Non-static vars go here ////////////////////////////

	// /////////// Functions go here ////////////////////////////

	virtual void __fastcall CreateParams(Vcl::Controls::TCreateParams &params);
//	void MoveMouse(int x, int y);
	void MouseLeftClick();
//	void MouseRightClick();


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

	// /////////// Properties go here ////////////////////////////

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;

int MsgDlg(const UnicodeString &msg, const UnicodeString &title, TMsgDlgType dlgType, TMsgDlgButtons buttons);
//---------------------------------------------------------------------------
#endif
