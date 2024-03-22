//
// Created by finn on 3/15/24.
//
#include <iostream>
#include <random>
#include "mas_sim/agentA.h"

void AgentA::calculateMotion() {
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

        motion_input(0, 0) = cosf(angle) * config.dt * config.speed;
        motion_input(1, 0) = sinf(angle) * config.dt * config.speed;
//        static std::uniform_real_distribution<float> distr(-10*M_PI/180, 10 * M_PI/180);
//        float angle = distr(gen);
//        pose(2, 0) += angle;
//        motion_input(0, 0) = cosf(pose(2, 0)) * config.dt * config.speed;
//        motion_input(1, 0) = sinf(pose(2, 0)) * config.dt * config.speed;
    }
    else if(!inViewA || !inViewB) {
        Agent* visible_agent = (inViewA) ? chosen_agent_A : chosen_agent_B;
        Eigen::Matrix<float, 3, 1> dir = visible_agent->getPose() - pose;
        float norm_2 = powf(dir(0, 0), 2) + powf(dir(1, 0), 2);
        if (norm_2 > 0.5f) { // TODO hack for robustness
            dir = dir / sqrtf(norm_2);
        }
//        std::cout << norm_2 << std::endl;
        motion_input = dir * config.dt * config.speed;

    }
    else if((!inViewB)){

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
        auto projection = poseA + t * dir;
//     Eigen::Matrix<float, 3, 1> movement_dir = projection - pose;
        Eigen::Matrix<float, 3, 1> movement_dir = (poseB + poseA) / 2.0f - pose;
        float norm_2 = powf(movement_dir(0, 0), 2) + powf(movement_dir(1, 0), 2);
        if (norm_2 > 1.0f) { // TODO hack for robustness
            movement_dir = movement_dir / sqrtf(norm_2);
        }
//        std::cout << norm_2 << std::endl;
        motion_input = movement_dir * config.dt * config.speed;
    }
}

AgentA::AgentA(const std::vector<Agent *> *agents, AgentConfig config) : Agent(agents, config)
{

}

AgentA::AgentA(const std::vector<Agent *> *agents) : Agent(agents) {

}
