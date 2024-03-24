//
// Created by finn on 3/23/24.
//

#ifndef MASSIM_GRID_H
#define MASSIM_GRID_H
#include "mas_sim/agent.h"
#include <set>
#include <vector>

class GridCell {
public:
    std::vector<Agent*> agents;
    GridCell(){};
    void addAgent(Agent* agent);
    void removeAgent(Agent* agent);

};
class Grid {
private:
    float cell_size_x;
    float cell_size_y;
    int nx;
    int ny;

public:
    std::vector<std::vector<GridCell>> grid_cells;
    Grid(){};
    Grid(std::vector<Agent*> agents, float size_x, float size_y, float cell_size);
    void assignAgent(Agent* agent);
    int getNx();
    int getNy();
    GridCell* getAgents(int cell_idx, int cell_idy);
    void clear();
    void addAgents(std::vector<Agent*> agents);
};
#endif //MASSIM_GRID_H
