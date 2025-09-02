# TerrainForge--Procedural Terrain Generator Engine  
**By Trevor Belt**

A lightweight, modular terrain engine built in C++ using OpenGL and the Win32 API, featuring real-time procedural generation, elevation-based biome coloring, and smooth camera control. This project was built from scratch, without any external windowing libraries. My goal was to deepen my understanding of graphics programming, demonstrate my ability to architect systems in native C++, and to please my own curiosity.

It generates dynamic 3D terrain using Perlin-based fractal noise via FastNoiseLite, rendered in real time with OpenGL. Terrain is shaded based on elevation thresholds (water, grass, rock, snow), with optional gradient blending. The engine is designed for clarity, extensibility, and education.




## Features

Procedural Terrain Generation
- Uses FastNoiseLite for layered Perlin noise
- Adjustable frequency and octaves for terrain detail
- Elevation exaggeration by using large scaling
Elevation-Based Coloring
- Water, grass, rock, and snow zones based on height
- Easily customizable thresholds, can easily alter to specific limits/colors. 
- Optional gradient shading
Modular Architecture
- Clean separation of terrain logic, rendering, and input using .h / .cpp files.
- Easy to extend or refactor
Native Win32 Windowing
- No GLFW or SDL. Pure Win32 API within the code execution
- Manual OpenGL context setup and input handling
Camera Controls & Rotation
- Arrow keys for panning
- Automatic terrain rotation (toggleable)
- Adjustable zoom via camera Y/Z
- Regenerating terrain to appear 'endless'





## File Structure
TerrainEngine/
- main.cpp           # OpenGL initialization, graphics & window creation, render loop
-terrain.h/.cpp      # Terrain generation class & function using FastNoiseLite
-renderer.h/.cpp     # Using OpenGL to render our terrain on screen and elevation-based coloring
-FastNoiseLite.h     # Noise library (externally downloaded, added to project to be able to use)
-README.md	     # Here now 




## Build

1. Window & OpenGL Setup
- Used raw Win32 API  within Dev-C++ to create a native window (CreateWindow) and handle input (WndProc).
- Enabled depth testing and set up a perspective projection with gluPerspective.

2. Terrain Generation
- Created a Terrain class with a 2D heightMap vector.
- Used FastNoiseLite to generate Perlin-based fractal noise:
- SetNoiseType(Perlin)
- SetFractalType(FBm)
- SetFractalOctaves(5) for layered detail
- SetFrequency(0.1f) for large-scale features

1. Window & OpenGL Setup
- Used raw Win32 API to create a native window (CreateWindow) and handle input (WndProc).
- Manually configured the OpenGL context using PIXELFORMATDESCRIPTOR and wglCreateContext.
- Enabled depth testing and set up a perspective projection with gluPerspective.
2. Terrain Generation
- Created a Terrain class with a 2D heightMap vector.
- Used FastNoiseLite to generate Perlin-based fractal noise:
- SetNoiseType(Perlin)
- SetFractalType(FBm)
- SetFractalOctaves(5) for layered detail
- SetFrequency(0.1f) for large-scale features


3. Rendering
-Built a  class to draw terrain using OpenGL triangles.
	For each quad in the heightmap:
		-Calculated average elevation
		-Assigned color based on elevation thresholds:

if (avg < 10.0f)      glColor3f(0.0f, 0.3f, 0.8f); // Water  
else if (avg < 30.0f) glColor3f(0.2f, 0.7f, 0.2f); // Grass  
else if (avg < 45.0f) glColor3f(0.5f, 0.5f, 0.5f); // Rock  
else                 glColor3f(1.0f, 1.0f, 1.0f); // Snow

4. Camera & Interaction
-Implemented camera offsets (, , ) for navigation.
-Used  to position the camera dynamically.
-Enabled keyboard input (arrow keys) for panning.
-Automatic rotation via for visual flair.





## FastNoiseLite
FastNoiseLite is used for generating layered Perlin noise to simulate natural terrain variation. It’s lightweight, fast, and easy to integrate directly into the project.




## Terrain Generation and Heightmap System
The terrain is built on a 2D heightmap, which stores elevation values for each grid coordinate. Here's how it works:
1. Heightmap Initialization
std::vector<std::vector<float>> heightMap;
heightMap.resize(width, std::vector<float>(height));

Each cell in the grid represents a vertex in 3D space. The heightmap is populated using FastNoiseLite, with optional elevation offset and edge falloff to simulate floating islands.

2. Edge Falloff (Floating Island Effect)
To create the illusion of a floating landmass, I apply a radial falloff based on distance from the center:

float distX = abs(x - width / 2.0f) / (width / 2.0f);
float distY = abs(y - height / 2.0f) / (height / 2.0f);
float edgeFactor = 1.0f - pow(max(distX, distY), 2.0f);
elevation *= edgeFactor;

This smoothly reduces elevation near the edges, forming natural cliffs and voids.

3. Rendering Loop
In Renderer::drawTerrain, I iterate over the heightmap and render each quad as two triangles:


This system demonstrates:
- Procedural synthesis using layered noise
- Efficient data structures (2D heightmap)
- Real-time rendering with OpenGL
- Modular design for terrain, rendering, and input
- Native system-level programming with Win32 API



## Necessary Parameters to Run
In Dev-C++, go to project- project options
			parameters - linkers
	We need to link the project: 

-lopengl32 links the core OpenGL functions like glViewport, glClear, glVertex3f, etc.
-lglu32 links GLU functions like gluPerspective, gluLookAt
-lgdi32 is needed for Windows graphics device interface
-mwindows tells the linker this is a GUI app (not console), so it looks for WinMain




## TODO List
- Trees and Hills. Making the terrain appear nicer. 
- Collision Detection
- More Controls
- Seasons Button/ UI to change colors
- Screenshots, additional media for display.
