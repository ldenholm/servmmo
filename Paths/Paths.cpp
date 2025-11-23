#include <iostream>
#include <vector>
#include "Grid.h"
#include "Pathing.h"

using namespace smmo::paths;

int main()
{
    Grid g(8, 8);
    g.Build();
    Coord_2D first{ 1, 1 };
    
    for (auto& neighbor : smmo::pathing::Neighbors(g, first))
    {
        std::cout << "neighbor point, x: " << neighbor.x
            << " y: " << neighbor.y << std::endl;
    }

    std::cout << "test second coord." << std::endl;

    Coord_2D second{ 6, 1 };
    for (auto& neighbor : smmo::pathing::Neighbors(g, second))
    {
        std::cout << "neighbor point, x: " << neighbor.x
            << " y: " << neighbor.y << std::endl;
    }
    
    g.SetWall({ 4, 1 });
    g.SetWall({ 4, 2 });
    g.SetWall({ 4, 3 });
    g.SetWall({ 4, 4 });

    //smmo::pathing::Dijkstra(g, g.gscore_table, g.came_from_table, first, second);


    smmo::pathing::AStar(g, g.gscore_table, g.came_from_table, first, second);
}