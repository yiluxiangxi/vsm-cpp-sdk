// Copyright (c) 2017, Smart Projects Holdings Ltd
// All rights reserved.
// See LICENSE file for license details.

/**
 * Windows-specific implementation of Utils.
 */

#include <ugcs/vsm/utils.h>

regex::regex_constants::syntax_option_type
ugcs::vsm::platform_independent_filename_regex_matching_flag = regex::regex_constants::icase;
