//
// Created by finn on 3/15/24.
//
#include "mas_sim/mas_sim.h"
#include "mas_sim/agent.h"
#include "vector"
#include <iostream>

MASSim::MASSim(int n_agents):
        n_agents(n_agents),
        agents()
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
    bool collision_free = false;
    while(!collision_free) {
        collision_free = true;
        if(correctAgentPairs()){
            collision_free = false;
        }
    }

}


bool MASSim::correctAgentPairs() {
    bool collision_free = false;
    bool required_correction = false;
    // Get all possible agent pairs
    std::vector<std::array<int, 2>> agent_pairs;
    for(int i = 0; i < n_agents; i++){
        for(int j = i+1; j < n_agents; j++){
            std::array<int, 2> pair = {i, j};
            agent_pairs.push_back(pair);
        }
    }
    while(!collision_free){
        collision_free = true;
        for (auto& pair: agent_pairs){
            int i = pair[0];
            int j = pair[1];
            auto pose1 = agents[i]->getPose();
            auto pose2 = agents[j]->getPose();
            double x1 = pose1(0,0);
            double y1 = pose1(1, 0);
            double x2 = pose2(0, 0);
            double y2 = pose2(1, 0);
            double dx = x2 - x1;
            double dy = y2 - y1;
            double d = sqrt(dx * dx + dy * dy);
            float col_distance = agents[i]->getRadius() + agents[j]->getRadius();
            if(d < col_distance) {
                double difference = col_distance - d + 0.01;
                // Correct the position such that the distance is collision_radius
                x1 -= 0.5*difference * dx / d;
                y1 -= 0.5*difference * dy / d;
                x2 += 0.5*difference * dx / d;
                y2 += 0.5*difference * dy / d;
                collision_free = false;
                required_correction = true;
            }
            agents[i]->correctPose(x1, y1);
            agents[j]->correctPose(x2, y2);
        }
    }
    return required_correction;
}
