/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *  EECS 281 Lab 9 - Number of Islands                     *
 *  Identifier: 472D3C8289DE4915774A47683EC45FFBA373B980   *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Authors: Calvin Tran (cktran)
//          Manuel Rodriguez (mrrodrig)

#ifndef ISLANDS_H
#define ISLANDS_H

#include <vector>
#include <queue>

    struct pair {
        int i;
        int j;
    };

void foundlandNeighbor(std::vector<std::vector<char>>& grid, int posx, int posy, std::queue<pair>& visited);

int number_of_islands(std::vector<std::vector<char>>& grid) {
    // TODO
    int counter = 0;

    std::queue<pair> visited;
    for(size_t i = 0; i < grid.size(); ++i)
    {
        for(size_t j = 0; j < grid[i].size(); ++j)
        {
            if (grid[i][j] == 'o')
            {
                counter++;
                grid[i][j] = 'x';
                //Do a DFS or BFS
                foundlandNeighbor(grid, i, j, visited);
                while(!visited.empty())
                {
                    pair now = visited.front();                                //grab from top
                    visited.pop();                                             //pop top
                    foundlandNeighbor(grid, now.i, now.j, visited);            //search for more neighbors
                }
            }
        }
    }

    return counter;
}

void foundlandNeighbor(std::vector<std::vector<char>>& grid, int posx, int posy, std::queue<pair>& visited)
{
    if(posx != 0)   // left edge
    {
        if (grid[posx - 1][posy] == 'o')
        {
            visited.push({ posx - 1, posy });
            grid[posx - 1][posy] = 'x';
        }
    }
    if(posx != grid.size()-1) //right edge
    {
        if (grid[posx + 1][posy] == 'o')
        {
            visited.push({ posx + 1, posy });
            grid[posx + 1][posy] = 'x';
        }
    }
    if(posy != 0)   // top edge
    {
        if (grid[posx][posy - 1] == 'o')
        {
            visited.push({ posx, posy - 1 });
            grid[posx][posy - 1] = 'x';

        }
    }
    if(posy != grid[posx].size()-1)    //bottom edge
    {
        if (grid[posx][posy + 1] == 'o')
        {
            visited.push({ posx, posy + 1 });
            grid[posx][posy + 1] = 'x';
        }
    }
}

#endif /* ISLANDS_H */
