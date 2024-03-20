//
// Created by finn on 3/20/24.
//
#include <emscripten/bind.h>
#include "mas_sim/mas_sim.h"

EMSCRIPTEN_BINDINGS(mas_sim_web){
        class_<MASSim>("MASSim")
                .constructor<int>()
//                .function("incrementX", &MyClass::incrementX)
//                .property("x", &MyClass::getX, &MyClass::setX)
//                .property("x_readonly", &MyClass::getX)
//                .class_function("getStringFromInstance", &MyClass::getStringFromInstance)
        ;
}