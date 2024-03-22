//
// Created by finn on 3/15/24.
//

#ifndef MASSIM_MAS_SIM_H
#define MASSIM_MAS_SIM_H
#include "agentA.h"
#include "agentB.h"
#include "agent.h"
#include "vector"
class MASSim{
private:
    int n_agents;
    std::vector<Agent*> agents;

    bool correctAgentPairs();
public:
    void step();
    void reset();
    std::vector<std::array<float, 2>> getAgents();
    std::vector<Agent*> getAgentsDirect();
    int getN() const;
    MASSim(int n_agents);

};

#endif //MASSIM_MAS_SIM_H
