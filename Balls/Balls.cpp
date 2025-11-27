#include <iostream>
#include <vector>
#include <numbers>
#include <cmath>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "data_qtree.h"

using namespace std;


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

		for (int i = 0; i < 20; i++)
		{
			AddBall(rand() % ScreenWidth(), rand() % ScreenWidth(), fDefaultRadius);
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
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

		// Track colliding balls.
		vector<pair<smmo::data::sBall*, smmo::data::sBall*>> collidingPairs;

		// Update ball positions.
		for (auto& ball : vecBalls)
		{
			// Introduce some drag.
			ball.ax = -0.8f * ball.vx;
			ball.ay = -0.8f * ball.vy;

			// V = at, m/s = m/s^2 * s
			// p = m/s * s
			ball.vx += ball.ax * fElapsedTime;
			ball.vy += ball.ay * fElapsedTime;
			ball.px += ball.vx * fElapsedTime;
			ball.py += ball.vy * fElapsedTime;

			// Balls travel through screen bounds.
			if (ball.px < 0) ball.px += (float)ScreenWidth();
			if (ball.px >= ScreenWidth()) ball.px -= (float)ScreenWidth();
			if (ball.py < 0) ball.py += (float)ScreenHeight();
			if (ball.py >= ScreenHeight()) ball.py -= (float)ScreenHeight();

			// Breaks conservation of momentum.
			// For speeds < 0.01 we hardcode them to 0.
			if (fabs(ball.vx * ball.vx + ball.vy * ball.vy) < 0.01f)
			{
				ball.vx = 0;
				ball.vy = 0;
			}

		}
		
		// Collision detection (quadratic, replace with uniform grid).
		for (auto& ball : vecBalls)
		{
			smmo::data::CreateAABB(ball);
			bounding_boxes.push_back(ball.AABB);

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

						// First track them in our vector.
						collidingPairs.push_back({ &ball, &target });
						
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
						
						// We only need d/2 so we can use it to displace the balls.
						fOverlap *= 0.5f;

						// Displace ball.
						// This is simply u = v/||v||, to get a unit vector in the direction of our displacement
						// then simply scaling it by fOverlap. Note also ball moves away from the collision, and
						// the converse is true for the target.
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
		if (GetMouse(0).bPressed || GetMouse(1).bPressed)
		{
			pSelectedBall = nullptr;
			for (auto& ball : vecBalls)
			{
				if (IsCircleSelected(ball.px, GetMouseX(), ball.py, GetMouseY(), ball.radius))
				{
					pSelectedBall = &ball;
#ifdef _DEBUG
					cout << "Seleced ball: " << pSelectedBall->id << endl;
#endif
					break;
				}
			}
		}

		if (GetMouse(0).bHeld)
		{
			if (pSelectedBall != nullptr)
			{
				pSelectedBall->px = GetMouseX();				
				pSelectedBall->py = GetMouseY();
			}
		}

		// Deselect ball on mouse release.
		if (GetMouse(0).bReleased)
		{
			pSelectedBall = nullptr;
		}

		// Apply velocity to selected ball on RMB release.
		if (GetMouse(1).bReleased)
		{
			if (pSelectedBall != nullptr)
			{
				// Create vector from mouse pointer to center of the ball
				// and multiply velocity by this vector.
				pSelectedBall->vx = 5.0f * ((pSelectedBall->px - (float)GetMouseX()));
				pSelectedBall->vy = 5.0f * ((pSelectedBall->py - (float)GetMouseY()));
			}
			pSelectedBall = nullptr;
		}
		
		Clear(olc::BLACK);

		// Visualize dynamic collisions.
		for (auto c : collidingPairs)
		{
			// Draw line from center of one the colliding pair to its counterpart.
			DrawLine(c.first->px, c.first->py, c.second->px, c.second->py, olc::WHITE);
		}


		// Draw balls
		for (auto &i : vecBalls)
		{
			DrawCircle((int)i.px, (int)i.py, (int)i.radius, olc::CYAN);
			DrawRect(i.AABB.x_min, i.AABB.y_min, (i.AABB.x_max - i.AABB.x_min), (i.AABB.y_max - i.AABB.y_min));
		}

		// Draw velocity vector visualization.
		if (pSelectedBall != nullptr)
		{
			// Line from selected balls center to the mouse coordinates.
			DrawLine(pSelectedBall->px, pSelectedBall->py, GetMouseX(), GetMouseY(), olc::DARK_GREY);
		}
			
		return true;
	}

private:
	// Vector of balls
	vector<smmo::data::sBall> vecBalls;
	smmo::data::sBall* pSelectedBall = nullptr;
	vector<smmo::data::sAABB> bounding_boxes;

	void AddBall(float x, float y, float r = 5.0f)
	{
		smmo::data::sBall b = {
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
	if (game.Construct(200, 200, 8, 8))
		game.Start();
	else
		cout << "Could not initialize game";
	return 0;
}