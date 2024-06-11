#pragma once
#ifndef _H_PRIMITIVES
#define _H_PRIMITIVES

#define _USE_MATH_DEFINES
#include <math.h>

static std::list<float> CUBE_MESH_UVS = {
	// Front face
	0.0f, 0.0f,  // Vertex 0
	1.0f, 0.0f,  // Vertex 1
	1.0f, 1.0f,  // Vertex 2
	0.0f, 1.0f,  // Vertex 3

	// Back face
	0.0f, 0.0f,  // Vertex 0
	1.0f, 0.0f,  // Vertex 1
	1.0f, 1.0f,  // Vertex 2
	0.0f, 1.0f,  // Vertex 3

	// Left face
	0.0f, 0.0f,  // Vertex 0
	1.0f, 0.0f,  // Vertex 1
	1.0f, 1.0f,  // Vertex 2
	0.0f, 1.0f,  // Vertex 3

	// Right face
	0.0f, 0.0f,  // Vertex 0
	1.0f, 0.0f,  // Vertex 1
	1.0f, 1.0f,  // Vertex 2
	0.0f, 1.0f,  // Vertex 3

	// Bottom face
	0.0f, 0.0f,  // Vertex 0
	1.0f, 0.0f,  // Vertex 1
	1.0f, 1.0f,  // Vertex 2
	0.0f, 1.0f,  // Vertex 3

	// Top face
	0.0f, 0.0f,  // Vertex 0
	1.0f, 0.0f,  // Vertex 1
	1.0f, 1.0f,  // Vertex 2
	0.0f, 1.0f,  // Vertex 3
};
static std::list<float> CUBE_MESH_NORMALS = {
	// Front face
	 0.0f,  0.0f, -1.0f,  // Vertex 0
	 0.0f,  0.0f, -1.0f,  // Vertex 1
	 0.0f,  0.0f, -1.0f,  // Vertex 2
	 0.0f,  0.0f, -1.0f,  // Vertex 3

	 // Back face
	  0.0f,  0.0f,  1.0f,  // Vertex 4
	  0.0f,  0.0f,  1.0f,  // Vertex 5
	  0.0f,  0.0f,  1.0f,  // Vertex 6
	  0.0f,  0.0f,  1.0f,  // Vertex 7

	  // Left face
	  -1.0f,  0.0f,  0.0f,  // Vertex 8
	  -1.0f,  0.0f,  0.0f,  // Vertex 9
	  -1.0f,  0.0f,  0.0f,  // Vertex 10
	  -1.0f,  0.0f,  0.0f,  // Vertex 11

	  // Right face
	   1.0f,  0.0f,  0.0f,  // Vertex 12
	   1.0f,  0.0f,  0.0f,  // Vertex 13
	   1.0f,  0.0f,  0.0f,  // Vertex 14
	   1.0f,  0.0f,  0.0f,  // Vertex 15

	   // Bottom face
		0.0f, -1.0f,  0.0f,  // Vertex 16
		0.0f, -1.0f,  0.0f,  // Vertex 17
		0.0f, -1.0f,  0.0f,  // Vertex 18
		0.0f, -1.0f,  0.0f,  // Vertex 19

		// Top face
		 0.0f,  1.0f,  0.0f,  // Vertex 20
		 0.0f,  1.0f,  0.0f,  // Vertex 21
		 0.0f,  1.0f,  0.0f,  // Vertex 22
		 0.0f,  1.0f,  0.0f   // Vertex 23
};

static std::list<float> CUBE_MESH_VERTICES = {
	// Front face
	-0.5f, -0.5f, -0.5f,  // Vertex 0
	 0.5f, -0.5f, -0.5f,  // Vertex 1
	 0.5f,  0.5f, -0.5f,  // Vertex 2
	-0.5f,  0.5f, -0.5f,  // Vertex 3

	// Back face
	-0.5f, -0.5f,  0.5f,  // Vertex 4
	 0.5f, -0.5f,  0.5f,  // Vertex 5
	 0.5f,  0.5f,  0.5f,  // Vertex 6
	-0.5f,  0.5f,  0.5f,  // Vertex 7

	// Left face
	-0.5f, -0.5f, -0.5f,  // Vertex 8
	-0.5f,  0.5f, -0.5f,  // Vertex 9
	-0.5f,  0.5f,  0.5f,  // Vertex 10
	-0.5f, -0.5f,  0.5f,  // Vertex 11

	// Right face
	 0.5f, -0.5f, -0.5f,  // Vertex 12
	 0.5f,  0.5f, -0.5f,  // Vertex 13
	 0.5f,  0.5f,  0.5f,  // Vertex 14
	 0.5f, -0.5f,  0.5f,  // Vertex 15

	 // Bottom face
	 -0.5f, -0.5f, -0.5f,  // Vertex 16
	  0.5f, -0.5f, -0.5f,  // Vertex 17
	  0.5f, -0.5f,  0.5f,  // Vertex 18
	 -0.5f, -0.5f,  0.5f,  // Vertex 19

	 // Top face
	 -0.5f,  0.5f, -0.5f,  // Vertex 20
	  0.5f,  0.5f, -0.5f,  // Vertex 21
	  0.5f,  0.5f,  0.5f,  // Vertex 22
	 -0.5f,  0.5f,  0.5f   // Vertex 23
};
static std::list<int> CUBE_MESH_INDICES = {
		
	// Front face
0, 1, 2, 2, 3, 0,

// Back face
4, 5, 6, 6, 7, 4,

// Left face
8, 9, 10, 10, 11, 8,

// Right face
12, 13, 14, 14, 15, 12,

// Bottom face
16, 17, 18, 18, 19, 16,

// Top face
20, 21, 22, 22, 23, 20
	
};

static float PLANE_MESH_VERTEX_ARRAY[3 * 6] = {
	 -1.0f, -1.0f, 0.0f,  // Bottom-left
	 1.0f, -1.0f, 0.0f, // Bottom-right
	 1.0f, 1.0f, 0.0f,  // Top-right

	 // Second triangle
	  1.0f, 1.0f, 0.0f,  // Top-right
	 -1.0f, 1.0f, 0.0f, // Top-left
	 -1.0f, -1.0f, 0.0f
};

static std::list<float> PYRAMID_MESH_VERTICES = {
	// Base
	// Triangle 1
	-0.5f, 0.0f, -0.5f,  // Bottom-left (0)
	 0.5f, 0.0f, -0.5f,  // Bottom-right (1)
	 0.5f, 0.0f,  0.5f,  // Top-right (2)
	 // Triangle 2
	  0.5f, 0.0f,  0.5f,  // Top-right (3)
	 -0.5f, 0.0f,  0.5f,  // Top-left (4)
	 -0.5f, 0.0f, -0.5f,  // Bottom-left (5)
	 // Apex
	 0.0f, 1.0f, 0.0f,   // Top (Apex) (6)
};

// Normals (x, y, z)
static std::list<float> PYRAMID_MESH_NORMALS = {
	// Base
	// Triangle 1
	0.0f, -1.0f, 0.0f,  // Bottom-left (0)
	0.0f, -1.0f, 0.0f,  // Bottom-right (1)
	0.0f, -1.0f, 0.0f,  // Top-right (2)
	// Triangle 2
	0.0f, -1.0f, 0.0f,  // Top-right (3)
	0.0f, -1.0f, 0.0f,  // Top-left (4)
	0.0f, -1.0f, 0.0f,  // Bottom-left (5)
	// Apex
	0.0f, 0.4472f, -0.8944f,  // Top (Apex) (6)
};

// UVs (u, v)
static std::list<float> PYRAMID_MESH_UVS = {
	// Base
	// Triangle 1
	0.0f, 0.0f,  // Bottom-left (0)
	1.0f, 0.0f,  // Bottom-right (1)
	1.0f, 1.0f,  // Top-right (2)
	// Triangle 2
	1.0f, 1.0f,  // Top-right (3)
	0.0f, 1.0f,  // Top-left (4)
	0.0f, 0.0f,  // Bottom-left (5)
	// Apex
	0.5f, 0.5f   // Top (Apex) (6)
};

// Indices (for element drawing)
static std::list<int> PYRAMID_MESH_INDICES = {
	// Base
	0, 1, 2,  // Triangle 1 (Bottom-right half)
	3, 4, 5,  // Triangle 2 (Top-left half)
	// Sides
	0, 1, 6,  // Front face (Base)
	1, 2, 6,  // Right face (Base)
	2, 3, 6,  // Back face (Base)
	3, 4, 6,  // Left face (Base)
	4, 5, 6,  // Apex triangle (Top)
	5, 0, 6   // Apex triangle (Top)
};

static SharedPtr<MeshData> generateCylinderMesh(float radius, float height, int numSides) {
	// Constants for the cylinder
	const float CYLINDER_HEIGHT = height;
	const float CYLINDER_RADIUS = radius;

	// Vertices (x, y, z)
	std::list<float> vertices;
	std::list<float> normals;
	std::list<float> uvs;

	// Add bottom cap vertices, normals, and UVs
	for (int i = 0; i < numSides; ++i) {
		float theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(numSides);
		float x = CYLINDER_RADIUS * std::cos(theta);
		float z = CYLINDER_RADIUS * std::sin(theta);

		// Bottom cap vertices
		vertices.push_back(x);                  // Vertex on the circumference
		vertices.push_back(-CYLINDER_HEIGHT / 2.0f);
		vertices.push_back(z);

		// Bottom cap normals
		normals.push_back(0.0f); // X coordinate
		normals.push_back(-1.0f);// Y coordinate
		normals.push_back(0.0f); // Z coordinate

		// Bottom cap UVs
		uvs.push_back(static_cast<float>(i) / numSides); // U coordinate
		uvs.push_back(0.0f);                             // V coordinate
	}

	// Add top cap vertices, normals, and UVs
	for (int i = 0; i < numSides; ++i) {
		float theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(numSides);
		float x = CYLINDER_RADIUS * std::cos(theta);
		float z = CYLINDER_RADIUS * std::sin(theta);

		// Top cap vertices
		vertices.push_back(x);                  // Vertex on the circumference
		vertices.push_back(CYLINDER_HEIGHT / 2.0f);
		vertices.push_back(z);

		// Top cap normals
		normals.push_back(0.0f); // X coordinate
		normals.push_back(1.0f);// Y coordinate
		normals.push_back(0.0f); // Z coordinate

		// Top cap UVs
		uvs.push_back(static_cast<float>(i) / numSides); // U coordinate
		uvs.push_back(1.0f);                             // V coordinate
	}

	// Indices (for element drawing)
	std::list<int> indices;
	for (int i = 0; i < numSides; ++i) {
		int index = i * 2;
		indices.push_back(index);
		indices.push_back(index + 1);
		indices.push_back((index + 2) % (numSides * 2));

		indices.push_back(index + 1);
		indices.push_back((index + 3) % (numSides * 2));
		indices.push_back((index + 2) % (numSides * 2));
	}

	// Create and return MeshData
	return std::make_shared<MeshData>(vertices, indices, normals, uvs);
}
static SharedPtr<MeshData> generateSphereMesh(float radius, int numSegments) {
	// Constants for the sphere
	const float SPHERE_RADIUS = radius;
	const int NUM_SEGMENTS = numSegments;

	// Vertices (x, y, z)
	std::list<float> vertices;
	std::list<float> normals;
	std::list<float> uvs;

	// Add vertices, normals, and UVs
	for (int j = 0; j <= NUM_SEGMENTS; ++j) {
		float theta = j * M_PI / NUM_SEGMENTS;
		float sinTheta = std::sin(theta);
		float cosTheta = std::cos(theta);

		for (int i = 0; i <= NUM_SEGMENTS; ++i) {
			float phi = i * 2 * M_PI / NUM_SEGMENTS;
			float sinPhi = std::sin(phi);
			float cosPhi = std::cos(phi);

			// Vertex
			float x = SPHERE_RADIUS * cosPhi * sinTheta;
			float y = SPHERE_RADIUS * cosTheta;
			float z = SPHERE_RADIUS * sinPhi * sinTheta;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// Normal
			normals.push_back(x / SPHERE_RADIUS);
			normals.push_back(y / SPHERE_RADIUS);
			normals.push_back(z / SPHERE_RADIUS);

			// UV
			uvs.push_back(static_cast<float>(i) / NUM_SEGMENTS);
			uvs.push_back(static_cast<float>(j) / NUM_SEGMENTS);
		}
	}

	// Indices (for element drawing)
	std::list<int> indices;
	for (int j = 0; j < NUM_SEGMENTS; ++j) {
		for (int i = 0; i < NUM_SEGMENTS; ++i) {
			int index = j * (NUM_SEGMENTS + 1) + i;
			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index + NUM_SEGMENTS + 1);
			indices.push_back(index + 1);
			indices.push_back(index + NUM_SEGMENTS + 2);
			indices.push_back(index + NUM_SEGMENTS + 1);
		}
	}

	// Create and return MeshData
	return std::make_shared<MeshData>(vertices, indices, normals, uvs);
}
#endif