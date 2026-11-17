#pragma once
#include <optional>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// The code might lack in performance but hey! It gets the job done! (For me, I don't know how it is for you)

struct RandomStuff {
	float Gravity = 9.81f;
	float ProbablyTheBestSpeedForTheRePositionFunctionIfBeingUsedInTheWhileLoop = 0.001f; // Big name D:
};


struct Cube {


	float Vertices[24] = {
		-0.5f, -0.5f, -0.5f,// 0
		0.5f, -0.5f, -0.5f, // 1
		0.5f, -0.5f, 0.5f,  // 2
		-0.5f, -0.5f, 0.5f, // 3

		0.5f, 0.5f, -0.5f, // 4
		-0.5f, 0.5f, -0.5f,// 5

		0.5f, 0.5f, 0.5f,  // 6
		-0.5f, 0.5f, 0.5f, // 7
	};

	unsigned int Indices[36] = {
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		0, 5, 4,
		1, 2, 4,
		2, 6, 4,
		6, 2, 3,
		3, 6, 7,
		5, 4, 6,
		5, 6, 7,
		0, 3, 7,
		0, 7, 5
	};

	int size = sizeof(Indices) / sizeof(Indices[0]);
};

struct Square {// eheh I could only do this :(
private:
	int ClonedTimes = 0;
	int LoopControl = 0;

	// Please change these if needed! 
	const static int VerticesSize = 64;
	const static int IndicesSize = 48;

	const static int MaxCloneLimit = VerticesSize / 8;
	// 0 = off
	// 1 = on
	// 2 = lock
	// 3 = temporary use
public:
	int Squares = 0;

	const float CONSTVertices[VerticesSize] = {
		-0.5f, 0.5f,// 0
		0.5f, 0.5f, // 1
		0.5f, -0.5f,// 2
		-0.5f, -0.5f// 3
	};

	float Vertices[VerticesSize] = {
		//-0.5f, 0.5f,// 0
		//0.5f, 0.5f, // 1
		//0.5f, -0.5f,// 2
		//-0.5f, -0.5f// 3

		// Vertices[8+]
	};

	unsigned int Indices[IndicesSize] = {
		//0, 1, 2,
		//0, 2, 3

		// Indices[6+]
	};

	float Colors[MaxCloneLimit*4] = {
		0.5f, 0.5f, 0.5f, 1.0f
	};

	static constexpr int size = sizeof(Indices) / sizeof(Indices[0]);
	/*
	* Use before declaring the Vertex Array Objects and the Vertex Buffer Objects
	* and after the Shader Program and the following shaders are declared. If not
	* done so, it will not work! WARNING: WILL RESIZE ALL SQUARES IF INDEX NOT PUT IN!
	*/
	void Resize(const float& size, std::optional<int> index = std::nullopt, std::optional<GLuint> VBO = std::nullopt) {
		if (index.has_value()) {

			int IndexNumberV = index.value() * 8;

			if (IndexNumberV + 1 >= VerticesSize) {
				std::cerr << "Exceeding the index for the vertices variable! Do change the size variables if needed.\n";
				return;
			}

			for (int i = 0; i < 8; ++i) {
				Vertices[IndexNumberV + i] = CONSTVertices[IndexNumberV + i] * size;
			}

		}
		else {
			for (size_t i = 0; i < VerticesSize; ++i) {
				Vertices[i] = Vertices[i] * size;
			}
		}

		if (VBO.has_value()) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO.value());
			glBufferSubData(GL_ARRAY_BUFFER, 0, VerticesSize, Vertices);
		}
	}
	
	/*
	Please use AFTER the clones!
	*/
	void FreeSpace(int Index) {
		int IndexNumberV = Index * 8;

		for (int i = 0; i < 8; ++i) {
			Vertices[IndexNumberV + i] = 0.0f;
		}
		--Squares;
	}

	void ReColor(int index, float R, float G, float B, float A = 1.0f) {
		int CI = index * 4;

		Colors[CI] = R;
		Colors[CI + 1] = G;
		Colors[CI + 2] = B;
		Colors[CI + 3] = A;
	}
	
	bool CheckIfMouseIsOnSquare(GLFWwindow* window, int WindowX, int WindowY, int Index = 0) {
		int IndexNumber = Index * 8;
		static double MouseXPos, MouseYPos;
		glfwGetCursorPos(window, &MouseXPos, &MouseYPos);

		float ndcX = ((float)MouseXPos / (float)WindowX) * 2.0f - 1.0f;
		float ndcY = 1.0f - ((float)MouseYPos / (float)WindowY) * 2.0f;

		float SXPL = Vertices[0 + IndexNumber];
		float SXPR = Vertices[2 + IndexNumber];
		float SYPU = Vertices[1 + IndexNumber];
		float SYPD = Vertices[5 + IndexNumber];
		
		if (ndcX >= SXPL && ndcX <= SXPR && ndcY <= SYPU && ndcY >= SYPD) { return true; }
		else { return false; }
	}
	/*
	 Use before declaring the Vertex Array Objects and the Vertex Buffer Objects
	 and after the Shader Program and the following shaders are declared. If not
	 done so, it will not work! 
	*/
	void RePosition(float offsetX, float offsetY, int index, std::optional<GLuint> VBO = std::nullopt) {
		int IndexNumberV = index * 8;
		

		for (int i = 0; i < 8; ++i) {
			if (i % 2 == 0 || i == 0) {
				Vertices[IndexNumberV + i] += offsetX;
			}
			else {
				Vertices[IndexNumberV + i] += offsetY;
			}
		}
		if (VBO.has_value()) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO.value());
			glBufferSubData(GL_ARRAY_BUFFER, 0, VerticesSize, Vertices);
		}
	}
	/*
	 Use before declaring the Vertex Array Objects and the Vertex Buffer Objects
	 and after the Shader Program and the following shaders are declared. If not
	 done so, it will not work! Don't put in the while loop.
	*/
	void New(float size, float Xpos, float Ypos, std::optional<int> index = std::nullopt) {
		int tempClonedTimes = ClonedTimes;

		if (index.has_value()) { ClonedTimes = index.value(); }

		int vSC = ClonedTimes * 4;
		int iB = ClonedTimes * 6;
		int CI = ClonedTimes * 4;
		if (LoopControl == 0) {
			Indices[iB] = vSC + 0;
			Indices[iB + 1] = vSC + 1;
			Indices[iB + 2] = vSC + 2;
			Indices[iB+3] = vSC + 0;
			Indices[iB+4] = vSC + 2;
			Indices[iB+5] = vSC + 3;

			Colors[CI] = 0.5f;
			Colors[CI + 1] = 0.5f;
			Colors[CI + 2] = 0.5f;
			Colors[CI + 3] = 1.0f;
			int step = 0;
			for (int i = 8 * ClonedTimes; i < ClonedTimes * 8 + 8; ++i) {
				int target = ClonedTimes * 8 + step;
				if (i % 2 == 0) {
					Vertices[i] = (CONSTVertices[step] + Xpos) * size;
				}
				else {
					Vertices[i] = (CONSTVertices[step] + Ypos) * size;
				}
				++step;
			}
			ClonedTimes = tempClonedTimes;
			if (ClonedTimes < MaxCloneLimit) {
				++ClonedTimes;
				++Squares;
			}
		}
	}	

	/*
	For debug use ONLY
	*/
	void PrintVertices() {
		int index = 0;
		for (size_t i = 0; i < VerticesSize; ++i) {
			if (i % 2 == 0) std::cout << "\n";
			if (i % 8 == 0 || i == 0) { ++index; std::cout << "\n"<<index - 1 << ": \n"; }
			std::cout << Vertices[i] << ", ";
		}
	}

	void PrintColors() {
		int index = 0;
		for (int i = 0; i < MaxCloneLimit; ++i) {
			if (i % 4 == 0) { std::cout <<"\n\n"; }
			std::cout << Colors[i] << ", ";
		}
	}
};

struct Triangle {
	float Vertices[8] = {
	 0.5f,  0.5f,
	 0.5f, -0.5f,
	-0.5f, -0.5f,
	-0.5f,  0.5f
	};
	unsigned int indices[6] = {
	0, 1, 3,
	1, 2, 3  
	};
};

