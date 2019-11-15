#pragma once

#include <string>
#include <vector>
#include "floats.hpp"

class Mesh;

class Mesh {
public:
	std::string name;
	std::vector<float4> vertices;
	std::vector<float4> colours;
	std::vector<float3> normals;
	std::vector<unsigned int> indices;

	Mesh(std::string vname) : name(vname) {}

	bool hasNormals;

	unsigned long faceCount() {
		return (this->vertices.size() / 3);
	}
};
