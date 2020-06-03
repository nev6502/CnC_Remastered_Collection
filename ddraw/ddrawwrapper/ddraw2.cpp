#pragma comment(linker, "/EXPORT:DirectDrawCreate=_DirectDrawCreate@12")

#include "DirectDrawWrapper.h"
//#include "resource.h"
//#include "detours.h"
#include <stdio.h>
#include <math.h>

// Main thread ddrawwrapper object
IDirectDrawWrapper *lpDD = NULL;
// Original setcursorpos function pointer
static BOOL (WINAPI *TrueSetCursorPos)(int,int) = SetCursorPos;
static HMODULE (WINAPI *TrueLoadLibraryA)(LPCSTR) = GetModuleHandleA;

// Are we in the settings menu
BOOL inMenu;
// Dll start time
DWORD start_time;
// The level of debug to display
int debugLevel;
//debug display mode (-1 = none, 0 = console, 1 = file)
int debugDisplay;
//the debug file handle
FILE *debugFile;

// Dll hmodule
HMODULE hMod;

/* Helper function for throwing debug/error messages
 *
 * int level      - Debug level
 * char *location - Message location
 * char *message  - Message
 */
void debugMessage(int level, char *location, char *message)
{
	// If above the current level then skip totally
	if(level > debugLevel) return;

    // Calculate HMS
	DWORD cur_time = GetTickCount() - start_time;
	long hours = (long)floor((double)cur_time / (double)3600000.0);
	cur_time -= (hours * 3600000);
	int minutes = (int)floor((double)cur_time / (double)60000.0);
	cur_time -= (minutes * 60000);
	double seconds = (double)cur_time / (double)1000.0;

    // Build error message
	char text[4096] = "\0";
	if(level == 0)
	{
		sprintf_s(text, 4096, "%d:%d:%#.1f ERR %s %s\n", hours, minutes, seconds, location, message);
	}
	else if(level == 1)
	{
		sprintf_s(text, 4096, "%d:%d:%#.1f WRN %s %s\n", hours, minutes, seconds, location, message);
	}
	else if(level == 2)
	{
		sprintf_s(text, 4096, "%d:%d:%#.1f INF %s %s\n", hours, minutes, seconds, location, message);
	}
    // Output and flush
	printf_s(text);
	fflush(stdout);
}

// Override function for cursor position
BOOL WINAPI OverrideSetCursorPos(int X, int Y)
{
	// If ddraw object exists and windowed mode
	if(lpDD != NULL && lpDD->isWindowed)
	{
		// X,Y are relative to client area within the code
		// Get client area location
		POINT cpos;
		cpos.x = 0;
		cpos.y = 0;
		ClientToScreen(lpDD->hWnd, &cpos);

		// Calculate correct cursor offset and move
		BOOL res = TrueSetCursorPos(cpos.x + X, cpos.y + Y);
		return res;
	}
	return TrueSetCursorPos(X, Y);
}

// Override function for load library
/*HMODULE WINAPI OverrideLoadLibraryA(LPCSTR lpModuleName)
{
	printf_s("%s\n", lpModuleName);
	return TrueLoadLibraryA(lpModuleName);
}*/

// Pretty standard winproc
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        // On syskey down
		case WM_SYSKEYDOWN:
			// ALT+ENTER trap keydown
			if (wParam == VK_RETURN)
			{
				return 0;
			}
			break;
        // On syskey up
		case WM_SYSKEYUP:
			// ALT+ENTER trap keyup
			if (wParam == VK_RETURN)
			{
				// If DDW exists
				if(lpDD != NULL)
				{
					lpDD->ToggleFullscreen();
				}
				return 0;
			}
			break;
        // On keydown
		case WM_KEYDOWN:
			// Overload printscreen(save a snapshot of the current screen
			if(wParam == VK_SNAPSHOT)
			{
				return 0;
			}
			// Always pass ~ to menu
			if(wParam == VK_OEM_3)
			{
				return 0;
			}
			// Everything gets passed if we are in the menu
			if(inMenu)
			{
				return 0;
			}
			break;
        // On keyup
		case WM_KEYUP:
			// Overload printscreen(save a snapshot of the current screen)
			if(!inMenu && wParam == VK_SNAPSHOT)
			{
				lpDD->DoSnapshot();
				return 0;
			}
			// Always pass ~ to menu
			if(wParam == VK_OEM_3)
			{
				// Pass to menu and set inMenu to result
				inMenu = lpDD->MenuKey(VK_OEM_3);
				return 0;
			}
			// Everything gets passed if we are in the menu
			if(inMenu)
			{
				// Pass to menu and set inMenu to result
				inMenu = lpDD->MenuKey(wParam);
				return 0;
			}
			break;
        // On destroy window
		case WM_DESTROY:
			// Restore the orignal window proc if we have it
			if(lpDD->lpPrevWndFunc != NULL)
			{
				SetWindowLong(hwnd, GWL_WNDPROC, (LONG)lpDD->lpPrevWndFunc);
			}
			break;
    }
	// Call original windiow proc by default
    return CallWindowProc(lpDD->lpPrevWndFunc, hwnd, message, wParam, lParam);
}

// Emulated direct draw create
HRESULT WINAPI DirectDrawCreate(GUID FAR* lpGUID, LPDIRECTDRAW FAR* lplpDD, IUnknown FAR* pUnkOuter)
{
	// Create directdraw object
	lpDD = new IDirectDrawWrapper();
	if(lpDD == NULL) 
	{
		debugMessage(0, "DirectDrawCreate", "Failed to create IDirectDrawWrapper.");
		return DDERR_OUTOFMEMORY;  // Simulate OOM error
	}
	
	// Initialize the ddraw object with new wndproc
	HRESULT hr = lpDD->WrapperInitialize(&WndProc, hMod);
	// If error then return error(message will have been taken care of already)
	if(hr != DD_OK) return hr;

	// Set return pointer to the newly created DirectDrawWrapper interface
	*lplpDD = (LPDIRECTDRAW)lpDD;
	
	// Return success
	return DD_OK;
}