#pragma once

#include "Scene.h"
#include "buffer.h"

namespace Engine {
	const int MAX_INSTANCES = 1024;

	class GeometryBuffers {
	public:
		GeometryBuffers() {};
		~GeometryBuffers() {};

		// Getters
		VkBuffer GetVertexBuffer() { return vertexBuffer; }
		VkDeviceMemory GetVertexBufferMemory() { return vertexBufferMemory; }

		VkBuffer GetInstanceBuffer() { return instanceBuffer; }
		VkDeviceMemory GetInstanceBufferMemory() { return instanceBufferMemory; }

		VkBuffer GetIndexBuffer() { return indexBuffer; }
		VkDeviceMemory GetIndexBufferMemory() { return indexBufferMemory; }

		// Setter
		void LinkGeometry(Geometry* sceneGeom) { geom = sceneGeom; }

		// Vertex 
		void createVertexBuffer(VkDevice device, VkCommandPool commandPool, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);
		// Instance
		void createInstanceBuffer(VkDevice device, VkCommandPool commandPool, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);
		void updateInstanceBuffer(VkDevice device, int instanceID, InstanceParams param);
		// Index
		void createIndexBuffer(VkDevice device, VkCommandPool commandPool, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);

	private:
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		VkBuffer instanceBuffer;
		VkDeviceMemory instanceBufferMemory;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

		Geometry* geom;
	};

	class SceneTranslator
	{
	public:
		SceneTranslator() ;
		~SceneTranslator() {};

		// Getters
		Scene* GetScene() { return m_scene; }
		GeometryBuffers* GetGeomBuffs() { return m_geomBuffs; }

		void showImGuiOptions(VkDevice device, unsigned int cmdBufferIndex, float engine_time);

		void updateCam(float time);

		void selectInstance(VkDevice device, UniformBufferObject ubo, int w, int h);
		void showInstanceMenu(VkDevice device, unsigned int cmdBufferIndex);

	private:
		void Tree(VkDevice device, int maxDepth);
		void RecursiveTree(std::vector<Instance>& parts, int maxDepth, int depth, Instance parent);

		Scene* m_scene;
		GeometryBuffers* m_geomBuffs;

		float m_time = 0.0;
		int m_maxDepth = 0;
		float m_angL = 25;
		float m_angR = -35;
	};
}