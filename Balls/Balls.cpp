#include <iostream>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace std;


class CirclePhysics : public olc::PixelGameEngine
{
public:
	CirclePhysics()
	{
		// wchar_t, 16bytes instead of 8, native string encoding
		// for win32 APIs.
		sAppName = "Circle Physics";

	}

public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsed) override
	{
		return true;
	}
};

int main()
{
	CirclePhysics game;
	if (game.Construct(160, 120, 8, 8))
		game.Start();
	else
		cout << "Could not initialize game";
	return 0;
}