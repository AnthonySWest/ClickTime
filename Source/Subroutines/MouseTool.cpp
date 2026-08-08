/* **************************************************************************
MouseTool.cpp
Author: Anthony S. West - ASW Software

See header for info.

Copyright 2023 Anthony S. West

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

************************************************************************** */

//---------------------------------------------------------------------------
// Module header
#include "MouseTool.h"
//---------------------------------------------------------------------------
#ifdef USE_ELOG
    #include "EasyLogger.h"
#endif

//---------------------------------------------------------------------------

namespace Subroutines
{

/////////////////////////////////////////////////////////////////////////////
// TMouseTool
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TMouseTool::TMouseTool()
{
    Reset_Private();
}
//---------------------------------------------------------------------------
TMouseTool::~TMouseTool()
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
void TMouseTool::Destroy_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
}
//---------------------------------------------------------------------------
void TMouseTool::Destroy() //virtual
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
bool TMouseTool::Reset_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
    Destroy_Private();

    //reset class vars here

    return true;
}
//---------------------------------------------------------------------------
bool TMouseTool::Reset() //virtual
{
    return Reset_Private();
}
//---------------------------------------------------------------------------
// -Static
// -clickUpDelayMS is how long, in milliseconds, to wait before sending the up click.
//  Delay defaults to Click_DefaultUpDelayMS.
// -Returns false for failure. Call Windows GetLastError() for reason.
bool TMouseTool::MouseLeftClick(DWORD clickUpDelayMS)
{
    bool result = true;
    INPUT mInput;
    UINT nEventsInserted;

    mInput.type = INPUT_MOUSE;
    mInput.mi.mouseData = 0;
    mInput.mi.time = 0;
    mInput.mi.dwExtraInfo = 0;

    //send down
    mInput.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN);
    nEventsInserted = ::SendInput(1, &mInput, sizeof(mInput));

    if (0 == nEventsInserted)
        result = false;

    ::Sleep(clickUpDelayMS);

    //send up
    if (result)
    {
        mInput.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
        nEventsInserted = ::SendInput(1, &mInput, sizeof(INPUT));

        if (0 == nEventsInserted)
            result = false;
    }

    return result;
}
//---------------------------------------------------------------------------
// -Static
// -clickUpDelayMS is how long, in milliseconds, to wait before sending the up click.
//  Delay defaults to Click_DefaultUpDelayMS.
// -Returns false for failure. Call Windows GetLastError() for reason.
bool TMouseTool::MouseRightClick(DWORD clickUpDelayMS)
{
    bool result = true;
    INPUT mInput;
    UINT nEventsInserted;

    mInput.type = INPUT_MOUSE;
    mInput.mi.mouseData = 0;
    mInput.mi.time = 0;
    mInput.mi.dwExtraInfo = 0;

    //send down
    mInput.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN);
    nEventsInserted = ::SendInput(1, &mInput, sizeof(INPUT));

    if (0 == nEventsInserted)
        result = false;

    ::Sleep(clickUpDelayMS);

    //send up
    if (result)
    {
        mInput.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP);
        nEventsInserted = ::SendInput(1, &mInput, sizeof(INPUT));

        if (0 == nEventsInserted)
            result = false;
    }

    return result;
}
//---------------------------------------------------------------------------
// -Static
// -Returns false for failure. Call Windows GetLastError() for reason.
bool TMouseTool::MoveMouse(int x, int y)
{
    bool result = true;
    INPUT mInput;
    UINT nEventsInserted;

    //tell Windows we ar a mouse
    mInput.type = INPUT_MOUSE;

    //set up mouse info
    mInput.mi.mouseData = 0;
    mInput.mi.time = 0;
    mInput.mi.dwExtraInfo = 0;

    //set mouse position
    mInput.mi.dx = x * (0xFFFF / ::GetSystemMetrics(SM_CXSCREEN));
    mInput.mi.dy = y * (0xFFFF / ::GetSystemMetrics(SM_CYSCREEN));

    //tell windows the operation to perform and how
    mInput.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK;

    //do the mouse move
    nEventsInserted = ::SendInput(1, &mInput, sizeof(INPUT));

    if (0 == nEventsInserted)
        result = false;

    return result;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

} // namespace Subroutines
