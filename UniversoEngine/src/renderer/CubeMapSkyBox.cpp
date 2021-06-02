#include "CubeMapSkyBox.h"

namespace engine {
	CubeMapSkyBox::CubeMapSkyBox(std::vector<std::string> facesFilesPaths) {
		this->cubeMap = std::make_unique<CubeMap>(facesFilesPaths);

		vertexArray.addVertexBuffer(vertexBuffer);

		vertexBuffer.addAttributePointer(AttriuteType::Vec3, 0);

		glm::vec3 skyboxVertices[36];

		Mesh cubeMesh{ "UniversoEngine/resources/meshes/cube.obj", false };
		const std::vector<Vertex>& cubeMeshVertices = cubeMesh.getVertices();

		for (int i = 0; i < cubeMeshVertices.size(); i++)
			skyboxVertices[i] = cubeMeshVertices[i].position;

		vertexBuffer.pushData(skyboxVertices, sizeof(skyboxVertices));

		shaderProgram.attachShader(vertexShader);
		shaderProgram.attachShader(fragShader);
	}
}