#include "Chunk.h"

const glm::vec3 faceVertexOffsets[6][4] = {
	{ {1,0,0}, {1,1,0}, {1,1,1}, {1,0,1} },	// +X
	
	{ {0,0,1}, {0,1,1}, {0,1,0}, {0,0,0} },	// -X
	
	{ {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1} },	// +Z

	{ {1,0,0}, {0,0,0}, {0,1,0}, {1,1,0} },	// -Z
	
	{ {0,1,0}, {0,1,1}, {1,1,1}, {1,1,0} },	// +Y
	
	{ {0,0,1}, {0,0,0}, {1,0,0}, {1,0,1} }	// -Y
};

const glm::ivec3 faceNormals[6] = {
	{ 1,  0,  0}, // +X
	{-1,  0,  0}, // -X
	{ 0,  0,  1}, // +Z
	{ 0,  0, -1}, // -Z
	{ 0,  1,  0}, // +Y
	{ 0, -1,  0}  // -Y
};

const glm::vec2 faceUVs[4] = {
	{0.0f, 0.0f}, // bottom-left
	{0.0f, 1.0f}, // top-left
	{1.0f, 1.0f}, // top-right
	{1.0f, 0.0f}  // bottom-right
};

Chunk::Chunk(ChunkCoord coord)
{
	m_coord = coord;

	m_blocks.resize(CHUNK_X_MAX * CHUNK_Y_MAX * CHUNK_Z_MAX);

	std::srand(std::time(nullptr));
}



void Chunk::generateChunk()
{
	generated = true;

	const int centerX = CHUNK_X_MAX / 2;
	const int centerZ = CHUNK_Z_MAX / 2;
	const int peakHeight = 65;
	const int baseHeight = 60;
	const float radius = std::min(CHUNK_X_MAX, CHUNK_Z_MAX) / 2.0f;

	for (int x = 0; x < CHUNK_X_MAX; x++)
	{
		for (int z = 0; z < CHUNK_Z_MAX; z++)
		{
			float dx = x - centerX + 0.5f;
			float dz = z - centerZ + 0.5f;
			float dist = std::sqrt(dx * dx + dz * dz);

			float t = std::max(0.0f, 1.0f - dist / radius);
			int hillHeight = static_cast<int>(baseHeight + t * (peakHeight - baseHeight));

			for (int y = 0; y < CHUNK_Y_MAX; y++)
			{
				BlockType blockType;

				if (x == centerX && z == centerZ)
				{
					blockType = BlockType::Air;
				}

				else if (y > hillHeight)
				{
					blockType = BlockType::Air;
				}
				else if (y == hillHeight)
				{
					blockType = BlockType::Grass;
				}
				else if (y >= hillHeight - 4)
				{
					blockType = BlockType::Dirt;
				}
				else
				{
					blockType = BlockType::Stone;
				}

				m_blocks[x + CHUNK_X_MAX * (z + CHUNK_Z_MAX * y)] = Block(glm::vec3(x, y, z), blockType);
			}
		}
	}
}

void Chunk::generateChunkMesh(const std::shared_ptr<Chunk>& negX, const std::shared_ptr<Chunk>& posX, const std::shared_ptr<Chunk>& negZ, const std::shared_ptr<Chunk>& posZ)
{
	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			for (int y = 0; y < 256; y++)
			{
				int idx = x + CHUNK_X_MAX * (z + CHUNK_Z_MAX * y);
				Block& block = m_blocks[idx];

				if (block.blockType == BlockType::Air)
					continue;

				for (int face = 0; face < 6; face++)
				{
					int nx = x + faceNormals[face].x;
					int ny = y + faceNormals[face].y;
					int nz = z + faceNormals[face].z;

					bool isNeighbourAir = false;

					if (nx < 0) {
						if (negX && negX->isValid()) {
							int nidx = (CHUNK_X_MAX - 1) + CHUNK_X_MAX * (nz + CHUNK_Z_MAX * ny);
							isNeighbourAir = negX->m_blocks[nidx].blockType == BlockType::Air;
						}
						else {
							isNeighbourAir = true;
						}
					}
					else if (nx >= CHUNK_X_MAX) {
						if (posX && posX->isValid()) {
							int nidx = 0 + CHUNK_X_MAX * (nz + CHUNK_Z_MAX * ny);
							isNeighbourAir = posX->m_blocks[nidx].blockType == BlockType::Air;
						}
						else {
							isNeighbourAir = true;
						}
					}
					else if (nz < 0) {
						if (negZ && negZ->isValid()) {
							int nidx = nx + CHUNK_X_MAX * ((CHUNK_Z_MAX - 1) + CHUNK_Z_MAX * ny);
							isNeighbourAir = negZ->m_blocks[nidx].blockType == BlockType::Air;
						}
						else {
							isNeighbourAir = true;
						}
					}
					else if (nz >= CHUNK_Z_MAX) {
						if (posZ && posZ->isValid()) {
							int nidx = nx + CHUNK_X_MAX * (0 + CHUNK_Z_MAX * ny);
							isNeighbourAir = posZ->m_blocks[nidx].blockType == BlockType::Air;
						}
						else {
							isNeighbourAir = true;
						}
					}
					else if (ny < 0 || ny >= CHUNK_Y_MAX) {
						isNeighbourAir = true;
					}
					else
					{
						int nidx = nx + CHUNK_X_MAX * (nz + CHUNK_Z_MAX * ny);
						isNeighbourAir = m_blocks[nidx].blockType == BlockType::Air;
					}



					if (isNeighbourAir)
						addfaceToMesh(x, y, z, face, mesh.vertices, mesh.indices, (int)block.blockType);
				}
			}
		}
	}
}

glm::vec3 Chunk::getWorldPosition()
{
	return glm::vec3(m_coord.x * CHUNK_X_MAX, 0, m_coord.z * CHUNK_Z_MAX);
}

void Chunk::addfaceToMesh(int x, int y, int z, int face, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, int texIndex)
{
	uint32_t baseIndex = vertices.size();
	for (int i = 0; i < 4; i++)
	{
		Vertex v;
		v.position = glm::vec3(x, y, z) + faceVertexOffsets[face][i];
		v.texCoords = faceUVs[i];
		v.texIndex = texIndex;

		vertices.push_back(v);
	}

	indices.push_back(baseIndex + 0);
	indices.push_back(baseIndex + 1);
	indices.push_back(baseIndex + 2);
	indices.push_back(baseIndex + 0);
	indices.push_back(baseIndex + 2);
	indices.push_back(baseIndex + 3);
}

bool ChunkCoord::operator==(const ChunkCoord& other) const
{
	return x == other.x && z == other.z;
}
