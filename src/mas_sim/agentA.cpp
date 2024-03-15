//
// Created by finn on 3/15/24.
//
#include <iostream>
#include "mas_sim/agentA.h"

void AgentA::calculateMotion() {
    /** Defines the Scenario A agent policy:
     *      - attempt ot always position between A and B
     *      - avoid collisions TODO
     *      - searches A and B, if not in view TODO
     *      - account for orientation TODO (maybe some smooth motion policy)
     */
    if(!inView(chosen_agent_A) || !inView(chosen_agent_B)){
        motion_input.setZero();
        return;
    }
    // Need to find distance to connecting vector of the other two agents
    auto poseA = chosen_agent_A->getPose();
    auto poseB = chosen_agent_B->getPose();
    auto dir = poseB - poseA;  // Connecting vector from A to B
    float length_dir_squared = powf(dir(0, 0), 2) + powf(dir(1, 0), 2);
    // need dot(ourPose - poseA, poseB - ourPose)
    float dot = (pose(0, 0) - poseA(0, 0)) * (poseB(0, 0) - pose(0, 0))
                + (pose(1, 0) - poseA(1, 0)) * (poseB(1, 0) - pose(1, 0));
    float t = fmax(0, fmin(1, dot/length_dir_squared));
    auto projection = poseA + t * dir;
//     Eigen::Matrix<float, 3, 1> movement_dir = projection - pose;
    Eigen::Matrix<float, 3, 1> movement_dir = (poseB + poseA)/2.0f - pose;
    float norm_2 = powf(movement_dir(0, 0), 2) + powf(movement_dir(1, 0), 2);
    if (norm_2 > 1.0f) { // TODO hack for robustness
        movement_dir = movement_dir / sqrtf(norm_2);
    }
    std::cout << norm_2 << std::endl;
    motion_input = movement_dir * config.dt * config.speed;
}

AgentA::AgentA(const std::vector<Agent *> *agents, AgentConfig config) : Agent(agents, config)
{

}

AgentA::AgentA(const std::vector<Agent *> *agents) : Agent(agents) {

}
