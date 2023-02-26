//---------------------------------------------------------------------------

#ifndef AboutFormH
#define AboutFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFrmAbout : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *BtnClose;
    TLabel *Label1;
    TLabel *LblVersion;
    TMemo *MemoAbout;
    TLabel *Label2;
private:	// User declarations
public:		// User declarations
    __fastcall TFrmAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmAbout *FrmAbout;
//---------------------------------------------------------------------------
#endif
