#include "../FUNCTION.H"
#include "UserInput.h"

extern KeyboardClass * Keyboard;

std::map<KeyNumType, SDL_KeyCode> sdl_keyMapping_reverse;

UserInputClass::UserInputClass()
{
	LastAction = InputAction::INPUT_ACTION_NONE;

	// generate reverse keymapping
	for ( auto it = sdl_keyMapping.begin(); it != sdl_keyMapping.end(); it++ )
	{
		sdl_keyMapping_reverse[it->second] = it->first;
	}
}

bool UserInputClass::KeyboardState::Key_Down(KeyNumType key) const
{
	auto sdl_Key = sdl_keyMapping_reverse.find( key );
	if ( sdl_Key != sdl_keyMapping_reverse.end() )
	{
		const SDL_Scancode scanCode = SDL_GetScancodeFromKey(sdl_Key->second);
		const Uint8* keyState = SDL_GetKeyboardState(NULL);
		return ( keyState[scanCode] );
	}
	return false;
}

bool UserInputClass::MouseState::Mouse_Down(KeyNumType key) const
{
	// get mouse state, we are only interested in the buttons
	const Uint32 MouseState = SDL_GetMouseState(NULL,NULL);

	switch (key)
	{
		case KN_LMOUSE:
			return MouseState & SDL_BUTTON(SDL_BUTTON_LEFT);
			break;
		case KN_RMOUSE:
			return MouseState & SDL_BUTTON(SDL_BUTTON_RIGHT);
			break;
	}
}

void UserInputClass::Process_Input(KeyNumType& key, int& flags)
{
	bool leftMouseProcessed = false;

	const Uint32 MouseState = SDL_GetMouseState(&Mouse.X, &Mouse.Y);

	flags = 0;

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

			/* Mouse is moving */
			case SDL_MOUSEMOTION:
				if ( !( MouseState & SDL_BUTTON(SDL_BUTTON_LEFT) ) )
				{
					flags |= GadgetClass::LEFTUP;
				}

				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					Mouse.Button_Left == MouseButtonState::MOUSE_BUTTON_DOWN;

					key = KN_LMOUSE;
					flags |= GadgetClass::LEFTPRESS;
					
					EventQueue.Put( EventMessage( Mouse.X, Mouse.Y, KN_LMOUSE ) );

					Keyboard->Put_Element(KN_LMOUSE);

					leftMouseProcessed = true;
					numFramesLMouse++;

					// after first left-mouse down, abort handling any other SDL events
					// because game might first need to switch into rubber band mode
					if (numFramesLMouse == 1) return;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					Mouse.Button_Right == MouseButtonState::MOUSE_BUTTON_DOWN;

					EventQueue.Put( EventMessage( Mouse.X, Mouse.Y, KN_RMOUSE ) );

					Keyboard->Put_Element(KN_RMOUSE);

					key = KN_RMOUSE;
					flags |= GadgetClass::RIGHTPRESS;

					numFramesLMouse = 0;
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					Mouse.Button_Left == MouseButtonState::MOUSE_BUTTON_RELEASE;

					EventQueue.Put( EventMessage(Mouse.X, Mouse.Y, KN_LMOUSE, MouseButtonState::MOUSE_BUTTON_RELEASE) );

					flags |= GadgetClass::LEFTRELEASE;
					numFramesLMouse = 0;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					Mouse.Button_Right == MouseButtonState::MOUSE_BUTTON_RELEASE;

					EventQueue.Put( EventMessage(Mouse.X, Mouse.Y, KN_RMOUSE, MouseButtonState::MOUSE_BUTTON_RELEASE) );

					flags |= GadgetClass::RIGHTRELEASE;
					numFramesLMouse = 0;
				}
				break;

			// Keyboard key lifted up
			case SDL_KEYUP:
				flags |= GadgetClass::KEYBOARD;

				break;

			// Keyboard key pressed down
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

				flags |= GadgetClass::KEYBOARD;

				int keyFlags = 0x00;
				KeyB.Shift = false;
				KeyB.Alt = false;
				KeyB.Control = false;
				KeyB.CapsLock = false;

				if (state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT]) {
					KeyB.Shift = true;
					keyFlags |= KN_SHIFT_BIT;
				}
				if (state[SDL_SCANCODE_RALT] || state[SDL_SCANCODE_LALT]) {
					keyFlags |= KN_ALT_BIT;
					KeyB.Alt = true;
				}
				if (state[SDL_SCANCODE_RCTRL] || state[SDL_SCANCODE_LCTRL]) {
					keyFlags |= KN_CTRL_BIT;
					KeyB.Control = true;
				}
				if (state[SDL_SCANCODE_CAPSLOCK]) {
					keyFlags |= KN_CAPSLOCK_BIT;
					KeyB.CapsLock = true;
				}

				// handle match between SDL and internals ( non-text keys )
				auto element = sdl_keyMapping.find((SDL_KeyCode)event.key.keysym.sym);
				if (element != sdl_keyMapping.end())
				{
					//EventQueue.Put( element->second | keyFlags );
					EventQueue.Put( EventMessage( (KeyNumType)element->second, element->second, KeyB.Shift, KeyB.Alt, KeyB.Control, KeyB.CapsLock ) );

					Keyboard->Put_Element( element->second | keyFlags);

					KeyB.LastKey = element->first;
					if (event.text.text[0] == (event.text.text[0] & 0xFF))
					{
						KeyB.ASCII = element->second;
					}
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
		flags |= GadgetClass::LEFTHELD;

		EventQueue.Put( EventMessage(Mouse.X, Mouse.Y, KN_LMOUSE, MouseButtonState::MOUSE_BUTTON_HOLD) );
	}
}