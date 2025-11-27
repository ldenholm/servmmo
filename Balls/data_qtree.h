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

		bool IntersectingAABB(const sAABB& a, const sAABB& b)
		{
			/* following conditions for NO overlap, using proof
			 by contradiction:

			  condition 1: box A right edge is to the left of box B left edge.
			  condition 2: box A left edge is to the right of box B right edge.
			  condition 3: if box A bottom edge is above box B top edge.
			  condition 4: if box A top edge is below box B bottom edge.

			  Any of these conditions signify no overlap so the converse
			  proves an overlap.

			 */

			bool notOverlapping =
				a.x_max < b.x_min ||
				a.x_min > b.x_max ||
				a.y_max < b.y_min ||
				a.y_min > b.y_max;

			return !notOverlapping;
		}

		class qtree
		{
		public:

		private:
		};
	}
}