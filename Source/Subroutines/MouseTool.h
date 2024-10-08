//---------------------------------------------------------------------------
// MouseTool.h
// Author: Anthony West - ASW Software
//
// Contains routines for manipulating the mouse.
//
// Every attempt should be made to keep this module at least Windows portable.
//---------------------------------------------------------------------------

#ifndef MouseToolH
#define MouseToolH
//---------------------------------------------------------------------------

namespace Subroutines
{

/////////////////////////////////////////////////////////////////////////////
// TMouseTool class
/////////////////////////////////////////////////////////////////////////////
class TMouseTool
{
typedef TMouseTool ThisType;

public:
    ////// only const static variables allowed in this public - see second public for regular stuff //////

    static const int Click_DefaultUpDelayMS = 10;

protected:
    ////// static variables //////

    ////// variables //////

    ////// functions //////

private:
    ////// static variables //////

    ////// variables //////

    ////// functions //////
    void Destroy_Private();
    bool Reset_Private();

public:
    TMouseTool();
    ~TMouseTool();

    ////// variables //////

    ////// functions //////
    virtual void Destroy(); //calls Destroy_Private()
    virtual bool Reset(); //calls Reset_Private() - It is good practice for the constructor to never call a virtual function

    static bool MouseLeftClick(int clickUpDelayMS=Click_DefaultUpDelayMS);
    static bool MouseRightClick(int clickUpDelayMS=Click_DefaultUpDelayMS);
    static bool MoveMouse(int x, int y);
};

} // namespace Subroutines

#endif // #ifndef MouseToolH
