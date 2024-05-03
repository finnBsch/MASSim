//
// Created by finn on 3/15/24.
//

#ifndef MASSIM_AGENT_H
#define MASSIM_AGENT_H
#include <eigen3/Eigen/Dense>

enum class Policy{
    A,
    B
};

struct AgentConfig {
    float speed = 5.0f; // m/s
    float sampled_deviation = 0.0f; // From -1 to 1
    float speed_variance = 0.0f;
    float accel = 1.0f;
    int fov_angle = 360;
    float perception_radius = 6.0f;
    float body_radius = 0.04f;
    Policy policy;

};

struct AgentVizConfig {
    int n_pts_fov = 40;
};

class Agent{
protected:
    int grid_id_x = -1;
    int grid_id_y = -1;
    AgentConfig config;
    AgentVizConfig viz_config;

    //
    Eigen::Matrix<float, 2, 1> pose; // x/y/yaw
    Eigen::Matrix<float, 2, 1> velocity;
    Eigen::Matrix<float, 2, 1> acceleration;

    // Perception



    // Runtime methods
    bool inView(Agent* agent, bool fov_check=false);
    void calculateMotionA();
    void calculateMotionB();

public:
    Agent(AgentConfig config);
    Agent();
    void calculateMotion();
    void step(float dt);

    // Runtime methods
    void pickAgents(std::vector<Agent*>& agents);
    const Eigen::Matrix<float, 2, 1>& getPose() const;
    const Eigen::Matrix<float, 2, 1>& getVelocity() const;
    void reset(float x_max, float y_max);
    void correctPose(float x, float y);
    void correctPose(Eigen::Matrix<float, 2, 1> pose);
    void setVelocity(Eigen::Matrix<float, 2, 1> velocity);

    // Param get set
    float getSpeed();
    float getRadius();
    float getX();
    float getY();
    int getGridX();
    int getGridY();
    void setConfig(AgentConfig& config);
    void setGridId(int idx, int idy);

    Agent* chosen_agent_A;
    Agent* chosen_agent_B;
    float distance_to_goal = -1;
};

#endif //MASSIM_AGENT_H
