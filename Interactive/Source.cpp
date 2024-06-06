// 3D Graphics and Animation - Main Template
// This uses Visual Studio Code - https://code.visualstudio.com/docs/cpp/introvideos-cpp
// Two versions available -  Win64 and Apple MacOS - please see notes
// Last changed September 2023

//#pragma comment(linker, "/NODEFAULTLIB:MSVCRT")

//#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h> // Extend OpenGL Specfication to version 4.5 for WIN64 and versions 4.1 for Apple (note: two different files).

#include <imgui/imgui.h>			  // Load GUI library - Dear ImGui - https://github.com/ocornut/imgui
#include <imgui/imgui_impl_glfw.h>	  // Platform ImGui using GLFW
#include <imgui/imgui_impl_opengl3.h> // Platform new OpenGL - aka better than 3.3 core version.

#include <GLFW/glfw3.h> // Add GLFW - library to launch a window and callback mouse and keyboard - https://www.glfw.org

#define GLM_ENABLE_EXPERIMENTAL	 // Enable all functions include non LTS
#include <glm/glm.hpp>			 // Add helper maths library - GLM 0.9.9.9 - https://github.com/g-truc/glm - for example variables vec3, mat and operators.
#include <glm/gtx/transform.hpp> // Help us with transforms
#include <glm/gtx/intersect.hpp>
using namespace glm;

#include <tinygltf/tiny_gltf.h> // Model loading library - tiny gltf - https://github.com/syoyo/tinygltf
// #include "src/stb_image.hpp" // Add library to load images for textures

//#include "src/Mesh.hpp" // Simplest mesh holder and OBJ loader - can update more - from https://github.com/BennyQBD/ModernOpenGLTutorial

#include "src/Pipeline.hpp"		// Setup pipeline and load shaders.
#include "src/Content.hpp"		// Setup content loader and drawing functions - https://github.com/KhronosGroup/glTF - https://github.com/syoyo/tinygltf 
#include "src/Debugger.hpp"		// Setup debugger functions.

struct frame
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	frame() : position(glm::vec3()), rotation(glm::vec3()), scale(glm::vec3()) {}
};

// start animation flags
bool playSwordAnimation = false;
bool playBombAnimation = false;
bool playChestAnimation = false;
bool playKeyAnimation = false;
bool playCannonAnimation = false;

// MODEL INDEXES ARE HARDCODED - UPDATE AS NECESSARY
const int SWORD_MODEL_INDEX = 1;
const int BOMB_MODEL_INDEX = 2;
const int CHEST_MODEL_INDEX = 3;
const int CHEST_LID_MODEL_INDEX = 4;
const int CANNON_MODEL_INDEX = 5;
const int CANNON_BALL_INDEX = 6;
const int KEY_MODEL_INDEX = 7;

struct model
{
	std::string file;
	std::string name;
	std::string description;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	int frameIndex;
	bool animate;
	std::vector<frame> animation;
	//! Animation is NULL by default
	model() : file(""), name(""), description(""), position(glm::vec3()), rotation(glm::vec3()), scale(glm::vec3()), frameIndex(0), animate(true) {}
};

// Main fuctions
void startup();
void update();
void render();
void ui();
void endProgram();

// HELPER FUNCTIONS OPENGL
void hintsGLFW();
//string readShader(string name);
//void checkErrorShader(GLuint shader);
inline void errorCallbackGLFW(int error, const char *description){cout << "Error GLFW: " << description << "\n";};
void debugGL();

void APIENTRY openGLDebugCallback(GLenum source,
								  GLenum type,
								  GLuint id,
								  GLenum severity,
								  GLsizei length,
								  const GLchar *message,
								  const GLvoid *userParam);
GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void genRotateAnim(model& model);
void genChestAnimation(model& chestLidModel);
void genCannonBackAnimation(model& cannonModel);
void genBombFiringAnimation(model& bombModel);
void genKeyAnimation(model& keyModel);

void animate(model& model);

// Setup all the message loop callbacks - do this before Dear ImGui
// Callback functions for the window and interface devices
void onResizeCallback(GLFWwindow *window, int w, int h);
void onKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void onMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void onMouseMoveCallback(GLFWwindow *window, double x, double y);
void onMouseWheelCallback(GLFWwindow *window, double xoffset, double yoffset);

// VARIABLES
GLFWwindow *window; 								// Keep track of the window
auto windowWidth = 800;								// Window width					
auto windowHeight =800;								// Window height
auto running(true);							  		// Are we still running our main loop
mat4 projMatrix;							 		// Our Projection Matrix
vec3 cameraPosition = vec3(0.0f, 7.5f, 15.0f);		// Where is our camera
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);			// Camera front vector
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);				// Camera up vector
auto aspect = (float)windowWidth / (float)windowHeight;	// Window aspect ration
auto fovy = 45.0f;									// Field of view (y axis)
bool keyStatus[1024];								// Track key strokes
auto currentTime = 0.0f;							// Framerate
auto deltaTime = 0.0f;								// time passed
auto lastTime = 0.0f;								// Used to calculate Frame rate

//camera variables
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 0.0f;
float lastY = 0.0f;
bool firstCall = true;

Pipeline pipeline;									// Add one pipeline plus some shaders.
vector<Content> contentList;						// content loaders (one for each file)
Debugger debugger;									// Add one debugger to use for callbacks ( Win64 - openGLDebugCallback() ) or manual calls ( Apple - glCheckError() ) 

vector<model> models;

float cameraSpeed = 0.05f;
bool crosshair = true;								// Crosshair enabled, disable for mouse selection mode
vec3 normCursorPosition;							// Normalised Crosshair position
vec3 cursorRay;										// Ray casted from crosshair
int selectedObject = -1;							// Target name
float sphereRad = 1.0f;

bool helpMenu = true;

int main()
{
	cout << endl << "===" << endl << "3D Graphics and Animation - Running..." << endl;

	if (!glfwInit()) // Check if GLFW is working, if not then stop all
	{
		cout << "Could not initialise GLFW...";
		return -1;
	} 

	glfwSetErrorCallback(errorCallbackGLFW); // Setup a function callback to catch and display all GLFW errors.
	hintsGLFW();							 // Setup glfw with various hints.

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor()); // Grab reference to monitor
	// windowWidth = mode->width; windowHeight = mode->height; //fullscreen
	// window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), glfwGetPrimaryMonitor(), NULL); // fullscreen

	// Create our Window
	const auto windowTitle = "My 3D Graphics and Animation OpenGL Application"s;
	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
	if (!window) // Test if Window or OpenGL context creation failed
	{
		cout << "Could not initialise GLFW...";
		glfwTerminate();
		return -1;
	} 

	glfwSetWindowPos(window, 10, 10); // Place it in top corner for easy debugging.
	glfwMakeContextCurrent(window);	  // making the OpenGL context current

	// GLAD: Load OpenGL function pointers - aka update specs to newest versions - plus test for errors.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD...";
		glfwMakeContextCurrent(NULL);
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, onResizeCallback);	   // Set callback for resize
	glfwSetKeyCallback(window, onKeyCallback);				   // Set Callback for keys
	glfwSetMouseButtonCallback(window, onMouseButtonCallback); // Set callback for mouse click
	glfwSetCursorPosCallback(window, onMouseMoveCallback);	   // Set callback for mouse move
	glfwSetScrollCallback(window, onMouseWheelCallback);	   // Set callback for mouse wheel.
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);	// Set mouse cursor Fullscreen
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// Set mouse cursor FPS fullscreen.

	// Setup Dear ImGui and add context	-	https://blog.conan.io/2019/06/26/An-introduction-to-the-Dear-ImGui-library.html
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); //(void)io;
								  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsLight(); // ImGui::StyleColorsDark(); 		// Setup Dear ImGui style

	// Setup Platform/Renderer ImGui backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const auto glsl_version = "#version 410";
	ImGui_ImplOpenGL3_Init(glsl_version);

	#if defined(__WIN64__)
		debugGL(); // Setup callback to catch openGL errors.	V4.2 or newer
	#elif(__APPLE__)
		glCheckError(); // Old way of checking for errors. Newest not implemented by Apple. Manually call function anywhere in code to check for errors.
	#endif

	glfwSwapInterval(1);			 // Ony render when synced (V SYNC) - https://www.tomsguide.com/features/what-is-vsync-and-should-you-turn-it-on-or-off
	glfwWindowHint(GLFW_SAMPLES, 2); // Multisampling - covered in lectures - https://www.khronos.org/opengl/wiki/Multisampling

	startup(); // Setup all necessary information for startup (aka. load texture, shaders, models, etc).

	cout << endl << "Starting main loop and rendering..." << endl;	

	do{											 // run until the window is closed
		auto currentTime = (float)glfwGetTime(); // retrieve timelapse
		deltaTime = currentTime - lastTime;		 // Calculate delta time
		lastTime = currentTime;					 // Save for next frame calculations.

		glfwPollEvents(); 						// poll callbacks

		update(); 								// update (physics, animation, structures, etc)
		render(); 								// call render function.
		ui();									// call function to render ui.

		#if defined(__APPLE__)
			glCheckError();				// Manually checking for errors for MacOS, Windows has a callback.
		#endif

		glfwSwapBuffers(window); 		// swap buffers (avoid flickering and tearing)

		running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE); // exit if escape key pressed
		running &= (glfwWindowShouldClose(window) != GL_TRUE);
	} while (running);

	endProgram(); // Close and clean everything up...

	// cout << "\nPress any key to continue...\n";
	// cin.ignore(); cin.get(); // delay closing console to read debugging errors.

	return 0;
}


void hintsGLFW(){
	
	auto majorVersion = 3; auto minorVersion = 3; // define OpenGL version - at least 3.3 for bare basic NEW OpenGL

	#if defined(__WIN64__)	
		majorVersion = 4; minorVersion = 5;					// Recommended for Windows 4.5, but latest is 4.6 (not all drivers support 4.6 version).
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // Create context in debug mode - for debug message callback
	#elif(__APPLE__)
		majorVersion = 4; minorVersion = 1; 				// Max support for OpenGL in MacOS
	#endif

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 	// https://www.glfw.org/docs/3.3/window_guide.html
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
}

void endProgram()
{
	// Clean ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwMakeContextCurrent(NULL); 	// destroys window handler
	glfwTerminate();				// destroys all windows and releases resources.
}

void startup()
{
	// Output some debugging information
	cout << "VENDOR: " << (char *)glGetString(GL_VENDOR) << endl;
	cout << "VERSION: " << (char *)glGetString(GL_VERSION) << endl;
	cout << "RENDERER: " << (char *)glGetString(GL_RENDERER) << endl;	

	cout << endl << "Loading content..." << endl;


	// ! MODELS LOAD HERE
	// ! Format for model loading:
	/*
	struct model MODEL_NAME;
	model.file MODEL_FILE_PATH (string);
	model.name MODEL_NAME (string);
	model.description MODEL_DESCRIPTION (string);
	model.position MODEL_POSITION (glm::vec3);
	model.rotation MODEL_ROTATION (glm::vec3);
	models.push(MODEL_NAME);
	*/

	// // struct model dog;
	// // dog.file = "assets/dog.gltf";
	// // dog.name = "dog";
	// // dog.description = "A dog";
	// // dog.position = glm::vec3(0.0f, 0.0f, 0.0f);
	// // dog.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	// // dog.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	// // models.push_back(dog);

	struct model floor;
	floor.file = "assets/floor.gltf";
	floor.name = "Floor";
	floor.description = "Scene floor, where all objects will be placed";
	floor.position = glm::vec3(0.0f, 0.0f, 0.0f);
	floor.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	floor.scale = glm::vec3(10.0f, 10.0f, 10.0f);
	models.push_back(floor);

	struct model sword;
	sword.file = "assets/pirate-sword-blend-cw.gltf";
	sword.name = "Sword";
	sword.description = "A pirate sword!";
	sword.position = glm::vec3(-7.0f, 1.5f, 0.0f);
	sword.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	sword.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	genRotateAnim(sword);
	models.push_back(sword);

	struct model bomb;
	bomb.file = "assets/bomb.gltf";
	bomb.name = "Bomb";
	bomb.description = "Bomb model created by Kyle, polygon count in fuse reduced for performance.";
	bomb.position = glm::vec3(-3.5f, 2.25f, 0.0f);
	bomb.rotation = glm::vec3(3.8f, 0.0f, 0.0f);
	//* for some reason, the y should be double the x/z values to have a normal scale
	bomb.scale = glm::vec3(0.1f, 0.2f, 0.1f); 
	genRotateAnim(bomb);
	models.push_back(bomb);

	struct model chest;
	chest.file = "assets/interactive_tc.gltf";
	chest.name = "Treasure Chest";
	chest.description = "Treasure Chest. Contains gold!";
	chest.position = glm::vec3(0.0f, 0.0f, 0.0f);
	chest.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	chest.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	models.push_back(chest);

	struct model chestLid;
	chestLid.file = "assets/interactive_tc_lidv2.gltf";
	chestLid.name = "Treasure Chest Lid";
	chestLid.description = "The lid of the treasure chest.";
	chestLid.position = glm::vec3(0.0f, 1.2f, -0.02f);
	chestLid.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	chestLid.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	genChestAnimation(chestLid);
	models.push_back(chestLid);

	struct model cannon;
	cannon.file = "assets/cannon.gltf";
	cannon.name = "Cannon";
	cannon.description = "Cannon that fires and emulates rollback recoil";
	cannon.position = glm::vec3(3.5f, 1.5f, 0.0f);
	cannon.rotation = glm::vec3(0.0f, 1.5708f, 0.0f);
	cannon.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	genCannonBackAnimation(cannon);
	models.push_back(cannon);

	// Creating a second bomb model instance named cannonBomb
	struct model cannonBomb;
	cannonBomb.file = "assets/bomb.gltf"; // Adjust the file path if necessary
	cannonBomb.name = "Cannon Bomb";
	cannonBomb.description = "Bomb model to be fired from the cannon.";
	cannonBomb.position = glm::vec3(6.0f, 3.0f, 0.0f);
	cannonBomb.rotation = glm::vec3(3.9f, 0.05f, 0.3f);
	cannonBomb.scale = glm::vec3(0.08f, 0.15f, 0.08f);
	genBombFiringAnimation(cannonBomb);
	models.push_back(cannonBomb);

	struct model key;
	key.file = "assets/key.gltf"; // Adjust the file path if necessary
	key.name = "Key";
	key.description = "Key for the Treasure Chest";
	key.position = glm::vec3(-1.0f, 1.0f, 1.0f);
	key.rotation = glm::vec3(1.5708f, 0.0f, 0.0f);
	key.scale = glm::vec3(0.25f, 0.25f, 0.25f);
	genKeyAnimation(key);
	models.push_back(key);

	// struct model cube;
	// cube.file = "assets/cube.gltf";
	// cube.name = "cube";
	// cube.description = "A cube";
	// cube.position = glm::vec3(0.0f, 0.0f, 0.0f);
	// cube.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	// cube.scale = glm::vec3(1.0f, 1.0f, 1.0f);
	// models.push_back(cube);

	for (int i = 0; i < models.size(); i++)
	{
		contentList.push_back(Content());
		contentList.at(i).LoadGLTF(models.at(i).file);
	}

	pipeline.CreatePipeline();
	pipeline.LoadShaders("shaders/vs_model.glsl", "shaders/fs_model.glsl");

	// A few optimizations.
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Get the correct size in pixels - E.g. if retina display or monitor scaling
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

	// Calculate proj_matrix for the first time.
	aspect = (float)windowWidth / (float)windowHeight;
	projMatrix = glm::perspective(glm::radians(fovy), aspect, 0.1f, 1000.0f);
}

void animate(model& model)
{
	if (!model.animate) return;
	if (model.frameIndex < model.animation.size() - 1)
	{
		model.frameIndex++;
	}
	else
	{
		model.frameIndex = 0;
	}

	model.position = model.animation.at(model.frameIndex).position;
	model.rotation = model.animation.at(model.frameIndex).rotation;
	model.scale = model.animation.at(model.frameIndex).scale;
}

void genKeyAnimation(model& keyModel)
{
	
	int frames = 90;
	for(int i = 0; i <= frames; i++) {
		frame f;
		if(i == 0) {
			f.position 	= keyModel.position;
			f.rotation 	= keyModel.rotation;
			f.scale 	= keyModel.scale;
		} else {
			f.position 	= keyModel.animation[i-1].position;
			f.rotation 	= keyModel.animation[i-1].rotation;
			f.scale 	= keyModel.animation[i-1].scale;

			f.rotation.x += glm::radians(1.0f); //moves 1 degree per frame (90/90 = 1)
			f.rotation.z += glm::radians(1.0f);
			f.position.x += 0.01111f;
		}
		keyModel.animation.push_back(f);
	}


	for(int i = 90; i <= 120; i++) {
		frame f;
		f.position 	= keyModel.animation[i].position;
		f.rotation 	= keyModel.animation[i].rotation;
		f.scale 	= keyModel.animation[i].scale;

		// rotate z by 360 degrees in 30 frames = 6 degress per frame
		f.rotation.z += glm::radians(6.0f);
		
		keyModel.animation.push_back(f);
	}
}

void genChestAnimation(model& chestLidModel)
{
	//* Source: https://registry.khronos.org/OpenGL-Refpages/gl4/html/radians.xhtml converting degrees to radians
	float maxRadians = glm::radians(140.0f); //! 140 degrees (x axis) means chest lid is fully open, do not exceed
	float degrees = 0.0f;

	for (float curRadians = 0; curRadians < maxRadians; degrees++)
	{
		curRadians = glm::radians(degrees);
		float rotation = curRadians * -1;

		frame f = frame();
		f.position = chestLidModel.position;
		f.rotation = glm::vec3(rotation, 0.0f, 0.0f);
		f.scale = chestLidModel.scale;

		chestLidModel.animation.push_back(f);
	}
}

void genCannonBackAnimation(model& cannonModel) {
    // Define the total distance to move back
    const float totalDistance = 10.0f;

    // Define the number of steps/frames in the animation
    const int steps = 100; // More steps mean smoother animation

    // Calculate the distance per step
    float distancePerStep = totalDistance / steps;

    for (int i = 0; i <= steps; i++) {
        frame f = frame();
        f.position = cannonModel.position;
        f.rotation = cannonModel.rotation;
        f.scale = cannonModel.scale;

        // Move the cannon back a bit more in each frame
        f.position.z -= distancePerStep * i;

        // Add this frame to the cannon model's animation sequence
        cannonModel.animation.push_back(f);
    }
}

void genBombFiringAnimation(model& bombModel) {

    // Define the motion parameters for the bomb
    float firingSpeed = 20.0f; // Adjust as needed
    int steps = 100; // More steps for smoother animation
	float upwardSpeed = 5.0f; // This value controls how fast the bomb moves upwards

    for (int i = 0; i <= steps; i++) {
        frame f = frame();
        f.position = bombModel.position;
        f.rotation = bombModel.rotation;
        f.scale = bombModel.scale;

        // Adjust bomb's position for each frame to simulate firing
        f.position.z += (firingSpeed / steps) * i;  // Update this based on bomb direction
		f.position.y += (upwardSpeed / steps) * i;

        bombModel.animation.push_back(f);
    }
}

void genRotateAnim(model& model)
{
	float speed 	= 10.0f;
	int frames 		= 360;

	for (int i = 0; i <= frames; i++)
	{
		frame f;
		if (i == 0)
		{
			f.position 	= model.position;
			f.rotation 	= model.rotation;
			f.scale 	= model.scale;
		}
		else
		{
			f.position 	= model.animation[i-1].position;
			f.rotation 	= model.animation[i-1].rotation;
			f.scale 	= model.animation[i-1].scale;

			//? radians = degrees * PI / 180
			f.rotation.y += 1 * 3.14/180;
		}
		model.animation.push_back(f);
	}
}

void update()
{
	// camera reference https://learnopengl.com/Getting-started/Camera
	if (keyStatus[GLFW_KEY_W]) cameraPosition += cameraSpeed * cameraFront;
	if (keyStatus[GLFW_KEY_S]) cameraPosition -= cameraSpeed * cameraFront;
	if (keyStatus[GLFW_KEY_A]) cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keyStatus[GLFW_KEY_D]) cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (keyStatus[GLFW_KEY_SPACE]) cameraPosition.y += 0.1f; // space to go up
	if (keyStatus[GLFW_KEY_LEFT_CONTROL]) cameraPosition.y -= 0.1f; // l control to go down
	if (keyStatus[GLFW_KEY_LEFT_SHIFT]) cameraSpeed = 0.3f; else cameraSpeed = 0.05f; //hold shift to increase camera speed

	// animation key listener mappings
	if (keyStatus[GLFW_KEY_1]) playSwordAnimation = true;
	if (keyStatus[GLFW_KEY_2]) playBombAnimation = true;
	if (keyStatus[GLFW_KEY_3]) playKeyAnimation = true;
	if (keyStatus[GLFW_KEY_4]) playChestAnimation = true;
	if (keyStatus[GLFW_KEY_5]) playCannonAnimation = true;

	// todo: this is just debug, remove for final
	if (keyStatus[GLFW_KEY_EQUAL]) sphereRad += 0.1f;
	if (keyStatus[GLFW_KEY_MINUS]) sphereRad -= 0.1f;

	if (keyStatus[GLFW_KEY_R]) pipeline.ReloadShaders();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void normalise_selection()
{
	// get 2D crosshair coords
	const ImGuiViewport *viewport = ImGui::GetMainViewport();

	double crosshair_x_2d = 0.0f;
	double crosshair_y_2d = 0.0f;

	if (crosshair)
	{
		float crosshair_x_2d = (viewport->WorkSize.x / 2.0f);
		float crosshair_y_2d = (viewport->WorkSize.y / 2.0f);
	}
	else
	{
		glfwGetCursorPos(window, &crosshair_x_2d, &crosshair_y_2d);
	}

	// normalise
	float crosshair_x = (2.0f * crosshair_x_2d) / (float)windowWidth - 1.0f;
	float crosshair_y = 1.0f - (2.0f * crosshair_y_2d) / (float)windowHeight;
	float crosshair_z = -1.0f;

	normCursorPosition = vec3(crosshair_x, crosshair_y, crosshair_z);
}

void raycast()
{
	// ray is 4d: normalised crosshair coords and direction (-z is 'forwards', so negate)
	glm::vec4 ray = glm::vec4(normCursorPosition.x, normCursorPosition.y, -normCursorPosition.z, 1.0f);

	// get 4d vector of camera/eye
	glm::vec4 ray_camera = inverse(projMatrix) * ray;
	ray = vec4(ray_camera.x, ray_camera.y, -1.0f, 0.0f);

	// world view vector
	float world_x = (inverse(glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp)) * ray).x;
	float world_y = (inverse(glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp)) * ray).y;
	float world_z = (inverse(glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp)) * ray).z;

	glm::vec3 ray_world = glm::vec3(world_x, world_y, world_z);
	ray_world = normalize(ray_world);

	cursorRay = ray_world;
}

void check_collision()
{
	raycast();

	vec3 intersectionPosition;
	vec3 intersectionNormal;

	// todo: some bug here, only seems to work if camera is close to model
	// see here: https://community.khronos.org/t/intersection-is-detected-only-when-a-camera-is-close-to-the-object/108120/6
	for (int i = 0; i < models.size(); i++)
	{
		selectedObject = glm::intersectRaySphere(cameraPosition, cursorRay, models.at(i).position, sphereRad, intersectionPosition, intersectionNormal) ? i : -1;
	}
}

void render()
{
	glViewport(0, 0, windowWidth, windowHeight);

	// Clear colour buffer
	glm::vec4 skyBlue = glm::vec4(0.529, 0.808, 0.922, 1.0f);
	glm::vec4 backgroundColor = skyBlue;
	glClearBufferfv(GL_COLOR, 0, &backgroundColor[0]);

	// Clear deep buffer
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_DEPTH, 0, &one);

	// Enable blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader programs
	glUseProgram(pipeline.pipe.program);

	// Setup camera
	glm::mat4 viewMatrix = glm::lookAt(cameraPosition,				 // eye
									   cameraPosition + cameraFront, // centre
									   cameraUp);					 // up

	for (int i = 0; i < contentList.size(); i++)
	{
		Content content = contentList.at(i);

		glBindTexture(GL_TEXTURE_2D, content.texid);

		// Do some translations, rotations and scaling
		// glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(modelPosition.x+rX, modelPosition.y+rY, modelPosition.z+rZ));
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), models.at(i).position);
		modelMatrix = glm::rotate(modelMatrix, models.at(i).rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, models.at(i).rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, models.at(i).rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, models.at(i).scale);

		// animate any models which have an animation
		if(playSwordAnimation) {
			animate(models[SWORD_MODEL_INDEX]);
			playSwordAnimation = false;
		}
		
		if(playBombAnimation && models.at(i).name == "Bomb") {
			animate(models.at(i));
			playBombAnimation = false;
		}
		
		if(playChestAnimation) {
			animate(models[CHEST_LID_MODEL_INDEX]);
			playChestAnimation = false;
		}
		
		if(playKeyAnimation) {
			animate(models[KEY_MODEL_INDEX]);
			playKeyAnimation = false;
		}
		
		if (playCannonAnimation) {
			animate(models[CANNON_MODEL_INDEX]);
			animate(models[CANNON_BALL_INDEX]);
			playCannonAnimation = false;
		}
		
		// uncomment to make all animations play repeatedly
		// if (models.at(i).animation.size() > 0)
		// {
		// 	animate(models.at(i));
		// }

		glm::mat4 mv_matrix = viewMatrix * modelMatrix;

		glUniformMatrix4fv(glGetUniformLocation(pipeline.pipe.program, "model_matrix"), 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(pipeline.pipe.program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(pipeline.pipe.program, "proj_matrix"), 1, GL_FALSE, &projMatrix[0][0]);

		content.DrawModel(content.vaoAndEbos, content.model);
	}

	normalise_selection();
	check_collision();

	#if defined(__APPLE__)
		glCheckError();
	#endif
}

void ui()
{
	ImGuiIO &io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration; 
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	window_flags |= ImGuiWindowFlags_NoSavedSettings; 
	window_flags |= ImGuiWindowFlags_NoFocusOnAppearing; 
	window_flags |= ImGuiWindowFlags_NoNav;

	const auto PAD = 10.0f;
	const ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
	ImVec2 work_size = viewport->WorkSize;
	ImVec2 window_pos, window_pos_pivot;
	window_pos.x = work_pos.x + work_size.x - PAD;
	window_pos.y = work_pos.y + work_size.y - PAD;
	window_pos_pivot.x = 1.0f;
	window_pos_pivot.y = 1.0f;

	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	window_flags |= ImGuiWindowFlags_NoMove;

	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	bool *p_open = NULL;
	if (ImGui::Begin("Program Info", nullptr, window_flags)) {
		ImGui::Text("About: 3D Graphics and Animation 2023/24 Group 6/Fig"); // ImGui::Separator();
		ImGui::Text("Performance: %.3fms/Frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Models loaded: %d", contentList.size());
		ImGui::Text("Pipeline: %s", pipeline.pipe.error?"ERROR":"OK");
	}
	ImGui::End();

	// if (selectedObject > -1)
	if(keyStatus[GLFW_KEY_1] || keyStatus[GLFW_KEY_2] || keyStatus[GLFW_KEY_3] || keyStatus[GLFW_KEY_4] || keyStatus[GLFW_KEY_5])
	{
		int index = -1;
		if(keyStatus[GLFW_KEY_1]) index = SWORD_MODEL_INDEX;
		else if(keyStatus[GLFW_KEY_2]) index = BOMB_MODEL_INDEX;
		else if(keyStatus[GLFW_KEY_3]) index = KEY_MODEL_INDEX;
		else if(keyStatus[GLFW_KEY_4]) index = CHEST_MODEL_INDEX;
		else if(keyStatus[GLFW_KEY_5]) index = CANNON_MODEL_INDEX;

		window_pos.x = work_pos.x + work_size.x - PAD;
		window_pos.y = work_pos.y + 80.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		if (ImGui::Begin("Model Info", nullptr, window_flags)) {
			// todo: remove sphere radius debug
			ImGui::Text("Selected object (%.1f): %s", sphereRad, models.at(index).name.c_str());
			ImGui::Text("Description: %s", models.at(index).description.c_str());
		}
		ImGui::End();
	}

	if (crosshair)
	{
		ImGui::SetNextWindowPos(ImVec2(-50.0f, -50.0f), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
		ImGui::Begin("crosshair", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
		ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

		// crosshair
		draw_list->AddLine(ImVec2((work_size.x / 2.0f) - 10.0f, work_size.y / 2.0f), 
						ImVec2((work_size.x / 2.0f) + 10.0f, work_size.y / 2.0f),
						IM_COL32(0, 255, 0, 255), 2.0f);
		draw_list->AddLine(ImVec2(work_size.x / 2.0f, (work_size.y / 2.0f) - 10.0f), 
						ImVec2(work_size.x / 2.0f, (work_size.y / 2.0f) + 10.0f),
						IM_COL32(0, 255, 0, 255), 2.0f);

		if (selectedObject >= 0)
		{
			// target text (below crosshair)
			draw_list->AddText(ImVec2((work_size.x / 2.0f) - 20.0f, (work_size.y / 2.0f) + 20.0f), 
							IM_COL32(0, 255, 0, 255), models.at(selectedObject).name.c_str());
		}

		ImGui::End();
	}

	if(helpMenu) 
	{
		ImGui::Begin("help menu", nullptr, window_flags);
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
		ImGui::Text("Help Menu"); 
		ImGui::Separator();
		ImGui::Text("Movement Keys: W A S D\nHold Shift to Run\nHold Space - Go Up\nHold Left Control - Go Down\nScroll Wheel Up/Down - Zoom In/Out");
		ImGui::Separator();
		ImGui::Text("Hold 1 - Play Sword Animation\nHold 2 - Play Bomb Animation\nHold 3 - Play Key Animation\nHold 4 - Play Chest Animation\nHold 5 - Play Cannon Firing Animation");
		ImGui::End();
	}

	// Rendering imgui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void onResizeCallback(GLFWwindow *window, int w, int h)
{
	windowWidth = w;
	windowHeight = h;

	// Get the correct size in pixels
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

	if (windowWidth > 0 && windowHeight > 0)
	{ // Avoid issues when minimising window - it gives size of 0 which fails division.
		aspect = (float)w / (float)h;
		projMatrix = glm::perspective(glm::radians(fovy), aspect, 0.1f, 1000.0f);
	}
}

void onKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		keyStatus[key] = true;
	else if (action == GLFW_RELEASE)
		keyStatus[key] = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void onMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
}


// mouse opengl tutorial reference: https://learnopengl.com/Getting-started/Camera
void onMouseMoveCallback(GLFWwindow *window, double x, double y)
{
	if(firstCall)
	{
		lastX = x;
		lastY = y;
		firstCall = false;
	}

	float xoffset = x - lastX;
    float yoffset = lastY - y; 
	lastX = x;
    lastY = y;
	
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

	glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void onMouseWheelCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	int yoffsetInt = static_cast<int>(yoffset);
	fovy -= yoffsetInt;	
	projMatrix = glm::perspective(glm::radians(fovy), aspect, 0.1f, 1000.0f);
}

void APIENTRY openGLDebugCallback(GLenum source,
								  GLenum type,
								  GLuint id,
								  GLenum severity,
								  GLsizei length,
								  const GLchar *message,
								  const GLvoid *userParam)  // Debugger callback for Win64 - OpenGL versions 4.2 or better.
{
	debugger.OpenGLDebugCallback(source, type, id, severity, length, message, userParam);
}

void debugGL() // Debugger function for Win64 - OpenGL versions 4.2 or better.
{
	// Enable Opengl Debug
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback((GLDEBUGPROC)openGLDebugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
}

GLenum glCheckError_(const char *file, int line) // Debugger manual function for Apple - OpenGL versions 3.3 to 4.1.
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) debugger.GlGetError(errorCode, file, line);

	return errorCode;
}
