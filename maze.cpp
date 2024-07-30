#include "maze.h"
#include <vector>
#include <queue>
#include <stack>

// DONE
SquareMaze::SquareMaze() {
    w_ = 0;
    h_ = 0;
    size_ = 0;
}

//DONE
bool SquareMaze::canTravel(int x, int y, int dir) const {
    switch (dir) {
        case 0:
            return x < w_ - 1 && !bool_vec[x+y*w_].first;
        case 1:
            return y < h_ - 1 && !bool_vec[x+y*w_].second;
        case 2:
            return x > 0 && !bool_vec[(x-1)+y*w_].first;
        case 3:
            return y > 0 && !bool_vec[x+(y-1)*w_].second;
        default:
            return false;
    }
}

// DONE
cs225::PNG * SquareMaze::drawMaze() const {

    cs225::PNG* mazeImage = new cs225::PNG(w_*10 + 1, h_*10 + 1);
    
    for (int i = 10; i < (w_*10 + 1); ++i) {
        mazeImage->getPixel(i, 0).l = 0;
    }

    for (int j = 0; j < (h_*10 + 1); ++j) {
        mazeImage->getPixel(0, j).l = 0;
    }

    for (int x = 0; x < w_; ++x) {
        for (int y = 0; y < h_; ++y) {
            int cellIndex = x + y * w_;
            if (bool_vec[cellIndex].first) {
                for (int k = 0; k <= 10; ++k) {
                    mazeImage->getPixel((x + 1) * 10, y * 10 + k).l = 0;
                }
            }
            if (bool_vec[cellIndex].second) {
                for (int k = 0; k <= 10; ++k) {
                    mazeImage->getPixel(x * 10 + k, (y + 1) * 10).l = 0;
                }
            }
        }
    }

    return mazeImage;
}

// DONE
cs225::PNG * SquareMaze::drawMazeWithSolution() {
  
    cs225::PNG* resultMaze = drawMaze();
    auto path = solveMaze();
    
    int posX = 5, posY = 5;

    for (size_t i = 0; i < path.size(); ++i) {
        int direction = path[i];
        for (int j = 0; j < 11; ++j) {
            cs225::HSLAPixel & currentPixel = 
                resultMaze->getPixel(
                    posX + (direction == 2 ? -j : (direction == 0 ? j : 0)),
                    posY + (direction == 3 ? -j : (direction == 1 ? j : 0))
                );
            currentPixel.h = 0;
            currentPixel.s = 1;
            currentPixel.l = 0.5;
            currentPixel.a = 1;
        }
        if (direction == 0) posX += 10;
        if (direction == 1) posY += 10;
        if (direction == 2) posX -= 10;
        if (direction == 3) posY -= 10;
    }

    int exitX = posX / 10 * 10 + 1; 
    for (int k = 0; k < 9; ++k) {
        cs225::HSLAPixel & exitPixel = resultMaze->getPixel(exitX + k, posY + 5);
        exitPixel.l = 1.0; 
    }

    return resultMaze;
    
}

// DONE
void SquareMaze::makeMaze(int width, int height) {
    w_ = width;
    h_ = height;
    size_ = w_ * h_;
    dsets.addelements(size_);
    bool_vec.assign(size_,{true,true});

    auto can_remove_wall = [&](int x, int y, bool is_right_wall) {
        return is_right_wall ? (x < w_ - 1) : (y < h_ - 1);
    };
    while (dsets.size(0)!=size_) {
        int X = rand() % w_;
        int Y = rand() % h_;
        bool is_right_wall = rand() % 2;
        int current_index = X + Y * w_;
        int adj_index = is_right_wall ? (X+1) + Y * w_ : X + (Y+1) * w_;

        if (can_remove_wall(X,Y,is_right_wall)&&bool_vec[current_index].second != is_right_wall) {
            if (dsets.find(current_index)!=dsets.find(adj_index)) {
                setWall(X,Y,!is_right_wall,false);
                dsets.setunion(current_index,adj_index);
            }
        }
    }
}

// DONE
void SquareMaze::setWall(int x, int y, int dir, bool exists) {
   
    int index = x + y * w_; 
    switch (dir) {
        case 0:
            bool_vec[index].first = exists;
            break;
        case 1:
            bool_vec[index].second = exists;
            break;
    }
}

// DONE
std::vector<int> SquareMaze::solveMaze() {

    std::vector<int> pathToEnd;
    std::vector<bool> explored(size_, false);
    std::vector<int> distance(size_, 0);
    std::vector<int> previous(size_, -1);
    std::queue<int> frontier;

    int startIndex = 0;
    explored[startIndex] = true;
    frontier.push(startIndex);

    while (!frontier.empty()) {
        int current = frontier.front();
        frontier.pop();
        int x = current % w_;
        int y = current / w_;

        for (int dir = 0; dir < 4; ++dir) {
            if (canTravel(x, y, dir)) {
                int next = current + ((dir == 0) ? 1 : (dir == 1) ? w_ : (dir == 2) ? -1 : -w_);
                if (!explored[next]) {
                    distance[next] = distance[current] + 1;
                    previous[next] = current;
                    explored[next] = true;
                    frontier.push(next);
                }
            }
        }
    }

    int maxDistance = 0;
    int finalIndex = startIndex;
    for (int i = size_ - w_; i < size_; ++i) {
        if (distance[i] > maxDistance) {
            maxDistance = distance[i];
            finalIndex = i;
        }
    }

    for (int at = finalIndex; at != startIndex; at = previous[at]) {
        int stepDirection = determineDirection(previous[at], at);
        pathToEnd.insert(pathToEnd.begin(), stepDirection);
    }

    return pathToEnd;

}

// DONE
int SquareMaze::determineDirection(int fromIndex, int toIndex) {
    if (toIndex == fromIndex + 1) return 0;
    if (toIndex == fromIndex + w_) return 1;
    if (toIndex == fromIndex - 1) return 2;
    if (toIndex == fromIndex - w_) return 3;
    return -1;
}
