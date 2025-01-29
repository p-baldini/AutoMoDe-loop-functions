/**
 * @file <loop-functions/lahmacun/SteadyRobot.cpp>
 * 
 * @author Paolo Baldini - <paolo.baldini.phd@gmail.com>
 * 
 * @package ARGoS3-AutoMoDe
 * 
 * @license MIT License
 */
#include "SteadyRobot.hpp"

#include <algorithm>

/***********************************************/
/***********************************************/

void SteadyRobot::Init(TConfigurationNode& t_node) {
    // set the wheel speed to zero to keep the robot still
    auto wheels = GetActuator<CCI_EPuckWheelsActuator>("epuck_wheels");
    wheels->SetLinearVelocity(0, 0);

    // save the pointers to the range and bearing sensor and actuator; save the pointer to the leds
    // actuator
    rab_actuator = GetActuator<CCI_EPuckRangeAndBearingActuator>("epuck_range_and_bearing");
    rab_sensor = GetSensor<CCI_EPuckRangeAndBearingSensor>("epuck_range_and_bearing");
    led_actuator = GetActuator<CCI_EPuckRGBLEDsActuator>("epuck_rgb_leds");

    // get the robot id from its name (max 6 characters)
    robot_id = atoi(GetId().substr(7, 6).c_str());

    // the initial message is the index of the robot + 1
    message = (robot_id << 24) + 1; 
}

/***********************************************/
/***********************************************/

void SteadyRobot::ControlStep() {
    // get the rab messages received
    auto messages = rab_sensor->GetPackets();

    // check if at least a message has the repair content
    auto checker = [this](auto o) { return o->Data[0] != 0 && o->Range < 10; };
    bool received_repair = std::any_of(messages.begin(), messages.end(), checker);
    rab_sensor->ClearPackets();

    // if the robot should answer repair messages, then answer
    if (received_repair && answer_messages) {
        rab_actuator->SetData((UInt8*)&message);
        answered = true;
        message++;
    } else {
        answered = false;
    }

    // set the led color as dictated by the loop function
    led_actuator->SetColors(current_color);
}

/***********************************************/
/***********************************************/

void SteadyRobot::Reset() {
    
}

/***********************************************/
/***********************************************/

void SteadyRobot::Destroy() {
    
}

/***********************************************/
/***********************************************/

REGISTER_CONTROLLER(SteadyRobot, "lahmacun_steady_controller");
