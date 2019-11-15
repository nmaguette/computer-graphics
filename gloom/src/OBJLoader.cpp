#include "OBJLoader.hpp"
#include <algorithm>
#include <exception>
#include "sceneGraph.hpp"
#include "toolbox.hpp"

std::vector<std::string> split(std::string target, std::string delimiter)
{
	std::vector<std::string> res;
	size_t pos = 0;
	while ((pos = target.find(delimiter)) != std::string::npos) {
		res.push_back(target.substr(0, pos));
		target.erase(0, pos + delimiter.length());
	}
	res.push_back(target);
	return res;
}

std::vector<Mesh> loadWavefront(std::string const srcFile, bool quiet)
{
	std::vector<Mesh> meshes;
	std::ifstream objFile(srcFile);
	std::vector<float4> vertices;
	std::vector<float3> normals;

	if (objFile.is_open()) {
		std::string line;
		while (std::getline(objFile, line)) {

			std::vector<std::string> parts = split(line, " ");
			if (parts.size() > 0) {
				// New Mesh object
				if (parts.at(0) == "o" && parts.size() >= 2) {
					meshes.emplace_back(parts.at(1));
				} else if (parts.at(0) == "v" && parts.size() >= 4) {
					vertices.emplace_back(
						std::stof(parts.at(1)),
						std::stof(parts.at(2)),
						std::stof(parts.at(3)),
						(parts.size() >= 5) ? std::stof(parts.at(4)) : 1.0f
					);
				} else if (parts.at(0) == "vn" && parts.size() >= 4) {
				   normals.emplace_back(
					   std::stof(parts.at(1)),
					   std::stof(parts.at(2)),
					   std::stof(parts.at(3))
				   );
			   } else if (parts.at(0) == "f" && parts.size() >= 4) {
				   if (meshes.size() == 0) {
					   if (!quiet) {
						   	std::cout << "[WARNING] face definition found, but no object" << std::endl;
							std::cout << "[WARNING] creating object 'noname'" << std::endl;
					   }
					   meshes.emplace_back("noname");
					   //continue;
				   }

				   	Mesh &mesh = meshes.back();

					bool quadruple = parts.size() >= 5;

					std::vector<std::string> parts1 = split(parts.at(1),"/");
					std::vector<std::string> parts2 = split(parts.at(2),"/");
					std::vector<std::string> parts3 = split(parts.at(3),"/");
					std::vector<std::string> parts4;
					if (quadruple) {
						parts4 = split(parts.at(4),"/");
					}

					if (parts1.size() < 1 || parts1.size() != parts2.size() || parts2.size() != parts3.size() || (quadruple && parts4.size() != parts1.size())) {
						if (!quiet)
							std::cout << "[WARNING] invalid face defintion '" << line << "'" << std::endl;
						continue;
					}

					mesh.hasNormals = parts1.size() >= 3;
					
					size_t n1_index, n2_index, n3_index, n4_index;
					size_t v4_index;
					size_t v1_index = std::stoi(parts1.at(0)) - 1;
					size_t v2_index = std::stoi(parts2.at(0)) - 1;
					size_t v3_index = std::stoi(parts3.at(0)) - 1;

					if (quadruple) {
						v4_index = std::stoi(parts4.at(0)) - 1;
					}

					if (v1_index >= vertices.size() ||
						v2_index >= vertices.size() ||
						v3_index >= vertices.size() ||
						(quadruple && v4_index >= vertices.size())) {
								if (!quiet) {
									std::cout << "[WARNING] Mesh " << mesh.name << " faces vertices(" << v1_index << ", " << v2_index << ", " << v3_index;
									if (quadruple)
										std::cout << ", " << v4_index;
									std::cout << ") do not exist!" << std::endl;
								}
								continue;
					}


					if (mesh.hasNormals) {
						n1_index = std::stoi(parts1.at(2)) - 1;
						n2_index = std::stoi(parts2.at(2)) - 1;
						n3_index = std::stoi(parts3.at(2)) - 1;
						if (quadruple) {
							n4_index = std::stoi(parts4.at(2)) - 1;
						}
						if (n1_index >= normals.size() ||
							n2_index >= normals.size() ||
							n3_index >= normals.size() ||
							(quadruple && n4_index >= normals.size())) {
									if (!quiet) {
										std::cout << "[WARNING] Mesh " << mesh.name << " faces normals(" << n1_index << ", " << n2_index << ", " << n3_index;
										if (quadruple)
											std::cout << ", " << n4_index;
										std::cout << ") do not exist!" << std::endl;
									}
									continue;
						}
					}

					if (quadruple) {
						mesh.vertices.push_back(vertices.at(v1_index));
						mesh.vertices.push_back(vertices.at(v3_index));
						mesh.vertices.push_back(vertices.at(v4_index));
						
						if (mesh.hasNormals) {
							mesh.normals.push_back(normals.at(n1_index));
							mesh.normals.push_back(normals.at(n3_index));
							mesh.normals.push_back(normals.at(n4_index));
						} else {
							mesh.normals.insert(mesh.normals.end(), { 0.0f, 0.0f, 0.0f });
						}

						mesh.indices.push_back(unsigned(mesh.indices.size()));
						mesh.indices.push_back(unsigned(mesh.indices.size()));
						mesh.indices.push_back(unsigned(mesh.indices.size()));
					}

					mesh.vertices.push_back(vertices.at(v1_index));
					mesh.vertices.push_back(vertices.at(v2_index));
					mesh.vertices.push_back(vertices.at(v3_index));
					if (mesh.hasNormals){
						mesh.normals.push_back(normals.at(n1_index));
						mesh.normals.push_back(normals.at(n2_index));
						mesh.normals.push_back(normals.at(n3_index));
					} else {
						mesh.normals.insert(mesh.normals.end(), { 0.0f, 0.0f, 0.0f });
					}

					mesh.indices.push_back(unsigned(mesh.indices.size()));
					mesh.indices.push_back(unsigned(mesh.indices.size()));
					mesh.indices.push_back(unsigned(mesh.indices.size()));
				}
			}
		}
	} else {
		throw std::runtime_error("Reading OBJ file failed. This is usually because the operating system can't find it. Check if the relative path (to your terminal's working directory) is correct.");
	}

	return meshes;
}

// This function assumes a mesh with rectangular sides (pairs of triangles), and assigns each side random colours.
// It also assumes vertices have been duplicated, which is done by the loadWavefront function.

void colourFaces(Mesh &mesh) {
	int sides = mesh.faceCount() / 2;

	// Allocate capacity
	mesh.colours.resize(mesh.vertices.size(), 0);

	for(int side = 0; side < sides; side++) {
		float rand_red = randomUniformFloat();
		float rand_green = randomUniformFloat();
		float rand_blue = randomUniformFloat();

		float4 randomColour(rand_red, rand_green, rand_blue, 1.0);

		mesh.colours.at(side * 6 + 0) = randomColour;
		mesh.colours.at(side * 6 + 1) = randomColour;
		mesh.colours.at(side * 6 + 2) = randomColour;
		mesh.colours.at(side * 6 + 3) = randomColour;
		mesh.colours.at(side * 6 + 4) = randomColour;
		mesh.colours.at(side * 6 + 5) = randomColour;
	}
}

MinecraftCharacter loadMinecraftCharacterModel(std::string const srcFile) {
	std::vector<Mesh> fileContents = loadWavefront(srcFile, true);

	MinecraftCharacter out;

	for(Mesh mesh : fileContents) {
	    // Applying some colour to the different parts
        // Feel free to replace this with something more decorative
        colourFaces(mesh);

		// You usually want to use enums for a situation like this.
		// It will do the job for us, though.
		if(mesh.name == "left_leg") {
			out.leftLeg = mesh;
		} else if(mesh.name == "right_leg") {
			out.rightLeg = mesh;
		} else if(mesh.name == "left_arm") {
			out.leftArm = mesh;
		} else if(mesh.name == "right_arm") {
			out.rightArm = mesh;
		} else if(mesh.name == "torso") {
			out.torso = mesh;
		} else if(mesh.name == "head") {
			out.head = mesh;
		} else {
			throw std::runtime_error("The OBJ file did not contain any parts with names the loading function recognises. Did you load the correct OBJ file?");
		}
	}

	return out;
}