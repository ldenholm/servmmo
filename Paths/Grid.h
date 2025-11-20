#pragma once

#include <vector>
#include <iostream>
#include <utility>
#include <limits>
#include <optional>
#include <queue>

using namespace std;

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

		class Grid {

		private:
			int m_iHeight, m_iWidth;

		public:
			vector<vector<int>> grid;
			vector<vector<float>> gscore_table;
			vector<vector<optional<Coord_2D>>> came_from_table;


		public:

			Grid(int h, int w) : m_iHeight(h), m_iWidth(w)
			{
				grid = vector<vector<int>>(h, vector<int>(w));
				gscore_table = vector<vector<float>>(h, vector<float>(w));
				came_from_table = vector<vector<optional<Coord_2D>>>(h, vector<optional<Coord_2D>>(w));
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
						//cout << "[";
						for (int j = 0; j < m_iWidth; j++)
						{
							int cell = grid.at(i).at(j);
							if (i == 0 || i == (m_iHeight - 1) || j == 0 || j == (m_iWidth - 1))
							{
								cell = 1;
								// print wall
								cout << "#";
							}
							// using this for easy bounds checking.
							else { cell = 0; cout << "."; }

							// g score init:
							gscore_table.at(i).at(j) = numeric_limits<float>::infinity();
						}

						cout << endl;
					}
				}
				catch (const out_of_range oor) {
					cout << oor.what() << endl;
				}

			}

			bool Coord_2D_WithinGrid(Coord_2D coord) const
			{
				return (coord.x >= 0 && coord.x < m_iHeight && coord.y >= 0 && coord.y < m_iWidth);
			}

			
		};

	}
}