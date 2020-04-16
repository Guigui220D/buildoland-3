#include "StructureFromJson.h"

#include <fstream>
#include "../../../external/json/Json.hpp"

#include "../../../common-source/Utils/Log.h"

#include <exception>

#include "../../../common-source/Blocks/Block.h"
#include "../../../common-source/Blocks/GameBlocks.h"

#include "../../../common-source/Grounds/Ground.h"
#include "../../../common-source/Grounds/GameGrounds.h"

#include "../Chunk.h"

StructureFromJson::StructureFromJson() :
    size_x(0),
    size_y(0),
    origin_x(0),
    origin_y(0)
{

}

StructureFromJson::~StructureFromJson()
{
    //dtor
}

const Block* StructureFromJson::getBlock(sf::Vector2i block_in_chunk) const
{
    sf::Vector2i diff = getRealBlockPos(chunk, block_in_chunk) - pos;
    diff += sf::Vector2i(origin_x, origin_y);

    if (diff.x < 0 || diff.y < 0 || diff.x >= size_x || diff.y >= size_y)
        return GameBlocks::STRUCTURE_VOID;

    return blocks.at(diff.x + diff.y * size_x);
}

const Ground* StructureFromJson::getGround(sf::Vector2i block_in_chunk) const
{
    sf::Vector2i diff = getRealBlockPos(chunk, block_in_chunk) - pos;
    diff += sf::Vector2i(origin_x, origin_y);

    if (diff.x < 0 || diff.y < 0 || diff.x >= size_x || diff.y >= size_y)
        return GameGrounds::STRUCTURE_VOID;

    return grounds.at(diff.x + diff.y * size_x);
}

void StructureFromJson::load(std::string json_path, const GameBlocks& game_blocks, const GameGrounds& game_grounds)
{
    std::ifstream is(json_path);

    if (is.is_open())
    {
        nlohmann::json json;

        try
        {
            is >> json;
            if (!json.is_object())
            {
                log(ERROR, "Structure file {} could not be loaded (incorrect json)\n", json_path);
                goto exit;
            }
        }
        catch (std::exception& e)
        {
            log(ERROR, "Structure file {} could not be loaded (incorrect json). Details :\n", json_path);
            log(ERROR, e.what());
            goto exit;
        }

        nlohmann::json::iterator j_size_x = json.find("size_x");
        nlohmann::json::iterator j_size_y = json.find("size_y");
        nlohmann::json::iterator j_origin_x = json.find("origin_x");
        nlohmann::json::iterator j_origin_y = json.find("origin_y");
        nlohmann::json::iterator j_blocks = json.find("blocks");
        nlohmann::json::iterator j_grounds = json.find("grounds");
        nlohmann::json::iterator j_block_layer = json.find("block_layer");
        nlohmann::json::iterator j_ground_layer = json.find("ground_layer");

        if (j_size_x == json.end() ||       !j_size_x->         is_number() ||
            j_size_y == json.end() ||       !j_size_y->         is_number() ||
            j_origin_x == json.end() ||     !j_origin_x->       is_number() ||
            j_origin_y == json.end() ||     !j_origin_y->       is_number() ||
            j_blocks == json.end() ||       !j_blocks->         is_array() ||
            j_grounds == json.end() ||      !j_grounds->        is_array() ||
            j_block_layer == json.end() ||  !j_block_layer->    is_string() ||
            j_ground_layer == json.end() || !j_ground_layer->   is_string()
            )
        {
            log(ERROR, "Structure file {} could not be loaded (json file doesn't meet structure file requirements)\n", json_path);
            goto exit;
        }

        int v_size_x = j_size_x->get<int>();
        int v_size_y = j_size_y->get<int>();

        if (v_size_x < 0 || v_size_y < 0 || v_size_x > Chunk::CHUNK_SIZE * 2 || v_size_y > Chunk::CHUNK_SIZE * 2)
        {
            log(ERROR, "Structure {} doesn't meet requirements (incorrect size, too big or negative)\n", json_path);
            goto exit;
        }

        int v_origin_x = j_origin_x->get<int>();
        int v_origin_y = j_origin_y->get<int>();

        std::vector<const Block*> block_palette; block_palette.push_back(GameBlocks::STRUCTURE_VOID);
        std::vector<const Ground*> ground_palette; ground_palette.push_back(GameGrounds::STRUCTURE_VOID);

        {
            std::vector<std::string> v_block_list = j_blocks->get<std::vector<std::string>>();
            for (std::string b : v_block_list)
                block_palette.push_back(game_blocks.getBlockByName(b));
        }

        {
            std::vector<std::string> v_ground_list = j_grounds->get<std::vector<std::string>>();
            for (std::string g : v_ground_list)
                ground_palette.push_back(game_grounds.getGroundByName(g));
        }

        std::string v_block_layer = j_block_layer->get<std::string>();
        std::string v_ground_layer = j_ground_layer->get<std::string>();

        for (char c : v_block_layer)
        {
            if (c < '0' || c > '9')
                continue;

            int i = c - '0';

            if (i >= block_palette.size())
            {
                blocks.push_back(GameBlocks::ERROR);
                continue;
            }

            blocks.push_back(block_palette.at(i));
        }

        for (char c : v_ground_layer)
        {
            if (c < '0' || c > '9')
                continue;

            int i = c - '0';

            if (i >= ground_palette.size())
            {
                grounds.push_back(GameGrounds::ERROR);
                continue;
            }

            grounds.push_back(ground_palette.at(i));
        }

        if (blocks.size() < v_size_x * v_size_y ||
            grounds.size() < v_size_x * v_size_y)
        {
            log(ERROR, "Structure {} doesn't meet requirements (not enough blocks or grounds)\n", json_path);
            blocks.clear();
            grounds.clear();
            goto exit;
        }

        size_x = v_size_x;
        size_y = v_size_y;
        origin_x = v_origin_x;
        origin_y = v_origin_y;
    }
    else
        log(ERROR, "Structure file {} could not be loaded (couldn't access file)\n", json_path);

exit:
    is.close();
}
