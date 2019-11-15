#include <iostream>
#include <chrono>
#include <fstream>
#include "toolbox.hpp"

Mesh generateChessboard(
        unsigned int width,  // Width and height of the chessboard, measured in tiles
        unsigned int height,
        float tileWidth,     // Width and height of each tile, measured in units
        float4 tileColour1,  // Colours of the chessboard tiles.
        float4 tileColour2) {
    std::vector<float4> vertices;
    std::vector<float4> vertexColours;
    std::vector<unsigned int> indices;

    unsigned int tileCount = width * height;

    vertices.reserve(4 * tileCount);
    vertexColours.reserve(4 * tileCount);
    indices.reserve(6 * tileCount);

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            float leftX = (float(x) - 0.5f) * tileWidth;
            float rightX = (float(x) + 0.5f) * tileWidth;
            float bottomZ = (float(y) - 0.5f) * tileWidth;
            float topZ = (float(y) + 0.5f) * tileWidth;

            unsigned int baseIndex = unsigned(vertices.size());

            indices.push_back(baseIndex + 0);
            indices.push_back(baseIndex + 2);
            indices.push_back(baseIndex + 1);
            indices.push_back(baseIndex + 0);
            indices.push_back(baseIndex + 3);
            indices.push_back(baseIndex + 2);

            vertices.push_back(float4(leftX, 0, bottomZ, 1));
            vertices.push_back(float4(rightX, 0, bottomZ, 1));
            vertices.push_back(float4(rightX, 0, topZ, 1));
            vertices.push_back(float4(leftX, 0, topZ, 1));

            bool tileColourType = ((x ^ y) & 1) == 1;

            if(tileColourType) {
                vertexColours.push_back(tileColour1);
                vertexColours.push_back(tileColour1);
                vertexColours.push_back(tileColour1);
                vertexColours.push_back(tileColour1);
            } else {
                vertexColours.push_back(tileColour2);
                vertexColours.push_back(tileColour2);
                vertexColours.push_back(tileColour2);
                vertexColours.push_back(tileColour2);
            }
        }
    }

    Mesh mesh("Chessboard terrain");
    mesh.vertices = vertices;
    mesh.colours = vertexColours;
    mesh.hasNormals = false;
    mesh.indices = indices;

    return mesh;
}

// The standard library's random number generator needs to be seeded in order to produce
// different results every time the program is run. This should only happen once, so
// we keep track here with a global variable whether this has happened previously.
bool isRandomInitialised = false;

float randomUniformFloat() {
    if (!isRandomInitialised) {
        // Initialise the random number generator using the current time as a seed
        srand(static_cast <unsigned> (time(0)));
        isRandomInitialised = true;
    }
    // rand() produces a random integer between 0 and RAND_MAX. This normalises it to a number between 0 and 1.
    return static_cast <float> (rand()) / static_cast <float>(RAND_MAX);
}

// In order to be able to calculate when the getTimeDeltaSeconds() function was last called, we need to know the point in time when that happened. This requires us to keep hold of that point in time.
// We initialise this value to the time at the start of the program.
static std::chrono::steady_clock::time_point _previousTimePoint = std::chrono::steady_clock::now();

// Calculates the elapsed time since the previous time this function was called.
double getTimeDeltaSeconds() {
    // Determine the current time
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();

    // Calculate the number of nanoseconds that elapsed since the previous call to this function
    long long timeDelta = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - _previousTimePoint).count();
    // Convert the time delta in nanoseconds to seconds
    double timeDeltaSeconds = (double)timeDelta / 1000000000.0;

    // Store the previously measured current time
    _previousTimePoint = currentTime;

    // Return the calculated time delta in seconds
    return timeDeltaSeconds;
}

float toRadians(float angleDegrees) {
    return angleDegrees * (float(3.1415926) / 180.0f);
}

// Reads a text file containing 2D integer coordinates in a specific format
// and returns a vector with these coordinates.
std::vector<int2> readCoordinatesFile(std::string filePath) {
    // Open input file
    std::ifstream inputFile(filePath);
    std::vector<int2> foundPoints;

    // Check in case the file failed to open
    if(!inputFile)
    {
        std::cerr << "Could not open file located at: " << filePath.c_str() << std::endl;
        std::cerr << "Most likely the file was not found." << std::endl;
        return foundPoints;
    }

    // Determine the number of points in the text file (specified by the first line)
    int pointCount;
    inputFile >> pointCount;

    // Read all coordinates
    for(int i = 0; i < pointCount; i++)
    {
        int2 currentPoint;
        inputFile >> currentPoint.x >> currentPoint.y;
        foundPoints.push_back(currentPoint);
    }

    // We'll be good citizens and let the OS know we're done with this file
    inputFile.close();

    // Return the points we loaded
    return foundPoints;
}

Path::Path(std::string const &coordinatesFile) {
    waypoints = readCoordinatesFile(coordinatesFile);
}

float2 Path::getCurrentWaypoint(float tileWidth) {
    int2 intWaypoint = waypoints.at(currentWaypoint);
    return float2(intWaypoint.x, intWaypoint.y) * tileWidth;
}

bool Path::hasWaypointBeenReached(float2 characterPosition, float tileWidth) {
    float2 currentWaypoint = getCurrentWaypoint(tileWidth);

    float dx = currentWaypoint.x - characterPosition.x;
    float dy = currentWaypoint.y - characterPosition.y;
    float distanceToCurrentWaypoint = std::sqrt(dx * dx + dy * dy);

    float distanceThreshold = tileWidth / 10.0f;

    return distanceToCurrentWaypoint < distanceThreshold;
}

void Path::advanceToNextWaypoint() {
    currentWaypoint++;
    currentWaypoint = currentWaypoint % unsigned(waypoints.size());
}