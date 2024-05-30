//
// Created by finn on 3/15/24.
//
#include <chrono>
#include "mas_sim/mas_sim.h"
#include <iostream>
using namespace std::chrono;
int main(){
    MASSim sim(500, 20.0, 10);
//    Viz viz;
//    viz.addDrawable(&sim);

    while(true){
        auto start = high_resolution_clock::now();
        sim.step();
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        std::cout << "Dt " << duration.count() <<std::endl;

//        viz.draw(1);
//        sf::sleep(sf::milliseconds(10));
    }
}
