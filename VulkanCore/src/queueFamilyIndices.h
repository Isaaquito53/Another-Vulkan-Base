#pragma once

#include <optional>

namespace Engine {

	// Definition of the QueueFamilyIndices struct from device.h
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
}