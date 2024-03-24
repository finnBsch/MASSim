//
// Created by finn on 3/23/24.
//
#include <iostream>
#include "mas_sim/grid.h"

Grid::Grid(std::vector<Agent *> agents, float size_x, float size_y, float cell_size) {
    nx = (int)ceilf(size_x/cell_size);
    ny = (int)ceilf(size_y/cell_size);
    cell_size_x = size_x/(float)nx;
    cell_size_y = size_y/(float)ny;
    std::cout << cell_size_x << " " << cell_size_y << std::endl;
    // Write vectors
    for(int i = 0; i < nx; i++){
        std::vector<GridCell> v;
        for(int j = 0; j < ny; j++){
            GridCell cell;
            v.push_back(cell);
        }
        grid_cells.push_back(v);
    }
//    grid_cells.resize(nx);
//    for (auto& row : grid_cells){
//        row.resize(ny);
//        for (auto& col : row){
//            col.agents.clear();
//        }
//    }
//    for(auto agent: agents){
//        assignAgent(agent);
//    }
}

void Grid::assignAgent(Agent *agent) {
    float x = agent->getX();
    float y = agent->getY();
    int id_x = (int)floorf(x/cell_size_x);
    int id_y = (int)floorf(y/cell_size_y);
//    std::cout << x << " " << id_x << std::endl;
//    int grid_id_x_old = agent->getGridX();
//    int grid_id_y_old = agent->getGridY();
//    if(id_x < 0){
//        std::cout << "WTF" << std::endl;
//    }
//    if(grid_id_x_old != id_x && grid_id_y_old != id_y) {
//        if (grid_id_x_old != -1) {
//            grid_cells[grid_id_x_old][grid_id_y_old].removeAgent(agent);
//        }
        grid_cells[id_x][id_y].addAgent(agent);
        agent->setGridId(id_x, id_y);
//        std::cout << "--" <<std::endl;
//        for(auto agent:grid_cells[id_x][id_y].agents){
//            std::cout << agent->getX() << std::endl;
//            std::cout << agent->getY() << std::endl;
//        }
//    }
}

int Grid::getNx() {
    return nx;
}

int Grid::getNy() {
    return ny;
}

GridCell* Grid::getAgents(int cell_idx, int cell_idy) {
    return &grid_cells[cell_idx][cell_idy];
}

void GridCell::addAgent(Agent * agent) {
    agents.push_back(agent);
}

void GridCell::removeAgent(Agent *agent) {
    agents.erase(std::remove(agents.begin(), agents.end(), agent), agents.end());
}

void Grid::clear(){
    for(auto& row: grid_cells){
        for(auto& col: row){
            col.agents.clear();
        }
    }
}

void Grid::addAgents(std::vector<Agent *> agents) {
    for(auto agent: agents){
        assignAgent(agent);
    }
}
