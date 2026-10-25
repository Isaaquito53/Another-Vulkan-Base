#include "Scene.h"

#include <iostream>

// MODULE

glm::vec3 Scene::RayCasting(glm::mat4 projection, glm::mat4 view, int w, int h)
{
	// 0. 2D Viewport coords
	float xpos = ImGui::GetIO().MousePos.x;
	float ypos = ImGui::GetIO().MousePos.y;
	// 1. 3D Normalised Device coords
	float x = (2.0f * xpos) / w - 1.0f;
	float y = 1.0f - (2.0f * ypos) / h;
	glm::vec3 ray_nds = glm::vec3(x, y, 1.0f);
	// 2. 4D Homogeneous Clip coords
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
	// 3. 4D Eye (Camera) coords
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
	// 4. 4D World coords
	glm::vec3 ray_world = (glm::inverse(view) * ray_eye);
	ray_world = glm::normalize(ray_world);

	return ray_world;
}

int Scene::SelectionCheck(glm::vec3 ray)
{
	float minDist = m_selectionDist, distBodyCam = 0.0f;
	int minDistBody = -1;
	// For each body (From 1 to N, because 0 is the skybox)
	for (int i = 0; i < m_geometry.GetInstances().size(); i++)
	{
		Instance inst = m_geometry.GetInstances()[i];
		distBodyCam = glm::distance(glm::vec3(inst.translation), m_cam.m_camPos);
		if (distBodyCam < m_selectionDist)
		{
			// Create vector cam<->body
			glm::vec3 camToBody = glm::vec3(inst.translation) - m_cam.m_camPos;
			// Ray<->camToBody Dot product
			float dp = glm::dot(camToBody, ray);
			float l = glm::length(ray);
			// Compute scalar projection (length of the camToBody projected on ray)
			float s = dp / l;
			// Compute vector camPos+ray*s (world coords of the projected point)
			glm::vec3 projectedBodyPosition = m_cam.m_camPos + ray * s;
			// Compute the distance between the center of the body and its projection
			float dist = glm::distance(glm::vec3(inst.translation), projectedBodyPosition);

			/*
			std::cout << ">>> Selection:" << std::endl;
			std::cout << inst.translation.x << "," << inst.translation.y << "," << inst.translation.z << std::endl;
			std::cout << projectedBodyPosition.x << "," << projectedBodyPosition.y << "," << projectedBodyPosition.z << std::endl;
			*/
			if (distBodyCam < minDist) {
				if (projectedBodyPosition.x <= inst.translation.x + inst.scale.x && projectedBodyPosition.x >= inst.translation.x - inst.scale.x)
				{
					if (projectedBodyPosition.y <= inst.translation.y + inst.scale.y && projectedBodyPosition.y >= inst.translation.y - inst.scale.y)
					{
						if (projectedBodyPosition.z <= inst.translation.z + inst.scale.z && projectedBodyPosition.z >= inst.translation.z - inst.scale.z)
						{
							//std::cout << i << " selected" << std::endl;
							minDist = distBodyCam;
							minDistBody = i;
						}
					}
				}
			}
			/*
			if (dist < (inst.scale.x + inst.scale.y + inst.scale.z) / 3 && distBodyCam < minDist)
			{
				minDist = distBodyCam;
				minDistBody = i;
			}*/
		}
	}
	return minDistBody;
}


void Scene::MarkBody(int instanceID)
{
	// If (valid current_instance) then select it
	if (instanceID != -1)
		m_geometry.GetInstance(instanceID).color = glm::vec3(0.3);
	// If (valid previous_instance) then  unselect it
	if (m_selectedInstance != -1)
		m_geometry.GetInstance(m_selectedInstance).color = glm::vec3(1.0);
	m_selectedInstance = instanceID;
}

std::vector<int> Scene::SelectBody(glm::mat4 projection, glm::mat4 view, int w, int h)
{
	// Returns the previous&current InstanceID to change its color from gpu
	int prevID = -1;
	int currID = -1;

	if (ImGui::IsMouseReleased(1))
	{
		glm::mat4 fixedProj = projection;
		fixedProj[1][1] *= -1;
		glm::vec3 ray = RayCasting(fixedProj, view, w, h);
		int instanceID = SelectionCheck(ray);
		//std::cout << instanceID << std::endl;
		
		// If instanceID != m_selectedInstance
		// then we need to change something
		if (instanceID != m_selectedInstance) {
			prevID = m_selectedInstance;
			currID = instanceID;
			MarkBody(instanceID);
		}
	}

	return { prevID, currID };
}

InstanceParams Scene::ShowSelectedMenu() {
	InstanceParams paramChanged = NA;
	// Show sliders,text,... to update values of the selected instance

	ImGui::Text("Instance ID: %d", m_selectedInstance);

	ImGui::Separator();

	// Translation:
	glm::vec3 translation = m_geometry.GetInstance(m_selectedInstance).translation;
	ImGui::SliderFloat3("Translation", &m_geometry.GetInstance(m_selectedInstance).translation[0], -10.0, 10.0);
	if (translation != m_geometry.GetInstance(m_selectedInstance).translation)
		paramChanged = TRANSLATION;

	// Scale:
	glm::vec3 scale = m_geometry.GetInstance(m_selectedInstance).scale;
	ImGui::SliderFloat3("Scale", &m_geometry.GetInstance(m_selectedInstance).scale[0], -10.0, 10.0);
	if (scale != m_geometry.GetInstance(m_selectedInstance).scale)
		paramChanged = SCALE;

	// Rotation:
	glm::vec3 rotation = m_geometry.GetInstance(m_selectedInstance).rotation;
	ImGui::SliderFloat3("Rotation", &m_geometry.GetInstance(m_selectedInstance).rotation[0], -10.0, 10.0);
	if (rotation != m_geometry.GetInstance(m_selectedInstance).rotation)
		paramChanged = ROTATION;

	return paramChanged;
}