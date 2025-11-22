#pragma once

#include "Grid.h"
#include <vector>

using namespace std;

namespace smmo 
{
	namespace pathing
	{
		vector<smmo::paths::Coord_2D> Neighbors(const smmo::paths::Grid& g, smmo::paths::Coord_2D coord)
		{
			// up/down/left/right 4 way movement.
			vector<smmo::paths::Coord_2D> neighbors;
			vector<pair<int, int>> offsets =
			{
				{coord.x, coord.y - 1}, {coord.x, coord.y + 1},
				{coord.x - 1, coord.y}, {coord.x + 1, coord.y}
			};

			for (auto& offset : offsets)
			{
				smmo::paths::Coord_2D potentialNextCoord = { offset.first, offset.second };
				if (g.Coord_2D_WithinGrid(potentialNextCoord) && g.Coord_Is_Walkable(potentialNextCoord))
				{
					neighbors.emplace_back(potentialNextCoord);
				}
			}

			return neighbors;
		}

		void Dijkstra(const smmo::paths::Grid& g, 
			vector<vector<float>> gscore_table, vector<vector<optional<smmo::paths::Coord_2D>>> came_from_table,
			const smmo::paths::Coord_2D& start, const smmo::paths::Coord_2D& goal)
		{
			// first quickly check the start and goal coords are walkable.
			if (!(g.Coord_Walkable_And_WithinGrid(start) && g.Coord_Walkable_And_WithinGrid(goal)))
			{
				std::cout << "Start/Goal Coords neither within grid nor walkable, please update." << endl;
				return;
			}


			priority_queue<smmo::paths::Node> pQueue;
			pQueue.push({ start, 0 });
			gscore_table[start.y][start.x] = 0;

			while (!pQueue.empty())
			{
				smmo::paths::Node current = pQueue.top();
				pQueue.pop();
				if (current.coord.x == goal.x && current.coord.y == goal.y)
				{
					std::cout << "arrived at the goal!!!" << endl;
					return;
				}
				else
				{
					for (auto& neighbor : Neighbors(g, current.coord))
					{
						// tentative cost: gscore[current] + move_cost.
						// using move cost of 1 for a single step.
						float tentative_cost = gscore_table[current.coord.y][current.coord.x] + 1.0f;
						if (tentative_cost < gscore_table[neighbor.y][neighbor.x])
						{
							// discovered a better path to neighbor.
							gscore_table[neighbor.y][neighbor.x] = tentative_cost;
							came_from_table[neighbor.y][neighbor.x] = current.coord;
							pQueue.push({ neighbor, tentative_cost });
						}
						// pathing to n is worse or equal so ignore.
					}
				}
			}

			std::cout << "cost of the shortest path: " << gscore_table[goal.y][goal.x] << endl;

		}
	}
}