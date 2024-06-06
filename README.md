# F20GA-CW

Heriot-Watt University 3D Graphics and Animation

Group: Fig  
Theme: Pirates

| Model          | Who   |
| -------------- | ----- |
| Bomb           | Kyle  |
| Cannon         | Calum |
| Key            | Omar  |
| Treasure Chest | John  |
| Sword          | Megan |
| Ship Stairs    | Calum |
| Ship Netting   | Kyle  |
| Ship Deck      | Omar  |
| Ship Mast      | Megan |
| Scene Sky      | John  |


# Animation README

To combine the videos run `./combine.sh` (ffmpeg).

# Bomb Animation
Exported models and materials/textures from blender using FBX and texture baking. Fixed various pipeline issues (scaling, textures not showing properly, combining meshes/models).

Used Unreal sequencer to animate scene using keyframes. First the camera pans in on the ship sailing the sea. This was achieved by moving the ocean model throughout the sequence. Then the camera settles and focuses on the bomb model. The ship then rocks as if it has hit rough waters and the bomb falls off. For the bomb rolling and falling I considered animation fundamentals mentioned in the lectures such as exaggeration for the bomb rolling and suspense by having the bomb hover for a moment before falling down into the sea.

References used:
1. https://www.youtube.com/watch?v=FsV3ZUxmWLo
2. https://www.youtube.com/watch?v=Oha-OKB6QP0
3. https://www.youtube.com/watch?v=dW9baOfDS9k
4. https://www.youtube.com/watch?v=c39vdC7jloo

# TreasureChest Animation 

OneDrive source files (inc. Unreal Project file and evidence of incremental work): https://heriotwatt-my.sharepoint.com/:f:/g/personal/mam2000_hw_ac_uk/EugC1d2YG3hEqvXF4LiJ1Z4B_P9walWZh-tEV3wvP-prig?e=USuqq6

Objects were imported into unreal as FBX files. Pipeline issues occured on all procedurally generated textures, such as musgrave texture used in the wooden pattern texture, would not display properly in unreal - it would be blank. I had to follow a tutorial (referenced below) to use the UV smart unwrap tool in blender and save the procedural texture as an image, then reapply it as a texture in unreal. The sky was not rendering properly from blender either, so I used the built in sky in unreal, which visually is a lotbetter than a blender sky texture.

Further pipeline issues were missing normals (some sides of the objects would not appear properly, or would be invisible at certian angles). The inner lid of the chest was invisible (which would appear when the chest opens) - had to add a soldify modifier to fix this issue.

Directional lights are used to remove the shadows that would cast on the ship deck and cover the main objects within view the camera. Spotlight object was added (Unreal light prop) was added inside of the chest, to exaggerate the golden hue of the coins as they burst out from the chest.

Animation was carried out using a sequencer, I pivoted the chest lid on it's 'roll' attribute to flip it open. I added multiple slow key frames, then added more to dramatise the effect of it flying open - Camera actor was also pivoted on its pitch and roll attributes to dramatise the view of the coins flying out the chest. 3 coins burst out from the pile of coins, as they fly through the air I created a realistic path for them to follow and changed their rotation properties to make them flip until they hit the floor.
The main coin in the camera view (which is tracked) trials a path (done by updating its coordinates) then updating the roll option to make it wobble realistically (playing into the anticipation definition) until it finally falls face down. The camera is tracking the main coin throughout the entire sequence. The key objective animation concern is to play off the previous animation - where the key is magnetically or 'magically' attracted to the key and the coins burst out.

Created a new blender model of a golden (£) coin and a pane with its vertices stretched upwards and smoothed to look like a pile of coins.

TreasureChest Animation References / Source used:
Tutorial on exporting procedurally generated textures into blender: https://www.youtube.com/watch?v=PencopBus

coin inspiration: https://youtu.be/r8ltW7pAN6M

rendering mp4 from sequencer cinematic: https://youtu.be/c39vdC7jlooi

# Cannon Firing Animation
OneDrive source files (Including Unreal Project files and evidence of incremental work): [https://heriotwatt-my.sharepoint.com/:f:/g/personal/mam2000_hw_ac_uk/EugC1d2YG3hEqvXF4LiJ1Z4B_P9walWZh-tEV3wvP-prig?e=USuqq6
](https://heriotwatt-my.sharepoint.com/:f:/g/personal/mam2000_hw_ac_uk/EgcsUO5dyQJLobRcd3dJcikBNRxY7cxX6yEf_xVcpJW3Vg?e=jboB1D)

Objects were imported into the Unreal project using FBX files.

The cannon first has some fire on the fuse which then has an explosion and the cannon does some anticipation and exaggeration to show the firing of the projectile. The cannon rolls back once it has finished firing to show the amount of force that would be exerted onto the cannon.

The camera is a simple pan where it starts of at the starting position of the cannon and then moves with the cannon to the end of the animation.

# Key Animation 

OneDrive source files: https://heriotwatt-my.sharepoint.com/personal/mam2000_hw_ac_uk/_layouts/15/onedrive.aspx?ga=1&id=%2Fpersonal%2Fmam2000%5Fhw%5Fac%5Fuk%2FDocuments%2FF20GA%20%2D%20Groupwork%2FAnimation%2FUnreal%20Project%20Files%2FKey%5Fand%5FChest

Key object created in blender was imported into unreal engine using FBX files.

The animation starts with the camera rotating into the shot with the key on the ground next to the chest, the key shakes and jumps around by adding in keyframes moving the location and rotation of the key, then comes to life to open the treasure chest using the same techniques. It then plays Johns animation of the chest opening and revealing of the coins inside.

https://github.com/omarriyaz/3D-Graphics-Project/assets/91089045/5e83e6f1-2241-45c4-accb-162f175e2c41



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



https://github.com/omarriyaz/3D-Graphics-Project/assets/91089045/49fcf1f9-1acb-401a-96bc-91c359ea932c





README for Final Render

# Scene - Ocean
Added ocean modifier onto pane objecet to mock ocean waves. Decreased the hue of the ocean colour to be more distinctive to the sky colour.

Reference: https://docs.blender.org/manual/en/latest/modeling/modifiers/physics/ocean.html


# Scene Lighting
Added 2 area lights - one to slightly light up the entire scene so the 'ozone' property of the sky would be a darker blue instead of light blue. Another to the ship deck so that the modeled objects would not be casted in shadows which affects the view and colour of the materials.

A sun light point was added into the scene, with its strength attribute increased, so all scene contents were brighter in the rendered image - looked too dark / dimly light in earlier versions.

Sun orientation attribute on sky pane was added to the top right corner of the image - introduces more natural feeling sun light.

# Ship Deck
- Removed side ship deck faces so cannon would not clip through ship deck siding.
- Extruded the edges on ship flooring to create 'wooden plank' effect instead of it all being one meshed object.

- Reference 1: https://www.youtube.com/watch?v=yCC9zCOMlgQ
- Reference 2: https://www.youtube.com/watch?v=Egd_BNAT3l8

# Ship Netting
- Reference: https://www.youtube.com/watch?v=I7KtPqic3lQ
- Reduced polygon count as it was using 8GB of RAM.

# Cannon
- Reference: https://polyhaven.com/a/dark_wood

# Stairs and Upper Deck
- Reference: https://www.youtube.com/watch?v=Egd_BNAT3l8

# Pirate Key
- Reference 1: https://www.youtube.com/watch?v=Ln9U7hQls_4
- Reference 2: https://www.youtube.com/watch?v=lleeZMRYZTg

# Bomb
- Reference for rope: https://www.youtube.com/watch?v=I7KtPqic3lQ

# Sword Material 
- Reference: https://www.youtube.com/watch?v=MwZb_vif9_A


![CW_RENDER_V3](https://github.com/omarriyaz/3D-Graphics-Project/assets/91089045/f72e30a1-47d8-4c24-b997-571fefe61469)



