#pragma once

#include <vector>
#include "dsets.h"
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"


class SquareMaze
{
  public:
    
    SquareMaze();

    void makeMaze(int width, int height);

    bool canTravel(int x, int y, int dir) const;

    void setWall(int x, int y, int dir, bool exists);

    std::vector<int> solveMaze();

    cs225::PNG* drawMaze() const;

    cs225::PNG* drawMazeWithSolution();
  private:
    int determineDirection(int fromIndex, int toIndex);
    DisjointSets dsets;
    int w_;
    int h_;
    int size_;
    std::vector<std::pair<bool,bool>> bool_vec;
};
