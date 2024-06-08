#pragma once
#ifndef _H_PRIMITIVES
#define _H_PRIMITIVES
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

static float TRIANGLE_MESH_VERTEX_ARRAY[9] = {
	-0.5f, -0.5f, -0.5f,  // 0
	 0.5f, -0.5f, -0.5f,  // 1
	 0.5f,  0.5f, -0.5f
};

static float CUBE_MESH_VERTEX_ARRAY[18 * 6] = {
	// Positions          
	-0.5f, -0.5f, -0.5f,  // 0
	 0.5f, -0.5f, -0.5f,  // 1
	 0.5f,  0.5f, -0.5f,  // 2
	 0.5f,  0.5f, -0.5f,  // 2
	-0.5f,  0.5f, -0.5f,  // 3
	-0.5f, -0.5f, -0.5f,  // 0

	-0.5f, -0.5f,  0.5f,  // 4
	 0.5f, -0.5f,  0.5f,  // 5
	 0.5f,  0.5f,  0.5f,  // 6
	 0.5f,  0.5f,  0.5f,  // 6
	-0.5f,  0.5f,  0.5f,  // 7
	-0.5f, -0.5f,  0.5f,  // 4

	-0.5f,  0.5f,  0.5f,  // 7
	-0.5f,  0.5f, -0.5f,  // 3
	-0.5f, -0.5f, -0.5f,  // 0
	-0.5f, -0.5f, -0.5f,  // 0
	-0.5f, -0.5f,  0.5f,  // 4
	-0.5f,  0.5f,  0.5f,  // 7

	 0.5f,  0.5f,  0.5f,  // 6
	 0.5f,  0.5f, -0.5f,  // 2
	 0.5f, -0.5f, -0.5f,  // 1
	 0.5f, -0.5f, -0.5f,  // 1
	 0.5f, -0.5f,  0.5f,  // 5
	 0.5f,  0.5f,  0.5f,  // 6

	-0.5f, -0.5f, -0.5f,  // 0
	 0.5f, -0.5f, -0.5f,  // 1
	 0.5f, -0.5f,  0.5f,  // 5
	 0.5f, -0.5f,  0.5f,  // 5
	-0.5f, -0.5f,  0.5f,  // 4
	-0.5f, -0.5f, -0.5f,  // 0

	-0.5f,  0.5f, -0.5f,  // 3
	 0.5f,  0.5f, -0.5f,  // 2
	 0.5f,  0.5f,  0.5f,  // 6
	 0.5f,  0.5f,  0.5f,  // 6
	-0.5f,  0.5f,  0.5f,  // 7
	-0.5f,  0.5f, -0.5f   // 3
};
#endif