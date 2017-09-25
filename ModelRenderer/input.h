#pragma once


class Input
{
public:

	Input::Input()
	{
		for (int i = 0; i < 256; i++)
		{
			// all keys start as false
			keys[i] = false;
		}
	}

	void KeyDown(unsigned int key)
	{
		keys[key] = true;
	}

	void KeyUp(unsigned int key)
	{
		keys[key] = false;
	}

	bool IsKeyDown(unsigned int key)
	{
		return keys[key];
	}

	void MousePosition(int x, int y)
	{

	}

private:
	bool keys[256];

}input;