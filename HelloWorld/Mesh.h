#pragma once
#include <GL\glew.h>

class Mesh
{

private :
	GLuint vao, vbo, ibo;
	GLsizei indexCount;

	 
public: 
	Mesh();


	void createMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void renderMesh();
	void clearMesh();



	~Mesh();

};

