#include "Mesh.h"	

Mesh::Mesh()
{
	// before calling create mesh function everything will be set to 0 by default. 
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat  *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;
	// need to make sure to unbind buffers and arrays when making new objects to avoid editing and drawing unintended objects.
	// Creating a vertex array in gpu and get reference to VAO we give it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); // no bind the vao to the array

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // element buffer stores indicies
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW); // just the pointer wont give us what we need so we'll get the first value  and multiply it by num of indicies. 


	//Create buffer object inside vao
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//connect buffer data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); // static draw as we wont change values in array. GL dynamic lets you do that though
	//NOTE be careful with bodmas wrapping the ) around num of verticies will have the size of verticies in the first position in the array
	// multiplying by 0 by default which will result in no verticies  being bassed in to buffer data resulting in no shape because of no verticies. 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	glBindVertexArray(0);

}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0; // deleting buffer of GPU memory to create space since there isn't any garbage collection. and prevent memeory overflow
	}
	if (VBO!= 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0; // deleting buffer of GPU memory to create space since there isn't any garbage collection. and prevent memeory overflow
	}
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0; 
	}
	indexCount = 0; // delete objects related to mesh. 
}

Mesh::~Mesh()
{
	ClearMesh(); // ensure mesh clearing in destructor. 
}