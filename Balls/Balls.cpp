#include <iostream>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

//using namespace std;

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand() % 255));
		return true;
	}
};


int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}


//class CirclePhysics : public olc::PixelGameEngine
//{
//public:
//	CirclePhysics()
//	{
//		// wchar_t, 16bytes instead of 8, native string encoding
//		// for win32 APIs.
//		sAppName = "Circle Physics";
//
//	}
//
//public:
//	bool OnUserCreate() override
//	{
//		return true;
//	}
//
//	bool OnUserUpdate(float fElapsed) override
//	{
//		return true;
//	}
//};
//
//int main()
//{
//	CirclePhysics game;
//	if (game.Construct(160, 120, 8, 8))
//		game.Start();
//	else
//		cout << "Could not initialize game";
//	return 0;
//}