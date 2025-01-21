/**
 * @file <loop-functions/lahmacun/LandmarkManager.hpp>
 * 
 * @author Paolo Baldini - <paolo.baldini.phd@gmail.com>
 * 
 * @package ARGoS3-AutoMoDe
 * 
 * @license MIT License
 */
#ifndef LAHMACUN_LANDMARK_MANAGER_LOOP_FUNCTION_HPP
#define LAHMACUN_LANDMARK_MANAGER_LOOP_FUNCTION_HPP

#include "../../src/CoreLoopFunctions.h"

using namespace argos;

/**
 * This loop function controls the `landmark` entities of the simulation by setting their color and
 * position.
 */
class LandmarkManager: public CoreLoopFunctions {
    public:
        /**
         * Initialization function called by the argos simulator. Called by passing the experiment
         * arguments. It positions the landmarks and decides their color.
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
         * Method called at the end of the step by the argos simulator. Currently, it does nothing.
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
        virtual CVector3 GetRandomPosition();;
};

#endif /* LAHMACUN_LANDMARK_MANAGER_LOOP_FUNCTION_HPP */
