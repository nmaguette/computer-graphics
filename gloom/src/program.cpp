// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "OBJLoader.hpp"
#include "sceneGraph.hpp"
#include "toolbox.hpp"
#include <iostream>
#include <string>
#include <iterator>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4


using namespace std;

// A2 - 4b : X Y Z cooordinates
float rotationX = 0.0f;
float rotationY = 0.0f;
float rotationZ = 0.0f;
float xCoordinate = -60.0f;
float yCoordinate = -30.0f;
float zCoordinate = -180.0f;
float tCurrent = 0;

unsigned int vertexArrayObject(std::vector<float4> vertices, size_t verticesLength, std::vector<unsigned int> indices, size_t indicesLength, std::vector<float4> colours, size_t coloursLength)
{
	// create a Vertex Array Object
	unsigned int arrayID = 0;
	int count = 1;
	glGenVertexArrays(count,&arrayID);
	// binding a Vertex Array Object
	glBindVertexArray(arrayID);

	// create a Vertex Buﬀer Object
	unsigned int bufferID;
	glGenBuffers(count, &bufferID);
	// binding a Vertex Buﬀer Object
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	//  transfer the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, verticesLength, vertices.data(), GL_STATIC_DRAW);
	/*for (int i = 0; i<vertices.size(); i++)
	{cout << "verticesData"; cout << i; cout <<" : "; cout << vertices.data()[i]; cout << "/n";}
	printGLError();*/
	
	// If you specify x, y and z cooordinates, size is 3
	int size = 4;
	// setting the Vertex Attribute Pointer
	unsigned int index = 1;
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	// enable the Vertex Buﬀer Objects
	glEnableVertexAttribArray(index);

	// create an index Buﬀer
	unsigned int indexBuffer;
	// create a buﬀer
	glGenBuffers(count, &indexBuffer);
	// binding the buﬀer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	//  transfer the data to the GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLength, indices.data(), GL_STATIC_DRAW);
	printGLError();

	// create a colour Buﬀer
	unsigned int colourBuffer;
	// create a buﬀer
	glGenBuffers(1, &colourBuffer);
	// binding the buﬀer
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	//  transfer the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, coloursLength, colours.data(), GL_STATIC_DRAW);
	printGLError();

	// If you specify r, g, b, a cooordinates, size is 4
	int sizeColour = 4;
	// setting the Vertex Attribute Pointer
	unsigned int indexColour = 4;
	glVertexAttribPointer(indexColour, sizeColour, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// enable the Vertex Buﬀer Objects
	glEnableVertexAttribArray(indexColour);
	
	return arrayID;
}


void visitSceneNode(SceneNode* node, glm::mat4 transformationThusFar) {
	// Do transformations here
	glm::mat4x4 translationBack = glm::translate(glm::mat4(), glm::vec3(node->referencePoint.x, node->referencePoint.y, node->referencePoint.z));
	glm::mat4x4 translationOriginPoint = glm::translate(glm::mat4(), glm::vec3(-node->referencePoint.x, -node->referencePoint.y, -node->referencePoint.z));
	glm::mat4x4 translation = glm::translate(glm::mat4(), glm::vec3(node->position.x, node->position.y, node->position.z));

	glm::mat4x4 x_rotation = glm::rotate(glm::radians(node->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4x4 y_rotation = glm::rotate(glm::radians(node->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4x4 z_rotation = glm::rotate(glm::radians(node->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	node->currentTransformationMatrix = transformationThusFar*translation*translationBack*z_rotation*y_rotation*x_rotation*translationOriginPoint;
	glm::mat4x4 combinedTransformation = node->currentTransformationMatrix;
	// send the uniform variable to the Vertex Shader
	glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(combinedTransformation));
	if (node->vertexArrayObjectID != -1) {
		glBindVertexArray(node->vertexArrayObjectID);
		glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, 0);
	}
	// Do rendering here
	for (SceneNode* child : node->children) {
		visitSceneNode(child, combinedTransformation);
	}
}

void runProgram(GLFWwindow* window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);
	
    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

	// setting the perspective transformation of the windows
	glm::mat4x4 perspectiveTransform = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 1.0f, 100.0f);

	// Functions to turn on the opacity
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set up your scene here (create Vertex Array Objects, etc.)
	float4 tileColour1 = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 tileColour2 = float4(1.0f, 1.0f, 1.0f, 1.0f);
	Mesh chess = generateChessboard(7, 5, 20.0f, tileColour1, tileColour2); 
	MinecraftCharacter character = loadMinecraftCharacterModel("./gloom/src/steve.obj");  
	unsigned int vaoChess = vertexArrayObject(chess.vertices, chess.vertices.size() * 4 * sizeof(float), chess.indices, chess.indices.size() * sizeof(unsigned int), chess.colours, chess.colours.size() * 4 * sizeof(float));
	unsigned int vaoHead = vertexArrayObject(character.head.vertices, character.head.vertices.size() * 4 * sizeof(float), character.head.indices, character.head.indices.size() * sizeof(unsigned int), character.head.colours, character.head.colours.size() * 4 * sizeof(float));
	unsigned int vaoTorso = vertexArrayObject(character.torso.vertices, character.torso.vertices.size() * 4 * sizeof(float), character.torso.indices, character.torso.indices.size() * sizeof(unsigned int), character.torso.colours, character.torso.colours.size() * 4 * sizeof(float));
	unsigned int vaoLeftArm = vertexArrayObject(character.leftArm.vertices, character.leftArm.vertices.size() * 4 * sizeof(float), character.leftArm.indices, character.leftArm.indices.size() * sizeof(unsigned int), character.leftArm.colours, character.leftArm.colours.size() * 4 * sizeof(float));
	unsigned int vaoRightArm = vertexArrayObject(character.rightArm.vertices, character.rightArm.vertices.size() * 4 * sizeof(float), character.rightArm.indices, character.rightArm.indices.size() * sizeof(unsigned int), character.rightArm.colours, character.rightArm.colours.size() * 4 * sizeof(float));
	unsigned int vaoLeftLeg = vertexArrayObject(character.leftLeg.vertices, character.leftLeg.vertices.size() * 4 * sizeof(float), character.leftLeg.indices, character.leftLeg.indices.size() * sizeof(unsigned int), character.leftLeg.colours, character.leftLeg.colours.size() * 4 * sizeof(float));
	unsigned int vaoRightLeg = vertexArrayObject(character.rightLeg.vertices, character.rightLeg.vertices.size() * 4 * sizeof(float), character.rightLeg.indices, character.rightLeg.indices.size() * sizeof(unsigned int), character.rightLeg.colours, character.rightLeg.colours.size() * 4 * sizeof(float));

	SceneNode* rootNode = createSceneNode();
	SceneNode* headNode = createSceneNode();
	SceneNode* torsoNode = createSceneNode();
	SceneNode* leftArmNode = createSceneNode();
	SceneNode* rightArmNode = createSceneNode();
	SceneNode* leftLegNode = createSceneNode();
	SceneNode* rightLegNode = createSceneNode();
	SceneNode* chessNode = createSceneNode();
	addChild(torsoNode, headNode);
	addChild(torsoNode, leftArmNode);
	addChild(torsoNode, rightArmNode);
	addChild(torsoNode, leftLegNode);
	addChild(torsoNode, rightLegNode);
	printNode(headNode);
	printNode(torsoNode);
	printNode(leftArmNode);
	printNode(rightArmNode);
	printNode(leftLegNode);
	printNode(rightLegNode);
	printNode(chessNode);
	addChild(rootNode, torsoNode);
	addChild(rootNode, chessNode);
	printNode(rootNode);
	headNode->vertexArrayObjectID = vaoHead;
	torsoNode->vertexArrayObjectID = vaoTorso;
	leftArmNode->vertexArrayObjectID = vaoLeftArm;
	rightArmNode->vertexArrayObjectID = vaoRightArm;
	leftLegNode->vertexArrayObjectID = vaoLeftLeg;
	rightLegNode->vertexArrayObjectID = vaoRightLeg;
	chessNode->vertexArrayObjectID = vaoChess;
	rootNode->vertexArrayObjectID = -1;


	headNode->VAOIndexCount = character.head.indices.size();
	torsoNode->VAOIndexCount = character.torso.indices.size();
	leftArmNode->VAOIndexCount = character.leftArm.indices.size();
	rightArmNode->VAOIndexCount = character.rightArm.indices.size();
	leftLegNode->VAOIndexCount = character.leftLeg.indices.size();
	rightLegNode->VAOIndexCount = character.rightLeg.indices.size();
	chessNode->VAOIndexCount = chess.indices.size();
	
	headNode->referencePoint = float3(-4.0f, 24.0f, 0.0f);
	torsoNode->referencePoint = float3(-4.0f, 24.0f, 0.0f);
	leftArmNode->referencePoint = float3(0.0f, 24.0f, 0.0f);
	rightArmNode->referencePoint = float3(-8.0f, 24.0f, 0.0f);
	leftLegNode->referencePoint = float3(-2.0f, 12.0f, 0.0f);
	rightLegNode->referencePoint = float3(-6.0f, 12.0f, 0.0f);

	Path* pathChess = new Path("./gloom/src/pathFiles/coordinates_0.txt");

	Gloom::Shader shader;
	shader.makeBasicShader("./gloom/shaders/simple.vert", "./gloom/shaders/simple.frag");
	printGLError();
	shader.activate();
	
    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
		// Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// identity Matrix
		glm::mat4x4 identityMatrix = 0.01f * glm::mat4(1.0f);

		// translation Matrix
		glm::mat4x4 translation = glm::translate(glm::mat4(), glm::vec3(xCoordinate, yCoordinate, zCoordinate));

		// rotation Matrix from the origin
		glm::mat4x4 rotationXAxis = glm::rotate(glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4x4 rotationYAxis = glm::rotate(glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 scalingMatrix = glm::scale(glm::vec3(0.5, 0.5f, 0.5f));

		// final transformation Matrix
		glm::mat4x4 transform = perspectiveTransform * scalingMatrix * rotationYAxis * rotationXAxis * translation * identityMatrix;

		float timeSincePreviousFrame = 10*getTimeDeltaSeconds();
		tCurrent += timeSincePreviousFrame;
		//cout << "time : "; cout << getTimeDeltaSeconds(); cout << "\n";
		if (3.1415926<std::fmod(tCurrent, 6.28318) || 6.28318 > std::fmod(tCurrent, 6.28318)) {
			leftArmNode->rotation.x = 20.0f * sin(tCurrent);
			rightArmNode->rotation.x = 20.0f * -sin(tCurrent);
			leftLegNode->rotation.x = 20.0f * -sin(tCurrent);
			rightLegNode->rotation.x = 20.0f * sin(tCurrent);
		}
		else if (3.1415926>std::fmod(tCurrent, 6.28318) || 0 < std::fmod(tCurrent, 6.28318)) {
			leftArmNode->rotation.x = 20.0f * -sin(tCurrent);
			rightArmNode->rotation.x = 20.0f * sin(tCurrent + 3.1415926);
			leftLegNode->rotation.x = 20.0f * sin(tCurrent + 3.1415926);
			rightLegNode->rotation.x = 20.0f * -sin(tCurrent);
		}

		glm::vec2 position = glm::vec2(torsoNode->position.x, torsoNode->position.z);
		float2 floatWaypoint = pathChess->getCurrentWaypoint(20.0f);
		glm::vec2 waypoint = glm::vec2(floatWaypoint.x, floatWaypoint.y);
		glm::vec2 deltaVector = waypoint - position;
		deltaVector = glm::normalize(deltaVector);
		torsoNode->rotation.y = -90 + glm::degrees(-std::atan2(deltaVector.y, deltaVector.x));
		deltaVector *= timeSincePreviousFrame;
		torsoNode->position.x += deltaVector.x;
		torsoNode->position.z += deltaVector.y;

		if(pathChess->hasWaypointBeenReached(float2(position.x, position.y), 20)) {
			pathChess->advanceToNextWaypoint();
		}
	
		// Draw your scene here
		// in the case of GL_TRIANGLES, count should always be a multiple of 3
		visitSceneNode(rootNode, transform);
		printGLError();

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }
	shader.deactivate();
	shader.destroy();
}

void handleKeyboardInput(GLFWwindow* window)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
	// Use left key for X sideway left
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		xCoordinate -= 0.002;
	}
	// Use right key for X sideway right
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		xCoordinate += 0.002;
	}
	// Use UP key for Y upward
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		yCoordinate += 0.002;
	}
	// Use DOWN key for Y downward
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		yCoordinate -= 0.002;
	}
	// Use B for Z Forward
	else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		zCoordinate += 0.02;
	}
	// Use SPACE key for Z backward
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		zCoordinate -= 0.02;
	}
	// Use G for X clockwise rotation
	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		rotationX += 0.02;
	}
	// Use F for X counterclockwise rotation
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		rotationX -= 0.02;
	}
	// Use V for Y counterclockwise rotation
	else if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	{
		rotationY -= 0.02;
	}
	// Use T for Y clockwise rotation
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		rotationY += 0.02;
	}
}