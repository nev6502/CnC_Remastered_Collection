//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection

/***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Westwood 32 bit Library                                      *
 *                                                                                             *
 *                    File Name : MOUSE.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Philip W. Gorrow                                             *
 *                                                                                             *
 *                   Start Date : 12/12/95                                                     *
 *                                                                                             *
 *                  Last Update : December 12, 1995 [PWG]                                      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   WWMouseClass::WWMouseClass -- Constructor for the Mouse Class                                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "imgui.h"
#include "../FUNCTION.H"
#include "MOUSE.H"
#include "../image.h"

static WWMouseClass *_Mouse=NULL;
void CALLBACK Process_Mouse( UINT event_id, UINT res1 , DWORD user, DWORD  res2, DWORD  res3 );

extern bool GameInFocus;
extern UserInputClass UserInput;

/***********************************************************************************************
 * MOUSECLASS::MOUSECLASS -- Constructor for the Mouse Class                                   *
 *                                                                                             *
 * INPUT:		GraphicViewPortClass * screen - pointer to screen mouse is created for				 *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/12/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
WWMouseClass::WWMouseClass(GraphicViewPortClass *scr, int mouse_max_width, int mouse_max_height)
{
	MouseCursor 	= new char[mouse_max_width * mouse_max_height];
	MouseXHot		= 0;
	MouseYHot		= 0;
	CursorWidth		= 0;
	CursorHeight	= 0;

	MouseBuffer		= new char[mouse_max_width * mouse_max_height];
	MouseBuffX		= -1;
	MouseBuffY  	= -1;
	MaxWidth			= mouse_max_width;
	MaxHeight		= mouse_max_height;

	MouseCXLeft		= 0;
	MouseCYUpper	= 0;
	MouseCXRight	= 0;
	MouseCYLower	= 0;
	MCFlags			= 0;
	MCCount			= 0;

	Screen			= scr;
	PrevCursor		= NULL;
	MouseUpdate		= 0;
	State				= 1;
	timeBeginPeriod ( 1000/ 60);

	InitializeCriticalSection (&MouseCriticalSection);
	//
	// Install the timer callback event handler
	//

	EraseBuffer		= new char[mouse_max_width * mouse_max_height];
	EraseBuffX		= -1;
	EraseBuffY  	= -1;
	EraseBuffHotX	= -1;
	EraseBuffHotY	= -1;
	EraseFlags		= FALSE;

	_Mouse			= this;
	
	// Add TIME_KILL_SYNCHRONOUS flag. ST - 2/13/2019 5:07PM
	//TimerHandle = timeSetEvent( 1000/60 , 1 , ::Process_Mouse, 0 , TIME_PERIODIC);
	//TimerHandle = timeSetEvent( 1000/60 , 1 , ::Process_Mouse, 0 , TIME_PERIODIC | TIME_KILL_SYNCHRONOUS);		// Removed. ST - 2/13/2019 5:12PM

	/*
	** Force the windows mouse pointer to stay withing the graphic view port region
	*/
	Set_Cursor_Clip();

	for (int i = 0; i < MAX_CURSOR_FRAMES; i++) {
		char mouseFrameName[512];
		sprintf(mouseFrameName, "ui/mouse/mouse-%05d.png", i);
		
		cursor_frames[i] = Image_LoadImage(mouseFrameName);
	}

	isMouseVisible = false;
}

WWMouseClass::~WWMouseClass()
{
	MouseUpdate++;

	if (MouseCursor) delete[] MouseCursor;
	if (MouseBuffer) delete[] MouseBuffer;
	if (TimerHandle) {
		timeKillEvent(TimerHandle);
		TimerHandle = 0;		//ST - 2/13/2019 5:12PM
	}
	timeEndPeriod (1000/60);
	DeleteCriticalSection(&MouseCriticalSection);

	/*
	** Free up the windows mouse pointer movement
	*/
	Clear_Cursor_Clip();
}


void Block_Mouse(GraphicBufferClass *buffer)
{
	if (_Mouse){
		_Mouse->Block_Mouse(buffer);
	}
}


void Unblock_Mouse(GraphicBufferClass *buffer)
{
	if (_Mouse){
		_Mouse->Unblock_Mouse(buffer);
	}
}



void WWMouseClass::Block_Mouse(GraphicBufferClass *buffer)
{
	if (buffer == Screen->Get_Graphic_Buffer()){
		EnterCriticalSection(&MouseCriticalSection);
	}
}


void WWMouseClass::Unblock_Mouse(GraphicBufferClass *buffer)
{
	if (buffer == Screen->Get_Graphic_Buffer()){
		LeaveCriticalSection(&MouseCriticalSection);
	}
}





void WWMouseClass::Set_Cursor_Clip(void)
{
#if (0)		// Not needed. ST - 1/3/2019 2:18PM
	if (Screen){
		RECT	region;

		region.left		= 0;
		region.top 		= 0;
		region.right	= Screen->Get_Width();
		region.bottom	= Screen->Get_Height();

		ClipCursor(&region);
	}
#endif
}



void WWMouseClass::Clear_Cursor_Clip(void)
{
#if (0)
	ClipCursor(NULL);
#endif
}



void WWMouseClass::Process_Mouse(void)
{
	
}

void *WWMouseClass::Set_Cursor(int xhotspot, int yhotspot, int cursor)
{
	Set_Hotspot_X(xhotspot);
	Set_Hotspot_Y(yhotspot);

	if (cursor >= MAX_CURSOR_FRAMES) {
		assert(!"Max_Cursor_Frames out of range!");
	}

	current_cursor = cursor_frames[cursor];
	return NULL;
}

void WWMouseClass::Low_Hide_Mouse()
{
	//if (!State) {
		if (MouseBuffX != -1 || MouseBuffY != -1) {
			if (Screen->Lock()){
				Mouse_Shadow_Buffer(*this, *Screen, MouseBuffer, MouseBuffX, MouseBuffY, MouseXHot, MouseYHot, 0);
				Screen->Unlock();
			}
		}
		MouseBuffX = -1;
		MouseBuffY = -1;
	//}

	State++;
}
void WWMouseClass::Hide_Mouse()
{
	MouseUpdate++;
	//Low_Hide_Mouse();
	isMouseVisible = false;
	MouseUpdate--;
}

void WWMouseClass::RenderMouse(void) {
	if( this == NULL || !isMouseVisible)
		return;

	//ImVec2 size(current_cursor->width, current_cursor->height);
	// Image((ImTextureID)current_cursor->image, size);
	int _my = UserInput.Mouse.Y + (current_cursor->height * 0.25);
	int xstart = UserInput.Mouse.X - Get_Hotspot_X();
	int ystart = _my - Get_Hotspot_Y();
	int yend = current_cursor->height + ystart - 1;
	int xend = current_cursor->width + xstart - 1;

	ImVec2 mi(xstart, ystart);
	ImVec2 ma(xend, yend);
	
	
	ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	ImGui::GetForegroundDrawList()->AddImage((ImTextureID)current_cursor->HouseImages[0].image[0][0], mi, ma);
}

void WWMouseClass::Low_Show_Mouse(int x, int y)
{

}

void WWMouseClass::Show_Mouse()
{
	isMouseVisible = true;
}

void WWMouseClass::Conditional_Hide_Mouse(int x1, int y1, int x2, int y2)
{
	POINT	pt;

	MouseUpdate++;

	//
	// First of all, adjust all the coordinates so that they handle
	// the fact that the hotspot is not necessarily the upper left
	// corner of the mouse.
	//
	x1 -= (CursorWidth - MouseXHot);
	x1  = MAX(0, x1);
	y1 -= (CursorHeight - MouseYHot);
	y1  = MAX(0, y1);
	x2  += MouseXHot;
	x2  = MIN(x2, Screen->Get_Width());
	y2  += MouseYHot;
	y2  = MIN(y2, Screen->Get_Height());

	// The mouse could be in one of four conditions.
	// 1) The mouse is visible and no conditional hide has been specified.
	// 	(perform normal region checking with possible hide)
	// 2) The mouse is hidden and no conditional hide as been specified.
	// 	(record region and do nothing)
	// 3) The mouse is visible and a conditional region has been specified
	// 	(expand region and perform check with possible hide).
	// 4) The mouse is already hidden by a previous conditional.
	// 	(expand region and do nothing)
	//
	// First: Set or expand the region according to the specified parameters
	if (!MCCount) {
		MouseCXLeft		= x1;
		MouseCYUpper	= y1;
		MouseCXRight	= x2;
		MouseCYLower	= y2;
	} else {
		MouseCXLeft		= MIN(x1, MouseCXLeft);
		MouseCYUpper	= MIN(y1, MouseCYUpper);
		MouseCXRight	= MAX(x2, MouseCXRight);
		MouseCYLower	= MAX(y2, MouseCYLower);
	}
	//
	// If the mouse isn't already hidden, then check its location against
	// the hiding region and hide if necessary.
	//
	if (!(MCFlags & CONDHIDDEN)) {
		GetCursorPos(&pt);
		if (MouseBuffX >= MouseCXLeft && MouseBuffX <= MouseCXRight && MouseBuffY >= MouseCYUpper && MouseBuffY <= MouseCYLower) {
			Low_Hide_Mouse();
			MCFlags |= CONDHIDDEN;
		}
	}
	//
	// Record the fact that a conditional hide was called and then exit
	//
	//
	MCFlags |= CONDHIDE;
	MCCount++;
	MouseUpdate--;

}
void WWMouseClass::Conditional_Show_Mouse(void)
{
	MouseUpdate++;

	//
	// if there are any nested hides then dec the count
	//
	if (MCCount) {
		MCCount--;
		//
		// If the mouse is now not hidden and it had actually been
		// hidden before then display it.
		//
		if (!MCCount) {
			if (MCFlags & CONDHIDDEN) {
				Show_Mouse();
			}
			MCFlags = 0;
		}
	}

	MouseUpdate--;
}


void WWMouseClass::Draw_Mouse(GraphicViewPortClass *scr)
{
	scr;
	return;
//ST - 1/3/2019 10:50AM	
#if (0)
	
	POINT pt;

	if (State != 0) return;
	MouseUpdate++;
	//
	//	Get the position that the mouse is currently located at
	//
	GetCursorPos(&pt);
	if (MCFlags & CONDHIDE && pt.x >= MouseCXLeft && pt.x <= MouseCXRight && pt.y >= MouseCYUpper && pt.y <= MouseCYLower) {
		Hide_Mouse();
		MCFlags |= CONDHIDDEN;
	} else {
		//
		// If the mouse is already visible then just ignore the problem.
		//
		EraseFlags = TRUE;

		//
		// Try to lock the screen  - dont do video stuff if we cant.
		//
		if (scr->Lock()){
			//
			// Save off the area behind the mouse into two different buffers, one
			// which will be used to restore the mouse and the other which will
			// be used to restore the hidden surface when we get a chance.
			//
			Mouse_Shadow_Buffer(this, scr, EraseBuffer, pt.x, pt.y, MouseXHot, MouseYHot, 1);
			memcpy(MouseBuffer, EraseBuffer, MaxWidth * MaxHeight);
			//
			// Draw the mouse in its new location
			//
			::Draw_Mouse(this, scr, pt.x, pt.y);
			//
			// Save off the positions that we saved the buffer from
			//
			EraseBuffX 		= pt.x;
			MouseBuffX 		= pt.x;
			EraseBuffY 		= pt.y;
			MouseBuffY 		= pt.y;
			EraseBuffHotX	= MouseXHot;
			EraseBuffHotY	= MouseYHot;
			//
			// Unlock the screen and lets get moving.
			//
			scr->Unlock();
		}
	}

	MouseUpdate--;
#endif
}

void WWMouseClass::Erase_Mouse(GraphicViewPortClass *scr, int forced)
{
//ST - 1/3/2019 10:50AM	
	scr;
	forced;
	return;
#if (0)
	//
	// If we are forcing the redraw of a mouse we already managed to
	// restore then just get outta here.
	//
	if (forced && EraseBuffX == -1 && EraseBuffY == -1) return;

	MouseUpdate++;

	//
	// If this is not a forced call, only update the mouse is we can legally
	//	lock the buffer.
	//
	if (!forced) {
#if(0)
		if (scr->Lock()) {
			//
			// If the surface has not already been restore then restore it and erase the
			// restoration coordinates so we don't accidentally do it twice.
			//
			if (EraseBuffX != -1 || EraseBuffY != -1) {
				Mouse_Shadow_Buffer(this, scr, EraseBuffer, EraseBuffX, EraseBuffY, 0);
				EraseBuffX = -1;
				EraseBuffY = -1;
			}
			//
			// We are done writing to the buffer so unlock it.
			//
			scr->Unlock();
		}
#endif
	} else	{
			//
			// If the surface has not already been restore then restore it and erase the
			// restoration coordinates so we don't accidentally do it twice.
			//
			if (EraseBuffX != -1 || EraseBuffY != -1) {
				if (scr->Lock()){
					Mouse_Shadow_Buffer(this, scr, EraseBuffer, EraseBuffX, EraseBuffY, EraseBuffHotX, EraseBuffHotY, 0);
					scr->Unlock();
				}
				EraseBuffX = -1;
				EraseBuffY = -1;
			}
	}
	MouseUpdate--;
  	EraseFlags = FALSE;
#endif
}

int WWMouseClass::Get_Mouse_State(void)
{
	return 0;//(State);
}
/***********************************************************************************************
 * WWKeyboardClass::Get_Mouse_X -- Returns the mouses current x position in pixels             *
 *                                                                                             *
 * INPUT:		none                                                                            *
 *                                                                                             *
 * OUTPUT:     int		- returns the mouses current x position in pixels                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
int WWMouseClass::Get_Mouse_X(void)
{
	return UserInput.Mouse.X;
}


/***********************************************************************************************
 * WWKeyboardClass::Get_Mouse_Y -- returns the mouses current y position in pixels             *
 *                                                                                             *
 * INPUT:		none                                                                            *
 *                                                                                             *
 * OUTPUT:     int		- returns the mouses current y position in pixels                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
int WWMouseClass::Get_Mouse_Y(void)
{
	return UserInput.Mouse.Y;
}

/***********************************************************************************************
 * WWKeyboardClass::Get_Mouse_XY -- Returns the mouses x,y position via reference vars         *
 *                                                                                             *
 * INPUT:		int &x		- variable to return the mouses x position in pixels                *
 *					int &y		- variable to return the mouses y position in pixels					  *
 *                                                                                             *
 * OUTPUT:     none - output is via reference variables                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void WWMouseClass::Get_Mouse_XY(int &x, int &y)
{	
	x = UserInput.Mouse.X;
	y = UserInput.Mouse.Y;
}

//#pragma off(unreferenced)

void CALLBACK Process_Mouse( UINT event_id, UINT res1 , DWORD user, DWORD  res2, DWORD  res3 )
{
	static	BOOL	in_mouse_callback = false;

	if (_Mouse && !in_mouse_callback) {
		in_mouse_callback = TRUE;
		_Mouse->Process_Mouse();
		in_mouse_callback = FALSE;
	}
}
//#pragma on(unreferenced)

void Hide_Mouse(void)
{
	if (!_Mouse) return;
	_Mouse->Hide_Mouse();
}

void Show_Mouse(bool clearPrevious)
{
	if (!_Mouse) return;

	if(clearPrevious)
		_Mouse->Hide_Mouse();

	_Mouse->Show_Mouse();
}

void Conditional_Hide_Mouse(int x1, int y1, int x2, int y2)
{
	if (!_Mouse) return;
	_Mouse->Conditional_Hide_Mouse(x1, y1, x2, y2);
}

void Conditional_Show_Mouse(void)
{
	if (!_Mouse) return;
	_Mouse->Conditional_Show_Mouse();
}

int Get_Mouse_State(void)
{
	if (!_Mouse) return(0);
	return(_Mouse->Get_Mouse_State());
}

void *Set_Mouse_Cursor(int hotx, int hoty, int cursor)
{
	if (!_Mouse) return(0);
	return(_Mouse->Set_Cursor(hotx,hoty,cursor));
}

extern int DLLForceMouseX;
extern int DLLForceMouseY;

int Get_Mouse_X(void)
{
	return UserInput.Mouse.X;
}

int Get_Mouse_Y(void)
{
	return UserInput.Mouse.Y;
}