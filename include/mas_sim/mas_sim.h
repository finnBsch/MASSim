//
// Created by finn on 3/15/24.
//

#ifndef MASSIM_MAS_SIM_H
#define MASSIM_MAS_SIM_H
#include "agentA.h"
#include "agentB.h"

class MASSim{
private:
    int n_agents;
    std::vector<Agent*> agents;

    bool correctAgentPairs();
public:
    void step();
    MASSim(int n_agents);

};

#endif //MASSIM_MAS_SIM_H
