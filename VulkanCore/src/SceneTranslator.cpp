#include "SceneTranslator.h"

namespace Engine {
	// ------ GEOMETRYBUFFERS ------
	// Vertex 
	void GeometryBuffers::createVertexBuffer(VkDevice device, VkCommandPool commandPool, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue) {
		/*createVertices(vertices, {{0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
			{ {0.5f, 0.0f}, {0.0f, 1.0f, 0.0f} },
			{ {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} }, 1);
			*/

		geom->SetVertices({
			{{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			});
		// TODO: try "sizeof(Vertex)"
		VkDeviceSize bufferSize = sizeof(geom->GetVertices()[0]) * geom->GetVertices().size();

		// Use a local stagingBuffer and copy it to the vertexBuffer
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);

		// Filling the stagingBuffer
		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, geom->GetVertices().data(), (size_t)bufferSize);
		vkUnmapMemory(device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory, device, physicalDevice);

		// Copy the local stagingBuffer to the vertexBuffer
		copyBuffer(commandPool, stagingBuffer, vertexBuffer, bufferSize, device, graphicsQueue);

		// Free the local stagingBuffer
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
	}

	// Instance
	void GeometryBuffers::createInstanceBuffer(VkDevice device, VkCommandPool commandPool, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue) {
		geom->SetInstances({
			{{1.0, 1.0, 1.0}, {4.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, glm::radians(270.0f), 0.0f}},
			{{1.0, 1.0, 1.0}, {0.0f, 4.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {glm::radians(270.0f), 0.0f, 0.0f}},
			{{1.0, 1.0, 1.0}, {0.0f, 0.0f, 4.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, glm::radians(270.0f)}},
			{{1.0, 1.0, 1.0}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}}
			});
		// TODO: try "sizeof(Instance)"
		VkDeviceSize bufferSize = sizeof(geom->GetInstances()[0]) * MAX_INSTANCES;

		// Use a local stagingBuffer and copy it to the vertexBuffer
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);

		// Filling the stagingBuffer
		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, geom->GetInstances().data(), (size_t)bufferSize);
		vkUnmapMemory(device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, instanceBuffer, instanceBufferMemory, device, physicalDevice);

		// Copy the local stagingBuffer to the vertexBuffer
		copyBuffer(commandPool, stagingBuffer, instanceBuffer, bufferSize, device, graphicsQueue);

		// Free the local stagingBuffer
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
	}
	void GeometryBuffers::updateInstanceBuffer(VkDevice device, int instanceID, InstanceParams param) {
		// Map the buffer
		void* data;
		vkMapMemory(device, instanceBufferMemory, 0, sizeof(Instance), 0, &data);
		// Copy data to the buffer (change data)
		Instance* gpuData = static_cast<Instance*>(data);
		// Change in GPU
		if (instanceID != -1) {
			if (param != NA) {
				switch (param)
				{
				case 1:
					gpuData[instanceID].color = geom->GetInstances()[instanceID].color; break;
				case 2:
					gpuData[instanceID].translation = geom->GetInstances()[instanceID].translation; break;
				case 3:
					gpuData[instanceID].scale = geom->GetInstances()[instanceID].scale; break;
				case 4:
					gpuData[instanceID].rotation = geom->GetInstances()[instanceID].rotation; break;
				}
			}
			else
				gpuData[instanceID] = geom->GetInstances()[instanceID];
		}
		else
			for (int i = 0; i < geom->GetInstances().size(); i++)
				gpuData[i] = geom->GetInstances()[i];

		// Unmap UniformBufferMemory
		vkUnmapMemory(device, instanceBufferMemory);
	}

	// Index
	void GeometryBuffers::createIndexBuffer(VkDevice device, VkCommandPool commandPool, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue) {
		//createIndices();

		geom->SetIndices({
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			1, 5, 2, 5, 4, 2,
			6, 0, 7, 0, 3, 7,
			2, 4, 3, 4, 7, 3,
			6, 5, 0, 5, 1, 0
			});

		VkDeviceSize bufferSize = sizeof(geom->GetIndices()[0]) * geom->GetIndices().size();

		// Use a local stagingBuffer and copy it to the indexBuffer
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);

		// Filling the stagingBuffer
		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, geom->GetIndices().data(), (size_t)bufferSize);
		vkUnmapMemory(device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory, device, physicalDevice);

		// Copy the local stagingBuffer to the indexBuffer
		copyBuffer(commandPool, stagingBuffer, indexBuffer, bufferSize, device, graphicsQueue);

		// Free the local stagingBuffer
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
	}

	// ------ SCENETRANSLATOR ------
	SceneTranslator::SceneTranslator() {
		// We initially link our main scene geometry (a cube) to the main GeometryBuffers
		if (m_geomBuffs == nullptr) m_geomBuffs = new GeometryBuffers(); 
		if (m_scene == nullptr) m_scene = new Scene();
		m_geomBuffs->LinkGeometry(m_scene->GetGeometry());
	}

	void SceneTranslator::showImGuiOptions(VkDevice device, unsigned int cmdBufferIndex, float engine_time)
	{
		ImGui::Begin("Menu");
		if (ImGui::Button("- Instance"))
			m_scene->GetGeometry()->PopBackInstances();
		if (ImGui::Button("+ Instance")) {
			glm::vec3 newPos = { float(rand() % 20) - 10,float(rand() % 20) - 10,float(rand() % 20) - 10 };
			Instance newInst = { {1.0,1.0,1.0}, newPos, {1.0,1.0,1.0}, {0.0,0.0,0.0} };
			m_scene->GetGeometry()->PushBackInstances(newInst);
			m_geomBuffs->updateInstanceBuffer(device, m_scene->GetGeometry()->GetInstances().size() - 1, NA);
		}
		if (ImGui::Button("Clear Instances"))
			m_scene->GetGeometry()->ClearInstances();
		if (ImGui::Button("Fill Instances")) {
			std::vector<Instance> insts;
			for (int i = 0; i < MAX_INSTANCES; i++) {
				glm::vec3 newPos = { float(rand() % 20) - 10,float(rand() % 20) - 10,float(rand() % 20) - 10 };
				insts.push_back({ {1.0,1.0,1.0}, newPos, {1.0,1.0,1.0}, {0.0,0.0,0.0} });
			}
			m_scene->GetGeometry()->FillInstances(insts);
			m_geomBuffs->updateInstanceBuffer(device, -1, NA);
		}


		ImGui::Text("NumInstances: %.d", m_scene->GetGeometry()->GetInstances().size());

		ImGui::Separator();
		ImGui::SliderInt("Depth", &m_maxDepth, 0, 10);
		ImGui::SliderFloat("AngL", &m_angL, 0, 90);
		ImGui::SliderFloat("AngR", &m_angR, -90, 0);

		if (ImGui::Button("Recursive Tree")) {
			Tree(device, m_maxDepth);
		}
		ImGui::Separator();

		ImGui::Checkbox("SphericalView", &m_scene->GetCam()->m_spherical);
		ImGui::Text("Cam position: x: %.3f, y: %.3f, z: %.3f", m_scene->GetCam()->m_camPos.x, m_scene->GetCam()->m_camPos.y, m_scene->GetCam()->m_camPos.z);
		ImGui::Text("Cam direction: x: %.3f, y: %.3f, z: %.3f", m_scene->GetCam()->m_camDirection.x, m_scene->GetCam()->m_camDirection.y, m_scene->GetCam()->m_camDirection.z);

		ImGui::Separator();

		ImGui::Text("FrameRate: %.3f", engine_time - m_time);
		m_time = engine_time;

		ImGui::End();

		//ImGui::ShowDemoWindow();
		/*
		ImGui::Begin("Try different songs");

		if (ImGui::Button("Load song"))
		{
			m_loadShaders = true;
		}

		ImGui::InputText("Loaded song", m_fragmentLoaded, 50);

		if (ImGui::Button("Save song"))
			m_saveShaders = true;

		ImGui::InputText("Saved song", m_fragmentSaved, 50);

		ImGui::Separator();

		std::string sTime = std::to_string(glfwGetTime());
		const char* cTime = sTime.c_str();
		ImGui::Text(cTime);

		ImGui::End();
		*/
		if (ImGui::IsKeyReleased(ImGuiKey_R))
			glfwSetTime(0.0f);
	}
	
	void SceneTranslator::Tree(VkDevice device, int maxDepth) {
		std::vector<Instance> parts;

		RecursiveTree(parts, maxDepth, 0, Instance());

		m_scene->GetGeometry()->FillInstances(parts);
		m_geomBuffs->updateInstanceBuffer(device, -1, NA);
	}

	void SceneTranslator::RecursiveTree(std::vector<Instance>& parts, int maxDepth, int depth, Instance parent) {
		if (maxDepth == depth)
			return;
		else {
			float iSize = maxDepth - depth;
			glm::vec2 posL = glm::vec2(0.0);
			glm::vec2 posR = glm::vec2(0.0);
			float angL = 0.0;
			float angR = 0.0;
			float prevAngL = 0.0;
			float prevAngR = 0.0;

			if (depth != 0) {
				angL = glm::radians(m_angL);
				angR = glm::radians(m_angR);
				posL.x = parent.translation.x;
				posL.y = parent.translation.y;
				posR.x = parent.translation.x;
				posR.y = parent.translation.y;

				prevAngL = parent.rotation.z;
				prevAngR = parent.rotation.z;
				posL = posL + glm::vec2(sin(prevAngL), cos(prevAngL)) * parent.scale.y;
				posL = posL + glm::vec2(sin(angL + prevAngL), cos(angL + prevAngL)) * (iSize / maxDepth * 5);

				posR = posR + glm::vec2(sin(prevAngR), cos(prevAngR)) * parent.scale.y;
				posR = posR + glm::vec2(sin(angR + prevAngR), cos(angR + prevAngR)) * (iSize / maxDepth * 5);

				Instance instR = { {1.0,1.0,1.0}, {posR.x,posR.y,0.0}, {iSize / maxDepth,iSize / maxDepth * 5,iSize / maxDepth}, {0.0,0.0,angR + prevAngR} };
				parts.push_back(instR);
				RecursiveTree(parts, maxDepth, depth + 1, instR);
			}
			Instance instL = { {1.0,1.0,1.0}, {posL.x,posL.y,0.0}, {iSize / maxDepth,iSize / maxDepth * 5,iSize / maxDepth}, {0.0,0.0,angL + prevAngL} };
			parts.push_back(instL);
			return RecursiveTree(parts, maxDepth, depth + 1, instL);
		}
	}

	void SceneTranslator::updateCam(float time) {
		m_scene->GetCam()->UpdateCam(time);
	}

	void SceneTranslator::selectInstance(VkDevice device, UniformBufferObject ubo, int w, int h) {
		std::vector<int> instanceID = m_scene->SelectBody(ubo.proj, ubo.view, w, h);
		if (instanceID[0] != -1)
			m_geomBuffs->updateInstanceBuffer(device, instanceID[0], COLOR);
		if (instanceID[1] != -1)
			m_geomBuffs->updateInstanceBuffer(device, instanceID[1], COLOR);
	}

	void SceneTranslator::showInstanceMenu(VkDevice device, unsigned int cmdBufferIndex) {
		if (m_scene->GetSelectedInstance() != -1) {
			ImGui::Begin("Instance Editor");
			InstanceParams paramChanged = m_scene->ShowSelectedMenu();
			m_geomBuffs->updateInstanceBuffer(device, m_scene->GetSelectedInstance(), paramChanged);
			ImGui::End();
		}
	}
}