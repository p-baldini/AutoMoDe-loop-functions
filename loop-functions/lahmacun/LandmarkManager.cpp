/**
 * @file <loop-functions/lahmacun/LandmarkManager.cpp>
 * 
 * @author Paolo Baldini - <paolo.baldini.phd@gmail.com>
 * 
 * @license MIT License
 */
#include "LandmarkManager.hpp"

#include <argos3/plugins/robots/arena/simulator/arena_entity.h>

/****************************************/
/****************************************/

void LandmarkManager::Init(TConfigurationNode& t_tree) {
    CoreLoopFunctions::Init(t_tree);

    /*********************************************************************************************/
    /* PARAMETER PARSING                                                                         */
    /*********************************************************************************************/

    Real arenaRadius;

    // check that the node is of the desired type
    assert(t_tree.Value() == "loop_functions");
    assert(NodeExists(t_tree, "params"));
    t_tree = GetNode(t_tree, "params");

    // check that the node contains the desired information and retrieve it
    assert(NodeAttributeExists(t_tree, "arena_radius"));
    GetNodeAttribute(t_tree, "arena_radius", arenaRadius);

    /*********************************************************************************************/
    /* ARENA CREATION                                                                            */
    /*********************************************************************************************/

    CArenaEntity* pcArena;
    pcArena = new CArenaEntity(
        "arena",
        CVector3(0,0,0),
        CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO), // TODO
        CVector3(0.01,0.06,0.1), // assuming arena wall size = 60cm; in previous exp it was 2.5m
        "leds",
        6,
        6,
        0.125f,
        1.0f
    );
    AddEntity(*pcArena);

    /*********************************************************************************************/
    /* WALLS COLOR SET-UP                                                                        */
    /*********************************************************************************************/

    // set the wall colors according to the argos specifications
    for (UInt32 i = 0; i < 6; i++) {
        UInt8 colorCode;
        std::ostringstream param;
        param.str("");
        param << "c" << i;
        std::cout << param.str() << " " << i << "\n";
        std::flush(std::cout);
        assert(NodeAttributeExists(t_tree, param.str()));
        GetNodeAttribute(t_tree, param.str(), colorCode);
        pcArena->SetWallColor(i + 1, CColor::BLACK);
        if (colorCode == 1) {
            pcArena->SetWallColor(i + 1, CColor::GREEN);
        }
        if (colorCode == 3) {
            pcArena->SetWallColor(i + 1, CColor::RED);
        }
    }
}

/****************************************/
/****************************************/

void LandmarkManager::PostExperiment() {}

/****************************************/
/****************************************/

void LandmarkManager::PostStep() {
    CoreLoopFunctions::PostStep();
}

/****************************************/
/****************************************/

void LandmarkManager::Reset() {
    CoreLoopFunctions::Reset();
}

/****************************************/
/****************************************/

void LandmarkManager::Destroy() {
    std::ostringstream id;
    id << "arena";
    RemoveEntity(id.str().c_str());
}

/****************************************/
/****************************************/

CColor LandmarkManager::GetFloorColor(const CVector2& c_position_on_plane) {
    CVector2 left_center(0, 0.175);
    CVector2 right_center(0, -0.175);

    if (Distance(left_center, c_position_on_plane) < 0.1) {
        return CColor::BLACK;
    }

    if (Distance(right_center, c_position_on_plane) < 0.1) {
        return CColor::WHITE;
    }

    return CColor::GRAY50;
}

/****************************************/
/****************************************/

Real LandmarkManager::GetObjectiveFunction() {
    return 0;
}

/****************************************/
/****************************************/

CVector3 LandmarkManager::GetRandomPosition() {
    return CVector3::ZERO;
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(LandmarkManager, "lahmacun_landmark_manager_loop_function");
