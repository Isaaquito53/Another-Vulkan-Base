#pragma once

#include "Geometry.h"
#include "Camera.h"

// MODULE

class Scene
{
public:
	Scene() {};
	~Scene() {};

	// Getters
	Geometry* GetGeometry() { return &m_geometry; }
	Camera* GetCam() { return &m_cam; }					// Pointer -> ImGui param
	int GetSelectedInstance() { return m_selectedInstance; }

	// Instance Selection
	glm::vec3 RayCasting(glm::mat4 projection, glm::mat4 view, int w, int h);
	int SelectionCheck(glm::vec3 ray);
	void MarkBody(int instanceID);
	std::vector<int> SelectBody(glm::mat4 projection, glm::mat4 view, int w, int h);

	InstanceParams ShowSelectedMenu();

private:
	Geometry m_geometry;
	Camera m_cam;
	float m_selectionDist = 100.0;
	int m_selectedInstance = -1;
};