/**
 * @file <loop-functions/lahmacun/ColorManager.hpp>
 * 
 * @author Paolo Baldini - <paolo.baldini.phd@gmail.com>
 * 
 * @package ARGoS3-AutoMoDe
 * 
 * @license MIT License
 */
#ifndef LAHMACUN_COLOR_MANAGER_LOOP_FUNCTION_HPP
#define LAHMACUN_COLOR_MANAGER_LOOP_FUNCTION_HPP

#include <argos3/core/simulator/space/space.h>
#include <argos3/core/utility/math/rng.h>

#include "../../src/CoreLoopFunctions.h"

using namespace argos;

/**
 * This loop function controls the `SteadyRobot` entities of the simulation by setting their color
 * and telling them whether they should answer or not a message. It guarantee that there will be an
 * exact number of entities with each type of color in the simulation.
 */
class ColorManager: public CoreLoopFunctions {
    public:
        /**
         * Initialization function called by the argos simulator. Called by passing the experiment
         * arguments. It decides which color will prize the robot and it set the entities in the
         * environment with a specific state.
         * 
         * @param[in] t_tree The parameters of the experiment.
         */
        virtual void Init(TConfigurationNode& t_tree);

        /**
         * Method called at the end of the experiment by the argos simulator. Currently, it does
         * nothing.
         */
        virtual void PostExperiment();

        /**
         * Check which entities must change color / state.
         */
        virtual void PostStep();

        /**
         * Method called at reset by the argos simulator. Currently, it does nothing.
         */
        virtual void Reset();

        /**
         * Method called by the argos simulator at the end of the process. Currently, it does
         * nothing.
         */
        virtual void Destroy();

        /**
         * Return the color of the floor in a specific point. Used to build the floor instance from
         * the loop functions when specified in the arena file.
         */
        CColor GetFloorColor(const CVector2& c_position_on_plane);

        /**
         * Method to obtain the performance computed by the loop-function. In this experiment we do
         * not compute the performance with the loop function and therefore it returns 0.
         * 
         * @return Always 0.
         */
        virtual Real GetObjectiveFunction();

        /**
         * Unused method.
         * 
         * @return A 0s vector.
         */
        virtual CVector3 GetRandomPosition();

    private:
        std::vector<CColor> m_cColors;          ///< The state map of each entity in the
                                                ///< experiment. The state is directly mapped to a
                                                ///< color.
        std::vector<UInt32> m_unTimes;          ///< The time passed since the last state update.
        UInt32 m_unRefractoryTime;              ///< Minimum time that has to pass before an entity
                                                ///< can change state.
        Real m_fTransitionProbability;          ///< The probability that an entity will change
                                                ///< state after the refractory time has passed.
        CColor m_cWColor;                       ///< Color indicating working robots.
        CColor m_cFColor;                       ///< Color indicating fixable robots.
        CColor m_cUColor;                       ///< Color indicating unfixable robots.

        CRandom::CRNG* m_pcRNG;                 ///< The random number generator of this
                                                ///< simulation.
        std::vector<CEPuckEntity*> m_cEntities; ///< Pointers to the entities in the simulation.

        /**
         * Checks wether the position of the entities in the simulation is acceptable: they are
         * enough distant from each other.
         * 
         * @param[in] positions The vector of entity positions.
         * @param[in] entitiesDistance The minimum distance between entities.
         * @return True if all the entities are `entitiesDistance` from each other.
         */
        bool ArePositionValid(std::vector<CVector2> positions, Real entitiesDistance);

        /**
         * Manages the state switch logic of the entities.
         */
        void ChangeState(UInt32 index);
};

#endif /* LAHMACUN_COLOR_MANAGER_LOOP_FUNCTION_HPP */
