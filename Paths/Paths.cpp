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
    Coord_2D first{ 3, 7 };
    if (first.Coord_2D_WithinGrid(g.Height(), g.Width()))
    {
        std::cout << "Coord is within grid";
    }
    else { std::cout << "Coord lies outside grid."; }
    
}