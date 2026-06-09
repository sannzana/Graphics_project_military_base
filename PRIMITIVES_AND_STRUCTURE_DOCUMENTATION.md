# MILITARY BASE CAMP - OBJECT PRIMITIVES & STRUCTURE DOCUMENTATION

## Table of Contents
1. [Primitive Types Overview](#primitive-types-overview)
2. [Base Primitives Used](#base-primitives-used)
3. [All Objects and Their Primitives](#all-objects-and-their-primitives)
4. [Wrapping/Texturing System](#wrappingtexturing-system)
5. [Object Hierarchies](#object-hierarchies)

---

## PRIMITIVE TYPES OVERVIEW

### What are Primitives?

**Primitives** are the basic geometric building blocks used to construct 3D objects in computer graphics. They are fundamental shapes that can be combined and transformed to create complex structures.

### Why Use Primitives?

- **Efficiency**: Pre-optimized rendering for performance
- **Simplicity**: Easy to position, scale, and rotate
- **Flexibility**: Can be combined to create complex objects
- **Memory**: Minimal storage footprint compared to custom meshes

---

## BASE PRIMITIVES USED

### 1. **Cube/Box**
**Characteristics**:
- 8 vertices, 12 edges, 6 faces
- Each face is a square (2 triangles)
- Total: 36 vertex indices (6 faces × 2 triangles × 3 vertices)
- Axis-aligned by default (can be transformed)

**Structure**:
```
Vertices (0-1 normalized):
  (0,0,0) to (1,1,1)

Faces:
  Front (Z+):    (0,0,0), (1,0,0), (1,1,0), (0,1,0)
  Back (Z-):     (0,0,1), (0,1,1), (1,1,1), (1,0,1)
  Right (X+):    (1,0,0), (1,0,1), (1,1,1), (1,1,0)
  Left (X-):     (0,0,0), (0,1,0), (0,1,1), (0,0,1)
  Top (Y+):      (1,1,1), (1,1,0), (0,1,0), (0,1,1)
  Bottom (Y-):   (0,0,0), (0,0,1), (1,0,1), (1,0,0)
```

**OpenGL Rendering**:
```cpp
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
```

**Used In Scene**: Ground, walls, roofs, platforms, furniture, etc.

---

### 2. **Sphere**
**Characteristics**:
- Smooth curved surface with bands and slices
- Variable vertex count (144 × 72 in radar dish = 10,368 vertices)
- Generated using latitude/longitude grid
- Supports texture mapping (UV coordinates)

**Structure**:
```
Parameters:
  Radius: 2.0 units
  Bands (latitude): 144 divisions
  Slices (longitude): 72 divisions
  
Vertex Generation:
  for band in 0 to bands:
    latitude = ? * (band - bands/2) / bands
    for slice in 0 to slices:
      longitude = 2? * slice / slices
      x = radius * cos(latitude) * cos(longitude)
      y = radius * sin(latitude)
      z = radius * cos(latitude) * sin(longitude)
```

**Mathematical Formula**:
- Parametric: `(r·sin(?)·cos(?), r·sin(?)·sin(?), r·cos(?))`
- Where ? = latitude, ? = longitude, r = radius

**Used In Scene**: Radar dish, sun

---

### 3. **Cylinder**
**Characteristics**:
- Circular top and bottom with vertical sides
- Variable vertex count based on segments
- Can represent both solid and hollow shapes
- Supports axis-aligned or rotated

**Structure** (typical):
```
Parameters:
  Base Radius: varies
  Top Radius: varies (can be same for cylinder)
  Height: varies
  Slices (around circumference): 48
  Stacks (height segments): 24

Vertex Generation:
  Top circle: radius·cos(?), height, radius·sin(?)
  Bottom circle: radius·cos(?), 0, radius·sin(?)
  Where ? ranges 0 to 2? in 'slices' steps
```

**Used In Scene**: 
- Jeep wheels (black tyre face)
- Wheel face cylinders (textured)
- Watchtower posts
- Various structural elements

---

### 4. **Model (GLTF/Scene Graph)**
**Characteristics**:
- Complex meshes loaded from external files
- Contains multiple primitives and sub-meshes
- Supports hierarchical transformations
- Includes material and animation data

**Structure**:
```
GLTF File Format:
  ?? Meshes (geometry data)
  ?  ?? Primitives
  ?  ?  ?? Positions (vertex coordinates)
  ?  ?  ?? Normals (surface orientation)
  ?  ?  ?? Indices (face definitions)
  ?  ?  ?? Textures
  ?  ?? Materials
  ?? Nodes (scene hierarchy)
  ?? Animations (if any)
  ?? Buffers (raw data)
```

**Used In Scene**:
- Tank (Tank_model/scene.gltf)
- Tent (tent_military/scene.gltf)
- Jeep (jeep/scene.gltf)
- Truck (truck/scene.gltf)

---

## ALL OBJECTS AND THEIR PRIMITIVES

### SECTION A: GROUND & TERRAIN

#### 1. **Ground Plane**
```
Name: cube (ground)
Primitive: CUBE
?? Type: Single textured box
?? Dimensions: 200 × 0.1 × 200 units
?  ?? Width (X): 200.0
?  ?? Height (Y): 0.1
?  ?? Depth (Z): 200.0
?? Position: Centered at origin
?? Texture: "ff.png" (sand/dirt texture)
?? Material:
?  ?? Diffuse: Textured
?  ?? Specular: Textured
?  ?? Shininess: 32.0
?? Wrapping: GL_REPEAT
?? Render Mode: Color/Texture/Blend toggle (key M)
?? VAO: cubeVAO (shared)
```

---

### SECTION B: GARAGE PLATFORMS

#### 2. **Jeep Garage Platform**
```
Object: GaragePlatform (garagePlatform)
Position: (-22, 0.1, 10)
Dimensions:
  ?? Inner Width: 14 units
  ?? Inner Depth: 18 units
  ?? Wall Height: 7 units
  ?? Floor Raise: 1.2 units

Primitives Breakdown:

2.1 FLOOR (Platform Base)
  Primitive: CUBE
  ?? Dimensions: 15.2 × 1.2 × 19.2 units
  ?? Position: (-29 to -15, 0 to 1.2, 1 to 20)
  ?? Texture: "floor.png"
  ?? Material: Textured
  ?? Count: 1 cube

2.2 RAMP (Front)
  Primitives: CUBE + complex geometry
  ?? Main Ramp:
  ?  ?? Type: Scaled cube rotated
  ?  ?? Angle: ~16.7° (tan?¹(1.2/4.3))
  ?  ?? Dimensions: 10.5 × 0.22 × 4.3 units
  ?  ?? Texture: "floor.png"
  ?? Support Block:
  ?  ?? Type: Cube
  ?  ?? Dimensions: 10.5 × 0.36 × 1.5 units
  ?  ?? Color: Solid gray (0.30, 0.28, 0.26)
  ?? Stairs (4 steps):
  ?  ?? Type: 4 cubes stacked
  ?  ?? Each Step:
  ?  ?  ?? Dimensions: 3.6 × increasing height × 1.075 units
  ?  ?  ?? Color: Solid gray
  ?  ?? Total: 4 cubes
  ?? Total Primitives: 6 cubes

2.3 WALLS
  Primitives: CUBE × 3
  ?? Left Wall:
  ?  ?? Dimensions: 0.4 × 7 × 19.2 units
  ?  ?? Position: Left side
  ?  ?? Texture: "wall.png"
  ?? Right Wall:
  ?  ?? Dimensions: 0.4 × 7 × 19.2 units
  ?  ?? Position: Right side
  ?  ?? Texture: "wall.png"
  ?? Back Wall:
  ?  ?? Dimensions: 15.2 × 7 × 0.4 units
  ?  ?? Position: Back side
  ?  ?? Texture: "wall.png"
  ?? Total: 3 cubes

2.4 ROOF
  Primitive: CUBE
  ?? Dimensions: 15.2 × 0.3 × 19.2 units
  ?? Position: Top
  ?? Texture: "wall.png"
  ?? Material: Textured

2.5 LAMPS (4 corner lamps)
  Primitives: CUBE × 4 per lamp × 4 lamps
  Each Lamp: 3-part structure
    ?? Base Platform:
    ?  ?? Type: Cube
    ?  ?? Dimensions: 0.5 × 0.24 × 0.5 units
    ?  ?? Color: (0.30, 0.30, 0.32)
    ?  ?? Material: Solid
    ?? Vertical Pole:
    ?  ?? Type: Cube
    ?  ?? Dimensions: 0.12 × 7.2 × 0.12 units
    ?  ?? Color: (0.50, 0.50, 0.52)
    ?  ?? Material: Solid
    ?? Horizontal Arm:
    ?  ?? Type: Cube
    ?  ?? Dimensions: 1.1 × 0.12 × 0.12 units
    ?  ?? Color: (0.50, 0.50, 0.52)
    ?  ?? Material: Solid
    ?? Lamp Head:
       ?? Type: Cube (2 parts)
       ?? Main Head:
       ?  ?? Dimensions: 0.756 × 0.252 × 0.756 units
       ?  ?? Color: (0.96, 0.92, 0.72)
       ?  ?? Shininess: 64
       ?? Reflector Ring:
       ?  ?? Dimensions: 0.882 × 0.084 × 0.882 units
       ?  ?? Position: Top of head
       ?  ?? Color: (0.20, 0.20, 0.22)
       ?? Grate:
           ?? Dimensions: 0.63 × 0.0672 × 0.63 units
           ?? Position: Bottom
           ?? Color: (0.18, 0.18, 0.18)

TOTAL GARAGE 1 PRIMITIVES:
  Floor: 1
  Ramp: 6
  Walls: 3
  Roof: 1
  Lamps: 12 (3 per lamp × 4 lamps)
  ????????????
  TOTAL: 23 cubes
```

#### 3. **Truck Garage Platform**
```
Same structure as Jeep Garage but:
Position: (-50, 0.1, 10)
Dimensions:
  ?? Inner Width: 16 units (wider)
  ?? Inner Depth: 20 units (longer)
  ?? Wall Height: 7 units

Primitives: Same breakdown as Garage 1
TOTAL: 23 cubes
```

---

### SECTION C: STRUCTURES

#### 4. **Barrack Building**
```
Object: Barrack
Position: (40, 0.1, -40)
Scale: 0.66× or 0.55× depending on variant

Primitives Used: CUBE (all walls, floors, doors, roof)

Structure:
4.1 FLOOR
  Type: Cube
  ?? Dimensions: Scaled 9 × 0.3 × 13.5 units
  ?? Position: Interior ground level
  ?? Material: Textured "wood.png"

4.2 WALLS (4)
  Type: Cube × 4
  ?? Front Wall:
  ?  ?? Dimensions: Scaled 9 × 3.5 × 0.4 units
  ?  ?? Door Opening: 2 units wide
  ?  ?? Texture: "wood.png"
  ?? Back Wall:
  ?  ?? Dimensions: Scaled 9 × 3.5 × 0.4 units
  ?  ?? Texture: "wood.png"
  ?? Left Wall:
  ?  ?? Dimensions: Scaled 0.4 × 3.5 × 13.5 units
  ?  ?? Texture: "wood.png"
  ?? Right Wall:
     ?? Dimensions: Scaled 0.4 × 3.5 × 13.5 units
     ?? Texture: "wood.png"

4.3 ROOF
  Type: Cube
  ?? Dimensions: Scaled 9.4 × 0.3 × 14 units
  ?? Position: Top
  ?? Color: Dark gray
  ?? Material: Textured

4.4 DOOR
  Type: Cube (animated)
  ?? Dimensions: 2.0 × 3.2 × 0.1 units
  ?? Hinge: Left edge
  ?? Animation: Rotates 0-90° (key B)
  ?? Texture: "wood.png"
  ?? Material: Wooden

4.5 INTERIOR SPACE
  ?? For Tube Lights (3 point lights)
     (Not primitives, but reference points)

TOTAL BARRACK PRIMITIVES: ~10 cubes per barrack
```

#### 5. **Campfire**
```
Object: Campfire
Position: (30, 0.1, -25)
Scale: 2.5×

Primitives: CUBE + animated fire effect

5.1 PEBBLE BASE
  Type: Cube
  ?? Dimensions: 2.0 × 0.4 × 2.0 units
  ?? Color: Gray/brown (0.5, 0.45, 0.4)
  ?? Texture: "pebble.png"
  ?? Material: Textured

5.2 FIRE GEOMETRY
  Type: Cube (scaled and rotated)
  ?? Number: 6 layers
  ?? Each Layer:
  ?  ?? Dimensions: Varies (smaller towards top)
  ?  ?? Rotation: Animated rotation
  ?  ?? Color: Orange/yellow with fade
  ?  ?? Opacity: Animated (0-1)
  ?? Height Stack: 0 to 3 units
  ?? Material: Transparent with glow

TOTAL CAMPFIRE PRIMITIVES: 1 base + 6 fire cubes = 7 cubes
```

#### 6. **Watch Towers (4 corners)**
```
Objects: WatchTower × 4
Positions:
  ?? Tower 1: (60, 0, -60) Northeast
  ?? Tower 2: (-60, 0, -60) Northwest
  ?? Tower 3: (-60, 0, 60) Southwest
  ?? Tower 4: (60, 0, 60) Southeast

Each Tower Primitives:

6.1 POST (CYLINDER)
  Type: Cylinder
  ?? Base Radius: 0.4 units
  ?? Height: 2.0 units
  ?? Segments: 100 (circumference)
  ?? Stacks: 36 (height)
  ?? Texture: "metal.jpeg"
  ?? Material: Textured

6.2 PLATFORM (CUBE)
  Type: Cube
  ?? Dimensions: 3.0 × 0.5 × 3.0 units
  ?? Position: Top of post
  ?? Texture: "wood.png"
  ?? Material: Textured

6.3 ROOF (CUBE)
  Type: Cube
  ?? Dimensions: 3.0 × 0.3 × 3.0 units
  ?? Position: Top of platform
  ?? Color: Gray
  ?? Material: Solid

6.4 RAILINGS (CUBES) × 4
  Type: Cube × 4
  ?? Each Railing:
  ?  ?? Dimensions: 0.1 × 1.0 × 2.8 units
  ?  ?? Position: Each side
  ?  ?? Color: Dark gray
  ?? Total: 4 cubes

TOTAL PER TOWER: 1 cylinder + 6 cubes = 7 primitives
TOTAL ALL TOWERS: 4 × 7 = 28 primitives
```

---

### SECTION D: VEHICLES

#### 7. **Jeep**
```
Object: Jeep (myJeep, myJeep2)
Class: Jeep.h
Positions: 
  ?? Jeep A: (-24.6, 1.3, 9.0)
  ?? Jeep B: (-19.4, 1.3, 9.0)
Scale: 1.15×

Primitives Used: CUBE + CYLINDER

7.1 BODY (CUBE)
  Type: Cube
  ?? Dimensions: 2.2 × 1.5 × 4.0 units
  ?? Texture: "jeep.png"
  ?? Material: Textured metallic

7.2 CABIN (CUBE)
  Type: Cube
  ?? Dimensions: 1.8 × 1.2 × 1.5 units
  ?? Position: Rear of body
  ?? Texture: "jeep.png"
  ?? Material: Textured

7.3 WINDSHIELD (CUBE)
  Type: Cube
  ?? Dimensions: 1.8 × 0.8 × 0.1 units
  ?? Position: Front top
  ?? Color: Light blue (semi-transparent effect)
  ?? Material: Solid

7.4 DOOR (CUBE) - Animated
  Type: Cube
  ?? Dimensions: 0.8 × 1.1 × 0.1 units
  ?? Animation: Rotates 0-70° (key D)
  ?? Hinge: Side
  ?? Texture: "jeep.png"
  ?? Material: Textured

7.5 WINDOWS (CUBE) × 2
  Type: Cube × 2
  ?? Dimensions: 0.6 × 0.7 × 0.1 units
  ?? Animation: Drop 0-0.35 units (key W)
  ?? Color: Light blue (transparent)
  ?? Material: Solid

7.6 HEADLIGHTS (CUBE) × 2
  Type: Cube × 2
  ?? Dimensions: 0.3 × 0.3 × 0.1 units
  ?? Position: Front corners
  ?? Color: Yellow/clear
  ?? Material: Glossy
  ?? Animation: Lights ON/OFF (key L)

7.7 WHEELS (CYLINDER) × 4
  Type: Cylinder × 4
  ?? Radius: 0.5 units
  ?? Width: 0.3 units
  ?? Segments: 48
  ?? Stacks: 24
  ?? Texture:
  ?  ?? Tyre (black cylinder): "blackTexture"
  ?  ?? Face (textured): "wheel.png"
  ?? Animation: Spin (updates with movement)
  ?? Positions: 4 corners under body

TOTAL JEEP PRIMITIVES PER VEHICLE:
  Cubes: 8
  Cylinders: 4
  ??????????
  TOTAL: 12 primitives per jeep
  FOR 2 JEEPS: 24 primitives
```

#### 8. **Truck**
```
Object: Truck (myTruck)
Class: Truck.h
Position: (-50, 1.3, 10)
Scale: 1.30×

Primitives Used: CUBE + CYLINDER

8.1 CABIN (CUBE)
  Type: Cube
  ?? Dimensions: 2.5 × 2.0 × 2.0 units
  ?? Position: Front
  ?? Texture: "truck.png"
  ?? Material: Textured metallic

8.2 WINDSHIELD (CUBE)
  Type: Cube
  ?? Dimensions: 2.4 × 1.5 × 0.1 units
  ?? Position: Front top
  ?? Color: Light blue (transparent)
  ?? Material: Solid

8.3 CARGO BED (CUBE)
  Type: Cube
  ?? Dimensions: 3.0 × 2.0 × 5.0 units
  ?? Position: Rear
  ?? Texture: "back.png"
  ?? Material: Textured

8.4 LIGHTS (CUBE) × 4
  Type: Cube × 4
  ?? Dimensions: 0.3 × 0.3 × 0.1 units
  ?? Position: Front 2 + Rear 2
  ?? Color: Red (rear) / Yellow (front)
  ?? Material: Solid

8.5 WHEELS (CYLINDER) × 6
  Type: Cylinder × 6
  ?? Arrangement: 2 front, 4 rear (dual rear)
  ?? Radius: 0.6 units
  ?? Width: 0.35 units
  ?? Segments: 48
  ?? Stacks: 24
  ?? Texture:
  ?  ?? Tyre (black): Procedural black
  ?  ?? Face: Textured
  ?? Animation: Spin (updates with movement)

8.6 BUMPER (CUBE) × 2
  Type: Cube × 2
  ?? Dimensions: 3.0 × 0.2 × 0.1 units
  ?? Position: Front + Rear
  ?? Color: Dark gray
  ?? Material: Solid

8.7 SIDE MIRRORS (CUBE) × 2
  Type: Cube × 2
  ?? Dimensions: 0.2 × 0.3 × 0.2 units
  ?? Position: Both sides
  ?? Color: Black
  ?? Material: Solid

TOTAL TRUCK PRIMITIVES:
  Cubes: 10
  Cylinders: 6
  ??????????
  TOTAL: 16 primitives
```

---

### SECTION E: BOUNDARY & GATES

#### 9. **Military Boundary**
```
Object: MilitaryBoundary
Dimensions:
  ?? Outer Width: 160 units
  ?? Outer Length: 150 units
  ?? Wall Height: 14 units

Primitives Used: CUBE

9.1 PERIMETER WALLS (4)
  Type: Cube × 4
  ?? Wall Thickness: 0.85 units
  ?? Height: 14 units
  ?? Each Wall:
  ?  ?? North Wall:
  ?  ?  ?? Dimensions: 160 × 14 × 0.85 units
  ?  ?  ?? Texture: "floor.png"
  ?  ?? South Wall:
  ?  ?  ?? Dimensions: 160 × 14 × 0.85 units
  ?  ?  ?? Texture: "floor.png"
  ?  ?? East Wall:
  ?  ?  ?? Dimensions: 0.85 × 14 × 150 units
  ?  ?  ?? Texture: "floor.png"
  ?  ?? West Wall:
     ?? Dimensions: 0.85 × 14 × 150 units
     ?? Texture: "floor.png"

9.2 GATE (Animated)
  Type: Cube
  ?? Dimensions: 26.0 × 10.5 × 0.4 units
  ?? Position: North wall opening
  ?? Animation: Rotates 0-95° (key U)
  ?? Hinge: Left side
  ?? Texture: "gate.png"
  ?? Material: Textured

TOTAL BOUNDARY PRIMITIVES: 5 cubes
```

---

### SECTION F: BUILDINGS & INTERIORS

#### 10. **Build Object (Quarters)**
```
Object: Build (myBuild)
Position: (-30, 0.1, -30)
Dimensions:
  ?? Width: 14 units
  ?? Depth: 18 units
  ?? Height: 6.5 units
  ?? Floor: 0.15 units

Primitives Used: CUBE

10.1 FLOOR
  Type: Cube
  ?? Dimensions: 14 × 0.15 × 18 units
  ?? Position: Ground level
  ?? Texture: "floor.png"
  ?? Material: Textured

10.2 WALLS (4)
  Type: Cube × 4
  ?? Front Wall:
  ?  ?? Dimensions: 14 × 6.5 × 0.3 units
  ?  ?? Door Opening: 2 units wide (animated)
  ?  ?? Texture: "build.png"
  ?? Back Wall:
  ?  ?? Dimensions: 14 × 6.5 × 0.3 units
  ?  ?? Texture: "build.png"
  ?? Left Wall:
  ?  ?? Dimensions: 0.3 × 6.5 × 18 units
  ?  ?? Window: 1.5×1.5 units (animated)
  ?  ?? Texture: "build.png"
  ?? Right Wall:
     ?? Dimensions: 0.3 × 6.5 × 18 units
     ?? Window: 1.5×1.5 units (animated)
     ?? Texture: "build.png"

10.3 DOOR (Animated)
  Type: Cube
  ?? Dimensions: 2.0 × 3.0 × 0.1 units
  ?? Animation: Rotates 0-90° (key G)
  ?? Hinge: Left side
  ?? Texture: "wood.png"
  ?? Material: Wooden

10.4 WINDOWS (Animated) × 2
  Type: Cube × 2
  ?? Left Window:
  ?  ?? Dimensions: 1.5 × 1.5 × 0.1 units
  ?  ?? Animation: Drops 0-1.2 units (key H)
  ?  ?? Texture: "build.png"
  ?? Right Window:
     ?? Dimensions: 1.5 × 1.5 × 0.1 units
     ?? Animation: Drops 0-1.2 units (key I)
     ?? Texture: "build.png"

10.5 ROOF
  Type: Cube
  ?? Dimensions: 14.3 × 0.3 × 18.3 units
  ?? Position: Top
  ?? Color: Dark gray
  ?? Material: Solid

10.6 INTERIOR FURNITURE

  10.6.1 BEDS × 2
    Type: Cube × 4 per bed (frame + 2 legs + mattress)
    Position: Back left and right corners
    Dimensions: 2.0 × 1.5 × 1.0 units each
    
  10.6.2 TABLE × 1
    Type: Cube × 2 (top + 4 legs)
    Position: Center front
    Dimensions: 2.0 × 0.8 × 1.5 units
    
  10.6.3 MEDICAL BOX
    Type: Cube
    Position: On table top
    Dimensions: 0.5 × 0.5 × 0.5 units
    Texture: "box.png"
    Color: White cross markings

TOTAL BUILD PRIMITIVES: ~15 cubes
```

---

### SECTION G: ROADS & PATHS

#### 11. **Military Road**
```
Object: MilitaryRoad
Dimensions:
  ?? Width: 16 units
  ?? Length: 260 units
  ?? Height: 0.05 units

Primitives: CUBE

11.1 MAIN ROAD SURFACE
  Type: Cube
  ?? Dimensions: 16 × 0.05 × 260 units
  ?? Position: (0, 0.11, 35)
  ?? Texture: "metal.jpeg" or solid
  ?? Material: Dark gray

TOTAL ROAD PRIMITIVES: 1 cube
```

---

### SECTION H: TERRAIN & LANDSCAPE

#### 12. **Trees (Fractal)**
```
Object: Fractal Tree
Depth: 4 recursive levels

Primitives: CUBE + custom branching

12.1 TRUNK (Levels 0-4)
  Type: Cube
  ?? Per Level:
  ?  ?? Scale: 0.5 × 0.75 × 0.5 × previous
  ?  ?? Dimensions (Level 0): 1.0 × 3.0 × 1.0 units
  ?  ?? Color: Brown (textured: "Treeroot.png")
  ?? Total Trunk Cubes: ~31 (2^0 + 2^1 + 2^2 + 2^3 + 2^4)
  ?? Material: Textured wood

12.2 LEAVES (Levels 1-4)
  Type: Bush (custom geometry)
  ?? Position: Top of each branch (when depth == 1)
  ?? Texture: "TreeLeaf.png"
  ?? Material: Transparent with alpha blending
  ?? Count: 16 leaf clusters (one per final branch)

12.3 RECURSION STRUCTURE
  Level 0: 1 trunk
  Level 1: 1 ? 2 branches (rotated ±45°)
           1 ? 2 branches (rotated ±45°, 90° offset)
  Level 2: 4 branches ? 8 branches
  Level 3: 8 branches ? 16 branches
  Level 4: 16 branches ? 32 branches

TOTAL TREE PRIMITIVES:
  Trunk Cubes: 63
  Leaves: 16 (Bush objects)
  ????????????
  TOTAL: 79 primitives per tree

TREES IN SCENE: ~3-4 trees
TOTAL TREE PRIMITIVES: ~250 primitives
```

---

### SECTION I: LANDSCAPE MODELS (GLTF)

#### 13. **Tank**
```
File: Tank_model/scene.gltf
Position: (0, 0.25, tank_z) - animated Z position
Scale: 0.0264×

Primitives: Multiple complex meshes in GLTF

Components (estimated):
  ?? Hull (tank body): Complex mesh
  ?? Turret: Complex mesh
  ?? Gun barrel: Cylinder
  ?? Treads: Complex geometry
  ?? Wheels: Cylinders
  ?? Details: Various primitives

TOTAL TANK PRIMITIVES: ~50-100 (varies by GLTF)
STATUS: Movable with animation (key T)
```

#### 14. **Tent (Military)**
```
File: tent_military/scene.gltf
Position: (30, -0.1, 10)
Scale: 0.015×

Primitives: Multiple meshes in GLTF

Components (estimated):
  ?? Canvas structure: Complex mesh
  ?? Poles: Cylinders
  ?? Ropes: Lines/cylinders
  ?? Ground connection: Cubes

TOTAL TENT PRIMITIVES: ~30-50 (varies by GLTF)
STATUS: Static (non-animated)
```

#### 15. **Radar Dish (High-res Sphere)**
```
Primitive: SPHERE
?? Radius: 2.0 units
?? Bands (latitude): 144
?? Slices (longitude): 72
?? Vertex Count: 10,368
?? Face Count: 10,224
?? Position: (43.375, 10.0, 23.5)
?? Rotation: 45° X-axis + animated Y-axis
?? Color: Light gray (0.92, 0.92, 0.92)
?? Texture: "radartex.jpg"
?? Material: Reflective
?? Animation: Rotates around Y-axis (key R)
?? Scale: 3.0×
```

#### 16. **Sun (Sphere)**
```
Primitive: SPHERE
?? Radius: 1.0 unit
?? Bands: 144
?? Slices: 72
?? Vertex Count: 10,368
?? Position: (80, 90, -80)
?? Scale: 6.0×
?? Color: Warm yellow-orange (1.0, 0.95, 0.4)
?? Material: Emissive (glowing)
?? Shininess: 64.0
```

---

## WRAPPING/TEXTURING SYSTEM

### Texture Wrapping Modes

#### GL_REPEAT (Default)
```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
```

**Used For**:
- Ground plane (sand texture tiles)
- Walls (brick/stone patterns)
- Floors (wood/tile patterns)
- Roofs

**Effect**: Texture repeats seamlessly across surface

**Example**: Ground
```
Tiling: 4.0× (groundTextureTiling)
Size: 200 × 200 units
Tile Size: 50 × 50 units (200/4)
Tiles: 16 × 16 = 256 repeats
```

---

#### GL_CLAMP_TO_EDGE
```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
```

**Used For**:
- Wheel face textures (no repetition needed)
- Doors (single texture)
- Windows (single texture)
- One-off textures

**Effect**: Texture stretches to fill area, no repeating

---

### Texture Filtering Modes

#### GL_LINEAR_MIPMAP_LINEAR (High Quality)
```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

**Used For**:
- Main textures (walls, floors, vehicles)
- High-visibility surfaces
- Distant objects (mipmapping)

**Mipmap Levels**:
```
Original: 1024 × 1024
Level 1:   512 × 512 (1/4 pixels)
Level 2:   256 × 256 (1/16 pixels)
Level 3:   128 × 128 (1/64 pixels)
... (continues to 1×1)
```

---

### Texture Files Used

| Texture File | Used For | Format | Wrapping | Filtering |
|--------------|----------|--------|----------|-----------|
| ff.png | Ground, cubes | PNG | REPEAT | LINEAR_MIPMAP |
| OIP.jpeg | Various cubes | JPEG | REPEAT | LINEAR_MIPMAP |
| metal.jpeg | Metal surfaces | JPEG | REPEAT | LINEAR_MIPMAP |
| wood.png | Barrack, doors | PNG | REPEAT | LINEAR_MIPMAP |
| pebble.png | Campfire base | PNG | REPEAT | LINEAR_MIPMAP |
| jeep.png | Jeep body | PNG | REPEAT | LINEAR_MIPMAP |
| wheel.png | Wheel faces | PNG | CLAMP | LINEAR_MIPMAP |
| wall.png | Walls, garage | PNG | REPEAT | LINEAR_MIPMAP |
| floor.png | Floors, platforms | PNG | REPEAT | LINEAR_MIPMAP |
| gate.png | Boundary gate | PNG | REPEAT | LINEAR_MIPMAP |
| build.png | Quarters building | PNG | REPEAT | LINEAR_MIPMAP |
| box.png | Medical box | PNG | REPEAT | LINEAR_MIPMAP |
| truck.png | Truck body | PNG | REPEAT | LINEAR_MIPMAP |
| back.png | Truck cargo bed | PNG | REPEAT | LINEAR_MIPMAP |
| radartex.jpg | Radar dish | JPEG | REPEAT | LINEAR_MIPMAP |
| Treeroot.png | Tree trunk | PNG | REPEAT | LINEAR_MIPMAP |
| TreeLeaf.png | Tree leaves | PNG | REPEAT | LINEAR_MIPMAP |

---

### Procedural Textures

#### Black Texture (Tyre Cylinders)
```cpp
unsigned int createProceduralTexture(int width, int height, 
                                     unsigned char r, unsigned char g, 
                                     unsigned char b)
{
    // Parameters: width=64, height=64
    // Colors: (0, 0, 0) = pure black
    // Pattern: Solid black (no checkerboard)
    // Used for: Jeep and truck wheel tyres
}
```

---

## OBJECT HIERARCHIES

### Geometric Composition Tree

```
MILITARY BASE CAMP SCENE
?
???? TERRAIN
?    ?? Ground Plane (1 cube)
?
???? STRUCTURES
?    ?? Jeep Garage (23 cubes)
?    ?  ?? Floor (1)
?    ?  ?? Ramp (6)
?    ?  ?? Walls (3)
?    ?  ?? Roof (1)
?    ?  ?? Lamps (12)
?    ?
?    ?? Truck Garage (23 cubes)
?    ?  ?? [Same as Jeep Garage]
?    ?
?    ?? Barracks × 2 (10 cubes each)
?    ?  ?? Floor
?    ?  ?? Walls × 4
?    ?  ?? Roof
?    ?  ?? Door (animated)
?    ?  ?? Windows × 2 (animated)
?    ?
?    ?? Campfire (7 cubes)
?    ?  ?? Base (1)
?    ?  ?? Fire Layers (6)
?    ?
?    ?? Watch Towers × 4 (7 each)
?    ?  ?? Post (1 cylinder)
?    ?  ?? Platform (1 cube)
?    ?  ?? Roof (1 cube)
?    ?  ?? Railings × 4 (4 cubes)
?    ?  ?? [Total: 28 primitives]
?    ?
?    ?? Military Boundary (5 cubes)
?    ?  ?? Walls × 4
?    ?  ?? Gate (animated)
?    ?
?    ?? Quarters Building (15 cubes)
?    ?  ?? Floor
?    ?  ?? Walls × 4 (with door + windows)
?    ?  ?? Roof
?    ?  ?? Furniture
?    ?      ?? Beds × 2
?    ?      ?? Table × 1
?    ?      ?? Medical Box
?    ?
?    ?? Military Road (1 cube)
?
???? VEHICLES
?    ?? Jeep A (12 primitives)
?    ?  ?? Body (1 cube)
?    ?  ?? Cabin (1 cube)
?    ?  ?? Windshield (1 cube)
?    ?  ?? Door (1 cube, animated)
?    ?  ?? Windows × 2 (cubes, animated)
?    ?  ?? Headlights × 2 (cubes)
?    ?  ?? Wheels × 4 (cylinders, animated)
?    ?
?    ?? Jeep B (12 primitives)
?    ?  ?? [Same structure as Jeep A]
?    ?
?    ?? Truck (16 primitives)
?    ?  ?? Cabin (1 cube)
?    ?  ?? Windshield (1 cube)
?    ?  ?? Cargo Bed (1 cube)
?    ?  ?? Lights × 4 (cubes)
?    ?  ?? Bumpers × 2 (cubes)
?    ?  ?? Side Mirrors × 2 (cubes)
?    ?  ?? Wheels × 6 (cylinders)
?    ?
?    ?? Tank (GLTF Model)
?    ?  ?? ~50-100 primitives
?    ?
?    ?? Tent (GLTF Model)
?        ?? ~30-50 primitives
?
???? LANDSCAPE
?    ?? Trees × 3-4 (79 each ? 250 total)
?    ?  ?? Trunk Levels (63 cubes)
?    ?  ?? Leaves (16 Bush objects)
?    ?
?    ?? Radar Dish (1 high-res sphere)
?    ?  ?? 10,368 vertices
?    ?
?    ?? Sun (1 sphere)
?        ?? 10,368 vertices
?
???? LIGHTING (Non-rendered)
     ?? Point Lights × 20
     ?? Directional Light × 1
     ?? Tube Lights × 3
     ?? Garage Lamps × 4
     ?? Spot Lights × 4 (Jeep headlights)
```

---

## SUMMARY STATISTICS

### Total Primitives Count

| Category | Cube Count | Cylinder Count | Sphere Count | GLTF Models | Total |
|----------|-----------|----------------|--------------|------------|-------|
| Terrain | 1 | - | - | - | 1 |
| Garages | 46 | - | - | - | 46 |
| Structures | 45 | 4 | - | - | 49 |
| Campfire | 7 | - | - | - | 7 |
| Vehicles | 28 | 10 | - | 2 | 40 |
| Landscape | 63 | - | 2 | - | 65 |
| Road | 1 | - | - | - | 1 |
| **TOTAL** | **191** | **14** | **2** | **2** | **209** |

### High-Resolution Geometry

| Object | Vertex Count | Triangle Count | Type |
|--------|-------------|-----------------|------|
| Radar Dish | 10,368 | 10,224 | Sphere |
| Sun | 10,368 | 10,224 | Sphere |
| **TOTAL** | **20,736** | **20,448** | Spheres |

### GLTF Models

| Model | File | Primitives (est.) | Status |
|-------|------|------------------|--------|
| Tank | Tank_model/scene.gltf | 50-100 | Animated |
| Tent | tent_military/scene.gltf | 30-50 | Static |
| Jeep | jeep/scene.gltf | - | Imported |
| Truck | truck/scene.gltf | - | Imported |

---

## RENDERING PERFORMANCE NOTES

### Vertex Count by Object Type

```
CUBES (191 total):
  Vertices per cube: 36
  Total cube vertices: 191 × 36 = 6,876
  
CYLINDERS (14 total):
  Segments × Stacks per cylinder: 48 × 24 = 1,152
  Total cylinder vertices: 14 × 1,152 = 16,128
  
SPHERES (2 total):
  Bands × Slices per sphere: 144 × 72 = 10,368
  Total sphere vertices: 2 × 10,368 = 20,736
  
TOTAL VERTICES: 43,740 (excluding GLTF models)
```

### Draw Call Optimization

**Current Approach**:
- Shared VAO for all cubes (cubeVAO)
- Individual VAO for complex shapes
- Material-based batching

**Potential Improvements**:
- Instancing for repeated cubes
- Mesh compression for high-res spheres
- LOD (Level of Detail) for distant objects

---

This documentation provides complete understanding of all primitives, their wrapping modes, and structural composition in the military base camp scene.
