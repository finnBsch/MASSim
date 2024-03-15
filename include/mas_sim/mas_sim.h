//
// Created by finn on 3/15/24.
//

#ifndef MASSIM_MAS_SIM_H
#define MASSIM_MAS_SIM_H
#include "agentA.h"
#include "agentB.h"

class MASSim: public DrawableObject {
private:
    int n_agents;
    std::vector<Agent*> agents;

    bool correctAgentPairs();
public:
    void step();
    MASSim(int n_agents);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif //MASSIM_MAS_SIM_H
