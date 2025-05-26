#include "Block.h"

Block::Block(glm::vec3 startingPos, BlockType blockType) 
{
    switch (blockType)
    {
	case BlockType::Air:
		this->blockType = BlockType::Air;
		break;
	case BlockType::Grass:
		this->blockType = BlockType::Grass;
		break;
	case BlockType::Dirt:
		this->blockType = BlockType::Dirt;
		break;
	case BlockType::Stone:
		this->blockType = BlockType::Stone;
		break;
    }


	position = startingPos;
}
