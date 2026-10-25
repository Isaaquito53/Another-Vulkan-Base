#pragma once

#include "vertex.h"

class Geometry
{
public:
	Geometry() {};
	~Geometry() {};

	// Getters
	std::vector<Vertex> GetVertices() { return m_vertices; }
	std::vector<uint32_t> GetIndices() { return m_indices; }
	std::vector<Instance> GetInstances() { return m_instances; }
	Instance& GetInstance(int id) { return m_instances[id]; }
	int* GetNumInstances() { return &m_numInstances; }	// Pointer -> ImGui param

	// Setters
	void SetVertices(std::vector<Vertex> vertices) { m_vertices = vertices; }
	void SetIndices(std::vector<uint32_t> indices) { m_indices = indices; }
	void SetInstances(std::vector<Instance> instances) {
		m_instances = instances;
		m_numInstances = instances.size();
	}

	// Instance Managing
	void PopBackInstances() { if (!m_instances.empty()) m_instances.pop_back(); }
	void PushBackInstances(Instance inst) { m_instances.push_back(inst); }
	void ClearInstances() { m_instances.clear(); }
	void FillInstances(std::vector<Instance> instances) { m_instances = instances; }

private:
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
	std::vector<Instance> m_instances;
	int m_numInstances = 0;
};