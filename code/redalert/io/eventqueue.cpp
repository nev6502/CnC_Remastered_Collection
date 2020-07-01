#include "../function.h"
#include "eventqueue.h"

/*
	Clears the event queue and frees memory
*/
void EventQueueClass::Clear()
{
	while (this->eventQueue.size() > 0) {
		this->eventQueue.pop();
	}
}

/*	
	Inserts a new event message event into the FIFO buffer.
*/
void EventQueueClass::Put(EventMessage input)
{
	if ( Is_Buffer_Full() ) return;
	this->eventQueue.push( input );
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
EventMessage EventQueueClass::Get()
{
	// wait for buffer
	while( !this->Check() ) { }
		
	EventMessage returnVal = this->eventQueue.front();
	this->eventQueue.pop();

	return returnVal;
}

/*
	Returns ASCII value of the key in the buffer
*/
char EventQueueClass::Get_ASCII()
{
	// get the last key from the buffer
	EventMessage eventMessage = this->Get();
	return eventMessage.Keyboard.ASCII;
}

EventMessage::EventMessage(KeyNumType key, char ascii, bool shift, bool alt, bool control, bool capsLock) {
	this->Clear();

	this->Valid = true;
	this->EventType = EventMessageType::EVENT_KEYBOARD;

	this->Keyboard.Key = key;
	this->Keyboard.Shift = shift;
	this->Keyboard.Alt = alt;
	this->Keyboard.Control = control;
	this->Keyboard.CapsLock = capsLock;
	this->Keyboard.ASCII = ascii;
}

EventMessage::EventMessage(int x, int y, unsigned short button, MouseButtonState buttonState)
{
	this->Clear();

	this->Valid = true;
	this->EventType = EventMessageType::EVENT_MOUSE;

	this->Mouse.x = x;
	this->Mouse.y = y;
	this->Mouse.Button = button;
	this->Mouse.ButtonState = buttonState;

	this->Flags = 0;

	if (buttonState == MouseButtonState::MOUSE_BUTTON_UP) {
		this->Flags = (button == KN_LMOUSE) ? GadgetClass::LEFTUP : GadgetClass::RIGHTUP;
	}

	if (buttonState == MouseButtonState::MOUSE_BUTTON_DOWN) {
		this->Flags = (button == KN_LMOUSE) ? GadgetClass::LEFTPRESS : GadgetClass::RIGHTPRESS;
	}

	if (buttonState == MouseButtonState::MOUSE_BUTTON_HOLD) {
		this->Flags = (button == KN_LMOUSE) ? GadgetClass::LEFTHELD : GadgetClass::RIGHTHELD;
	}

	if (buttonState == MouseButtonState::MOUSE_BUTTON_RELEASE) {
		this->Flags = (button == KN_LMOUSE) ? GadgetClass::LEFTRELEASE : GadgetClass::RIGHTRELEASE;
	}
}


void EventMessage::Key_Flags(KeyNumType& key, int& flags)
{
	if (this->EventType == EventMessageType::EVENT_KEYBOARD)
	{
		flags = this->Flags;
		flags |= GadgetClass::KEYBOARD;

		unsigned short outputKey = this->Keyboard.Key;

		if (this->Keyboard.Shift) outputKey |= KN_SHIFT_BIT;
		if (this->Keyboard.Alt) outputKey |= KN_ALT_BIT;
		if (this->Keyboard.Control) outputKey |= KN_CTRL_BIT;

		key = (KeyNumType)outputKey;
	}

	if (this->EventType == EventMessageType::EVENT_MOUSE)
	{
		key = (KeyNumType)this->Mouse.Button;

		if (key == KN_LMOUSE) {
			switch (this->Mouse.ButtonState)
			{
				case MouseButtonState::MOUSE_BUTTON_UP:
					flags = GadgetClass::LEFTUP;
					break;
				case MouseButtonState::MOUSE_BUTTON_DOWN:
					flags = GadgetClass::LEFTPRESS;
					break;
				case MouseButtonState::MOUSE_BUTTON_RELEASE:
					flags = GadgetClass::LEFTRELEASE;
					break;
				case MouseButtonState::MOUSE_BUTTON_HOLD:
					flags = GadgetClass::LEFTHELD;
					break;
			}
		}

		if (key == KN_RMOUSE) {
			switch (this->Mouse.ButtonState)
			{
				case MouseButtonState::MOUSE_BUTTON_UP:
					flags = GadgetClass::LEFTUP;
					break;
				case MouseButtonState::MOUSE_BUTTON_DOWN:
					flags = GadgetClass::RIGHTPRESS;
					break;
				case MouseButtonState::MOUSE_BUTTON_RELEASE:
					flags = GadgetClass::RIGHTRELEASE;
					break;
				case MouseButtonState::MOUSE_BUTTON_HOLD:
					flags = GadgetClass::RIGHTHELD;
					break;
			}
		}
	}
}