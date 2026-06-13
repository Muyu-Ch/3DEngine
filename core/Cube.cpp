#include "Cube.h"

Cube::Cube(float halfX, float halfY, float zNear, float zFar) {
    setVertices({
        Vector3( halfX,  halfY, zNear),
        Vector3( halfX, -halfY, zNear),
        Vector3(-halfX, -halfY, zNear),
        Vector3(-halfX,  halfY, zNear),
        Vector3( halfX,  halfY, zFar),
        Vector3( halfX, -halfY, zFar),
        Vector3(-halfX, -halfY, zFar),
        Vector3(-halfX,  halfY, zFar)
    });
    setAllPairEdges();
}
