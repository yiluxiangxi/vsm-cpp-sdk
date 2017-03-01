// Copyright (c) 2014-2016, Smart Projects Holdings Ltd
// All rights reserved.
// See LICENSE file for license details.

#include <ugcs/vsm/property.h>
#include <cmath>

using namespace ugcs::vsm;

namespace {

ugcs::vsm::proto::Field_semantic
Get_default_semantic(std::string name)
{
    if (name == "latitude") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_LATITUDE;
    } else if (name == "longitude") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_LONGITUDE;
    } else if (name == "altitude_amsl") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_ALTITUDE_AMSL;
    } else if (name == "altitude_origin") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_ALTITUDE_AMSL;
    } else if (name == "altitude_raw") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_ALTITUDE_RAW;
    } else if (name == "ground_elevation") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_GROUND_ELEVATION;
    } else if (name == "acceptance_radius") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_ACCEPTANCE_RADIUS;
    } else if (name == "heading") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_HEADING;
    } else if (name == "ms") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_MILLISECONDS;
    } else if (name == "course") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_HEADING;
    } else if (name == "pitch") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_PITCH;
    } else if (name == "yaw") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_HEADING;
    } else if (name == "roll") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_ROLL;
    } else if (name == "ground_speed") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_GROUND_SPEED;
    } else if (name == "air_speed") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_AIR_SPEED;
    } else if (name == "fov_h") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_FOV_H;
    } else if (name == "fov_v") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_FOV_V;
    } else if (name == "main_voltage") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_VOLTAGE;
    } else if (name == "main_current") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_CURRENT;
    } else if (name == "satellite_count") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_SATELLITE_COUNT;
    } else if (name == "gps_fix") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_GPS_FIX_TYPE;
    } else if (name == "gcs_link_quality") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_GCS_LINK_QUALITY;
    } else if (name == "rc_link_quality") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_RC_LINK_QUALITY;
    } else if (name == "control_mode") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_CONTROL_MODE;
    } else if (name == "vertical_speed") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_VERTICAL_SPEED;
    } else if (name == "climb_rate") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_VERTICAL_SPEED;
    } else if (name == "descent_rate") {
        return ugcs::vsm::proto::FIELD_SEMANTIC_VERTICAL_SPEED;
    } else {
        return ugcs::vsm::proto::FIELD_SEMANTIC_DEFAULT;
    }
}

ugcs::vsm::Property::Value_type
Get_type_from_semantic(ugcs::vsm::proto::Field_semantic sem)
{
    switch (sem){
    case ugcs::vsm::proto::FIELD_SEMANTIC_LATITUDE:
    case ugcs::vsm::proto::FIELD_SEMANTIC_LONGITUDE:
        return ugcs::vsm::Property::VALUE_TYPE_DOUBLE;
    case ugcs::vsm::proto::FIELD_SEMANTIC_BOOL:
        return ugcs::vsm::Property::VALUE_TYPE_BOOL;
    case ugcs::vsm::proto::FIELD_SEMANTIC_ENUM:
    case ugcs::vsm::proto::FIELD_SEMANTIC_ADSB_MODE:
    case ugcs::vsm::proto::FIELD_SEMANTIC_CONTROL_MODE:
    case ugcs::vsm::proto::FIELD_SEMANTIC_GPS_FIX_TYPE:
        return ugcs::vsm::Property::VALUE_TYPE_ENUM;
    case ugcs::vsm::proto::FIELD_SEMANTIC_NUMERIC:
    case ugcs::vsm::proto::FIELD_SEMANTIC_ACCEPTANCE_RADIUS:
    case ugcs::vsm::proto::FIELD_SEMANTIC_ALTITUDE_AMSL:
    case ugcs::vsm::proto::FIELD_SEMANTIC_ALTITUDE_AGL:
    case ugcs::vsm::proto::FIELD_SEMANTIC_ALTITUDE_RAW:
    case ugcs::vsm::proto::FIELD_SEMANTIC_HEADING:
    case ugcs::vsm::proto::FIELD_SEMANTIC_VOLTAGE:
    case ugcs::vsm::proto::FIELD_SEMANTIC_AIR_SPEED:
    case ugcs::vsm::proto::FIELD_SEMANTIC_GROUND_SPEED:
    case ugcs::vsm::proto::FIELD_SEMANTIC_VERTICAL_SPEED:
    case ugcs::vsm::proto::FIELD_SEMANTIC_ROLL:
    case ugcs::vsm::proto::FIELD_SEMANTIC_PITCH:
    case ugcs::vsm::proto::FIELD_SEMANTIC_YAW:
    case ugcs::vsm::proto::FIELD_SEMANTIC_RC_LINK_QUALITY:
    case ugcs::vsm::proto::FIELD_SEMANTIC_GCS_LINK_QUALITY:
    case ugcs::vsm::proto::FIELD_SEMANTIC_CURRENT:
    case ugcs::vsm::proto::FIELD_SEMANTIC_FOV_H:
    case ugcs::vsm::proto::FIELD_SEMANTIC_FOV_V:
    case ugcs::vsm::proto::FIELD_SEMANTIC_GROUND_ELEVATION:
    case ugcs::vsm::proto::FIELD_SEMANTIC_LOITER_RADIUS:
        return ugcs::vsm::Property::VALUE_TYPE_FLOAT;
    case ugcs::vsm::proto::FIELD_SEMANTIC_SATELLITE_COUNT:
    case ugcs::vsm::proto::FIELD_SEMANTIC_ICAO:
    case ugcs::vsm::proto::FIELD_SEMANTIC_SQUAWK:
    case ugcs::vsm::proto::FIELD_SEMANTIC_MILLISECONDS:
        return ugcs::vsm::Property::VALUE_TYPE_INT;
    case ugcs::vsm::proto::FIELD_SEMANTIC_STRING:
        return ugcs::vsm::Property::VALUE_TYPE_STRING;
    default:
        VSM_EXCEPTION(Invalid_param_exception, "No internal type for semantic %d", sem);
    }
}
}   // anon namespace

#define ADD_BUILT_IN_ENUM(x) Add_enum(#x, ugcs::vsm::proto::x)

Property::Property(
    int id,
    const std::string& name,
    ugcs::vsm::proto::Field_semantic sem):
    semantic(sem), field_id(id), name(name)
{
    if (sem == ugcs::vsm::proto::FIELD_SEMANTIC_DEFAULT) {
        semantic = Get_default_semantic(name);
    }
    if (semantic == ugcs::vsm::proto::FIELD_SEMANTIC_DEFAULT) {
        VSM_EXCEPTION(Invalid_param_exception, "No semantic specified for field %s", name.c_str());
    }

    type = Get_type_from_semantic(semantic);

    // Add built-in enum values.
    switch (semantic) {
    case ugcs::vsm::proto::FIELD_SEMANTIC_ADSB_MODE:
        break;
    case ugcs::vsm::proto::FIELD_SEMANTIC_CONTROL_MODE:
        ADD_BUILT_IN_ENUM(CONTROL_MODE_MANUAL);
        ADD_BUILT_IN_ENUM(CONTROL_MODE_AUTO);
        ADD_BUILT_IN_ENUM(CONTROL_MODE_CLICK_GO);
        ADD_BUILT_IN_ENUM(CONTROL_MODE_JOYSTICK);
        break;
    case ugcs::vsm::proto::FIELD_SEMANTIC_GPS_FIX_TYPE:
        break;
    default:
        break;
    }
}

Property::Property(int id, const std::string& name, Value_type type):
    type(type), field_id(id), name(name)
{
    switch (type) {
    case VALUE_TYPE_DOUBLE:
    case VALUE_TYPE_INT:
    case VALUE_TYPE_FLOAT:
        semantic = ugcs::vsm::proto::FIELD_SEMANTIC_NUMERIC;
        break;
    case VALUE_TYPE_BOOL:
        semantic = ugcs::vsm::proto::FIELD_SEMANTIC_BOOL;
        break;
    case VALUE_TYPE_STRING:
        semantic = ugcs::vsm::proto::FIELD_SEMANTIC_STRING;
        break;
    case VALUE_TYPE_ENUM:
        semantic = ugcs::vsm::proto::FIELD_SEMANTIC_ENUM;
        break;
    default:
        VSM_EXCEPTION(Invalid_param_exception, "Invalid enum id %s", name.c_str());
        break;
    }
}

Property::Property(Property::Ptr src)
{
    *this = *src;
}

bool
Property::Set_value(const ugcs::vsm::proto::Field_value& v)
{
    is_changed = true;
    if (v.has_meta_value()) {
        switch (v.meta_value()) {
        case proto::META_VALUE_NA:
            value_spec = VALUE_SPEC_NA;
            return true;
        }
    }

    int ival = 0;
    double dval = 0;

    if (v.has_double_value()) {
        ival = v.double_value();
        dval = v.double_value();
    } else if (v.has_float_value()) {
        ival = v.float_value();
        dval = v.float_value();
    } else if (v.has_int_value()) {
        ival = v.int_value();
        dval = v.int_value();
    }

    value_spec = VALUE_SPEC_REGULAR;
    switch (type) {
    case VALUE_TYPE_FLOAT:
    case VALUE_TYPE_DOUBLE:
        if (max_value && dval > max_value->double_value) {
            VSM_EXCEPTION(Invalid_param_exception, "Value %f exceeds specified max:%f", dval, max_value->double_value);
        }
        if (min_value && dval < min_value->double_value) {
            VSM_EXCEPTION(Invalid_param_exception, "Value %f is lower than specified min:%f", dval, min_value->double_value);
        }
        double_value = dval;
        return true;
    case VALUE_TYPE_ENUM:
        if (v.has_int_value()) {
            if (enum_values.find(ival) != enum_values.end()) {
                int_value = ival;
                return true;
            } else {
                VSM_EXCEPTION(Invalid_param_exception, "Value %d is not part of enum %s", ival, name.c_str());
            }
        } else {
            VSM_EXCEPTION(Invalid_param_exception, "No int value found for enum");
        }
        break;
    case VALUE_TYPE_INT:
        if (max_value && ival > max_value->int_value) {
            VSM_EXCEPTION(Invalid_param_exception, "Value %d exceeds specified max:%d", ival, max_value->int_value);
        }
        if (min_value && ival < min_value->int_value) {
            VSM_EXCEPTION(Invalid_param_exception, "Value %d lower than specified min:%d", ival, max_value->int_value);
        }
        int_value = ival;
        return true;
    case VALUE_TYPE_STRING:
        if (v.has_string_value()) {
            string_value = v.string_value();
            return true;
        }
        break;
    case VALUE_TYPE_BOOL:
        if (v.has_bool_value()) {
            bool_value = v.bool_value();
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

void
Property::Set_timeout(int t)
{
    timeout = std::chrono::seconds(t);
}

void
Property::Add_enum(const std::string& name, int value)
{
    auto it = enum_values.emplace(value, name);
    if (it.second) {
        if (type != VALUE_TYPE_ENUM) {
            VSM_EXCEPTION(Invalid_param_exception, "Property %s type (%d) not enum", name.c_str(), type);
        }
    } else {
        VSM_EXCEPTION(Invalid_param_exception, "Invalid enum id %s", name.c_str());
    }
}

void
Property::Set_value(double v)
{
    switch (type) {
    case VALUE_TYPE_DOUBLE:
    case VALUE_TYPE_FLOAT:
        // since NAN != NAN we need additional checks for isnan to avoid updating NAN with NAN.
        if ((double_value != v && (!std::isnan(v) || !std::isnan(double_value))) || value_spec != VALUE_SPEC_REGULAR) {
            double_value = v;
            is_changed = true;
        }
        break;
    case VALUE_TYPE_INT:
        if (int_value != v || value_spec != VALUE_SPEC_REGULAR) {
            int_value = v;
            is_changed = true;
        }
        break;
    default:
        VSM_EXCEPTION(Invalid_param_exception, "Property %s type (%d) not double", name.c_str(), type);
    }
    value_spec = VALUE_SPEC_REGULAR;
    update_time = std::chrono::system_clock::now();
}

void
Property::Set_value(int v)
{
    switch (type) {
    case VALUE_TYPE_DOUBLE:
    case VALUE_TYPE_FLOAT:
        if (double_value != v || value_spec != VALUE_SPEC_REGULAR) {
            double_value = v;
            is_changed = true;
        }
        break;
    case VALUE_TYPE_INT:
    case VALUE_TYPE_ENUM:
        if (int_value != v || value_spec != VALUE_SPEC_REGULAR) {
            int_value = v;
            is_changed = true;
        }
        break;
    case VALUE_TYPE_BOOL:
        if (bool_value != (v != 0) || value_spec != VALUE_SPEC_REGULAR) {
            bool_value = v;
            is_changed = true;
        }
        break;
    default:
        VSM_EXCEPTION(Invalid_param_exception, "Property %s type (%d) not int", name.c_str(), type);
    }
    value_spec = VALUE_SPEC_REGULAR;
    update_time = std::chrono::system_clock::now();
}

void
Property::Set_value(unsigned int v)
{
    switch (type) {
    case VALUE_TYPE_DOUBLE:
    case VALUE_TYPE_FLOAT:
        if (double_value != v || value_spec != VALUE_SPEC_REGULAR) {
            double_value = v;
            is_changed = true;
        }
        break;
    case VALUE_TYPE_INT:
    case VALUE_TYPE_ENUM:
        if (int_value != static_cast<int>(v) || value_spec != VALUE_SPEC_REGULAR) {
            int_value = static_cast<int>(v);
            is_changed = true;
        }
        break;
    default:
        VSM_EXCEPTION(Invalid_param_exception, "Property %s type (%d) not int", name.c_str(), type);
    }
    value_spec = VALUE_SPEC_REGULAR;
    update_time = std::chrono::system_clock::now();
}

void
Property::Set_value(bool v)
{
    switch (type) {
    case VALUE_TYPE_BOOL:
        if (bool_value != v || value_spec != VALUE_SPEC_REGULAR) {
            bool_value = v;
            is_changed = true;
        }
        break;
    default:
        VSM_EXCEPTION(Invalid_param_exception, "Property %s type (%d) not bool", name.c_str(), type);
    }
    value_spec = VALUE_SPEC_REGULAR;
    update_time = std::chrono::system_clock::now();
}

void
Property::Set_value(const char* v)
{
    if (type == VALUE_TYPE_STRING) {
        if (string_value != std::string(v) || value_spec != VALUE_SPEC_REGULAR) {
            string_value = std::string(v);
            is_changed = true;
            value_spec = VALUE_SPEC_REGULAR;
        }
    } else {
        VSM_EXCEPTION(Invalid_param_exception, "Property %s type (%d) not string", name.c_str(), type);
    }
    update_time = std::chrono::system_clock::now();
}

void
Property::Set_value(const std::string& v)
{
    if (type == VALUE_TYPE_STRING) {
        if (string_value != v || value_spec != VALUE_SPEC_REGULAR) {
            string_value = v;
            is_changed = true;
            value_spec = VALUE_SPEC_REGULAR;
        }
    } else {
        VSM_EXCEPTION(Invalid_param_exception, "Property %s type (%d) not string", name.c_str(), type);
    }
    update_time = std::chrono::system_clock::now();
}

void
Property::Set_value_na()
{
    if (value_spec != VALUE_SPEC_NA) {
        is_changed = true;
        value_spec = VALUE_SPEC_NA;
    }
    update_time = std::chrono::system_clock::now();
}

bool
Property::Get_value(bool &v)
{
    if (    value_spec == VALUE_SPEC_REGULAR
        &&  type == VALUE_TYPE_BOOL)
    {
        v = bool_value;
        return true;
    } else {
        return false;
    }
}

bool
Property::Get_value(float &v)
{
    if (value_spec == VALUE_SPEC_REGULAR) {
        switch (type) {
        case VALUE_TYPE_DOUBLE:
        case VALUE_TYPE_FLOAT:
            v = double_value;
            break;
        case VALUE_TYPE_INT:
        case VALUE_TYPE_ENUM:
            v = int_value;
            break;
        default:
            return false;
        }
        return true;
    } else {
        return false;
    }
}

bool
Property::Get_value(double &v)
{
    if (value_spec == VALUE_SPEC_REGULAR) {
        switch (type) {
        case VALUE_TYPE_DOUBLE:
        case VALUE_TYPE_FLOAT:
            v = double_value;
            break;
        case VALUE_TYPE_INT:
        case VALUE_TYPE_ENUM:
            v = int_value;
            break;
        default:
            return false;
        }
        return true;
    } else {
        return false;
    }
}

bool
Property::Get_value(std::string& v)
{
    if (    value_spec == VALUE_SPEC_REGULAR
        &&  type == VALUE_TYPE_STRING)
    {
        v = string_value;
        return true;
    } else {
        return false;
    }
}

bool
Property::Get_value(int &v)
{
    if (value_spec == VALUE_SPEC_REGULAR) {
        switch (type) {
        case VALUE_TYPE_DOUBLE:
        case VALUE_TYPE_FLOAT:
            v = double_value;
            break;
        case VALUE_TYPE_INT:
        case VALUE_TYPE_ENUM:
            v = int_value;
            break;
        default:
            return false;
        }
        return true;
    } else {
        return false;
    }
}

bool
Property::Is_value_na()
{
    return value_spec == VALUE_SPEC_NA;
}

void
Property::Set_changed()
{
    is_changed = true;
}

bool
Property::Is_changed()
{
    if (is_changed) {
        return true;
    }
    if (!Is_value_na() && timeout.count()) {
        // timeout specified and value is still present.
        if ((std::chrono::system_clock::now() - update_time) > timeout) {
            // value expired, set to na.
            LOG("Setting %d to na", field_id);
            Set_value_na();
            return true;
        }
    }
    return false;
};

Property::Ptr
Property::Default_value()
{
    if (default_value == nullptr) {
        default_value = Create(0, "", semantic);
    }
    return default_value;
}

Property::Ptr
Property::Min_value()
{
    if (min_value == nullptr) {
        min_value = Create(0, "", semantic);
    }
    return min_value;
}

Property::Ptr
Property::Max_value()
{
    if (max_value == nullptr) {
        max_value = Create(0, "", semantic);
    }
    return max_value;
}

void
Property::Register(ugcs::vsm::proto::Register_field* field)
{
    field->Clear();
    field->set_name(name);
    field->set_field_id(field_id);
    field->set_semantic(semantic);
    for (auto& it : enum_values) {
        auto e = field->add_enumeration();
        e->set_id(it.first);
        e->set_description(it.second);
    }
    if (default_value) {
        default_value->Write_value(field->mutable_default_value());
    }
    if (min_value) {
        min_value->Write_value(field->mutable_min_value());
    }
    if (max_value) {
        max_value->Write_value(field->mutable_max_value());
    }
}

void
Property::Write_as_property(ugcs::vsm::proto::Property_field* field)
{
    field->Clear();
    field->set_name(name);
    Write_value(field->mutable_value());
}

void
Property::Write_as_parameter(ugcs::vsm::proto::Parameter_field* tf)
{
    tf->Clear();
    tf->set_field_id(field_id);
    Write_value(tf->mutable_value());
}

void
Property::Write_as_telemetry(ugcs::vsm::proto::Telemetry_field* tf)
{
    tf->Clear();
    tf->set_field_id(field_id);
    Write_value(tf->mutable_value());
    is_changed = false;
}

void
Property::Write_value(ugcs::vsm::proto::Field_value* field)
{
    switch (value_spec) {
    case VALUE_SPEC_REGULAR:
        switch (type) {
        case VALUE_TYPE_DOUBLE:
            field->set_double_value(double_value);
            break;
        case VALUE_TYPE_INT:
        case VALUE_TYPE_ENUM:
            field->set_int_value(int_value);
            break;
        case VALUE_TYPE_FLOAT:
            field->set_float_value(double_value);
            break;
        case VALUE_TYPE_BOOL:
            field->set_bool_value(bool_value);
            break;
        case VALUE_TYPE_STRING:
            field->set_string_value(string_value);
            break;
        default:
            VSM_EXCEPTION(Invalid_param_exception, "Property %s type not set", name.c_str());
            break;
        }
        break;
    case VALUE_SPEC_NA:
    default:
        field->set_meta_value(proto::META_VALUE_NA);
        break;
    }
}

std::string
Property::Dump_value()
{
    switch (value_spec) {
    case VALUE_SPEC_NA:
        return name + "(" + std::to_string(field_id) + ")=N/A";
    case VALUE_SPEC_REGULAR:
        switch (type) {
        case VALUE_TYPE_DOUBLE:
        case VALUE_TYPE_FLOAT:
            return name + "(" + std::to_string(field_id) + ")=" + std::to_string(double_value);
        case VALUE_TYPE_ENUM:
        case VALUE_TYPE_INT:
            return name + "(" + std::to_string(field_id) + ")=" + std::to_string(int_value);
        case VALUE_TYPE_BOOL:
            return name + "(" + std::to_string(field_id) + ")=" + std::to_string(bool_value);
        case VALUE_TYPE_STRING:
            return name + "(" + std::to_string(field_id) + ")='" + string_value + "'";
        case VALUE_TYPE_NONE:
            return name + "(" + std::to_string(field_id) + ")=<none>";
        }
    }
    return name + "=<invalid>";
}