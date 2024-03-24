//
// Created by finn on 3/15/24.
//

#ifndef MASSIM_MAS_SIM_H
#define MASSIM_MAS_SIM_H
#include "agentA.h"
#include "agentB.h"
#include "agent.h"
#include "vector"
#include "grid.h"
#include <queue>
class MASSim{
private:
    int sub_steps=16;
    std::vector<std::array<int, 2>> agent_pairs;
    Grid grid;
    int n_agents;
    std::vector<Agent*> agents;
    float size_x;
    float size_y;
    bool correctAgentPairs();
    bool correctGridBased();
    bool correctSingleCell(int nx, int ny);
    bool correctAgentGroup(Agent* agent0, GridCell* agents_group);
    void checkSingleAgent(int i);
    void correctSingleAgent(Agent* agent);
    std::queue<int> toCheck;
public:

    void step();
    void reset();
    std::vector<std::array<float, 2>> getAgents();
    std::vector<Agent*> getAgentsDirect();
    int getN() const;
//    MASSim(int n_agents);
    MASSim(int n_agents, float size_x, float size_y);
    // API
    void setSpeed(float speed);
    void setNumAgents(int num_agents);

};

#endif //MASSIM_MAS_SIM_H
