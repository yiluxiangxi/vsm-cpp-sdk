// Copyright (c) 2014, Smart Projects Holdings Ltd
// All rights reserved.
// See LICENSE file for license details.

/**
 * @file ucs_to_vsm_transformations.h
 */
#ifndef _UCS_TO_VSM_TRANSFORMATIONS_H_
#define _UCS_TO_VSM_TRANSFORMATIONS_H_

#include <ugcs/vsm/action.h>
#include <ugcs/vsm/mavlink.h>
#include <ugcs/vsm/optional.h>

namespace ugcs {
namespace vsm {

/** Transformations from UCS data representation to user visible (i.e. VSM)
 * data structures.
 */
class Ucs_to_vsm_transformations {
public:

    /** Thrown when some value is unsupported and could not be transformed/understood. */
    VSM_DEFINE_EXCEPTION(Unsupported_exception);

    /** Parse extended Mavlink mission item and return appropriate action and optionally
     * take-off altitude if applicable to the mission item.
     * @throw Unsupported_exception if Mavlink mission item command is unsupported.
     * @throw ugcs::vsm::Action::Format_exception if internal action representation is
     *        wrong.
     */
    static Action::Ptr
    Parse_mission_item_ex(const mavlink::ugcs::Pld_mission_item_ex& item, Optional<double>& takeoff_altitude);
};

} /* namespace vsm */
} /* namespace ugcs */

#endif /* _UCS_TO_VSM_TRANSFORMATIONS_H_ */
