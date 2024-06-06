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
