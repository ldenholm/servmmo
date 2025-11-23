#pragma once

#include "Grid.h"
#include <vector>
#include <optional>
#include <algorithm>

namespace smmo 
{
	namespace pathing
	{

		inline float Heuristic(const smmo::paths::Coord_2D& start, const smmo::paths::Coord_2D& goal)
		{
			// manhattan distance (L1 norm)
			return static_cast<float>(abs(start.x - goal.x) + abs(start.y - goal.y));
		}

		void ReconstructPath(const std::vector<std::vector<std::optional<smmo::paths::Coord_2D>>>& came_from_table, const smmo::paths::Coord_2D& finalCoord)
		{
			std::vector<smmo::paths::Coord_2D> path;
			smmo::paths::Coord_2D currentCoord = finalCoord; // copy ctor
			while (came_from_table[currentCoord.y][currentCoord.x].has_value()) // while a valid came from coord exists.
			{
				path.push_back(currentCoord);
				smmo::paths::Coord_2D came_from_coord = came_from_table[currentCoord.y][currentCoord.x].value();
				currentCoord = { came_from_coord.x, came_from_coord.y };
			}

			// reverse our path
			reverse(path.begin(), path.end());

			std::cout << "Path we discovered: " << std::endl;

			for (auto& p : path)
			{
				std::cout << "x: " << p.x << " y: " << p.y << std::endl;
			}
		}

		std::vector<smmo::paths::Coord_2D> Neighbors(const smmo::paths::Grid& g, smmo::paths::Coord_2D coord)
		{
			// up/down/left/right 4 way movement.
			std::vector<smmo::paths::Coord_2D> neighbors;
			std::vector<std::pair<int, int>> offsets =
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
			std::vector<std::vector<float>>& gscore_table, std::vector<std::vector<std::optional<smmo::paths::Coord_2D>>>& came_from_table,
			const smmo::paths::Coord_2D& start, const smmo::paths::Coord_2D& goal)
		{
			// first quickly check the start and goal coords are walkable.
			if (!(g.Coord_Walkable_And_WithinGrid(start) && g.Coord_Walkable_And_WithinGrid(goal)))
			{
				std::cout << "Start/Goal Coords neither within grid nor walkable, please update." << std::endl;
				return;
			}


			std::priority_queue<smmo::paths::Node> pQueue;
			pQueue.push({ start, 0 });
			gscore_table[start.y][start.x] = 0;

			while (!pQueue.empty())
			{
				smmo::paths::Node current = pQueue.top();
				pQueue.pop();
				if (current.coord.x == goal.x && current.coord.y == goal.y)
				{
					std::cout << "arrived at the goal!!!" << std::endl;
					ReconstructPath(came_from_table, current.coord);
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
		}

		void AStar(const smmo::paths::Grid& g,
			std::vector<std::vector<float>>& gscore_table, std::vector<std::vector<std::optional<smmo::paths::Coord_2D>>>& came_from_table,
			const smmo::paths::Coord_2D& start, const smmo::paths::Coord_2D& goal)
		{
			// first quickly check the start and goal coords are walkable.
			if (!(g.Coord_Walkable_And_WithinGrid(start) && g.Coord_Walkable_And_WithinGrid(goal)))
			{
				std::cout << "Start/Goal Coords neither within grid nor walkable, please update." << std::endl;
				return;
			}

			// A* uses f_cost which is calculated via heuristic (l1 norm).
			std::priority_queue<smmo::paths::Node_AStar> pQueue;
			// g is zero at this stage.
			pQueue.push({ start, 0.0f, Heuristic(start, goal) });
			gscore_table[start.y][start.x] = 0;

			while (!pQueue.empty())
			{
				smmo::paths::Node_AStar current = pQueue.top();
				pQueue.pop();
				if (current.coord.x == goal.x && current.coord.y == goal.y)
				{
					std::cout << "arrived at the goal!!!" << std::endl;
					ReconstructPath(came_from_table, current.coord);
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
							pQueue.push({ neighbor, tentative_cost, (tentative_cost + Heuristic(neighbor, goal))});
						}
						// pathing to n is worse or equal so ignore.
					}
				}
			}
		}
	}
}