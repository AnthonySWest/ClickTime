/* **************************************************************************
AboutForm.h
Author: Anthony S. West - ASW Software

Displays the version, copyright, and license to the user.

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

#ifndef AboutFormH
#define AboutFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// TFrmAbout
/////////////////////////////////////////////////////////////////////////////
class TFrmAbout : public TForm
{
__published: // IDE-managed Components
    TBitBtn *BtnClose;
    TLabel *Label1;
    TLabel *LblVersion;
    TMemo *MemoAbout;
    TLabel *Label2;
private: // User declarations
public: // User declarations
    __fastcall TFrmAbout(TComponent* owner);
};

extern PACKAGE TFrmAbout *FrmAbout;

//---------------------------------------------------------------------------
#endif // #ifndef AboutFormH
