//
// Created by finn on 3/15/24.
//

#ifndef MASSIM_AGENTA_H
#define MASSIM_AGENTA_H
#include "agent.h"

class AgentA: public Agent {
public:
    AgentA( AgentConfig config);
    AgentA();
    void calculateMotion() override;
};
#endif //MASSIM_AGENTA_H
