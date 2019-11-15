#include "sceneGraph.hpp"
#include <iostream>

// --- Matrix Stack related functions ---

// You can use these to create a more "realistic" scene graph implementation 

// Allocate a new empty matrix stack on the heap
std::stack<glm::mat4>* createEmptyMatrixStack() {
	return new std::stack<glm::mat4>();
}

// Push a matrix on top of the stack
void pushMatrix(std::stack<glm::mat4>* stack, glm::mat4 matrix) {
	stack->push(matrix);
}

// Remove a matrix from the top of the stack. The popped value is not returned.
void popMatrix(std::stack<glm::mat4>* stack) {
	stack->pop();
}

// Return the matrix which is currently at the top of the stack
glm::mat4 peekMatrix(std::stack<glm::mat4>* stack) {
	return stack->top();
}

// Pretty prints the values of a matrix to stdout. 
void printMatrix(glm::mat4 matrix) {
	float* values = glm::value_ptr(matrix);

	printf("(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n(%f, %f, %f, %f)\n",
		values[0], values[4], values[8], values[12], 
		values[1], values[5], values[9], values[13], 
		values[2], values[6], values[10], values[14], 
		values[3], values[7], values[11], values[15]);
}

// --- Scene Graph related functions ---

// Creates an empty SceneNode instance.
// Values are initialised because otherwise they may contain garbage memory.
SceneNode* createSceneNode() {
	return new SceneNode();
}

// Add a child node to its parent's list of children
void addChild(SceneNode* parent, SceneNode* child) {
	parent->children.push_back(child);
}

// Pretty prints the current values of a SceneNode instance to stdout
void printNode(SceneNode* node) {
	printf(
		"SceneNode {\n"
		"    Child count: %i\n"
		"    Rotation: (%f, %f, %f)\n"
		"    Location: (%f, %f, %f)\n"
		"    Reference point: (%f, %f, %f)\n"
		"    VAO ID: %i\n"
		"}\n",
		int(node->children.size()),
		node->rotation.x, node->rotation.y, node->rotation.z,
		node->position.x, node->position.y, node->position.z,
		node->referencePoint.x, node->referencePoint.y, node->referencePoint.z, 
		node->vertexArrayObjectID);
}

