//
// Created by finn on 3/15/24.
//
#include <random>
#include <iostream>
#include "mas_sim/agent.h"

void Agent::pickAgents(std::vector<Agent*>& agents) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, agents.size()-1);
    int id_0 = distr(gen);
    while (agents.at(id_0) == this){
        id_0 = distr(gen);
    }
    int id_1 = distr(gen);
    while (agents.at(id_1) == this || id_1 == id_0){
        id_1 = distr(gen);
    }
    chosen_agent_A = agents.at(id_0);
    chosen_agent_B = agents.at(id_1);
}

Agent::Agent(AgentConfig config):
    Agent() {
    this->config = config;
}

Agent::Agent()
{

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

const Eigen::Matrix<float, 2, 1> &Agent::getPose() const {
    return pose;
}

void Agent::reset(float x_max, float y_max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> distr_x(0, x_max);
    static std::uniform_real_distribution<float> distr_y(0, y_max);
    pose(0, 0) = distr_x(gen);
    pose(1, 0) = distr_y(gen);
    acceleration.setZero();
    grid_id_x = -1;
    grid_id_y = -1;
}

void Agent::step(float dt) {
    velocity = velocity + acceleration * dt - velocity*0.8 * dt;
    pose = pose + velocity*dt * config.speed;
}

void Agent::correctPose(float x, float y) {
    pose(0,0) = x;
    pose(1,0) = y;
}

float Agent::getRadius() {
    return config.body_radius;
}

float Agent::getX() {
    return pose(0, 0);
}

float Agent::getY() {
    return pose(1, 0);
}

void Agent::setSpeed(float speed) {
    config.speed = speed;
}

void Agent::setGridId(int idx, int idy) {
    grid_id_x = idx;
    grid_id_y = idy;
}

int Agent::getGridX() {
    return grid_id_x;
}

int Agent::getGridY() {
    return grid_id_y;
}

void Agent::setVelocity(Eigen::Matrix<float, 2, 1> velocity) {
    this->velocity = velocity;
}

void Agent::correctPose(Eigen::Matrix<float, 2, 1> pose) {
    this->pose = pose;
}

const Eigen::Matrix<float, 2, 1> &Agent::getVelocity() const {
    return velocity;
}

void Agent::setPerceptionRadius(float radius) {
    config.perception_radius = radius;
}
