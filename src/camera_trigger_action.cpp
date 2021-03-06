// Copyright (c) 2017, Smart Projects Holdings Ltd
// All rights reserved.
// See LICENSE file for license details.

#include <ugcs/vsm/camera_trigger_action.h>

using namespace ugcs::vsm;

Camera_trigger_action::State
Camera_trigger_action::Mavlink_to_state(double mav_param)
{
    using Type = mavlink::ugcs::MAV_CAMERA_TRIGGER_STATE;

    switch (static_cast<unsigned>(mav_param)) {
    case Type::CAMERA_TRIGGER_STATE_START_RECORDING: return State::ON;
    case Type::CAMERA_TRIGGER_STATE_STOP_RECORDING: return State::OFF;
    case Type::CAMERA_TRIGGER_STATE_SINGLE_SHOT: return State::SINGLE_PHOTO;
    }
    VSM_EXCEPTION(Action::Format_exception, "State value %d unknown",
            static_cast<unsigned>(mav_param));
}

