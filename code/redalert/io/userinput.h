#pragma once

#include <SDL.h>
#include <SDL_syswm.h>
#include <map>
#include <queue>

#include "examples/imgui_impl_sdl.h"

extern KeyNumType g_globalKeyNumType;
extern int g_globalKeyFlags;

// Mapping of SDL KeyCode values to internal KeyNumType
std::map<SDL_KeyCode, KeyNumType> const sdl_keyMapping = {
	{ SDLK_a, KN_A },
	{ SDLK_b, KN_B },
	{ SDLK_c, KN_C },
	{ SDLK_d, KN_D },
	{ SDLK_e, KN_E },
	{ SDLK_f, KN_F },
	{ SDLK_g, KN_G },
	{ SDLK_h, KN_H },
	{ SDLK_i, KN_I },
	{ SDLK_j, KN_J },
	{ SDLK_k, KN_K },
	{ SDLK_l, KN_N },
	{ SDLK_m, KN_M },
	{ SDLK_n, KN_N },
	{ SDLK_o, KN_O },
	{ SDLK_p, KN_P },
	{ SDLK_q, KN_Q },
	{ SDLK_r, KN_R },
	{ SDLK_s, KN_S },
	{ SDLK_t, KN_T },
	{ SDLK_u, KN_U },
	{ SDLK_v, KN_V },
	{ SDLK_w, KN_W },
	{ SDLK_x, KN_X },
	{ SDLK_y, KN_Y },
	{ SDLK_z, KN_Z },

	{ SDLK_0, KN_0 },
	{ SDLK_1, KN_1 },
	{ SDLK_2, KN_2 },
	{ SDLK_3, KN_3 },
	{ SDLK_4, KN_4 },
	{ SDLK_5, KN_5 },
	{ SDLK_6, KN_6 },
	{ SDLK_7, KN_7 },
	{ SDLK_8, KN_8 },
	{ SDLK_9, KN_9 },

	{ SDLK_KP_MULTIPLY, KN_KEYPAD_ASTERISK },
	{ SDLK_KP_DIVIDE, KN_KEYPAD_SLASH },
	{ SDLK_MINUS, KN_MINUS },
	{ SDLK_EQUALS, KN_EQUAL },

	{ SDLK_RIGHTBRACKET, KN_RBRACKET },
	{ SDLK_LEFTBRACKET, KN_LBRACKET },
	{ SDLK_SEMICOLON, KN_SEMICOLON },
	{ SDLK_COMMA, KN_COMMA },
	{ SDLK_PERIOD, KN_PERIOD },
	{ SDLK_SLASH, KN_SLASH },
	{ SDLK_BACKSLASH, KN_BACKSLASH },

	{ SDLK_F1, KN_F1 },
	{ SDLK_F2, KN_F2 },
	{ SDLK_F3, KN_F3 },
	{ SDLK_F4, KN_F4 },
	{ SDLK_F5, KN_F5 },
	{ SDLK_F6, KN_F6 },
	{ SDLK_F7, KN_F7 },
	{ SDLK_F8, KN_F8 },
	{ SDLK_F9, KN_F9 },
	{ SDLK_F10, KN_F10 },
	{ SDLK_F11, KN_F12 },
	{ SDLK_F12, KN_F12 },

	{ SDLK_BACKSPACE, KN_BACKSPACE },
	{ SDLK_SPACE, KN_SPACE },
	{ SDLK_KP_ENTER, KN_RETURN },
	{ SDLK_RETURN, KN_RETURN },
	{ SDLK_ESCAPE, KN_ESC },

	{ SDLK_UP, KN_UP },
	{ SDLK_DOWN, KN_DOWN },
	{ SDLK_LEFT, KN_LEFT },
	{ SDLK_RIGHT, KN_RIGHT },

	{ SDLK_LALT, KN_LALT },
	{ SDLK_RALT, KN_RALT },
	{ SDLK_LCTRL, KN_LCTRL },
	{ SDLK_RCTRL, KN_RCTRL },
	{ SDLK_LSHIFT, KN_LSHIFT },
	{ SDLK_RSHIFT, KN_RSHIFT },
};
extern std::map<KeyNumType, SDL_KeyCode> sdl_keyMapping_reverse;

const int USERINPUT_SHIFT_BIT = 0x100;
const int USERINPUT_CTRL_BIT = 0x200;
const int USERINPUT_ALT_BIT = 0x400;

typedef enum {
	MOUSE_BUTTON_UP,
	MOUSE_BUTTON_DOWN,
	MOUSE_BUTTON_HOLD,
	MOUSE_BUTTON_RELEASE,
} MouseButtonState;

typedef enum {
	INPUT_ACTION_NONE,
	INPUT_ACTION_KEYBOARD,
	INPUT_ACTION_MOUSE,
} InputAction;

class UserInputClass
{
public:
	UserInputClass();
	void UserInputClass::Process_Input(KeyNumType& key = g_globalKeyNumType, int& flags = g_globalKeyFlags);

	InputAction LastAction;

	class KeyboardState {
	public:
		SDL_KeyCode LastKey;
		char ASCII;

		bool Shift;
		bool Control;
		bool Alt;
		bool CapsLock;

		bool Key_Down(KeyNumType key)  const;

		void ResetState() {
			this->Shift = false;
			this->Control = false;
			this->Alt = false;
		}
	};

	KeyboardState KeyB;

	class MouseState {
	public:
		int X;
		int Y;

		MouseButtonState Button_Left;
		MouseButtonState Button_Middle;
		MouseButtonState Button_Right;

		bool UserInputClass::MouseState::Mouse_Down(KeyNumType key) const;

		void ResetState() {
			this->Button_Left = MouseButtonState::MOUSE_BUTTON_UP;
			this->Button_Middle = MouseButtonState::MOUSE_BUTTON_UP;
			this->Button_Right = MouseButtonState::MOUSE_BUTTON_UP;
		}
	};

	MouseState Mouse;
private:
	int numFramesLMouse = 0;
};
