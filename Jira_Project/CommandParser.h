#pragma once
#include <string>
#include <vector>
#include "Enums.h"

struct Command {
    CommandType type;
    std::vector<std::string> args;
};

class CommandParser {
public:
    static Command parse(const std::string& input);
private:
    static CommandType stringToCommandType(const std::string& cmdStr);
};
