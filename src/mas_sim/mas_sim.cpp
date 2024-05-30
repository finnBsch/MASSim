//
// Created by finn on 3/15/24.
//
#include "mas_sim/mas_sim.h"
#include "mas_sim/agent.h"
#include "vector"
#include "array"
#include <iostream>

#include <chrono>
#include <random>

using namespace std::chrono;

template <typename T>
void remove_at(std::vector<T>& v, typename std::vector<T>::size_type n)
{
    std::swap(v[n], v.back());
    v.pop_back();
}

MASSim::MASSim(int n_agents, float size_x, float size_y): n_agents(n_agents), agents(),
                                                          size_x(size_x), size_y(size_y),
                                                          toCheck()
{
    for(int i = 0; i < n_agents; i++){
        agents.push_back(new Agent(defaultConf));
        agents.back()->reset(size_x, size_y);
    }
    for(auto agent: agents){
        agent->pickAgents(agents);
    }
//    std::vector<std::array<int, 2>> agent_pairs;
    for(int i = 0; i < n_agents; i++){
        for(int j = i+1; j < n_agents; j++){
            std::array<int, 2> pair = {i, j};
            agent_pairs.push_back(pair);
        }
    }
    grid = Grid(agents, size_x, size_y, 0.5);
}

void MASSim::step() {
//    auto start = high_resolution_clock::now();
    for (auto &agent: agents) {
        agent->calculateMotion();
    }
    for(int i = 0; i < sub_steps; i++) {

//    auto stop = high_resolution_clock::now();
//    auto duration = duration_cast<microseconds>(stop - start);
//    std::cout << "After motion update " << duration.count() <<std::endl;
        for (auto &agent: agents) {
            agent->step(1.0f/100.0f / (float)sub_steps);
            correctSingleAgent(agent);
        }
//    stop = high_resolution_clock::now();
//    duration = duration_cast<microseconds>(stop - start);
//    std::cout << "After step " << duration.count() <<std::endl;
        correctAgentPairs();
//        for(int k = 0; k < agents.size(); k++) {
//            checkSingleAgent(k);
//        }
//        while(!toCheck.empty()){
//            int agent_id = toCheck.front();
//            checkSingleAgent(agent_id);
//            toCheck.pop();
//        }
//        for(int k = 0; k < 20 ; k++) {
//            collision_free = true;
//            grid.clear();
//            grid.addAgents(agents);
//            if(!correctGridBased()) {
//                collision_free = false;
//            }
//        }
//}
//    stop = high_resolution_clock::now();
//    duration = duration_cast<microseconds>(stop - start);
//    std::cout << "After step " << duration.count() <<std::endl;
    }
}
void MASSim::checkSingleAgent(int i) {
//    bool collision_free = false;
//    bool required_correction = false;
    for (int j = 0; j < agents.size(); j++){
        if(i != j) {
            float col_distance = agents[i]->getRadius() + agents[j]->getRadius();

            auto pose1 = agents[i]->getPose();
            auto pose2 = agents[j]->getPose();
            if (abs(pose1(0) - pose2(0)) < col_distance && abs(pose1(1) - pose2(1)) < col_distance) {
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
//                    collision_free = false;
//                    required_correction = true;
                    agents[i]->correctPose(x1, y1);
                    agents[j]->correctPose(x2, y2);
//                    checkSingleAgent(i);
//                    checkSingleAgent(j);
                    toCheck.push(i);
                    toCheck.push(j);
                }

//                checkSingleAgent(agent);
//                checkSingleAgent(agent1);
            }
        }
    }
}

bool MASSim::correctAgentPairs() {
    // TODO Add walls
    bool collision_free = false;
    bool required_correction = false;
    // Get all possible agent pairs
//    std::vector<std::array<int, 2>> agent_pairs;
//    for(int i = 0; i < n_agents; i++){
//        for(int j = i+1; j < n_agents; j++){
//            std::array<int, 2> pair = {i, j};
//            agent_pairs.push_back(pair);
//        }
//    }
    while(!collision_free){
        collision_free = true;
        for (int i = 0; i < agents.size(); i++){
            for (int j = i + 1; j < agents.size(); j++) {
//                int i = pair[0];
//                int j = pair[1];
                float col_distance = 0.08; // TODO THIS IS HARDCODED FOR SPEED

                auto pose1 = agents[i]->getPose();
                auto pose2 = agents[j]->getPose();
                if (abs(pose1(0) - pose2(0)) < col_distance && abs(pose1(1) - pose2(1)) < col_distance) {
                    auto delta = pose2 - pose1;
                    double d = delta.norm();
                    if (d < col_distance && d > 0.0001) {
                        auto vel1 = agents[i]->getVelocity();
                        auto vel2 = agents[j]->getVelocity();
                        auto delta_speed = vel2 - vel1;
                        auto col_normal = delta / d;
                        float constraint_speed = col_normal.dot(delta_speed);
                        double difference = col_distance - d + 0.001f;
                        // Correct the position such that the distance is collision_radius
                        pose1 -= difference * col_normal * 0.5;
                        pose2 += difference * col_normal * 0.5;
//                    collision_free = false;
                        required_correction = true;
                        agents[i]->correctPose(pose1);
                        agents[j]->correctPose(pose2);
                        auto impulse = col_normal * (-constraint_speed * (1.0f + elasticity));
                        agents[i]->setVelocity(vel1 - impulse * 0.5f);
                        agents[j]->setVelocity(vel2 + impulse * 0.5f);
                        correctSingleAgent(agents[i]);
                        correctSingleAgent(agents[j]);
                    }

                }
            }
        }
    }
    return false;
}

int MASSim::getN() const {
    return n_agents;
}

std::vector<std::array<float, 4>> MASSim::getAgents() {
    std::vector<std::array<float, 4>> agents_vec(n_agents);
    for (int i = 0; i < n_agents; i++){
        auto pose = agents[i]->getPose();
        agents_vec[i] = std::array<float, 4>{pose[0], pose[1], agents[i]->distance_to_goal, agents[i]->getSpeed()};
    }
    return agents_vec;
}

void MASSim::reset() {
    for(int i = 0; i < n_agents; i++){
        agents[i]->reset(size_x, size_y);
        correctSingleAgent(agents[i]);
    }
    for(auto& agent: agents){
        agent->pickAgents(agents);
    }
}

std::vector<Agent *> MASSim::getAgentsDirect() {
    return agents;
}

void MASSim::setSpeed(float speed) {
    defaultConf.speed = speed;
    for (auto& agent:agents){
        agent->setConfig(defaultConf);
    }

}

void MASSim::setSpeedHeterogeneity(float speedHeterogeneity) {
    defaultConf.speedHeterogeneity = speedHeterogeneity;
    for (auto& agent:agents){
        agent->setConfig(defaultConf);
    }
}

bool MASSim::correctGridBased() {
    int gnx = grid.getNx();
    int gny = grid.getNy();
    bool collision_free = true;
    for (int i = 0; i < gnx; i++){
        for (int j = 0; j < gny; j++){

            collision_free = collision_free &&  correctSingleCell(i, j);
        }
    }
    return collision_free;
}

bool MASSim::correctSingleCell(int nx, int ny) {
    bool collision_free = true;
    GridCell* agents_cell = &grid.grid_cells[nx][ny];
    int idx0 = (nx > 0) ? -1 : 0;
    int idx1 = (nx < grid.getNx() - 1) ? 1 : 0;
    int idy0 = (ny > 0) ? -1 : 0;
    int idy1 = (ny < grid.getNy() - 1) ? 1 : 0;
    for (auto agent: agents_cell->agents){
        for (int i = idx0; i <= idx1; i++){
            for (int j = idy0; j <= idy1; j++){
                collision_free = collision_free && correctAgentGroup(agent, &grid.grid_cells[nx + i][ny + j]);
            }
        }
    }
    return collision_free;
}

bool MASSim::correctAgentGroup(Agent *agent0, GridCell * agents_group) {
//    std::cout << agents_group->agents.size() <<std::endl;
    bool collision_free = true;
    for (auto agent1: agents_group->agents) {
        // Do collision checks..
        if (agent1 != agent0) {
            float col_distance = agent0->getRadius() + agent1->getRadius();

            auto pose1 = agent0->getPose();
            auto pose2 = agent1->getPose();
            if (true || abs(pose1(0) - pose2(0)) < col_distance && abs(pose1(1) - pose2(1)) < col_distance) {

                float x1 = pose1(0, 0);
                float y1 = pose1(1, 0);
                float x2 = pose2(0, 0);
                float y2 = pose2(1, 0);
                float dx = x2 - x1;
                float dy = y2 - y1;
                float d = sqrtf(dx * dx + dy * dy);
                if (d < col_distance) {
                    float delta = (col_distance - d + 0.01f)*0.5f;
                    // Correct the position such that the distance is collision_radius
                    x1 -= delta * dx / d;
                    y1 -= delta * dy / d;
                    x2 += delta * dx / d;
                    y2 += delta * dy / d;
                    collision_free = false;
//                    std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
                    agent0->correctPose(x1, y1);
                    agent1->correctPose(x2, y2);
                    correctSingleAgent(agent0);
                    correctSingleAgent(agent1);
                }
            }
        }
    }
    return collision_free;
}

void MASSim::correctSingleAgent(Agent *agent) {
    if (!walls_enabled) {
        return;
    }
    float x = agent->getX();
    float y = agent->getY();
    if (x < defaultConf.body_radius){
        auto vel = agent->getVelocity();
        x = defaultConf.body_radius;
        agent->correctPose(x, y);
        auto impulse = -vel[0] * (1.0f + elasticity);
        Eigen::Vector2f new_vel = vel;
        new_vel[0] += impulse;
        agent->setVelocity(new_vel);
    }
    else if (x > size_x - defaultConf.body_radius){
        auto vel = agent->getVelocity();
        x = size_x - defaultConf.body_radius;
        agent->correctPose(x, y);
        auto impulse = -vel[0] * (1.0f + elasticity);
        Eigen::Vector2f new_vel = vel;
        new_vel[0] += impulse;
        agent->setVelocity(new_vel);
    }
    if (y < defaultConf.body_radius){
        auto vel = agent->getVelocity();
        y = defaultConf.body_radius;
        agent->correctPose(x, y);
        auto impulse = -vel[1] * (1.0f + elasticity);
        Eigen::Vector2f new_vel = vel;
        new_vel[1] += impulse;
        agent->setVelocity(new_vel);
    }
    else if (y > size_y - defaultConf.body_radius){
        auto vel = agent->getVelocity();
        y = size_y - defaultConf.body_radius;
        agent->correctPose(x, y);
        auto impulse = -vel[1] * (1.0f + elasticity);
        Eigen::Vector2f new_vel = vel;
        new_vel[1] += impulse;
        agent->setVelocity(new_vel);
    }
//    x = std::max(defaultConf.body_radius*2, std::min(x, size_x - defaultConf.body_radius*2));
//    y = std::max(defaultConf.body_radius*2, std::min(y, size_y - defaultConf.body_radius*2));

    agent->correctPose(x, y);
}

void MASSim::setNumAgents(int num_agents) {
    if(num_agents == n_agents){
        return;
    }
    static std::random_device rd; // obtain a random number from hardware
    static std::mt19937 gen(rd()); // seed the generator
    if(num_agents > n_agents){
        while(num_agents > n_agents){
            agents.push_back(new Agent(defaultConf)); // TODO Merge config
            agents.back()->reset(size_x, size_y);
            n_agents++;
        }
    }
    else{
        agents.erase(agents.begin() + num_agents, agents.end());
        n_agents = num_agents;
    }
    for(auto agent: agents){
        agent->pickAgents(agents);
    }
}

void MASSim::setPerceptionRadius(float radius) {
    defaultConf.perception_radius = radius;
    for (auto& agent:agents){
        agent->setConfig(defaultConf);
    }
}

std::array<int, 3> MASSim::getFocusAgent(float x, float y) {
    for (int i = 0; i < agents.size(); i++){
        auto agent = agents[i];
        if (abs(x - agent->getX()) < agent->getRadius() && abs(y - agent->getY()) < agent->getRadius() ){
            auto it = find(agents.begin(), agents.end(), agent->chosen_agent_A);
            int id0 = it - agents.begin();
            it = find(agents.begin(), agents.end(), agent->chosen_agent_B);
            int id1 = it - agents.begin();
            return std::array<int, 3>{i, id0, id1};
        }
    }
    return std::array<int, 3>{-1, -1, -1};
}

void MASSim::setElasticity(float elasticity) {
    this->elasticity = elasticity;
}

void MASSim::setPolicy1() {
    defaultConf.policy = Policy::A;
    for (auto& agent:agents){
        agent->setConfig(defaultConf);
    }
    // for(int i = 0; i < n_agents; i++){
    //     agents[i]->reset(size_x, size_y);
    //     correctSingleAgent(agents[i]);
    // }
    // for(auto& agent: agents){
    //     agent->pickAgents(agents);
    // }
}

void MASSim::setPolicy2() {
    defaultConf.policy = Policy::B;
    for (auto& agent:agents){
        agent->setConfig(defaultConf);
    }
    // for(int i = 0; i < n_agents; i++){
    //     agents[i]->reset(size_x, size_y);
    //     correctSingleAgent(agents[i]);
    // }
    // for(auto& agent: agents){
    //     agent->pickAgents(agents);
    // }
}

void MASSim::toggleWalls(bool state) {
    walls_enabled = state;
}
