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
    velocity.setZero();
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

void Agent::calculateMotionA()
{
    /** Defines the Scenario A agent policy:
     *      - attempt ot always position between A and B
     *      - avoid collisions TODO
     *      - searches A and B, if not in view TODO
     *      - account for orientation TODO (maybe some smooth motion policy)
     */
    bool inViewA = inView(chosen_agent_A);
    bool inViewB = inView(chosen_agent_B);
    if(!inView(chosen_agent_A) && !inView(chosen_agent_B)){
        // Random Movement
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> distr(0, 2*(float)M_PI);
        float angle = distr(gen);

        acceleration(0, 0) = cosf(angle) * config.accel;
        acceleration(1, 0) = sinf(angle) * config.accel;

    }
    else if(!inViewA || !inViewB) {
        Agent* visible_agent = (inViewA) ? chosen_agent_A : chosen_agent_B;
        Eigen::Matrix<float, 2, 1> dir = visible_agent->getPose() - pose;
        float norm_2 = powf(dir(0, 0), 2) + powf(dir(1, 0), 2);
//        if (sqrtf(norm_2) < 2 * config.body_radius){
//            dir *= 0;
//        }
        if (norm_2 > 0.5f) { // TODO hack for robustness
            dir = dir / sqrtf(norm_2);
        }
//        std::cout << norm_2 << std::endl;
        acceleration = dir * config.accel;

    }
    else {
        // Need to find distance to connecting vector of the other two agents
        auto poseA = chosen_agent_A->getPose();
        auto poseB = chosen_agent_B->getPose();
        auto dir = poseB - poseA;  // Connecting vector from A to B
        float length_dir_squared = powf(dir(0, 0), 2) + powf(dir(1, 0), 2);
        // need dot(ourPose - poseA, poseB - ourPose)
        float dot = (pose(0, 0) - poseA(0, 0)) * (poseB(0, 0) - pose(0, 0))
                + (pose(1, 0) - poseA(1, 0)) * (poseB(1, 0) - pose(1, 0));
        float t = fmax(0, fmin(1, dot / length_dir_squared));
//        auto projection = poseA + t * dir;
//     Eigen::Matrix<float, 3, 1> movement_dir = projection - pose;
        Eigen::Matrix<float, 2, 1> movement_dir = (poseB + poseA) / 2.0f - pose;
        float norm_2 = powf(movement_dir(0, 0), 2) + powf(movement_dir(1, 0), 2);
//        if(sqrtf(norm_2) < 2 * config.body_radius){
//            movement_dir*=0;
//        }
        if (norm_2 > 1.0f) { // TODO hack for robustness
            movement_dir = movement_dir / sqrtf(norm_2);
        }
//        std::cout << norm_2 << std::endl;
        acceleration = movement_dir * config.accel;
    }
}
void Agent::calculateMotionB()
{
    // Want to get away from agent B and be protected from them by agent A
    bool inViewA = inView(chosen_agent_A);
    bool inViewB = inView(chosen_agent_B);
    if(!inView(chosen_agent_A) && !inView(chosen_agent_B)){
        // Random Movement
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> distr(0, 2*(float)M_PI);
        float angle = distr(gen);

        acceleration(0, 0) = cosf(angle) * config.accel;
        acceleration(1, 0) = sinf(angle) * config.accel;

    }
    else if(!inViewA || !inViewB) {
        if (!inViewA){
            // Means we only see B -> run!
            Eigen::Matrix<float, 2, 1> dir = chosen_agent_B->getPose() - pose;
            float norm_2 = powf(dir(0, 0), 2) + powf(dir(1, 0), 2);
            if (norm_2 > 0.5f) { // TODO hack for robustness
                dir = dir / sqrtf(norm_2);
            }
            acceleration = -dir * config.accel;
        }
        else {
            // Hug agent A
            Eigen::Matrix<float, 2, 1> dir = chosen_agent_A->getPose() - pose;
            float norm_2 = powf(dir(0, 0), 2) + powf(dir(1, 0), 2);
            if (norm_2 > 0.5f) { // TODO hack for robustness
                dir = dir / sqrtf(norm_2);
            }
            acceleration = dir * config.accel;
        }
    }
    else {
        // Both in view, we want to get behind agent A, we offset that by half a meter!
        Eigen::Matrix<float, 2, 1> dir_agent = (chosen_agent_A->getPose() - chosen_agent_B->getPose());
        Eigen::Matrix<float, 2, 1> goal_point = dir_agent/dir_agent.norm() * 0.5f + chosen_agent_A->getPose();
        Eigen::Matrix<float, 2, 1> movement_dir = goal_point - pose;
        float norm_2 = powf(movement_dir(0, 0), 2) + powf(movement_dir(1, 0), 2);
//        if(sqrtf(norm_2) < 2 * config.body_radius){
//            movement_dir*=0;
//        }
        if (norm_2 > 1.0f) { // TODO hack for robustness
            movement_dir = movement_dir / sqrtf(norm_2);
        }
//        std::cout << norm_2 << std::endl;
        acceleration = movement_dir * config.accel;
    }
}
void Agent::calculateMotion()
{
    if (config.policy == Policy::A){
        calculateMotionA();
    }
    else if (config.policy == Policy::B){
        calculateMotionB();
    }
}
void Agent::setConfig(AgentConfig& config)
{
    this->config = config;
}
