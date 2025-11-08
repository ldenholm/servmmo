#include <iostream>
#include <vector>
#include <numbers>
#include <cmath>
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

		// Check circles overlap
		auto DoCirclesOverlap = [](float x1, float y1, float r1,
			float x2, float y2, float r2)
			{
				// Apparently sqrt() is costly so we can just the following:
				// It's simply a^2 + b^2 = c^2 and check c^2 <= sum of radius'
				return ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) <= (r1 + r2) * (r1 + r2));

			};


		// Helper function to determine if a point exists within a circle (ie cardinality within a set of points).
		auto IsCircleSelected = [](float x1, float x2, float y1, float y2, float radius)
			{
				return ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) < (radius * radius));
			};
		
		// Collision detection (quadratic, replace with uniform grid).
		for (auto& ball : vecBalls)
		{
			for (auto& target : vecBalls)
			{
				if (ball.id != target.id)
				{
					if (DoCirclesOverlap(ball.px, ball.py, ball.radius, 
						target.px, target.py, target.radius))
					{
						// Collision detected, resolve by translating balls d/2 in opposite direction
						// of the collision, where d is the magnitude of the displacement vector.
						// Note that (r1 + r2) - sqrt(a^2+b^2) yields the magnitude of the displacement vector.
						
						// Distance between ball centers:
						float fDistance = sqrtf((target.px - ball.px) * (target.px - ball.px) + (target.py - ball.py) * (target.py - ball.py));
						
						/* Classic physics way of summing vectors so the sign is correct.
						* Just to nail this point home should I need reminding in the future. Think back to kinematics.
						* We have fDistance, r1, r2. We know overlap (displacement) is the sum of the radii minus the 
						* distance between centers of the balls: r1+r2 - fDistance.
						* But when we do the summation like this, we (for no good reason) swap the sign of fDistance
						* so we give it the direction along the -x axis. This is why we do the summation: fDistance - r1 - r2.
						* We leave fDistance sign unchanged and simply subtrace r1 and r2. This way fDistance maintains 
						* its direction information and sign after the sum tells us the correct  direction of the 
						* displacement vector. Phew that took some explaining but good to revisit by classical
						* mechanics days! 
						*/

						float fOverlap = fDistance - ball.radius - target.radius;
						
						// We only need d/2 and it has negative sign so we can use it to displace the balls.
						fOverlap *= 0.5f;

						// Displace ball.
						// This is simply u = v/||v||, to get a unit vector in the direction of our displacement
						// then simply scaling it by fOverlap.
						ball.px -= fOverlap * ((ball.px - target.px) / fDistance);
						ball.py -= fOverlap * ((ball.py - target.py) / fDistance);

						// Displace target
						target.px += fOverlap * ((ball.px - target.px) / fDistance);
						target.py += fOverlap * ((ball.py - target.py) / fDistance);
					}
				}
			}
		}

		// Handle Input
		if (GetMouse(0).bPressed)
		{
			pSelectedBall = nullptr;
			for (auto& ball : vecBalls)
			{
				if (IsCircleSelected(ball.px, GetMouseX(), ball.py, GetMouseY(), ball.radius))
				{
					pSelectedBall = &ball;
#ifdef _DEBUG
					cout << "Seleced ball: " << pSelectedBall->id;
#endif
					break;
				}
			}
		}
		
		Clear(olc::BLACK);

		// Draw balls
		for (auto &i : vecBalls)
		{
			DrawCircle((int)i.px, (int)i.py, (int)i.radius, olc::CYAN);
		}
			
		return true;
	}

private:
	// Vector of balls
	vector<sBall> vecBalls;
	sBall* pSelectedBall = nullptr;

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

	void LaunchBall(int32_t mouseX, int32_t mouseY)
	{
		// Determine mouse over which ball.
		// Do we check cardinality for mouse x/y coords 
		// within set of points describing the circle? Maybe more simply just find
		// which pixel is entirely filled by the circle.
		
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