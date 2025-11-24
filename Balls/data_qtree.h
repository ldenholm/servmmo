#pragma once


namespace smmo
{
	namespace data
	{

		

		// We're going to need AABB for the quick checks to determine
		// whether our balls might be colliding.

		// Defines a bounding box (approximate)
		// for a game object.
		struct sAABB
		{
			float x_min;
			float x_max;
			float y_min;
			float y_max;
		};

		struct sBall
		{
			int id;
			float px, py;
			float vx, vy;
			float ax, ay;
			float radius;
			sAABB AABB;
		};

		// Returns a bounding box for provided ball.
		sAABB getAABB(const sBall& ball)
		{
			return {
				.x_min = ball.px - ball.radius,
				.x_max = ball.px + ball.radius,
				.y_min = ball.py - ball.radius,
				.y_max = ball.py + ball.radius
			};
		}

		void CreateAABB(sBall& ball)
		{
			ball.AABB = {
				.x_min = ball.px - ball.radius,
				.x_max = ball.px + ball.radius,
				.y_min = ball.py - ball.radius,
				.y_max = ball.py + ball.radius
			};
		}

		class qtree
		{
		public:

		private:
		};
	}
}