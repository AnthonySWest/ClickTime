/* **************************************************************************
MouseTool.h
Author: Anthony S. West - ASW Software

Contains routines for manipulating the mouse.

Every attempt should be made to keep this module at least Windows portable.

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

#ifndef MouseToolH
#define MouseToolH
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------

namespace Subroutines
{

/////////////////////////////////////////////////////////////////////////////
// TMouseTool
/////////////////////////////////////////////////////////////////////////////
class TMouseTool
{
private:
    typedef TMouseTool ThisType;

public:
    static const DWORD Click_DefaultUpDelayMS = 10;

public:
    static bool MouseLeftClick(DWORD clickUpDelayMS = Click_DefaultUpDelayMS);
    static bool MouseRightClick(DWORD clickUpDelayMS = Click_DefaultUpDelayMS);
    static bool MoveMouse(int x, int y);

private:
    void Destroy_Private();
    bool Reset_Private();

public:
    TMouseTool();
    ~TMouseTool();

    virtual void Destroy(); //calls Destroy_Private()
    virtual bool Reset(); //calls Reset_Private() - It is good practice for the constructor to never call a virtual function
};

} // namespace Subroutines

#endif // #ifndef MouseToolH
