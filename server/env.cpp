/************************************************************
 * Author    : Pablo Levy
 * Created   : 4/19/25$
 * Project   : PushYourLuck$
 * File      : env.cpp$
 * Description :
 *     $USER_COMMENT$
 ************************************************************/

#include "env.hpp"
#include <fstream>
#include <sstream>

std::unordered_map<std::string, std::string> loadEnv(const std::string& path) {
    std::unordered_map<std::string, std::string> envMap;
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(std::getline(iss, key, '='), value)) {
            envMap[key] = value;
        }
    }
    return envMap;
}
