/**
 * @file <loop-functions/lahmacun/SteadyRobot.hpp>
 * 
 * @author Paolo Baldini - <paolo.baldini.phd@gmail.com>
 * 
 * @package ARGoS3-AutoMoDe
 * 
 * @license MIT License
 */
#ifndef LAHMACUN_STEADY_ROBOT_HPP
#define LAHMACUN_STEADY_ROBOT_HPP

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_rgb_leds_actuator.h>

using namespace argos;

/**
 * A controller for a robot which is part of the experiment environment. It is a robot that stays
 * still and assumes different states, represented by its leds color. A loop-function is expected
 * to set the robot color and decide wether it should answer incomin RAB messages. When the robot
 * answers a RAB message, it set a variable to notify the loop function. The answer contains the
 * robot ID, the message ID, and 1s.
 */
class SteadyRobot : public CCI_Controller {
    public:
        /**
         * Init the robot by getting its sensors and actuators and by setting its ID.
         * 
         * @param[in] t_node Points to the <parameters> section in the XML file in the
         * <controllers><lahmacun-steady-robot> section.
         */
        virtual void Init(TConfigurationNode& t_node);

        /**
         * Set the leds color and possibly answer incoming messages.
         */
        virtual void ControlStep();

        /**
         * Currently, this method does nothing.
         */
        virtual void Reset();

        /**
         * Currently, this method does nothing.
         */
        virtual void Destroy();

        CColor current_color;                           ///< The current color of the robot leds.
        bool answer_messages;                           ///< True if the robot should answer
                                                        ///< incoming messages, false otherwise. 
        bool answered;                                  ///< True if the robot answered a message
                                                        ///< at the previous step.

    private:
        UInt8 robot_id;                                 ///< The ID of the robot in argos.
        UInt32 message_id;                              ///< Incremental counter containing the ID
                                                        ///< of the message: it avoids perceiving
                                                        ///< messages from other sensors
                                                        ///< considering them different.

        CCI_EPuckRangeAndBearingActuator* rab_actuator; ///< Pointer to the range and bearing
                                                        ///< actuator.
        CCI_EPuckRangeAndBearingSensor* rab_sensor;     ///< Pointer to the range and bearing
                                                        ///< sensor.
        CCI_EPuckRGBLEDsActuator* led_actuator;         ///< Pointer to the leds actuator.
};

#endif /* LAHMACUN_STEADY_ROBOT_HPP */
