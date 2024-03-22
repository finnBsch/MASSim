//
// Created by finn on 3/20/24.
//
#include <emscripten/bind.h>
#include "mas_sim/mas_sim.h"
using namespace emscripten;

using FloatArray2 = std::array<float, 2>;
using VectorOfFloatArrays = std::vector<FloatArray2>;
EMSCRIPTEN_BINDINGS(my_module){
        value_array<std::array<float, 2>>("array_float_2")
            .element(emscripten::index<0>())
            .element(emscripten::index<1>())
            ;
//        class_<Agent>("Agent")
//                .function("getX", &Agent::getX)
//                .function("getY", &Agent::getY);
//        value_object<Agent>("Agent")
//            .field("x", &Agent::getX)
//            .field("y", &Agent::getY);
        register_vector<std::array<float, 2>>("ArrayVec");
//        register_vector<Agent*>("AgentVec");
        class_<MASSim>("MASSim")
                .constructor<int>()
                .constructor<int, float, float>()
                .function("nAgents", &MASSim::getN)
                .function("step", &MASSim::step)
                .function("getAgents", &MASSim::getAgents)
//                .function("getAgentsDirect", &MASSim::getAgentsDirect)
                .function("reset", &MASSim::reset)
                .function("setSpeed", &MASSim::setSpeed)
                ;
//                .function("incrementX", &MyClass::incrementX)
//                .property("x", &MyClass::getX, &MyClass::setX)
//                .property("x_readonly", &MyClass::getX)
//                .class_function("getStringFromInstance", &MyClass::getStringFromInstance)
}