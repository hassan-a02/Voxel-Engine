#pragma once

#include "Block.h"
#include <vector>
#include <random>
#include <memory>

struct ChunkCoord
{
	int x;
	int z;

	ChunkCoord() : x(0), z(0) {}
	ChunkCoord(int x, int z) : x(x), z(z) {}

	bool operator==(const ChunkCoord& other) const;
};

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
	int texIndex;

	Vertex() : position(0.0f), texCoords(0.0f), texIndex(0) {}
	Vertex(glm::vec3 pos, glm::vec2 texCoords, int texIndex) : position(pos), texCoords(texCoords), texIndex(texIndex) {}
};

struct ChunkMesh
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	unsigned int vao, vbo, veo;
};

class Chunk
{
public:
	ChunkMesh mesh;

	Chunk() = default;
	Chunk(ChunkCoord coord);

	void generateChunk();
	void generateChunkMesh(const std::shared_ptr<Chunk>& negX, const std::shared_ptr<Chunk>& posX, const std::shared_ptr<Chunk>& negZ, const std::shared_ptr<Chunk>& posZ);


	glm::vec3 getWorldPosition();
	ChunkCoord getChunkCoord() const { return m_coord; }
	bool isValid() const { return generated; }

	Chunk operator =(const Chunk& other)
	{
		if (this != &other)
		{
			m_coord = other.m_coord;
			m_blocks = other.m_blocks;
			generated = other.generated;
		}
		return *this;
	}
private:
	ChunkCoord m_coord;
	std::vector<Block> m_blocks;
	bool generated = false;

	static constexpr int CHUNK_X_MAX = 16;
	static constexpr int CHUNK_Y_MAX = 256;
	static constexpr int CHUNK_Z_MAX = 16;

	void addfaceToMesh(int x, int y, int z, int face, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, int texIndex);
	
};