#pragma once

#include <vector>
#include <iostream>
#include <utility>
#include <limits>
#include <optional>
#include <queue>
#include <cmath>

namespace smmo
{
	namespace paths
	{
		struct Coord_2D {
			int x, y;
			// constructor for temp rvalue coords
			Coord_2D(int x, int y) : x(x), y(y) {};
		};

		struct Node {
			Coord_2D coord;
			float cost_so_far;

			Node(Coord_2D coord, float cost_so_far) : coord(coord), cost_so_far(cost_so_far) {};

			bool operator <(const Node& other) const {
				// lower cost is considered higher priority
				return cost_so_far > other.cost_so_far;
			}
		};

		struct Node_AStar {
			Coord_2D coord;
			float cost_so_far;
			float f_cost = 0.0f;

			Node_AStar(Coord_2D coord, float cost_so_far, float f_cost) : coord(coord), cost_so_far(cost_so_far), f_cost(f_cost) {};

			bool operator <(const Node_AStar& other) const {
				// lower cost is considered higher priority
				return f_cost > other.f_cost;
			}
		};

		class Grid {

		private:
			int m_iHeight, m_iWidth;

		public:
			std::vector<std::vector<int>> grid;
			std::vector<std::vector<float>> gscore_table;
			std::vector<std::vector<std::optional<Coord_2D>>> came_from_table;


		public:

			Grid(int h, int w) : m_iHeight(h), m_iWidth(w)
			{
				grid = std::vector<std::vector<int>>(h, std::vector<int>(w));
				gscore_table = std::vector<std::vector<float>>(h, std::vector<float>(w));
				came_from_table = std::vector<std::vector<std::optional<Coord_2D>>>(h, std::vector<std::optional<Coord_2D>>(w));
			}

			int Height() { return m_iHeight; }
			int Width() { return m_iWidth; }


			// Creates walls and uses 0 = walkable, 1 = wall
			// Printing: # = wall, . = walkable.
			// Initializes gscore table.
			void Build()
			{
				try {
					for (int i = 0; i < m_iHeight; i++)
					{
						for (int j = 0; j < m_iWidth; j++)
						{
							 
							if (i == 0 || i == (m_iHeight - 1) || j == 0 || j == (m_iWidth - 1))
							{
								grid.at(i).at(j) = 1;
								// print wall
								std::cout << "#";
							}
							// using this for easy bounds checking.
							else { grid.at(i).at(j) = 0; std::cout << "."; }

							// g score init:
							gscore_table.at(i).at(j) = std::numeric_limits<float>::infinity();
						}

						std::cout << std::endl;
					}
				}
				catch (const std::out_of_range oor) {
					std::cout << oor.what() << std::endl;
				}

			}

			bool Coord_2D_WithinGrid(const Coord_2D& coord) const
			{
				return (coord.x >= 0 && coord.x < m_iHeight && coord.y >= 0 && coord.y < m_iWidth);
			}

			bool Coord_Is_Walkable(const Coord_2D& coord) const
			{
				return (grid.at(coord.y).at(coord.x) == 0);
			}

			bool Coord_Walkable_And_WithinGrid(const Coord_2D& coord) const
			{
				return (Coord_2D_WithinGrid(coord) && Coord_Is_Walkable(coord));
			}

			void Print() const
			{
				for (auto& row : grid)
				{
					for (auto& col : row)
					{
						if (col == 1) { std::cout << "#"; }
						else { std::cout << "0"; }
					}
					std::cout << std::endl;
				}
			}

			void SetWall(const Coord_2D& coord)
			{
				try {
					grid.at(coord.y).at(coord.x) = 1;
					std::cout << "===========================new map layout===========================" << std::endl << std::endl;
					Print();
				}
				catch (std::out_of_range& oor) {
					std::cout << oor.what() << "failed to set wall, out of bounds.";
					return;
				}
			}
		};

	}
}