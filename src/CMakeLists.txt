# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

include(CMakePrintHelpers)

# set(RES_LOC ${CMAKE_BINARY_DIR}/../resources_build)

# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../include/res_loc.h.in ${CMAKE_CURRENT_SOURCE_DIR}/../include/res_loc.h @ONLY)
# Optionally glob, but only for CMake 3.12 or later:
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h")
cmake_print_variables(HEADER_LIST)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -g")

find_package(Eigen3 3.3 NO_MODULE)
#find_package(SFML COMPONENTS graphics REQUIRED)



#include(FetchContent)
#FetchContent_Declare(
#        tomlplusplus
#        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
#        GIT_TAG        v3.4.0
#)
#FetchContent_MakeAvailable(tomlplusplus)


# Make an automatic library - will be static or dynamic based on user setting
if (DEFINED EMSCRIPTEN)

    add_library(mas_sim
            ${HEADER_LIST}
            mas_sim/mas_sim.cpp
            #        viz.cpp
            mas_sim/agent.cpp
            bindings.cpp)
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --bind")
#    set_target_properties(mas_sim PROPERTIES LINK_FLAGS "--bind")

    target_include_directories(mas_sim PRIVATE ../include  ../ext_include/)
    set_target_properties(mas_sim PROPERTIES LINK_FLAGS "-s ENVIRONMENT=web -s SINGLE_FILE=1 -s MODULARIZE -s 'EXPORT_NAME=DummyCppLibrary' --bind")


else()
    find_package(SFML COMPONENTS graphics REQUIRED)
    add_library(mas_sim_lib  ${HEADER_LIST}
                mas_sim/mas_sim.cpp
    #        viz.cpp
            mas_sim/agent.cpp
            mas_sim/grid.cpp
    )
    target_include_directories(mas_sim_lib PUBLIC ../include PRIVATE ${tomlplusplus_SOURCE_DIR}/include)
    target_link_libraries (mas_sim_lib PUBLIC Eigen3::Eigen sfml-graphics)
    target_compile_features(mas_sim_lib PUBLIC cxx_std_20)
endif()

#add_custom_target(copy_assets
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_CURRENT_SOURCE_DIR}/../resources/ ${RES_LOC})
#
#add_dependencies(particle_filter_lib copy_assets)

# We need this directory, and users of our library will need it too



# This depends on (header only) boost
#target_link_libraries(gmrf_lib PRIVATE Boost::boost)

# All users of this library will need at least C++11

# IDEs should put the headers in a nice place
source_group(
        TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})
