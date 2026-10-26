# Another Vulkan Base
This repo contains **Another Vulkan Base** made by me using the [Vulkan tutorial](https://vulkan-tutorial.com/).

---

## Content:
This Vulkan Base is divided into two different folders:
- VulkanCore: the actual Vulkan Base. This is all the code used to create a GLFW and Vulkan context (it is the basic part of a Vulkan project that allows you to use the Vulkan API to show thing on your screen).
To look at it from another perspective, the VulkanCore contains a lot of code that you (assuming you are like me, a newbie) will rearly need to change. The most important file is *pipeline.cpp*,
where you can find everything related to CPU-GPU communications and buffer usage.
- NoVulkan: everything that doesn't need a Vulkan context to be understood. For this repo I've implemented an example where you can move the camera, add or remove the last instance of a 3D geometry or generate a fractal canopy with this geometry.
In short, here you can put any data structure that represents things related to the user, such as a camera or a scene with its geometries.

You can see that there are some things related to the user in *pipeline.cpp*. I decided to put it here because they are also related to GPU stuff (for example, the main ImGui menu that handles some GPU buffer updates).

### Example using this Vulkan Base (10 recursive iterations of 3D fractal canopy, 1024 instances):
<img width="1920" height="1017" alt="Vulkan app 23_10_2026 18_03_05" src="https://github.com/user-attachments/assets/38be0f25-84ba-45b0-b3e8-ec12103f1806" />

---

Made by Isaaquito53 aka izzqy
