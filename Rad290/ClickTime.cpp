//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("..\MainForm.cpp", FrmMain);
USEFORM("..\AboutForm.cpp", FrmAbout);
//---------------------------------------------------------------------------
#include "MainForm.h"
#include "PathTool.h"
#include "AppTool.h"

using namespace Subroutines;
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    const wchar_t *codeSectStr = L"_tWinMain";
    //_TCHAR** argvP = argv; //for _tmain //_TCHAR** argvP = __targv; //for _tWinMain
    _TCHAR** argvP = __targv;
    int nArgs = __argc;//argc;
    unsigned short major = 0, minor = 0, build = 0, revision = 0;
    const size_t buffer_t_size = 999;
    TCHAR buffer_t[buffer_t_size];
    std::wstring exePathW;

    try
    {
        //get full path, not local path, and not DOS path either
        exePathW = TAppTool::GetAppPathW();

        if (exePathW.length() == 0)
            exePathW = argvP[0]; //fall back to local

        if (TAppTool::GetAppVersion(exePathW.c_str(), &major, &minor, &build, &revision))
            TFrmMain::AppVersion.SetVersion(major, minor, build, revision);

        //set up dir and file locations
        TFrmMain::DirApp = TPathTool::File_ExtractDir(exePathW).c_str();
        TFrmMain::FileSettingsINI = TPathTool::Path_Combine(TFrmMain::DirApp.w_str(), TFrmMain::FileSettingsINI.w_str());

        //load ini (future)

        Application->Initialize();
        Application->MainFormOnTaskBar = true;
        Application->Title = "ClickTime";
        TStyleManager::TrySetStyle("Windows10 SlateGray");
        Application->CreateForm(__classid(TFrmMain), &FrmMain);
        Application->CreateForm(__classid(TFrmAbout), &FrmAbout);
        Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}

//---------------------------------------------------------------------------

