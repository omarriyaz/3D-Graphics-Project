# Interactive README
## Sources
### Raycasting:
1. https://antongerdelan.net/opengl/raycasting.html  
2. https://gist.githubusercontent.com/saywassup/00672d131d9b8ec0ec0b1a2e96020578/raw/8da8bb7b986f48b94ce9163d539b49eb7a5d4478/gistfile1.glsl  

### Camera Movement (inc. mouse 'look-around' feature): 
1. https://learnopengl.com/Getting-started/Camera  
Moving the camera position forward and backward (by pressing W and S keys) works by multiplying the cameraFront vector by some multiplier (camerSpeed). cameraFront.z value is multiplied by camerSpeed which determines how much the z position of the camera is increased or decreased. Every frame will inject this change onto the z position as long as the appropriate keys are being held down.

Moving left and right (pressing A and D) works by getting the cross product of two vectors. If we know the up vector (y axis) of the camera, and we know the cameraFront (z axis), a cross product between these two (which shows the difference) will give us the direction which is perpendicular to these two vectors (2 lines, resulting line is perpendicular). The resulting perpendicular vector is either facing towards the positive x-axis (moving left) or negative x-axis (moving right).

2. Mouse 'Look-Around'
Moving the mouse left, right, up or down updates (inc and dec) the x and y position values - which `onMouseMoveCallback` is constantly listening to and sending I/O values. These values are float differences.
The logic is to capture the difference between the x and y position since the last frame, then apply this update to the camera direction. `sensitivity` is a multiplier which amplifies the difference in updating camera x and y direction, so the camera coordinates are updated further on the same input value. It then calculates the updated direction using trigonometry and passing in the changed values of yaw and pitch.
Pitch (how far the camera is looking up or down) has an if condition to check if it is 90 degrees up or -90 degrees down so the user cannot look past the sky or past their feet (informally) - this means movement is easier as otherwise the camera would flip entirely in direction and the contents of the scene would appear upside down.

3. Mouse 'Zoom-In and Out'
Mouse scrollwheel up/down events are listened to by `onMouseWheelCallback` - the events update a variable `fovy` by either 1 or -1 (zooming in or out). `projMatrix` - is the viewing matrix for the camera and has its values recalculated using `fovy` to update its zoom.


### Converting Degrees to Radians (glm::radians method):
1. https://registry.khronos.org/OpenGL-Refpages/gl4/html/radians.xhtml  


### Evidence of Incremental Work (Screenshots)
https://heriotwatt-my.sharepoint.com/:f:/g/personal/mam2000_hw_ac_uk/Et3qA1NOEMVBnW1YjRMto9kBeiHzdjrmbcfLE_WEsiyPGQ?e=Q6MRaF

### Loading Models
Models are instantiated by using the `models` struct and storing this in a vector, `vector<model> models;`
The model struct stores multiple fields which store information about the supplied .gltf file. Each of these models is added to `contentList` where in the `render()` function iteratively accesses the contentList to retrieve each model, and renders it.

### Building the Scene
The scene was built by importing our 5 main objects that were modelled in the Render deliverable:  
1. Sword  
2. Bomb  
3. Treasure Chest  
4. Key  
5. Cannon  

Additionally, a floor was added so that all main objects could be positioned in a proper manner (instead of just floating). A lightblue sky background was added to give the scene some more colour.  

### Exporting and Importing Materials/Textures
To import and apply materials/shaders/textures, specific blender exporting options were selected to achieve this.  
- All models had their materials baked into an image file, and this was saved and placed in the `assets/textures` directory.  
- Models were exported using the embedded .gltf option, with no-export dropdown selected under the 'materials' section.  
- The exported .gltf file was manually edited to include "images" and "textures" attributes, which supply the `uri` of where the texture of the model is stored.  

### Keystroke Interaction
Keystroke interaction supplies all functionality behind camera movement and running animations. The GLFW library sets key and mouse callbacks that listen to physical I/O events caused by mouse and key interactions. The help menu guide in the application details the functionality behind every mapped and actively 'listened' key stroke.  

Our implementation of keystrokes takes advantage of the existing `update()` method which provides the template to write handle logic on keystroke / input events.

### Collision Detection
Collision detection was attempted to be implemented but somewhere my maths doesn't quite add up. The concept was to use a vector with direction from the cursor/crosshair and raytrace by so many units and checking if the ray collided with a sphere positioned around the object. This involved heavy use of vectors, trigonometry and geometry.
With more time this would have been properly implemented and polished.  

### Animation
Animation was implemented by having models contain a C++ vector of 'frames'. Each fram included position/translation, rotation and scale information for the model in that moment. The update function iterates over this array to deliver the 'frames' one at a time to the render function.  
When first attempted the animation function was being called between each `render()` call which gave the output of objects completing their animation in one frame (what was really happening is that the movements were happening, just not being rendered).  
This code very modular which saved a lot of time for the group as all that needed to be done to add animation to a model was populating the vector.  
