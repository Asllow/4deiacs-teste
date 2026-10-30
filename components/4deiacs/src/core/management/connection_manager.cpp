#include "connection_manager.h"
#include "esp_log.h"
#include <sstream>

namespace deiacs{

static const char* TAG = "CONNECTION_MANAGER";

static IFunctionBlock* findBlock(const std::vector<IFunctionBlock*>& blocks, const std::string& id) {
    for (auto* b : blocks) {
        if (b->getId() == id) {
            return b;
        }
    }
    return nullptr;
}

bool ConnectionManager::wireConnections(cJSON* conns_array, const std::vector<IFunctionBlock*>& blocks) {
    if (!cJSON_IsArray(conns_array)) {
        ESP_LOGW(TAG, "O manifesto nao contem um array 'connections' valido. Roteamento abortado.");
        return false;
    }

    ESP_LOGI(TAG, "Iniciando Roteamento (Wiring) IEC 61499...");
    cJSON* conn = nullptr;

    cJSON_ArrayForEach(conn, conns_array) {
        cJSON* src_node = cJSON_GetObjectItem(conn, "source");
        cJSON* tgt_node = cJSON_GetObjectItem(conn, "target");

        if (!cJSON_IsString(src_node) || !cJSON_IsString(tgt_node)) {
            continue;
        }

        std::string source_str = src_node->valuestring;
        std::string target_str = tgt_node->valuestring;

        size_t src_dot = source_str.find('.');
        size_t tgt_dot = target_str.find('.');

        if (src_dot == std::string::npos || tgt_dot == std::string::npos) {
            ESP_LOGE(TAG, "Formato de conexao invalido: %s -> %s", source_str.c_str(), target_str.c_str());
            continue;
        }

        std::string src_block_id = source_str.substr(0, src_dot);
        std::string src_port     = source_str.substr(src_dot + 1);

        std::string tgt_block_id = target_str.substr(0, tgt_dot);
        std::string tgt_port     = target_str.substr(tgt_dot + 1);

        IFunctionBlock* src_block = findBlock(blocks, src_block_id);
        IFunctionBlock* tgt_block = findBlock(blocks, tgt_block_id);

        if (src_block == nullptr || tgt_block == nullptr) {
            ESP_LOGE(TAG, "Bloco nao encontrado na malha para a conexao: %s -> %s", src_block_id.c_str(), tgt_block_id.c_str());
            continue;
        }

        void* data_ptr = src_block->getDataOutput(src_port);
        
        if (data_ptr != nullptr) {
            if (tgt_block->connectDataInput(tgt_port, data_ptr)) {
                ESP_LOGI(TAG, "Fio de DADOS ligado: [%s].%s ---> [%s].%s", 
                         src_block_id.c_str(), src_port.c_str(), tgt_block_id.c_str(), tgt_port.c_str());
            } else {
                ESP_LOGE(TAG, "A porta de entrada de dados %s.%s rejeitou a conexao.", tgt_block_id.c_str(), tgt_port.c_str());
            }
        } else {
            src_block->addEventOutputConnection(src_port, tgt_block, tgt_port);
            ESP_LOGI(TAG, "Fio de EVENTO ligado: [%s].%s ---> [%s].%s", 
                     src_block_id.c_str(), src_port.c_str(), tgt_block_id.c_str(), tgt_port.c_str());
        }
    }
    return true;
}

void ConnectionManager::clearAll() {
    ESP_LOGI(TAG, "Roteamento desfeito (Fios destruidos nativamente em cascata pelos blocos).");
}

}
