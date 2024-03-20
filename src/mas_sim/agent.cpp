//
// Created by finn on 3/15/24.
//
#include <random>
#include "mas_sim/agent.h"

void Agent::pickAgents() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distr(0, agents->size()-1);
    int id_0 = distr(gen);
    while (agents->at(id_0) == this){
        id_0 = distr(gen);
    }
    int id_1 = distr(gen);
    while (agents->at(id_1) == this || id_1 == id_0){
        id_1 = distr(gen);
    }
    chosen_agent_A = agents->at(id_0);
    chosen_agent_B = agents->at(id_1);
}

Agent::Agent(const std::vector<Agent *>* agents, AgentConfig config):
    Agent(agents) {
    this->config = config;
}

Agent::Agent(const std::vector<Agent *>* agents)
{
    this->agents = agents;

}

bool Agent::inView(Agent *agent, bool fov_check) {

    // Option with 360 field of view
    Eigen::Vector2f pose_diff;
    pose_diff << this->pose(0,0) - agent->pose(0,0), this->pose(1,0) - agent->pose(1,0);
    if (pose_diff.norm() <= this->config.perception_radius) {
        if (fov_check) {
            // TODO: check if in FOV
        }
        else {
            return true;
        }
    }
    // Additional check for limited FOV

    return false;
}

const Eigen::Matrix<float, 3, 1> &Agent::getPose() const {
    return pose;
}

void Agent::reset(float x_max, float y_max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> distr_x(0, x_max);
    static std::uniform_real_distribution<float> distr_y(0, y_max);
    static std::uniform_real_distribution<float> distr_phi(0, 2 * M_PI);
    pose(0, 0) = distr_x(gen);
    pose(1, 0) = distr_y(gen);
    pose(2, 0) = distr_phi(gen);
}

void Agent::step() {
    pose = pose + motion_input;
}

void Agent::correctPose(float x, float y) {
    pose(0,0) = x;
    pose(1,0) = y;
}

float Agent::getRadius() {
    return config.body_radius;
}
