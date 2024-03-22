//
// Created by finn on 3/15/24.
//
#include "mas_sim/mas_sim.h"
#include "mas_sim/agent.h"
#include "vector"
#include <iostream>

#include <chrono>
using namespace std::chrono;
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


MASSim::MASSim(int n_agents, float size_x, float size_y): n_agents(n_agents), agents() {
    for(int i = 0; i < n_agents; i++){
        agents.push_back(new AgentA(&agents, AgentConfig()));
        agents.back()->reset(size_x, size_y);
    }
    for(auto& agent: agents){
        agent->pickAgents();
    }
}

void MASSim::step() {
    auto start = high_resolution_clock::now();
    for (auto& agent: agents) {
        agent->calculateMotion();
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "After motion update " << duration.count() <<std::endl;
    for (auto& agent: agents) {
        agent->step();
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    std::cout << "After step " << duration.count() <<std::endl;
    bool collision_free = false;
    while(!collision_free) {
        collision_free = true;
        if(correctAgentPairs()){
            collision_free = false;
        }
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    std::cout << "After step " << duration.count() <<std::endl;
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
            float col_distance = agents[i]->getRadius() + agents[j]->getRadius();

            auto pose1 = agents[i]->getPose();
            auto pose2 = agents[j]->getPose();
            if(abs(pose1(0) - pose2(0)) < col_distance && abs(pose1(1) - pose2(1)) < col_distance) {

                double x1 = pose1(0, 0);
                double y1 = pose1(1, 0);
                double x2 = pose2(0, 0);
                double y2 = pose2(1, 0);
                double dx = x2 - x1;
                double dy = y2 - y1;
                double d = sqrt(dx * dx + dy * dy);
                if (d < col_distance) {
                    double difference = col_distance - d + 0.01;
                    // Correct the position such that the distance is collision_radius
                    x1 -= 0.5 * difference * dx / d;
                    y1 -= 0.5 * difference * dy / d;
                    x2 += 0.5 * difference * dx / d;
                    y2 += 0.5 * difference * dy / d;
                    collision_free = false;
                    required_correction = true;
                }
                agents[i]->correctPose(x1, y1);
                agents[j]->correctPose(x2, y2);
            }
        }
    }
    return required_correction;
}

int MASSim::getN() const {
    return n_agents;
}

std::vector<std::array<float, 2>> MASSim::getAgents() {
    std::vector<std::array<float, 2>> agents_vec(n_agents);
    for (int i = 0; i < n_agents; i++){
        auto pose = agents[i]->getPose();
        agents_vec[i] = std::array<float, 2>{pose[0], pose[1]};
    }
    return agents_vec;
}

void MASSim::reset() {
    for(int i = 0; i < n_agents; i++){
        agents[i]->reset(10.0f, 10.0f);
    }
    for(auto& agent: agents){
        agent->pickAgents();
    }
}

std::vector<Agent *> MASSim::getAgentsDirect() {
    return agents;
}

