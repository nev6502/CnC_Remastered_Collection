#include "../FUNCTION.H"
#include "UserInput.h"

extern KeyboardClass * Keyboard;

/*
auto utf8_decode = [](const std::string& str) -> std::wstring
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
};
*/

void UserInputClass::Process_Input(KeyNumType& key, int& flags, bool presentBuffer)
{
	bool leftMouseProcessed = false;

	SDL_GetMouseState(&Mouse.X, &Mouse.Y);

	Keyboard->MouseQX = Mouse.X;
	Keyboard->MouseQY = Mouse.Y;

	static SDL_Event event;
	memset(&event, 0, sizeof(SDL_Event));

	while (SDL_PollEvent(&event)) {
		const Uint8* state = SDL_GetKeyboardState(NULL);

		switch (event.type)
		{
			case SDL_SYSWMEVENT:
				if (event.syswm.msg->msg.win.msg == WM_DESTROY)
				{
					Invalidate_Cached_Icons();
					VisiblePage.Un_Init();
					HiddenPage.Un_Init();
					AllSurfaces.Release();
					if (!InDebugger) Reset_Video_Mode();

					PostQuitMessage(0);

					switch (ReadyToQuit) {
					case 1:
						ReadyToQuit = 2;
						break;
					case 0:
						ExitProcess(0);
						break;
					}
				}

				break;

			case SDL_MOUSEMOTION:
				flags = GadgetClass::LEFTUP;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					Mouse.Button_Left == MouseButtonState::MOUSE_BUTTON_DOWN;

					flags = GadgetClass::LEFTPRESS;
					leftMouseProcessed = true;
					numFramesLMouse++;
					key = KN_LMOUSE;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					Mouse.Button_Right == MouseButtonState::MOUSE_BUTTON_DOWN;

					numFramesLMouse = 0;
					flags = GadgetClass::RIGHTPRESS;
					key = KN_RMOUSE;
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					Mouse.Button_Left == MouseButtonState::MOUSE_BUTTON_RELEASE;

					flags = GadgetClass::LEFTRELEASE;
					numFramesLMouse = 0;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					Mouse.Button_Right == MouseButtonState::MOUSE_BUTTON_RELEASE;

					flags = GadgetClass::RIGHTRELEASE;
					numFramesLMouse = 0;
				}
				break;

			case SDL_KEYDOWN:
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_GRAVE) {
					renderConsole = !renderConsole;

					if (renderConsole) {
						ShowCursor(TRUE);
						Hide_Mouse();
					}
					else {
						ShowCursor(FALSE);
						Show_Mouse();
					}
					break;
				}

				int keyFlags = 0x00;
				if (state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT]) keyFlags |= KN_SHIFT_BIT;
				if (state[SDL_SCANCODE_RALT] || state[SDL_SCANCODE_LALT]) keyFlags |= KN_ALT_BIT;
				if (state[SDL_SCANCODE_RCTRL] || state[SDL_SCANCODE_LCTRL]) keyFlags |= KN_CTRL_BIT;

				// handle match between SDL and internals (non-text keys)
				auto element = sdl_keyMapping.find((SDL_KeyCode)event.key.keysym.sym);
				if (element != sdl_keyMapping.end())
				{
					Keyboard->Put_Element(element->second | keyFlags);
					KeyB.ASCII = element->second;
				}

				break;
			}
			case SDL_TEXTINPUT:
				// limit to range 0-255
				if (event.text.text[0] == ( event.text.text[0] & 0xFF ) )
				{
					KeyB.ASCII = (char)event.text.text[0];
				}
		}

		if (Debug_Map || renderConsole)
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
		}
	}

	if (!leftMouseProcessed && numFramesLMouse != 0)
	{
		Mouse.Button_Left == MouseButtonState::MOUSE_BUTTON_HOLD;
		flags = GadgetClass::LEFTHELD;
	}

	if (presentBuffer)
	{
		Device_Present();
	}
}