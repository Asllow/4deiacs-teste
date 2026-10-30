#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include "i_function_block.h"
#include "cJSON.h"

namespace deiacs{

using BlockFactoryFunc = std::function<IFunctionBlock*(const std::string& block_id, cJSON* config)>;

class BlockRegistry {
    public:
        static void registerBlock(const std::string& block_type, BlockFactoryFunc factory);

        static IFunctionBlock* createBlock(const std::string& block_type, const std::string& block_id, cJSON* config);

        static void clearAll();
    private:
        static std::unordered_map<std::string, BlockFactoryFunc>& getRegistry();

        static std::vector<IFunctionBlock*>& getInstances();
};
}