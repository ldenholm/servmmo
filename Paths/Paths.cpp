#include <iostream>
#include <vector>
#include "Grid.h"
#include "Pathing.h"

using namespace smmo::paths;

int main()
{
    Grid g(8, 8);
    g.Build();
    Coord_2D first{ 0, 0 };
    
    for (auto& neighbor : smmo::pathing::Neighbors(g, first))
    {
        cout << "neighbor point, x: " << neighbor.x
            << " y: " << neighbor.y << endl;
    }

    cout << "test second coord." << endl;

    Coord_2D second{ 4, 4 };
    for (auto& neighbor : smmo::pathing::Neighbors(g, second))
    {
        cout << "neighbor point, x: " << neighbor.x
            << " y: " << neighbor.y << endl;
    }
    

    smmo::pathing::Dijkstra(g, g.gscore_table, g.came_from_table, first, second);


    cout << "test print fn" << endl << endl;
    g.Print();
}