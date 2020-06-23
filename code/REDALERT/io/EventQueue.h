#pragma once

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
typedef enum {
	EVENT_KEYBOARD,
	EVENT_MOUSE
} EventMessageType;

/*
	Event types
*/
class EventMessage {
	public:
		EventMessageType EventType;

		virtual void Key_Flags(KeyNumType& key, int& flags) = 0;
		virtual ~EventMessage() = default;
};

class KeyboardEventMessage : public EventMessage {
	public:
		KeyNumType Key;
		bool Shift;
		bool Alt;
		bool Control;
		bool CapsLock;

		// for cascading functions
		unsigned short Flags;

		KeyboardEventMessage(KeyNumType key, bool shift = false, bool alt = false, bool control = false, bool capsLock = false);
		virtual void Key_Flags(KeyNumType& key, int& flags);

		virtual ~KeyboardEventMessage() = default;
};

class MouseEventMessage : public EventMessage {
	public:
		int x;
		int y;
		unsigned short Button;
		UserInputClass::MouseButtonState ButtonState;

		// for cascading functions
		unsigned short Flags;

		MouseEventMessage(int x, int y, unsigned short button = KN_LMOUSE, UserInputClass::MouseButtonState buttonState = UserInputClass::MouseButtonState::MOUSE_BUTTON_DOWN);
		virtual void Key_Flags(KeyNumType& key, int& flags);

		virtual ~MouseEventMessage() = default;
};

class EventQueueClass {
	public:
		int Get_Mouse_X() const { return UserInput.Mouse.X; }
		int Get_Mouse_Y() const { return UserInput.Mouse.Y; }

		void Clear();
		void Put(EventMessage* input);
		bool Check() const;
		EventMessage* Get();
		char EventQueueClass::Get_ASCII();

	private:
		const int MaxCapacity = 100;

		std::queue<EventMessage*> eventQueue;

		bool Is_Buffer_Full() const { return this->eventQueue.size() == MaxCapacity; }
		bool Is_Buffer_Empty() const { return this->eventQueue.size() == 0; }
};
