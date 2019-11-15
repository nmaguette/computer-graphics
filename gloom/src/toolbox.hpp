#pragma once

#include <glm/mat4x4.hpp>
#include "mesh.hpp"

// Generates a mesh containing a 3D object which looks like a chessboard.
Mesh generateChessboard(unsigned int width, unsigned int height, float tileWidth, float4 tileColour1, float4 tileColour2);

// Returns a random float between 0 and 1
float randomUniformFloat();

// Return the amount of time elapsed since the LAST TIME this function was called, in seconds.
double getTimeDeltaSeconds();

// Converts an angle measured in degrees to radians.
float toRadians(float angleDegrees);

// A helpful class which loads in a path text file when it's created.
// Simplifies managing waypoints and checking whether the current one has been reached.
class Path {
private:
    // Vectors which internally stores the tile coordinates that make up the path.
    std::vector<int2> waypoints;

    // Index of the current waypoint within the waypoints vector.
    unsigned int currentWaypoint = 0;

public:
    // Constructor. Requires loading a coordinates text file from a specified path.
    Path(std::string const &coordinatesFile);

    // Returns the coordinates of the current waypoint, scaled to the coordinate space
    // of the terrain.
    float2 getCurrentWaypoint(float tileWidth);

    // Determines whether the character is close enough to move on to the next waypoint
    bool hasWaypointBeenReached(float2 characterPosition, float tileWidth);

    // Advances the current waypoint to the next one on the path.
    // If the end has been reached, it resets to the first waypoint.
    // Should be called if hasWaypointBeenReached() evaluates to true.
    void advanceToNextWaypoint();
};