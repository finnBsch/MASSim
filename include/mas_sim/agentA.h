//
// Created by finn on 3/15/24.
//

#ifndef MASSIM_AGENTA_H
#define MASSIM_AGENTA_H
#include "agent.h"

class AgentA: public Agent {
public:
    AgentA(const std::vector<Agent*>* agents, AgentConfig config);
    AgentA(const std::vector<Agent*>* agents);
    void calculateMotion() override;
};
#endif //MASSIM_AGENTA_H
