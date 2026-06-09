#ifndef SHAPE_H
#define SHAPE_H

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <vector>
#include <string>

using namespace std;
using namespace glm;

#define MPI 3.1416

class Shape
{
public:
	Shape(string objectType)
	{
		this->objectType = objectType;

		this->ambient = vec3(1.0f, 0.0f, 0.0f);
		this->diffuse = vec3(1.0f, 0.0f, 0.0f);
		this->specular = vec3(1.0f, 0.0f, 0.0f);
	}
	~Shape()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}


	void initializeCylinder(float cy_topRadius, float cy_baseRadius, float cy_height, int cy_stackCount, int cy_sectorCount,
		unsigned int texture0, unsigned int texture1, unsigned int texture2)
	{
		this->cy_topRadius = cy_topRadius;
		this->cy_baseRadius = cy_baseRadius;
		this->cy_height = cy_height;

		this->cy_stackCount = cy_stackCount;
		this->cy_sectorCount = cy_sectorCount;

		this->texture0 = texture0;
		this->texture1 = texture1;
		this->texture2 = texture2;

		this->cy_generateData();
		this->bindData();
	}


	void drawTexture(Shader& shader, float tx = 0.0f, float ty = 0.0f, float tz = 0.0f,
		float rx = 0.0f, float ry = 0.0f, float rz = 0.0f,
		float sx = 1.0f, float sy = 1.0f, float sz = 1.0f, glm::mat4 modelCenter = glm::mat4(1.0f)) const
	{
		tx *= 2, ty *= 2, tz *= 2;
		mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;
		translateMatrix = translate(mat4(1.0f), glm::vec3(tx, ty, tz));
		rotateXMatrix = rotate(translateMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
		rotateYMatrix = rotate(rotateXMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		rotateZMatrix = rotate(rotateYMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
		mat4 model = scale(rotateZMatrix, glm::vec3(sx, sy, sz));

		model = modelCenter * model;

		shader.setVec3("material.ambient", this->ambient);
		shader.setVec3("material.diffuse", this->diffuse);
		shader.setVec3("material.specular", this->specular);


		this->cy_drawTexture(shader, model);
	}

	void drawColor(Shader& shader, float tx = 0.0f, float ty = 0.0f, float tz = 0.0f,
		float rx = 0.0f, float ry = 0.0f, float rz = 0.0f,
		float sx = 1.0f, float sy = 1.0f, float sz = 1.0f, glm::mat4 modelCenter = glm::mat4(1.0f), vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)) const
	{
		tx *= 2, ty *= 2, tz *= 2;
		mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;
		translateMatrix = translate(mat4(1.0f), glm::vec3(tx, ty, tz));
		rotateXMatrix = rotate(translateMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
		rotateYMatrix = rotate(rotateXMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		rotateZMatrix = rotate(rotateYMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
		mat4 model = modelCenter * scale(rotateZMatrix, glm::vec3(sx, sy, sz));


		this->cy_drawColor(shader, model);

	}



private:
	void cy_drawColor(Shader& shader, mat4 model) const
	{
		shader.use();

		shader.setMat4("model", model);

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void cy_drawTexture(Shader& shader, mat4 model) const
	{
		shader.use();

		shader.setMat4("model", model);

		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->texture0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->texture1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->texture2);

		glBindVertexArray(VAO);

		// glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);

		shader.setInt("material.diffuseMap", 0);
		glDrawElements(GL_TRIANGLES, textureCount0, GL_UNSIGNED_INT, (void*)0);

		shader.setInt("material.diffuseMap", 1);
		glDrawElements(GL_TRIANGLES, textureCount1 - textureCount0, GL_UNSIGNED_INT, (void*)(textureCount0 * sizeof(unsigned int)));

		shader.setInt("material.diffuseMap", 2);
		glDrawElements(GL_TRIANGLES, textureCount2 - textureCount1, GL_UNSIGNED_INT, (void*)(textureCount1 * sizeof(unsigned int)));

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	void addCoordinates(float a, float b, float c)
	{
		coordinates.insert(coordinates.end(), { a , b , c });
	}
	void addNormals(float a, float b, float c)
	{
		normals.insert(normals.end(), { a, b, c });
	}
	void addTextureCoordinates(float a, float b)
	{
		textureCoordinates.insert(textureCoordinates.end(), { a, b });
	}
	void addVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		float xoffset = 0.0f, yoffset = 0.0f, zoffset = 0.0f;

		coordinates.insert(coordinates.end(), { x + xoffset, y + yoffset, z + zoffset });
		normals.insert(normals.end(), { nx, ny, nz });
		textureCoordinates.insert(textureCoordinates.end(), { u, v });

		vertices.insert(vertices.end(), { x + xoffset, y + yoffset, z + zoffset, nx, ny, nz, u, v });
	}
	void updateNormal(int idx1, int idx2, int idx3)
	{
		idx1 *= 8, idx2 *= 8, idx3 *= 8;

		float x1 = vertices[idx1 + 0], y1 = vertices[idx1 + 1], z1 = vertices[idx1 + 2];
		float x2 = vertices[idx2 + 0], y2 = vertices[idx2 + 1], z2 = vertices[idx2 + 2];
		float x3 = vertices[idx3 + 0], y3 = vertices[idx3 + 1], z3 = vertices[idx3 + 2];

		float v21x = x1 - x2;
		float v21y = y1 - y2;
		float v21z = z1 - z2;

		float v23x = x3 - x2;
		float v23y = y3 - y2;
		float v23z = z3 - z2;

		float nx = v23y * v21z - v23z * v21y;
		float ny = v23z * v21x - v23x * v21z;
		float nz = v23x * v21y - v23y * v21x;

		float length = sqrtf(nx * nx + ny * ny + nz * nz);

		nx /= length;
		ny /= length;
		nz /= length;

		vertices[idx2 + 3] = -nx, vertices[idx2 + 4] = -ny, vertices[idx2 + 5] = -nz;
	}


	void cy_generateData()
	{
		vector<float> topCircle, baseCircle;
		for (int i = 0; i <= this->cy_stackCount; ++i)
		{
			float y = (this->cy_height / 2.0f) - (float)i / this->cy_stackCount * this->cy_height;
			float radius = this->cy_topRadius + (float)i / this->cy_stackCount * (this->cy_baseRadius - this->cy_topRadius);

			float ny = 0.0f;

			float v = 1 - (float)i / this->cy_stackCount;

			for (int j = 0; j <= this->cy_sectorCount; ++j)
			{
				float sectorAngle = (float)j / this->cy_sectorCount * 2 * MPI;

				float x = radius * cosf(sectorAngle);
				float z = radius * sinf(sectorAngle);

				float nx = cosf(sectorAngle);
				float nz = sinf(sectorAngle);

				float u = (float)j / this->cy_sectorCount;

				addVertex(x, y, z, nx, ny, nz, u, v);

				if (i == 0)
				{
					float u = nx / 2.0f + 0.5f;
					float v = nz / 2.0f + 0.5f;

					topCircle.push_back(x), topCircle.push_back(y), topCircle.push_back(z);
					topCircle.push_back(0.0f), topCircle.push_back(1.0f), topCircle.push_back(0.0f);
					topCircle.push_back(u), topCircle.push_back(v);
				}
				else if (i == this->cy_stackCount)
				{
					float u = nx / 2.0f + 0.5f;
					float v = nz / 2.0f + 0.5f;

					baseCircle.push_back(x), baseCircle.push_back(y), baseCircle.push_back(z);
					baseCircle.push_back(0.0f), baseCircle.push_back(-1.0f), baseCircle.push_back(0.0f);
					baseCircle.push_back(u), baseCircle.push_back(v);
				}
			}
		}

		for (auto a : topCircle)
		{
			vertices.push_back(a);
		}
		for (auto a : baseCircle)
		{
			vertices.push_back(a);
		}

		addVertex(0.0f, this->cy_height / 2.0f, 0.0f, 0.0f, 1.0f, 0.f, 0.5f, 0.5f);
		addVertex(0.0f, -(this->cy_height / 2.0f), 0.0f, 0.0f, -1.0f, 0.f, 0.5f, 0.5f);

		for (int i = 0; i < this->cy_stackCount; ++i)
		{
			int k1 = i * (this->cy_sectorCount + 1), k2 = k1 + (this->cy_sectorCount + 1);
			for (int j = 0; j < this->cy_sectorCount; ++j, ++k1, ++k2)
			{
				indices.push_back(k1), indices.push_back(k2), indices.push_back(k2 + 1);
				indices.push_back(k2 + 1), indices.push_back(k1 + 1), indices.push_back(k1);

				if (i != 0)
				{
					updateNormal(k1, k2, k2 + 1);
					updateNormal(k2 + 1, k1 + 1, k1);
				}
			}
		}

		textureCount0 = indices.size();

		// top Circle
		int k = (this->cy_stackCount + 1) * (this->cy_sectorCount + 1);
		int center = (this->cy_stackCount + 3) * (this->cy_sectorCount + 1);

		for (int i = 0; i < this->cy_sectorCount; ++i, ++k)
		{
			indices.push_back(center), indices.push_back(k), indices.push_back(k + 1);
		}

		textureCount1 = indices.size();

		// base Circle
		k = (this->cy_stackCount + 2) * (this->cy_sectorCount + 1);
		center = center + 1;

		for (int i = 0; i < this->cy_sectorCount; ++i, ++k)
		{
			indices.push_back(center), indices.push_back(k), indices.push_back(k + 1);
		}

		textureCount2 = indices.size();
	}

	void bindData()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float),
			this->vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int),
			this->indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (int)(8 * sizeof(float)), (void*)(0 * sizeof(float)));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (int)(8 * sizeof(float)), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (int)(8 * sizeof(float)), (void*)(6 * sizeof(float)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	string objectType;

	unsigned int texture0, texture1, texture2, texture3, texture4, texture5;
	unsigned int textureCount0, textureCount1, textureCount2, textureCount3, textureCount4, textureCount5;

	vec3 ambient, diffuse, specular;

	// cylinder
	float cy_topRadius, cy_baseRadius, cy_height;
	int cy_stackCount, cy_sectorCount;

	unsigned int VAO, VBO, EBO;

	vector<float> coordinates;
	vector<float> normals;
	vector<float> textureCoordinates;

	vector<float> vertices;
	vector<unsigned int> indices;
};

#endif
