
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include "floats.hpp"
#include "mesh.hpp"

struct MinecraftCharacter {
	Mesh leftLeg = Mesh("<missing>");
	Mesh rightLeg = Mesh("<missing>");
	Mesh leftArm = Mesh("<missing>");
	Mesh rightArm = Mesh("<missing>");
	Mesh torso = Mesh("<missing>");
	Mesh head = Mesh("<missing>");
};

MinecraftCharacter loadMinecraftCharacterModel(std::string const srcFile); 

std::vector<Mesh> loadWavefront(std::string const srcFile, bool quiet = true);