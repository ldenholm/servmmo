#pragma once

#include <vector>
#include <iostream>
#include <utility>

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

		class Grid {

		private:
			int m_iHeight, m_iWidth;

		public:
			vector<vector<int>> grid;

			Grid(int h, int w) : m_iHeight(h), m_iWidth(w)
			{
				grid = vector<vector<int>>(h, vector<int>(w));
			}

			int Height() { return m_iHeight; }
			int Width() { return m_iWidth; }

		public:
			// Creates walls and uses 0 = walkable, 1 = wall
			// Printing: # = wall, . = walkable.
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
				return (coord.x >= 0 && coord.x <= m_iHeight && coord.y >= 0 && coord.y <= m_iWidth);
			}
		};

		vector<Coord_2D> Neighbors(const Grid& g, Coord_2D coord)
		{
			// up/down/left/right 4 way movement.
			vector<Coord_2D> neighbors;
			vector<pair<int, int>> offsets =
			{
				{coord.x, coord.y - 1}, {coord.x, coord.y + 1},
				{coord.x - 1, coord.y}, {coord.x + 1, coord.y}
			};

			for (auto& offset : offsets)
			{
				Coord_2D potentialNextCoord = { offset.first, offset.second };
				if (g.Coord_2D_WithinGrid(potentialNextCoord))
				{
					neighbors.emplace_back(potentialNextCoord);
				}
			}

			return neighbors;
		}
	}
}