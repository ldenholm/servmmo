#include <iostream>
#include <vector>
#include <numbers>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace std;


struct sBall
{
	int id;
	float px, py;
	float vx, vy;
	float ax, ay;
	float radius;
};



class CirclePhysics : public olc::PixelGameEngine
{
public:
	CirclePhysics()
	{
		sAppName = "Circle Physics";

	}

public:
	bool OnUserCreate() override
	{
		
		float fDefaultRadius = 4.0f;
		AddBall(ScreenWidth() * 0.25, ScreenWidth() * 0.25, fDefaultRadius);
		AddBall(ScreenWidth() * 0.75, ScreenWidth() * 0.25, fDefaultRadius);
		return true;
	}

	bool OnUserUpdate(float fElapsed) override
	{
		Clear(olc::BLACK);

		// Draw balls

		for (auto &i : vecBalls)
		{
			DrawCircle(i.px, i.py, i.radius, olc::CYAN);
		}
			
		return true;
	}

private:
	// Vector of balls
	vector<sBall> vecBalls;

	void AddBall(float x, float y, float r = 5.0f)
	{
		sBall b = {
			.px = x, .py = y,
			.vx = 0, .vy = 0,
			.ax = 0, .ay = 0,
			.radius = r
		};

		b.id = vecBalls.size(); // Assuming we never remove balls from the vector.
		vecBalls.emplace_back(b);
	}
};

int main()
{
	CirclePhysics game;
	if (game.Construct(160, 160, 8, 8))
		game.Start();
	else
		cout << "Could not initialize game";
	return 0;
}