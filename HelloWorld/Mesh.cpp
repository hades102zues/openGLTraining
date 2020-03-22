#include "Mesh.h"

Mesh::Mesh() {
	vao = 0;
	vbo = 0;
	ibo = 0;
	indexCount = 0;
}

void Mesh::createMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices) {

	indexCount = numOfIndices;


	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	/*
		Location of the vertex attribute in the shader,
		Number of items being passed in at a time,
		The data type of the data,
		Normalisation,
		how many values to skip to get to the next input submission, in btyes
		how many item we skip to get to the first value, in btyes
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5,  0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5,  (void*) ( sizeof(vertices[0]) * 3) );
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Mesh::renderMesh() {

	//THIS IS ACTUALLY THE CORRECT PLACE TO BE ACTIVATING THE VERTEXATTRIBARRAYS

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//THIS IS ACTUALLY THE CORRECT PLACE TO BE deactivate THE VERTEXATTRIBARRAYS

}

void Mesh::clearMesh() {

	if (ibo != 0) {
		glDeleteBuffers(1, &ibo);
		ibo = 0;
	}

	if (vao != 0) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}

	if (vbo != 0) {
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}

	indexCount = 0;
}