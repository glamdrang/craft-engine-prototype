#include "craft/common.h"
#include "InputText.h"

void InputText::handle(SDL_Event const& sdlevent)
{
	//if (!_active)
	//	return;

	switch (sdlevent.type)
	{
	case SDL_KEYUP:
		{
			if (sdlevent.key.keysym.scancode == SDL_SCANCODE_GRAVE
				&& (sdlevent.key.keysym.mod & KMOD_SHIFT))
			{
				this->set_text_sdl_event_handler(!this->is_text_sdl_event_handler_active());
			}
			if (sdlevent.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
			{
				if (_text.size() > 0)
					_text.pop_back();
			}
		}
		break;
	case SDL_TEXTEDITING:
		{
			// TODO: Have to figure out the correct usage of this event.
		}
		break;
	case SDL_TEXTINPUT:
		{
			int i = 0;
			while (sdlevent.text.text[i] != '\0')
			{
				_text.push_back(sdlevent.text.text[i]);
				i++;
			}
		}
		break;
	default:
		return;
	}
}

bool InputText::set_text_sdl_event_handler(bool value)
{
	if (_active && value)
		return true;

	if (!_active && !value)
		return false;

	if (!_active) // (value == true)
	{
		/*
		if (SDL_IsTextInputActive())
		{
		// TODO log error
		return false;
		}
		*/

		SDL_StartTextInput();
		_active = value;
		_text.clear();

		return true;
	}
	else // (value != true)
	{
		/*
		if (!SDL_IsTextInputActive())
		{
			// TODO log error
			return true;
		}
		*/

		_text.clear();
		_active = value;
		SDL_StopTextInput();

		return false;
	}
}

bool InputText::is_text_sdl_event_handler_active()
{
	/*
	bool sdl_test = SDL_IsTextInputActive() == SDL_TRUE;
	if (sdl_test != _active)
	{
		// TODO log error
		return sdl_test;
	}
	*/
	return _active;
}

std::string InputText::get_current_text()
{
	std::string s(this->_text.begin(), this->_text.end());
	return s;
}
