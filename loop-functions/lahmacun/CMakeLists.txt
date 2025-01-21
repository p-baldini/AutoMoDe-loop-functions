# Set up ARGoS compilation information
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIRS})
link_directories(${CMAKE_BINARY_DIR}/src)

#
# Lahmacun Color Reaction experiment loop functions
#

add_library(lahmacun_color_manager_loop_function
    SHARED
        ColorManager.hpp
        ColorManager.cpp
        SteadyRobot.hpp
        SteadyRobot.cpp
)
target_link_libraries(lahmacun_color_manager_loop_function
    argos3plugin_${ARGOS_BUILD_FOR}_epuck
)

#
# Lahmacun Color Landmark experiment loop functions
#

add_library(lahmacun_landmark_manager_loop_function
    SHARED
        LandmarkManager.hpp
        LandmarkManager.cpp
        SteadyRobot.hpp
        SteadyRobot.cpp
)
target_link_libraries(lahmacun_landmark_manager_loop_function
    argos3plugin_${ARGOS_BUILD_FOR}_epuck
)

#
# Lahmacun Color Reaction experiment controllers
#

add_library(lahmacun_steady_robot
    SHARED
        SteadyRobot.hpp
        SteadyRobot.cpp
)
target_link_libraries(lahmacun_steady_robot
    argos3plugin_${ARGOS_BUILD_FOR}_epuck
)
