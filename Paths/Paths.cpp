// Paths.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "Grid.h"

using namespace smmo::paths;

int main()
{
    Grid g(8, 8);
    g.Build();
    Coord_2D first{ 0, 0 };
    
    for (auto& neighbor : Neighbors(g, first))
    {
        cout << "neighbor point, x: " << neighbor.x
            << " y: " << neighbor.y << endl;
    }

    cout << "test second coord." << endl;

    Coord_2D second{ 4, 4 };
    for (auto& neighbor : Neighbors(g, second))
    {
        cout << "neighbor point, x: " << neighbor.x
            << " y: " << neighbor.y << endl;
    }
    
}