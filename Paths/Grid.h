#pragma once

#include <vector>
#include <iostream>

using namespace std;

namespace smmo
{
	namespace paths
	{

		struct Coord_2D {
			int x, y;
			bool Coord_2D_WithinGrid(int iGrdH, int iGrdW) {
				return (x >= 0 && x <= iGrdH && y >= 0 && y <= iGrdW);
			}
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
		};
	}
}