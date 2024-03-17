//
// Created by finn on 3/15/24.
//
#include "mas_sim/mas_sim.h"
int main(){
    MASSim sim(170);
    Viz viz;
    viz.addDrawable(&sim);
    while(true){
        sim.step();
        viz.draw(1);
        sf::sleep(sf::milliseconds(10));
    }
}