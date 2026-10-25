#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <array>

// TURN INTO MODULE

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    // Specifies the number of bytes between data entries 
    // and whether to move to the next data entry after each vertex or after each instance
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    // Describes how to extract a vertex attribute from a chunk of vertex data originating from a binding description
    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

enum InstanceParams
{
    NA = 0, COLOR = 1, TRANSLATION = 2, SCALE = 3, ROTATION = 4
};

struct Instance {
    glm::vec3 color;
    glm::vec3 translation;
    glm::vec3 scale;
    glm::vec3 rotation;

    // Specifies the number of bytes between data entries 
    // and whether to move to the next data entry after each vertex or after each instance
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 1;
        bindingDescription.stride = sizeof(Instance);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

        return bindingDescription;
    }

    // Describes how to extract a vertex attribute from a chunk of vertex data originating from a binding description
    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

        attributeDescriptions[0].binding = 1;
        attributeDescriptions[0].location = 3;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Instance, color);

        attributeDescriptions[1].binding = 1;
        attributeDescriptions[1].location = 4;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Instance, translation);

        attributeDescriptions[2].binding = 1;
        attributeDescriptions[2].location = 5;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Instance, scale);

        attributeDescriptions[3].binding = 1;
        attributeDescriptions[3].location = 6;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Instance, rotation);

        return attributeDescriptions;
    }

    bool operator==(const Instance& other) const {
        return color == other.color && translation == other.translation 
            && scale == other.scale && rotation == other.rotation;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

/*
// Fractal level
const unsigned int LEVEL = 1;

// Vertex data
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
};

// Indices
std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
};

void createIndices()
{
    for (uint16_t i = 0; i < pow(3, LEVEL+1); i++)
    {
        indices.push_back(i);
    }
}

void createVertices(std::vector<Vertex>& vert, Vertex botLeft, Vertex topMid, Vertex botRight, const unsigned int level)
{
    if (LEVEL == 0)
    {
        botLeft.pos.x -= 0.5f;
        botLeft.pos.y -= 0.5f;
        topMid.pos.x -= 0.5f;
        topMid.pos.y -= 0.5f;
        botRight.pos.x -= 0.5f;
        botRight.pos.y -= 0.5f;

        vert.push_back(botLeft);
        vert.push_back(topMid);
        vert.push_back(botRight);
        return;
    }

    Vertex botMid = { {topMid.pos.x, botLeft.pos.y},
                {botLeft.color.r / 2, botRight.color.g / 2, botRight.color.b / 2 } };
    Vertex midLeft = { {(topMid.pos.x - botLeft.pos.x) / 2 + botLeft.pos.x, (botLeft.pos.y - topMid.pos.y) / 2 + topMid.pos.y},
        {botLeft.color.r / 2, topMid.color.g / 2, topMid.color.b / 2 } };
    Vertex midRight = { {(botRight.pos.x - topMid.pos.x) / 2 + topMid.pos.x, (botRight.pos.y - topMid.pos.y) / 2 + topMid.pos.y},
        {topMid.color.r / 2, topMid.color.g / 2, botRight.color.b / 2 } };

    if (level == LEVEL)
    {
        botLeft.pos.x -= 0.5f;
        botLeft.pos.y -= 0.5f;
        botMid.pos.x -= 0.5f;
        botMid.pos.y -= 0.5f;
        midLeft.pos.x -= 0.5f;
        midLeft.pos.y -= 0.5f;
        botRight.pos.x -= 0.5f;
        botRight.pos.y -= 0.5f;
        midRight.pos.x -= 0.5f;
        midRight.pos.y -= 0.5f;
        topMid.pos.x -= 0.5f;
        topMid.pos.y -= 0.5f;

        vert.push_back(botLeft);
        vert.push_back(midLeft);
        vert.push_back(botMid);

        vert.push_back(midLeft);
        vert.push_back(topMid);
        vert.push_back(midRight);

        vert.push_back(botMid);
        vert.push_back(midRight);
        vert.push_back(botRight);
    }
    else
    {
        createVertices(vert, botLeft, midLeft, botMid, level + 1);
        createVertices(vert, midLeft, topMid, midRight, level + 1);
        createVertices(vert, botMid, midRight, botRight, level + 1);
    }
}
*/