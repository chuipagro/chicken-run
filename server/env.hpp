/************************************************************
 * Author    : Pablo Levy
 * Created   : 4/19/25$
 * Project   : chicken-run$
 * File      : env.hpp$
 * Description : 
 *     $USER_COMMENT$
 ************************************************************/

#pragma once
#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::string> loadEnv(const std::string& path = ".env");
