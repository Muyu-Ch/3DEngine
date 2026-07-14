#include "Cube.h"

Cube::Cube(float halfX, float halfY, float zNear, float zFar) {
    setVertices({
        Vector3( halfX,  halfY, zNear, 1.0f, 'A'),
        Vector3( halfX, -halfY, zNear, 1.0f, 'B'),
        Vector3(-halfX, -halfY, zNear, 1.0f, 'C'),
        Vector3(-halfX,  halfY, zNear, 1.0f, 'D'),
        Vector3( halfX,  halfY, zFar,  1.0f, 'E'),
        Vector3( halfX, -halfY, zFar,  1.0f, 'F'),
        Vector3(-halfX, -halfY, zFar,  1.0f, 'G'),
        Vector3(-halfX,  halfY, zFar,  1.0f, 'H')
    });
    setAllPairEdges();
}
