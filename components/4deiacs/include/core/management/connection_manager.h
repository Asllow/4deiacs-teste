#pragma once

#include <string>
#include <vector>
#include "i_function_block.h"
#include "cJSON.h"

namespace deiacs {
class ConnectionManager {
public:
    static bool wireConnections(cJSON* conns_array, const std::vector<IFunctionBlock*>& blocks);

    static void clearAll();
};
}