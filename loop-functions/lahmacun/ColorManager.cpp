/**
 * @file <loop-functions/lahmacun/ColorManager.cpp>
 * 
 * @author Paolo Baldini - <paolo.baldini.phd@gmail.com>
 * 
 * @license MIT License
 */
#include "ColorManager.hpp"

#include "SteadyRobot.hpp"

#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>
#include <argos3/plugins/simulator/entities/cylinder_entity.h>

#define CONTROLLER_NAME "steady-controller"

/****************************************/
/****************************************/

void ColorManager::Init(TConfigurationNode& t_tree) {
    CoreLoopFunctions::Init(t_tree);

    /*********************************************************************************************/
    /* PARAMETER PARSING                                                                         */
    /*********************************************************************************************/

    Real arenaRadius;
    Real entitiesDistance;
    UInt32 numberEntities;
    UInt32 maxTrials;

    // check that the node is of the desired type
    assert(t_tree.Value() == "loop_functions");
    assert(NodeExists(t_tree, "params"));
    t_tree = GetNode(t_tree, "params");

    // check that the node contains the desired information
    assert(NodeAttributeExists(t_tree, "entities_count"));
    assert(NodeAttributeExists(t_tree, "arena_radius"));
    assert(NodeAttributeExists(t_tree, "entities_distance"));
    assert(NodeAttributeExists(t_tree, "max_trials"));

    // get the parameters from the argos experiment file
    GetNodeAttribute(t_tree, "entities_count", numberEntities);
    GetNodeAttribute(t_tree, "arena_radius", arenaRadius);
    GetNodeAttribute(t_tree, "entities_distance", entitiesDistance);
    GetNodeAttribute(t_tree, "max_trials", maxTrials);
    GetNodeAttribute(t_tree, "refractory_time", m_unRefractoryTime);
    GetNodeAttribute(t_tree, "transition_probability", m_fTransitionProbability);

    /*********************************************************************************************/
    /* EXPERIMENT SET UP                                                                         */
    /*********************************************************************************************/

    // create a RNG (it is automatically disposed of by ARGoS)
    m_pcRNG = CRandom::CreateRNG("argos");

    // select the colors indicating that the robot is fixable, unfixable, or working
    auto sample = m_pcRNG->Bernoulli();
    m_cWColor = CColor::BLACK;
    m_cFColor = sample ? CColor::GREEN : CColor::RED;
    m_cUColor = sample ? CColor::RED : CColor::GREEN;

    /*********************************************************************************************/
    /* ROBOT DISTRIBUTION                                                                        */
    /*********************************************************************************************/

    std::vector<CVector2> positions;
    UInt32 trials = 0;

    do {
        positions.clear();

        for (UInt32 i = 0; i < numberEntities; i++) {
            CRadians angle = m_pcRNG->Uniform(CRange<CRadians>(CRadians::ZERO, CRadians::TWO_PI));
            Real magnitude = m_pcRNG->Uniform(CRange<Real>(0, arenaRadius));
            positions.push_back(CVector2(magnitude, angle));
        }
    } while (! ArePositionValid(positions, entitiesDistance) && trials++ < maxTrials);

    // if no valid position has been found, throw an exception
    if (trials >= maxTrials) {
        THROW_ARGOSEXCEPTION("Can't place new entity.");
    }

    for (UInt32 i = 0; i < numberEntities; i++) {
        // create a new epuck entity and add it to the arena
        std::ostringstream entity_id;
        entity_id.str("");
        entity_id << "entity_" << i;
        CVector3 position(positions[i].GetX(), positions[i].GetY(), .0);
        auto entity = new CEPuckEntity(entity_id.str(), CONTROLLER_NAME, position);
        AddEntity(*entity);
        m_cEntities.push_back(entity);
        m_unTimes.push_back(0);

        // create a block to prevent the epuck to be moved
        std::ostringstream entity_block_id;
        entity_block_id.str("");
        entity_block_id << "entity_block_" << i;
        auto entity_block = new CCylinderEntity(
            entity_block_id.str(),
            position, CQuaternion(),
            false, 0.05, 0.01, 1
        );
        AddEntity(*entity_block);        

        // select the robots state: 33% of each type
        switch (i * 3 / numberEntities) {
            case 0:
                m_cColors.push_back(m_cWColor);
                break;
            case 1:
                m_cColors.push_back(m_cFColor);
                break;
            case 2:
                m_cColors.push_back(m_cUColor);
                break;
        }
    }
}

/****************************************/
/****************************************/

void ColorManager::PostExperiment() {}

/****************************************/
/****************************************/

void ColorManager::PostStep() {
    std::flush(std::cout);
    // update the robots states maintaining the proportion states
    for (UInt32 i = 0; i < m_cEntities.size(); i++) {
        CEPuckEntity& e = *m_cEntities[i];
        SteadyRobot& c = dynamic_cast<SteadyRobot&>(e.GetControllableEntity().GetController());

        // if enough state time elapsed or the robot has answered a repair
        // message, change its state
        bool transit = c.answered ||
            (m_unTimes[i]++ > m_unRefractoryTime && m_pcRNG->Bernoulli(m_fTransitionProbability));
        if (transit) {
            ChangeState(i);
        }
    }

    // color / state set up in the robot: use wifi for the real robot experiments
    for (UInt32 i = 0; i < m_cEntities.size(); i++) {
        CEPuckEntity& e = *m_cEntities[i];
        SteadyRobot& c = dynamic_cast<SteadyRobot&>(e.GetControllableEntity().GetController());

        // change the robot state and need to answer a message
        c.current_color = m_cColors[i];
        c.answer_messages = m_cColors[i] == m_cFColor;
    }
}

/****************************************/
/****************************************/

void ColorManager::Reset() {
    CoreLoopFunctions::Reset();
}

/****************************************/
/****************************************/

void ColorManager::Destroy() {}

/****************************************/
/****************************************/

Real ColorManager::GetObjectiveFunction() {
    return 0;
}

/****************************************/
/****************************************/

CVector3 ColorManager::GetRandomPosition() {
    return CVector3::ZERO;
}

/****************************************/
/****************************************/

bool ColorManager::ArePositionValid(std::vector<CVector2> positions, Real entitiesDistance) {
    for (UInt32 i = 0; i < positions.size(); i++) {
        for (UInt32 j = 0; j < i; j++) {
            if (Distance(positions[i], positions[j]) < entitiesDistance) {
                return false;
            }
        }
    }
    return true;
}

/****************************************/
/****************************************/

void ColorManager::ChangeState(UInt32 index) {
    UInt32 j;
    do {
        j = m_pcRNG->Uniform(CRange<UInt32>(0, m_cColors.size()));
    } while(
        (m_cColors[index] == m_cWColor && m_cColors[j] == m_cWColor)
        ||
        (m_cColors[index] != m_cWColor && m_cColors[j] != m_cWColor)
    );
    CColor tmp = m_cColors[index];
    m_cColors[index] = m_cColors[j];
    m_cColors[j] = tmp;
    m_unTimes[index] = m_unTimes[j] = 0;
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(ColorManager, "lahmacun_color_manager_loop_function");
