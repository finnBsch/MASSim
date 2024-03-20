//
// Created by finn on 3/15/24.
//

#ifndef MASSIM_AGENT_H
#define MASSIM_AGENT_H
#include <eigen3/Eigen/Dense>

struct AgentConfig {
    float dt = 0.01f; // s
    float speed = 5.0f; // m/s
    int fov_angle = 360;
    float perception_radius = 4.0f;
    float body_radius = 0.04f;
};

struct AgentVizConfig {
    int n_pts_fov = 40;
};

class Agent{
protected:
    AgentConfig config;
    AgentVizConfig viz_config;

    Agent* chosen_agent_A;
    Agent* chosen_agent_B;
    //
    Eigen::Matrix<float, 3, 1> pose; // x/y/yaw
    Eigen::Matrix<float, 3, 1> motion_input;
    // Perception
    const std::vector<Agent*>* agents;



    // Runtime methods
    bool inView(Agent* agent, bool fov_check=false);

public:
    Agent(const std::vector<Agent*>* agents, AgentConfig config);
    Agent(const std::vector<Agent*>* agents);
    virtual void calculateMotion() = 0;
    void step();

    // Runtime methods
    void pickAgents();
    const Eigen::Matrix<float, 3, 1>& getPose() const;
    void reset(float x_max, float y_max);
    void correctPose(float x, float y);

    // Param get set
    float getRadius();
};

#endif //MASSIM_AGENT_H
