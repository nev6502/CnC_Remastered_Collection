#include "../FUNCTION.H"
#include "EventQueue.h"

/*
	Clears the event queue 
*/
void EventQueueClass::Clear()
{
	while (this->eventQueue.size() > 0) this->eventQueue.pop();
}

/*	
	Inserts a new KeyNumType event into the FIFO buffer. KeyNumType represents a key press or mouse button 
	Input: KeyNumType + optional flag bits for SHIFT/CTRL/etc. cast as unsigned short
*/
void EventQueueClass::Put(EventMessage* input)
{
	this->eventQueue.push(input);
}

/*
	Check if the buffer contains an element
*/
bool EventQueueClass::Check() const
{
	return !this->Is_Buffer_Empty();
}

/*
	Get the first (unprocessed) element in the event queue
*/
EventMessage* EventQueueClass::Get()
{
	// wait for buffer
	while( !this->Check() ) { }

	EventMessage* returnVal = this->eventQueue.front();
	this->eventQueue.pop();

	return returnVal;
}

/*
	Returns ASCII value of the key in the buffer
*/
char EventQueueClass::Get_ASCII()
{
	// get the last key from the buffer
	KeyboardEventMessage* keyMessage = dynamic_cast<KeyboardEventMessage*>( this->Get() );
	unsigned short key = keyMessage->Key;

	// check if it is a keyboard
	if (key == KN_LMOUSE || key == KN_RMOUSE) return 0x0;

	// convert to ascii
	char ascii;
	unsigned short key_raw = (UINT)(key & 0xFF);

	// find the SDL keycode
	auto sdl_Key = sdl_keyMapping_reverse.find( (KeyNumType)key_raw );
	if (sdl_Key != sdl_keyMapping_reverse.end())
	{
		// convert to ascii
		ascii = SDL_GetKeyName( sdl_Key->first )[0];

		// convert to uppercase
		if (keyMessage->Shift || keyMessage->CapsLock) ascii = toupper(ascii);
	} 

	return 0x0;
}

KeyboardEventMessage::KeyboardEventMessage(KeyNumType key, bool shift, bool alt, bool control, bool capsLock) {
	this->EventType = EventMessageType::EVENT_KEYBOARD;
	this->Key = key;
	this->Shift = shift;
	this->Alt = alt;
	this->Control = control;
	this->CapsLock = capsLock;

	this->Flags = 0;
	if (this->Shift) this->Flags |= KN_SHIFT_BIT;
	if (this->Alt) this->Flags |= KN_ALT_BIT;
	if (this->Control) this->Flags |= KN_CTRL_BIT;
}

void KeyboardEventMessage::Key_Flags(KeyNumType& key, int& flags)
{
	flags = this->Flags;
	flags |= GadgetClass::KEYBOARD;

	key = (KeyNumType)this->Key;
}

MouseEventMessage::MouseEventMessage(int x, int y, unsigned short button, UserInputClass::MouseButtonState buttonState)
{
	this->EventType = EventMessageType::EVENT_MOUSE;
	this->x = x;
	this->y = y;

	this->Button = button;
	this->ButtonState = buttonState;

	this->Flags = 0;
}

void MouseEventMessage::Key_Flags(KeyNumType& key, int& flags)
{
	key = (KeyNumType)this->Button;

	if (key == KN_LMOUSE) {
		switch (this->ButtonState)
		{
			case UserInputClass::MouseButtonState::MOUSE_BUTTON_DOWN:
				flags = GadgetClass::LEFTPRESS;
				break;
			case UserInputClass::MouseButtonState::MOUSE_BUTTON_RELEASE:
				flags = GadgetClass::LEFTRELEASE;
				break;
			case UserInputClass::MouseButtonState::MOUSE_BUTTON_HOLD:
				flags = GadgetClass::LEFTHELD;
				break;
		}
	}

	if (key == KN_RMOUSE) {
		switch (this->ButtonState)
		{
		case UserInputClass::MouseButtonState::MOUSE_BUTTON_DOWN:
			flags = GadgetClass::RIGHTPRESS;
			break;
		case UserInputClass::MouseButtonState::MOUSE_BUTTON_RELEASE:
			flags = GadgetClass::RIGHTRELEASE;
			break;
		case UserInputClass::MouseButtonState::MOUSE_BUTTON_HOLD:
			flags = GadgetClass::RIGHTHELD;
			break;
		}
	}
}