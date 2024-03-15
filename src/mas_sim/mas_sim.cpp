//
// Created by finn on 3/15/24.
//
#include "mas_sim/mas_sim.h"
#include "mas_sim/agent.h"
#include <iostream>

MASSim::MASSim(int n_agents):
    n_agents(n_agents)
{
    for(int i = 0; i < n_agents; i++){
        agents.push_back(new AgentA(&agents, AgentConfig()));
        agents.back()->reset(10.0f, 10.0f);
    }
    for(auto& agent: agents){
        agent->pickAgents();
    }
}

void MASSim::step() {
    for (auto& agent: agents) {
        agent->calculateMotion();
    }
    for (auto& agent: agents) {
        agent->step();
    }
}

void MASSim::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for(auto& agent: agents){
        target.draw(*agent, states);
        auto pose = agent->getPose();
    }
}
