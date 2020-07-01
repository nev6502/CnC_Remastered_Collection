#pragma once
#include <memory>

/*
* WWKeyboardClass::Buff_Get -- Lowlevel function to get a key from key buffer*
* WWKeyboardClass::Check -- Checks to see if a key is in the buffer*
* WWKeyboardClass::Down -- Checks to see if the specified key is being held down.*
* WWKeyboardClass::Fetch_Element -- Extract the next element in the keyboard buffer.*
* WWKeyboardClass::Get -- Logic to get a metakey from the buffer*
* WWKeyboardClass::Message_Handler -- Process a windows message as it relates to the keyboar*
* WWKeyboardClass::Peek_Element -- Fetches the next element in the keyboard buffer.*
* WWKeyboardClass::To_ASCII -- Convert the key value into an ASCII representation.*
* WWKeyboardClass::Available_Buffer_Room -- Fetch the quantity of free elements in the keybo*
* ----------------------------------------------*/

/**
* EVENT QUEUE class
* Supports pumping of event messages using a FIFO buffer.
*/

extern UserInputClass UserInput;

/*
	Event type definitions
*/
typedef enum EventMessageType {
	EVENT_NONE,
	EVENT_KEYBOARD,
	EVENT_MOUSE
};

/*
	Event types
*/
class EventMessage {
	public:
		EventMessageType EventType;
		bool Valid;

		EventMessage() {
			this->EventType == EVENT_NONE;

			this->Keyboard.Key = KN_NONE;
			this->Keyboard.Shift = false;
			this->Keyboard.Alt = false;
			this->Keyboard.Control = false;
			this->Keyboard.CapsLock = false;

			this->Mouse.x = -1;
			this->Mouse.y = -1;
			this->Mouse.Button = 0;
			this->Mouse.ButtonState = MouseButtonState::MOUSE_BUTTON_UP;

			this->Flags = 0;

			this->Valid = false;
		}

		// for keyboard
		struct {
			KeyNumType Key;
			char ASCII;
			bool Shift;
			bool Alt;
			bool Control;
			bool CapsLock;
		} Keyboard;

		// for mouse
		struct {
			int x;
			int y;
			unsigned short Button;
			MouseButtonState ButtonState;
		} Mouse;

		// for cascading functions
		unsigned short Flags;

		EventMessage( KeyNumType key, char asciis, bool shift, bool alt, bool control, bool capsLock );
		EventMessage( int x, int y, unsigned short button = KN_LMOUSE, MouseButtonState buttonState = MouseButtonState::MOUSE_BUTTON_DOWN );

		void Key_Flags(KeyNumType& key, int& flags);
	private:
		void Clear() {
			this->Keyboard.Key = KN_NONE;
			this->Keyboard.Shift = false;
			this->Keyboard.Alt = false;
			this->Keyboard.Control = false;
			this->Keyboard.CapsLock = false;

			this->Mouse.x = -1;
			this->Mouse.y = -1;
			this->Mouse.Button = 0;
			this->Mouse.ButtonState = MouseButtonState::MOUSE_BUTTON_UP;

			this->Flags = 0;

			this->Valid = false;
		}
};

class EventQueueClass {
	public:
		int Get_Mouse_X() const { return UserInput.Mouse.X; }
		int Get_Mouse_Y() const { return UserInput.Mouse.Y; }

		void Clear();
		void Put(EventMessage input);
		bool Check() const;
		EventMessage Get();
		char EventQueueClass::Get_ASCII();

	private:
		const int MaxCapacity = 100;

		std::queue< EventMessage > eventQueue;

		bool Is_Buffer_Full() const { return this->eventQueue.size() == MaxCapacity; }
		bool Is_Buffer_Empty() const { return this->eventQueue.size() == 0; }
};
