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
		public:
			int height, width;
			vector<vector<int>> grid;

			Grid(int h, int w) : height(h), width(w)
			{
				grid = vector<vector<int>>(h, vector<int>(w));
			}

		public:
			// Creates walls and uses 0 = walkable, 1 = wall
			// Printing: # = wall, . = walkable.
			void Build()
			{
				try {
					for (int i = 0; i < height; i++)
					{
						//cout << "[";
						for (int j = 0; j < width; j++)
						{
							int cell = grid.at(i).at(j);
							if (i == 0 || i == (height - 1) || j == 0 || j == (width - 1))
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