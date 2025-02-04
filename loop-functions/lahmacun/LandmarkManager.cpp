/**
 * @file <loop-functions/lahmacun/LandmarkManager.cpp>
 * 
 * @author Paolo Baldini - <paolo.baldini.phd@gmail.com>
 * 
 * @license MIT License
 */
#include "LandmarkManager.hpp"

#include "SteadyRobot.hpp"

#include <argos3/plugins/robots/arena/simulator/arena_entity.h>
#include <argos3/plugins/simulator/entities/cylinder_entity.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

#define CONTROLLER_NAME "steady-controller"

/****************************************/
/****************************************/

void LandmarkManager::Init(TConfigurationNode& t_tree) {
    CoreLoopFunctions::Init(t_tree);

    /*********************************************************************************************/
    /* PARAMETER PARSING                                                                         */
    /*********************************************************************************************/

    // check that the node is of the desired type
    assert(t_tree.Value() == "loop_functions");
    assert(NodeExists(t_tree, "params"));
    t_tree = GetNode(t_tree, "params");

    /*********************************************************************************************/
    /* ARENA CREATION                                                                            */
    /*********************************************************************************************/

    CArenaEntity* pcArena;
    pcArena = new CArenaEntity(
        "arena",
        CVector3(0,0,0),
        CQuaternion().FromEulerAngles(CRadians::ZERO,CRadians::ZERO,CRadians::ZERO), // TODO
        CVector3(0.01,0.1,0.1), // assuming arena wall size = 60cm
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

    /*********************************************************************************************/
    /* CENTRAL BEACON SET-UP                                                                     */
    /*********************************************************************************************/

    // check if the beacon color is specified in the argos file
    for (auto i = 0; true; i++) {
        std::ostringstream color, xstr, ystr;
        color.str("");
        color << "b" << i;

        xstr.str("");
        xstr << "x" << i;
        ystr.str("");
        ystr << "y" << i;

        if (
            ! NodeAttributeExists(t_tree, color.str())
            ||
            ! NodeAttributeExists(t_tree, xstr.str())
            ||
            ! NodeAttributeExists(t_tree, ystr.str())
        ) {
            break;
        }

        UInt8 colorCode;
        Real x, y;
        GetNodeAttribute(t_tree, color.str(), colorCode);
        GetNodeAttribute(t_tree, xstr.str(), x);
        GetNodeAttribute(t_tree, ystr.str(), y);

        // create the beacon entity
        std::ostringstream entity_id;
        entity_id.str("");
        entity_id << "entity_" << i;
        CVector3 position(y, x, .0);
        auto entity = new CEPuckEntity(entity_id.str(), CONTROLLER_NAME, position);
        AddEntity(*entity);

        // create a block to prevent the beacon to be moved
        std::ostringstream entity_block_id;
        entity_block_id.str("");
        entity_block_id << "entity_block_" << i;
        auto entity_block = new CCylinderEntity(
            entity_block_id.str(),
            position, CQuaternion(),
            false, 0.05, 0.01, 1
        );
        AddEntity(*entity_block);
        SteadyRobot& c = dynamic_cast<SteadyRobot&>(entity->GetControllableEntity().GetController());
        switch (colorCode) {
            case 0:
                c.current_color = CColor::BLACK;
                break;
            case 1:
                c.current_color = CColor::GREEN;
                break;
            case 2:
                c.current_color = CColor::BLUE;
                break;
            case 3:
                c.current_color = CColor::RED;
                break;
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
    CVector2 left_center(0, 0.3);
    CVector2 right_center(0, -0.3);

    if (Distance(left_center, c_position_on_plane) < 0.15) {
        return CColor::BLACK;
    }

    if (Distance(right_center, c_position_on_plane) < 0.15) {
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
