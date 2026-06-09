# MILITARY BASE CAMP - OBJECTS, PRIMITIVES, WRAPPING & TEXTURES

## Table of Contents
1. [Texture File Reference](#texture-file-reference)
2. [All Objects with Details](#all-objects-with-details)
3. [Wrapping & Filtering Modes](#wrapping--filtering-modes)
4. [Quick Reference Table](#quick-reference-table)

---

## TEXTURE FILE REFERENCE

### All Textures in Project

| Texture File | Format | Resolution | Wrapping | Filtering | Purpose |
|--------------|--------|-----------|----------|-----------|---------|
| ff.png | PNG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Ground, general cubes |
| OIP.jpeg | JPEG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Various cubes, radar base |
| metal.jpeg | JPEG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Metal surfaces, watchtower posts |
| wood.png | PNG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Barrack doors, furniture, watchtower platforms |
| pebble.png | PNG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Campfire pebble base |
| jeep.png | PNG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | **Jeep body, cabin, door** |
| wheel.png | PNG | 512×512 | GL_CLAMP_TO_EDGE | GL_LINEAR_MIPMAP_LINEAR | Wheel face (front-facing disc) |
| wall.png | PNG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Garage walls, roof, boundary perimeter |
| floor.png | PNG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Garage floor, boundaries, building floors |
| gate.png | PNG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Military boundary gate |
| build.png | PNG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Quarters building walls, windows |
| box.png | PNG | 512×512 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Medical box on table |
| truck.png | PNG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | **Truck cabin, body, windshield** |
| back.png | PNG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | **Truck cargo bed (rear)** |
| radartex.jpg | JPEG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Radar dish sphere |
| Treeroot.png | PNG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Tree trunk/branches (cubes) |
| TreeLeaf.png | PNG | 1024×1024 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | Tree leaves (Bush objects) |
| **blackTexture** | Procedural | 64×64 | GL_REPEAT | GL_LINEAR_MIPMAP_LINEAR | **Wheel tyres (black)** - Procedural (0,0,0) |

---

## ALL OBJECTS WITH DETAILS

### 1. GROUND PLANE
```
Object Name: cube (ground)
Position: (0, 0, 0) - centered at origin
Dimensions: 200 × 0.1 × 200 units

Primitive: CUBE (1)
?? Type: Box
?? Vertices: 36
?? Faces: 6

Wrapping Mode: GL_REPEAT (tiles texture)
Filtering: GL_LINEAR_MIPMAP_LINEAR
Texture: ff.png
?? Tiling: 4.0× (groundTextureTiling variable)
?? Tile Size: 50 × 50 units
?? Total Tiles: 16 × 16 = 256 repeats

Render Mode: Toggleable (key M)
?? Mode 0: Color only (0.8, 0.8, 0.7)
?? Mode 1: Texture only (ff.png)
?? Mode 2: Blend (50/50 color + texture)

Material Properties:
?? Ambient: Textured
?? Diffuse: Textured (ff.png)
?? Specular: Textured
?? Shininess: 32.0

VAO: cubeVAO (shared with all cubes)
```

---

### 2. JEEP GARAGE PLATFORM

#### 2.1 Garage Platform Floor
```
Object Name: garagePlatform (floor section)
Position: (-22, 0.1, 10)
Inner Dimensions: 14 × 18 units

Primitive: CUBE (1)
?? Type: Box
?? Dimensions: 15.2 × 1.2 × 19.2 units
?? Vertices: 36

Wrapping Mode: GL_REPEAT
Filtering: GL_LINEAR_MIPMAP_LINEAR
Texture: floor.png
?? Applied to all 6 faces
?? Tiling: 1× (full texture per face)
?? Effect: Wooden plank pattern

Material Properties:
?? Ambient: (0.5, 0.5, 0.5)
?? Diffuse: floor.png
?? Specular: floor.png
?? Shininess: 32.0

VAO: cubeVAO (shared)
```

#### 2.2 Garage Platform Ramp
```
Object Name: garagePlatform (ramp)
Position: (-22, 0.1 - 1.2, 10 + 9.6) - front of garage

Primitives: CUBE (6 total)
?? Main Ramp Incline (1 cube)
?  ?? Dimensions: 10.5 × 0.22 × 4.3 units
?  ?? Rotation: ~16.7° angle
?  ?? Texture: floor.png
?  ?? Wrapping: GL_REPEAT
?
?? Support Block (1 cube)
?  ?? Dimensions: 10.5 × 0.36 × 1.5 units
?  ?? Color: Solid gray (0.30, 0.28, 0.26)
?  ?? Material: Solid (no texture)
?  ?? Vertices: 36
?
?? Stairs (4 cubes)
   ?? Each Step:
   ?  ?? Dimensions: 3.6 × increasing height × 1.075 units
   ?  ?? Color: Solid gray
   ?  ?? Material: Solid (no texture)
   ?  ?? Vertices: 36
   ?? Total: 4 cubes

VAO: cubeVAO (shared)
```

#### 2.3 Garage Platform Walls (3)
```
Object Name: garagePlatform (walls)
Position: Around garage perimeter

Primitives: CUBE (3 total)

LEFT WALL:
?? Type: Box
?? Dimensions: 0.4 × 7 × 19.2 units
?? Wrapping Mode: GL_REPEAT
?? Texture: wall.png
?? Material:
?  ?? Diffuse: wall.png
?  ?? Specular: wall.png
?  ?? Shininess: 32.0
?? Vertices: 36

RIGHT WALL:
?? Type: Box
?? Dimensions: 0.4 × 7 × 19.2 units
?? Wrapping Mode: GL_REPEAT
?? Texture: wall.png
?? Material: Same as left
?? Vertices: 36

BACK WALL:
?? Type: Box
?? Dimensions: 15.2 × 7 × 0.4 units
?? Wrapping Mode: GL_REPEAT
?? Texture: wall.png
?? Material: Same as left
?? Vertices: 36

VAO: cubeVAO (shared)
```

#### 2.4 Garage Platform Roof
```
Object Name: garagePlatform (roof)
Position: Top of garage (Y = 7)

Primitive: CUBE (1)
?? Type: Box
?? Dimensions: 15.2 × 0.3 × 19.2 units
?? Wrapping Mode: GL_REPEAT
?? Texture: wall.png
?? Material:
?  ?? Diffuse: wall.png
?  ?? Specular: wall.png
?  ?? Shininess: 32.0
?? Vertices: 36

VAO: cubeVAO (shared)
```

#### 2.5 Garage Platform Lamps (4 corner lamps)
```
Object Name: garagePlatform (lamps)
Positions: 4 corners of garage front

EACH LAMP consists of 3 CUBES:

LAMP BASE PLATFORM:
?? Primitive: CUBE (1)
?? Dimensions: 0.5 × 0.24 × 0.5 units
?? Color: Solid (0.30, 0.30, 0.32)
?? Material: Solid (no texture)
?? Vertices: 36

VERTICAL POLE:
?? Primitive: CUBE (1)
?? Dimensions: 0.12 × 7.2 × 0.12 units
?? Color: Solid (0.50, 0.50, 0.52)
?? Material: Solid (no texture)
?? Vertices: 36

LAMP HEAD ASSEMBLY:
?? Main Head Cube:
?  ?? Dimensions: 0.756 × 0.252 × 0.756 units
?  ?? Color: Warm gold (0.96, 0.92, 0.72)
?  ?? Material: Solid with high specularity
?  ?? Shininess: 64.0
?  ?? Vertices: 36
?
?? Reflector Ring Cube:
?  ?? Dimensions: 0.882 × 0.084 × 0.882 units
?  ?? Position: Top of head
?  ?? Color: Dark (0.20, 0.20, 0.22)
?  ?? Vertices: 36
?
?? Grate Cube:
   ?? Dimensions: 0.63 × 0.0672 × 0.63 units
   ?? Position: Bottom of head
   ?? Color: Very dark (0.18, 0.18, 0.18)
   ?? Vertices: 36

TOTAL LAMP PRIMITIVES: 4 per lamp × 4 lamps = 16 cubes
VAO: cubeVAO (shared)

GARAGE 1 TOTAL: 23 cubes
```

---

### 3. TRUCK GARAGE PLATFORM (Same as Jeep Garage)
```
Object Name: truckGaragePlatform
Position: (-50, 0.1, 10)
Dimensions: Larger (16 × 20 units)

Primitives: CUBE (23 total)
?? Floor: 1 (floor.png with GL_REPEAT)
?? Ramp: 6 (floor.png with GL_REPEAT for incline)
?? Walls: 3 (wall.png with GL_REPEAT)
?? Roof: 1 (wall.png with GL_REPEAT)
?? Lamps: 12 (solid colors, no texture)

All Wrapping: GL_REPEAT
All Filtering: GL_LINEAR_MIPMAP_LINEAR
VAO: cubeVAO (shared)

TRUCK GARAGE TOTAL: 23 cubes
```

---

### 4. BARRACK BUILDING (×2)

```
Object Name: Barrack / Barrack2
Positions:
?? Barrack 1: (40, 0.1, -40) - Scaled 0.66×
?? Barrack 2: (40, 0.1, -10) - Scaled 0.66×

FLOOR:
?? Primitive: CUBE (1)
?? Dimensions (scaled): 9 × 0.3 × 13.5 units
?? Wrapping: GL_REPEAT
?? Texture: wood.png
?? Material:
?  ?? Diffuse: wood.png
?  ?? Specular: wood.png
?  ?? Shininess: 32.0
?? Vertices: 36

FRONT WALL:
?? Primitive: CUBE (1)
?? Dimensions (scaled): 9 × 3.5 × 0.4 units
?? Door Opening: 2 units wide
?? Wrapping: GL_REPEAT
?? Texture: wood.png
?? Vertices: 36

BACK WALL:
?? Primitive: CUBE (1)
?? Dimensions (scaled): 9 × 3.5 × 0.4 units
?? Wrapping: GL_REPEAT
?? Texture: wood.png
?? Vertices: 36

LEFT WALL:
?? Primitive: CUBE (1)
?? Dimensions (scaled): 0.4 × 3.5 × 13.5 units
?? Wrapping: GL_REPEAT
?? Texture: wood.png
?? Vertices: 36

RIGHT WALL:
?? Primitive: CUBE (1)
?? Dimensions (scaled): 0.4 × 3.5 × 13.5 units
?? Wrapping: GL_REPEAT
?? Texture: wood.png
?? Vertices: 36

ROOF:
?? Primitive: CUBE (1)
?? Dimensions (scaled): 9.4 × 0.3 × 14 units
?? Wrapping: GL_REPEAT
?? Texture: wood.png
?? Vertices: 36

DOOR (Animated):
?? Primitive: CUBE (1)
?? Dimensions: 2.0 × 3.2 × 0.1 units
?? Animation: Rotates 0-90° (key B)
?? Hinge: Left edge
?? Wrapping: GL_REPEAT
?? Texture: wood.png
?? Material:
?  ?? Diffuse: wood.png
?  ?? Specular: wood.png
?  ?? Shininess: 32.0
?? Vertices: 36

TOTAL PER BARRACK: 8 cubes
TOTAL FOR 2 BARRACKS: 16 cubes

All Wrapping: GL_REPEAT
All Filtering: GL_LINEAR_MIPMAP_LINEAR
VAO: cubeVAO (shared)
```

---

### 5. CAMPFIRE

```
Object Name: Campfire
Position: (30, 0.1, -25)
Scale: 2.5×

PEBBLE BASE:
?? Primitive: CUBE (1)
?? Dimensions: 2.0 × 0.4 × 2.0 units
?? Wrapping: GL_REPEAT
?? Texture: pebble.png
?? Material:
?  ?? Color: Gray/brown (0.5, 0.45, 0.4)
?  ?? Diffuse: pebble.png
?  ?? Specular: pebble.png
?  ?? Shininess: 32.0
?? Vertices: 36

FIRE LAYERS (6 cubes stacked):
?? Layer 1 (Bottom):
?  ?? Primitive: CUBE (1)
?  ?? Dimensions: 2.0 × 0.3 × 2.0 units
?  ?? Color: Orange (1.0, 0.6, 0.2)
?  ?? Opacity: Animated (0-1)
?  ?? Material: Transparent with glow
?  ?? Vertices: 36
?
?? Layers 2-6:
?  ?? Each is CUBE (1)
?  ?? Scaling: Gets smaller toward top
?  ?? Height: Stacked progression
?  ?? Color: Orange to yellow gradient
?  ?? Opacity: Animated
?  ?? Vertices: 36 per layer
?
?? Total Fire: 6 cubes

TOTAL CAMPFIRE PRIMITIVES: 7 cubes
Wrapping: GL_REPEAT
Filtering: GL_LINEAR_MIPMAP_LINEAR
VAO: cubeVAO (shared)
```

---

### 6. WATCH TOWERS (4 total - one per corner)

```
Object Names: WatchTower1, WatchTower2, WatchTower3, WatchTower4
Positions:
?? Tower 1 (NE): (60, 0, -60)
?? Tower 2 (NW): (-60, 0, -60)
?? Tower 3 (SW): (-60, 0, 60)
?? Tower 4 (SE): (60, 0, 60)

EACH TOWER:

POST (Cylinder):
?? Primitive: CYLINDER
?? Base Radius: 0.4 units
?? Height: 2.0 units
?? Segments: 100 (high fidelity)
?? Stacks: 36
?? Wrapping: GL_REPEAT
?? Texture: metal.jpeg
?? Material:
?  ?? Diffuse: metal.jpeg
?  ?? Specular: metal.jpeg
?  ?? Shininess: 32.0
?? Vertices: 3,600 (100 segments × 36 stacks)

PLATFORM (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 3.0 × 0.5 × 3.0 units
?? Position: Top of cylinder
?? Wrapping: GL_REPEAT
?? Texture: wood.png
?? Material:
?  ?? Diffuse: wood.png
?  ?? Specular: wood.png
?  ?? Shininess: 32.0
?? Vertices: 36

ROOF (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 3.0 × 0.3 × 3.0 units
?? Position: Top of platform
?? Color: Solid dark gray
?? Material: Solid (no texture)
?? Vertices: 36

RAILINGS (Cubes × 4):
?? Primitive: CUBE (4 total)
?? Dimensions per railing: 0.1 × 1.0 × 2.8 units
?? Positions: One per side of platform
?? Color: Dark gray (0.3, 0.3, 0.3)
?? Material: Solid (no texture)
?? Vertices: 36 per railing

TOTAL PER TOWER: 1 cylinder + 6 cubes = 7 primitives
TOTAL FOR 4 TOWERS: 28 primitives

All Wrapping: GL_REPEAT
All Filtering: GL_LINEAR_MIPMAP_LINEAR
VAO: cubeVAO (shared for cubes)
```

---

### 7. JEEP A & B (×2 vehicles)

```
Object Names: myJeep (Jeep A), myJeep2 (Jeep B)
Positions:
?? Jeep A: (-24.6, 1.3, 9.0)
?? Jeep B: (-19.4, 1.3, 9.0)
Scale: 1.15×

EACH JEEP:

BODY (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 2.2 × 1.5 × 4.0 units
?? Wrapping: GL_REPEAT
?? Texture: **jeep.png**
?? Material:
?  ?? Diffuse: jeep.png
?  ?? Specular: jeep.png
?  ?? Shininess: 32.0
?? Vertices: 36

CABIN (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 1.8 × 1.2 × 1.5 units
?? Position: Rear of body
?? Wrapping: GL_REPEAT
?? Texture: **jeep.png**
?? Material:
?  ?? Diffuse: jeep.png
?  ?? Specular: jeep.png
?  ?? Shininess: 32.0
?? Vertices: 36

WINDSHIELD (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 1.8 × 0.8 × 0.1 units
?? Position: Front top
?? Color: Light blue (0.7, 0.8, 0.9)
?? Material: Solid - semi-transparent effect
?? Wrapping: None (solid color)
?? Vertices: 36

DOOR (Cube - Animated):
?? Primitive: CUBE (1)
?? Dimensions: 0.8 × 1.1 × 0.1 units
?? Animation: Rotates 0-70° (key D)
?? Hinge: Left side
?? Wrapping: GL_REPEAT
?? Texture: **jeep.png**
?? Material:
?  ?? Diffuse: jeep.png
?  ?? Specular: jeep.png
?  ?? Shininess: 32.0
?? Vertices: 36

WINDOWS (Cubes × 2 - Animated):
?? Primitive: CUBE (2)
?? Dimensions: 0.6 × 0.7 × 0.1 units each
?? Animation: Drop 0-0.35 units (key W)
?? Color: Light blue (0.7, 0.8, 0.9)
?? Material: Solid - semi-transparent
?? Wrapping: None (solid color)
?? Vertices: 36 per window

HEADLIGHTS (Cubes × 2):
?? Primitive: CUBE (2)
?? Dimensions: 0.3 × 0.3 × 0.1 units each
?? Positions: Front left & right corners
?? Color: Yellow/clear (1.0, 1.0, 0.3)
?? Material: Glossy - animated ON/OFF (key L)
?? Wrapping: None (solid color)
?? Vertices: 36 per headlight

WHEELS (Cylinders × 4):
?? Primitive: CYLINDER (4 total)
?? Radius: 0.5 units
?? Width: 0.3 units
?? Segments: 48
?? Stacks: 24
?? Tyre (outer cylinder):
?  ?? Texture: **blackTexture** (procedural)
?  ?? Wrapping: GL_REPEAT
?  ?? Color: Pure black (0, 0, 0)
?  ?? Material: Solid rubber-like
?
?? Wheel Face (inner disc texture):
?  ?? Texture: **wheel.png**
?  ?? Wrapping: GL_CLAMP_TO_EDGE (single image)
?  ?? Filtering: GL_LINEAR_MIPMAP_LINEAR
?  ?? Material: Metallic
?
?? Animation: Spin (updates with movement)
?? Vertices: 2,304 per cylinder (48 × 24 × 2)

TOTAL PER JEEP: 8 cubes + 4 cylinders = 12 primitives
TOTAL FOR 2 JEEPS: 24 primitives

Primary Texture: **jeep.png** (GL_REPEAT)
Secondary Texture: **wheel.png** (GL_CLAMP_TO_EDGE)
Procedural Texture: **blackTexture** (GL_REPEAT)
VAO: cubeVAO (for cubes) + cylinder VAO
```

---

### 8. TRUCK

```
Object Name: myTruck
Position: (-50, 1.3, 10)
Scale: 1.30×

CABIN (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 2.5 × 2.0 × 2.0 units
?? Position: Front
?? Wrapping: GL_REPEAT
?? Texture: **truck.png**
?? Material:
?  ?? Diffuse: truck.png
?  ?? Specular: truck.png
?  ?? Shininess: 32.0
?? Vertices: 36

WINDSHIELD (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 2.4 × 1.5 × 0.1 units
?? Position: Front top
?? Color: Light blue (0.7, 0.8, 0.9)
?? Material: Solid - semi-transparent
?? Wrapping: None (solid color)
?? Vertices: 36

CARGO BED (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 3.0 × 2.0 × 5.0 units
?? Position: Rear
?? Wrapping: GL_REPEAT
?? Texture: **back.png** (truck cargo texture)
?? Material:
?  ?? Diffuse: back.png
?  ?? Specular: back.png
?  ?? Shininess: 32.0
?? Vertices: 36

LIGHTS (Cubes × 4):
?? Primitive: CUBE (4)
?? Dimensions: 0.3 × 0.3 × 0.1 units each
?? Positions: Front 2 (yellow) + Rear 2 (red)
?? Colors:
?  ?? Front: Yellow (1.0, 1.0, 0.0)
?  ?? Rear: Red (1.0, 0.0, 0.0)
?? Material: Solid - glowing
?? Wrapping: None (solid color)
?? Vertices: 36 per light

WHEELS (Cylinders × 6):
?? Primitive: CYLINDER (6 total)
?? Arrangement: 2 front + 4 rear (dual rear wheels)
?? Radius: 0.6 units
?? Width: 0.35 units
?? Segments: 48
?? Stacks: 24
?? Tyre (outer cylinder):
?  ?? Texture: **blackTexture** (procedural)
?  ?? Wrapping: GL_REPEAT
?  ?? Color: Pure black (0, 0, 0)
?  ?? Material: Solid rubber-like
?
?? Wheel Face:
?  ?? Texture: **wheel.png**
?  ?? Wrapping: GL_CLAMP_TO_EDGE
?  ?? Material: Metallic
?
?? Animation: Spin (updates with movement)
?? Vertices: 2,304 per cylinder

BUMPERS (Cubes × 2):
?? Primitive: CUBE (2)
?? Dimensions: 3.0 × 0.2 × 0.1 units each
?? Positions: Front + Rear
?? Color: Dark gray (0.3, 0.3, 0.3)
?? Material: Solid (no texture)
?? Vertices: 36 per bumper

SIDE MIRRORS (Cubes × 2):
?? Primitive: CUBE (2)
?? Dimensions: 0.2 × 0.3 × 0.2 units each
?? Positions: Both sides
?? Color: Black (0.1, 0.1, 0.1)
?? Material: Solid - highly reflective
?? Vertices: 36 per mirror

TOTAL TRUCK PRIMITIVES: 10 cubes + 6 cylinders = 16 primitives

Primary Textures:
?? **truck.png** (cabin) - GL_REPEAT
?? **back.png** (cargo bed) - GL_REPEAT
?? **wheel.png** (wheel faces) - GL_CLAMP_TO_EDGE

Procedural Texture:
?? **blackTexture** (tyres) - GL_REPEAT

VAO: cubeVAO (for cubes) + cylinder VAO
```

---

### 9. MILITARY BOUNDARY (PERIMETER WALLS + GATE)

```
Object Name: MilitaryBoundary
Position: (0, 0.1, 0) - centered at origin
Dimensions: 160 × 14 × 150 units

NORTH WALL (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 160 × 14 × 0.85 units
?? Wrapping: GL_REPEAT
?? Texture: **wall.png** OR **floor.png**
?? Material:
?  ?? Diffuse: wall.png
?  ?? Specular: wall.png
?  ?? Shininess: 32.0
?? Vertices: 36

SOUTH WALL (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 160 × 14 × 0.85 units
?? Wrapping: GL_REPEAT
?? Texture: wall.png
?? Material: Same as North
?? Vertices: 36

EAST WALL (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 0.85 × 14 × 150 units
?? Wrapping: GL_REPEAT
?? Texture: wall.png
?? Material: Same as North
?? Vertices: 36

WEST WALL (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 0.85 × 14 × 150 units
?? Wrapping: GL_REPEAT
?? Texture: wall.png
?? Material: Same as North
?? Vertices: 36

GATE (Cube - Animated):
?? Primitive: CUBE (1)
?? Dimensions: 26.0 × 10.5 × 0.4 units
?? Position: North wall opening
?? Animation: Rotates 0-95° (key U)
?? Hinge: Left side
?? Wrapping: GL_REPEAT
?? Texture: **gate.png**
?? Material:
?  ?? Diffuse: gate.png
?  ?? Specular: gate.png
?  ?? Shininess: 32.0
?? Vertices: 36

TOTAL BOUNDARY PRIMITIVES: 5 cubes

Primary Texture:
?? Walls: **wall.png** - GL_REPEAT
?? Gate: **gate.png** - GL_REPEAT

VAO: cubeVAO (shared)
```

---

### 10. QUARTERS BUILDING (BUILD OBJECT)

```
Object Name: myBuild
Position: (-30, 0.1, -30)
Dimensions: 14 × 18 units, Height 6.5 units

FLOOR (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 14 × 0.15 × 18 units
?? Wrapping: GL_REPEAT
?? Texture: **floor.png**
?? Material:
?  ?? Diffuse: floor.png
?  ?? Specular: floor.png
?  ?? Shininess: 32.0
?? Vertices: 36

FRONT WALL (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 14 × 6.5 × 0.3 units
?? Door Opening: 2 units wide
?? Wrapping: GL_REPEAT
?? Texture: **build.png**
?? Material:
?  ?? Diffuse: build.png
?  ?? Specular: build.png
?  ?? Shininess: 32.0
?? Vertices: 36

BACK WALL (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 14 × 6.5 × 0.3 units
?? Wrapping: GL_REPEAT
?? Texture: build.png
?? Material: Same as front
?? Vertices: 36

LEFT WALL (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 0.3 × 6.5 × 18 units
?? Window Opening: 1.5×1.5 units
?? Wrapping: GL_REPEAT
?? Texture: **build.png**
?? Material: Same as front
?? Vertices: 36

RIGHT WALL (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 0.3 × 6.5 × 18 units
?? Window Opening: 1.5×1.5 units
?? Wrapping: GL_REPEAT
?? Texture: build.png
?? Material: Same as front
?? Vertices: 36

DOOR (Cube - Animated):
?? Primitive: CUBE (1)
?? Dimensions: 2.0 × 3.0 × 0.1 units
?? Animation: Rotates 0-90° (key G)
?? Hinge: Left side
?? Wrapping: GL_REPEAT
?? Texture: **wood.png**
?? Material:
?  ?? Diffuse: wood.png
?  ?? Specular: wood.png
?  ?? Shininess: 32.0
?? Vertices: 36

LEFT WINDOW (Cube - Animated):
?? Primitive: CUBE (1)
?? Dimensions: 1.5 × 1.5 × 0.1 units
?? Animation: Drops 0-1.2 units (key H)
?? Color: Light blue (0.7, 0.8, 0.9)
?? Material: Solid - semi-transparent
?? Vertices: 36

RIGHT WINDOW (Cube - Animated):
?? Primitive: CUBE (1)
?? Dimensions: 1.5 × 1.5 × 0.1 units
?? Animation: Drops 0-1.2 units (key I)
?? Color: Light blue (0.7, 0.8, 0.9)
?? Material: Solid - semi-transparent
?? Vertices: 36

ROOF (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 14.3 × 0.3 × 18.3 units
?? Color: Dark gray
?? Material: Solid (no texture)
?? Vertices: 36

INTERIOR FURNITURE:

BED 1 (Cubes × 4):
?? Primitive: CUBE (4)
?? Dimensions: 2.0 × 1.5 × 1.0 units
?? Position: Back left corner
?? Color: Brown (wood-like)
?? Material: Solid (no texture)
?? Vertices: 36 per part

BED 2 (Cubes × 4):
?? Primitive: CUBE (4)
?? Dimensions: 2.0 × 1.5 × 1.0 units
?? Position: Back right corner
?? Color: Brown (wood-like)
?? Material: Solid (no texture)
?? Vertices: 36 per part

TABLE (Cubes × 2):
?? Primitive: CUBE (2)
?? Dimensions: 2.0 × 0.8 × 1.5 units
?? Position: Center front
?? Color: Brown (wood-like)
?? Material: Solid (no texture)
?? Vertices: 36 per part

MEDICAL BOX (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 0.5 × 0.5 × 0.5 units
?? Position: On table top
?? Wrapping: GL_REPEAT
?? Texture: **box.png**
?? Material:
?  ?? Diffuse: box.png
?  ?? Specular: box.png
?  ?? Shininess: 32.0
?? Vertices: 36

TOTAL BUILD PRIMITIVES: 15 cubes

Primary Textures:
?? Walls: **build.png** - GL_REPEAT
?? Floor: **floor.png** - GL_REPEAT
?? Door: **wood.png** - GL_REPEAT
?? Medical Box: **box.png** - GL_REPEAT

VAO: cubeVAO (shared)
```

---

### 11. MILITARY ROAD

```
Object Name: MilitaryRoad (road)
Position: (0, 0.11, 35) - on top of ground
Dimensions: 16 × 0.05 × 260 units

ROAD SURFACE (Cube):
?? Primitive: CUBE (1)
?? Dimensions: 16 × 0.05 × 260 units
?? Wrapping: GL_REPEAT
?? Texture: **metal.jpeg** (asphalt-like)
?? Material:
?  ?? Diffuse: metal.jpeg
?  ?? Specular: metal.jpeg
?  ?? Shininess: 32.0
?? Vertices: 36

TOTAL ROAD PRIMITIVES: 1 cube

Primary Texture: **metal.jpeg** - GL_REPEAT

VAO: cubeVAO (shared)
```

---

### 12. FRACTAL TREES (3-4 trees in scene)

```
Object Name: Fractal Tree (recursive)
Position: (-15, 0, 3) - main tree location
Depth: 4 recursive levels

TRUNK LEVELS (63 cubes total):
?? Level 0: 1 cube
?  ?? Dimensions: 1.0 × 3.0 × 1.0 units
?  ?? Scale: Base level
?  ?? Wrapping: GL_REPEAT
?
?? Level 1: 2 cubes
?  ?? Dimensions: 0.5 × 2.25 × 0.5 units (scaled)
?  ?? Rotation: ±45° on Z-axis + 90° offset
?  ?? Wrapping: GL_REPEAT
?
?? Level 2: 4 cubes
?  ?? Dimensions: 0.25 × 1.69 × 0.25 units
?  ?? Rotation: Progressive
?  ?? Wrapping: GL_REPEAT
?
?? Level 3: 8 cubes
?  ?? Scaling: Continues 0.5 × 0.75 factor
?  ?? Wrapping: GL_REPEAT
?
?? Level 4: 16 cubes
   ?? Dimensions: Smallest scale
   ?? Wrapping: GL_REPEAT

ALL TRUNK CUBES:
?? Texture: **Treeroot.png**
?? Wrapping: GL_REPEAT
?? Material:
?  ?? Diffuse: Treeroot.png
?  ?? Specular: Treeroot.png
?  ?? Shininess: 32.0
?? Vertices: 36 per cube

LEAF CLUSTERS (16 Bush objects):
?? Primitive: BUSH (custom geometry)
?? Count: 16 (one per final branch)
?? Positions: Top of each Level 4 branch
?? Texture: **TreeLeaf.png**
?? Material:
?  ?? Diffuse: TreeLeaf.png
?  ?? Specular: TreeLeaf.png
?  ?? Shininess: 32.0
?  ?? Transparency: Alpha blending enabled
?? Wrapping: GL_REPEAT

RECURSION PATTERN:
?? Each branch spawns 2 children
?? Rotations: ±45° on Z, ±45° on X with 90° Y offset
?? Scale factor per level: 0.5 × 0.75 × 0.5
?? Total structure: 31 trunk cubes + 16 leaf clusters

TOTAL PER TREE: 63 cubes + 16 Bush objects = 79 primitives
TOTAL FOR 3-4 TREES: ~250 primitives

Primary Textures:
?? Trunk: **Treeroot.png** - GL_REPEAT
?? Leaves: **TreeLeaf.png** - GL_REPEAT

VAO: cubeVAO (for trunk cubes) + Bush VAO
```

---

### 13. RADAR DISH

```
Object Name: SphereWithTexture (radar)
Position: (43.375, 10.0, 23.5)
Scale: 3.0×

SPHERE PRIMITIVE (1):
?? Radius: 2.0 units (× 3.0 scale = 6.0 effective)
?? Bands (latitude): 144
?? Slices (longitude): 72
?? Vertex Count: 10,368 (144 × 72)
?? Triangle Count: 10,224
?? Wrapping: GL_REPEAT
?? Texture: **radartex.jpg**
?? Material:
?  ?? Color: Light gray (0.92, 0.92, 0.92)
?  ?? Diffuse: radartex.jpg
?  ?? Specular: radartex.jpg
?  ?? Shininess: 64.0
?
?? Rotation:
?  ?? Static: 45° on X-axis
?  ?? Animated: Rotates on Y-axis (key R)
?  ?? Animation: RotateRadar flag
?
?? Vertices: 10,368

TOTAL RADAR PRIMITIVES: 1 sphere

Primary Texture: **radartex.jpg** - GL_REPEAT

VAO: Sphere VAO (dedicated)
```

---

### 14. SUN

```
Object Name: Sphere (sun)
Position: (80, 90, -80) - high in sky
Scale: 6.0×

SPHERE PRIMITIVE (1):
?? Radius: 1.0 unit (× 6.0 scale = 6.0 effective)
?? Bands: 144
?? Slices: 72
?? Vertex Count: 10,368
?? Triangle Count: 10,224
?? Material:
?  ?? Ambient: (1.0, 0.95, 0.4) - warm yellow
?  ?? Diffuse: (1.0, 0.95, 0.4)
?  ?? Specular: (1.0, 1.0, 0.8)
?  ?? Shininess: 64.0
?
?? Rendering:
?  ?? No texture (solid color)
?  ?? Emissive effect (glowing)
?  ?? Color-based rendering
?
?? Vertices: 10,368

TOTAL SUN PRIMITIVES: 1 sphere

Material: Solid - No texture wrapping needed

VAO: Sphere VAO (dedicated)
```

---

### 15. TANK (GLTF MODEL)

```
Object Name: Tank (imported model)
File: Tank_model/scene.gltf
Position: (0, 0.25, tank_z) - animated Z-axis movement
Scale: 0.0264×
Animation: Moves along Z-axis (key T)

GLTF STRUCTURE:
?? Hull (mesh):
?  ?? Primitives: Complex geometry
?  ?? Material: Textured
?  ?? Vertices: ~500-1000
?
?? Turret (mesh):
?  ?? Primitives: Complex geometry
?  ?? Material: Textured
?  ?? Vertices: ~300-500
?
?? Gun Barrel (cylinder):
?  ?? Primitive: Cylinder
?  ?? Material: Metal texture
?  ?? Vertices: ~100-200
?
?? Treads (mesh):
?  ?? Primitives: Complex geometry
?  ?? Material: Rubber texture
?  ?? Vertices: ~200-400
?
?? Wheels (cylinders):
   ?? Primitives: Cylinders
   ?? Material: Metal/rubber
   ?? Vertices: ~50-100 per wheel

TOTAL TANK PRIMITIVES: ~50-100 (varies by GLTF complexity)

Textures: Embedded in GLTF file
Status: Movable/Animated

VAO: Model-specific VAO
```

---

### 16. MILITARY TENT (GLTF MODEL)

```
Object Name: Main_Tent (imported model)
File: tent_military/scene.gltf
Position: (30, -0.1, 10)
Scale: 0.015×
Animation: Static (non-animated)

GLTF STRUCTURE:
?? Canvas (mesh):
?  ?? Primitives: Complex geometry
?  ?? Material: Canvas texture
?  ?? Vertices: ~400-600
?
?? Poles (mesh/cylinders):
?  ?? Primitives: Cylindrical geometry
?  ?? Material: Wood/metal texture
?  ?? Vertices: ~100-200
?
?? Ropes (mesh/lines):
?  ?? Primitives: Thin geometry
?  ?? Material: Rope texture
?  ?? Vertices: ~50-100
?
?? Ground Connection (mesh):
   ?? Primitives: Base geometry
   ?? Material: Soil/texture
   ?? Vertices: ~50-100

TOTAL TENT PRIMITIVES: ~30-50 (varies by GLTF complexity)

Textures: Embedded in GLTF file
Status: Static

VAO: Model-specific VAO
```

---

## WRAPPING & FILTERING MODES

### GL_REPEAT Mode
```
Used For: Tiled textures on large surfaces
Objects: Ground, walls, floors, roads, garages
Tiling Pattern: Texture repeats seamlessly
Effect: 
  ?? Fills entire surface with repeated pattern
  ?? Reduces file size for large areas
  ?? Creates seamless visual pattern

Example: Ground (200×200 units with 4× tiling = 16×16 tile grid)
```

### GL_CLAMP_TO_EDGE Mode
```
Used For: Single-image textures (wheels)
Objects: Wheel faces only
Effect:
  ?? Stretches single texture to fill area
  ?? No repetition
  ?? Edges repeat edge pixels (clamped)

Example: wheel.png on wheel disc face
```

### GL_LINEAR_MIPMAP_LINEAR Filter
```
Used For: All textured objects in scene
Mipmapping: Auto-generates reduced versions
?? Original: 1024×1024
?? Level 1: 512×512
?? Level 2: 256×256
?? Level 3: 128×128
?? Continues to 1×1

Benefits:
?? Better quality at distance
?? Faster rendering
?? Reduced aliasing artifacts
```

---

## QUICK REFERENCE TABLE

| Object Name | Primitive Type | Count | Texture(s) | Wrapping | Key Attribute |
|-------------|---|------|-----------|----------|---|
| **Ground** | Cube | 1 | ff.png | REPEAT | Toggleable render mode (M) |
| **Jeep Garage** | Cube | 23 | wall.png, floor.png | REPEAT | Lamps + garage structure |
| **Truck Garage** | Cube | 23 | wall.png, floor.png | REPEAT | Larger than jeep garage |
| **Barrack 1** | Cube | 8 | wood.png | REPEAT | Animated door (B) |
| **Barrack 2** | Cube | 8 | wood.png | REPEAT | Same as Barrack 1 |
| **Campfire** | Cube | 7 | pebble.png | REPEAT | Animated fire + light source |
| **Watch Tower 1** | Cyl+Cube | 7 | metal.jpeg, wood.png | REPEAT | Corner NE |
| **Watch Tower 2** | Cyl+Cube | 7 | metal.jpeg, wood.png | REPEAT | Corner NW |
| **Watch Tower 3** | Cyl+Cube | 7 | metal.jpeg, wood.png | REPEAT | Corner SW |
| **Watch Tower 4** | Cyl+Cube | 7 | metal.jpeg, wood.png | REPEAT | Corner SE |
| **Jeep A** | Cub+Cyl | 12 | **jeep.png**, wheel.png | REPEAT/CLAMP | Animated parts (D,W,L) |
| **Jeep B** | Cub+Cyl | 12 | **jeep.png**, wheel.png | REPEAT/CLAMP | Animated parts (D,W,L) |
| **Truck** | Cub+Cyl | 16 | **truck.png**, **back.png**, wheel.png | REPEAT/CLAMP | 6 wheels, render mode (K) |
| **Boundary** | Cube | 5 | wall.png, **gate.png** | REPEAT | Animated gate (U) |
| **Quarters** | Cube | 15 | **build.png**, floor.png, wood.png, box.png | REPEAT | Animated door/windows (G,H,I) |
| **Road** | Cube | 1 | metal.jpeg | REPEAT | Long runway |
| **Tree (×3-4)** | Cub+Bush | 79 | **Treeroot.png**, **TreeLeaf.png** | REPEAT | Fractal recursive structure |
| **Radar** | Sphere | 1 | **radartex.jpg** | REPEAT | Animated rotation (R) |
| **Sun** | Sphere | 1 | None (solid) | N/A | Emissive glowing object |
| **Tank** | GLTF | ~50-100 | Embedded | Embedded | Animated movement (T) |
| **Tent** | GLTF | ~30-50 | Embedded | Embedded | Static structure |

---

## TEXTURE SUMMARY BY USE

### Primary Vehicle Textures
- **jeep.png** ? Jeep body, cabin, door
- **truck.png** ? Truck cabin
- **back.png** ? Truck cargo bed
- **wheel.png** ? Wheel disc faces (GL_CLAMP_TO_EDGE)
- **blackTexture** ? Wheel tyres (procedural black)

### Building & Structure Textures
- **wood.png** ? Barrack all surfaces, doors, furniture
- **wall.png** ? Garage walls, watchtower posts, boundaries
- **floor.png** ? Garage floors, quarters floor, boundaries
- **gate.png** ? Military boundary gate
- **build.png** ? Quarters building walls & windows

### Landscape Textures
- **Treeroot.png** ? Tree trunk & branches (GL_REPEAT)
- **TreeLeaf.png** ? Tree foliage/leaves (GL_REPEAT)
- **radartex.jpg** ? Radar dish sphere (GL_REPEAT)
- **pebble.png** ? Campfire base (GL_REPEAT)

### Ground & Infrastructure
- **ff.png** ? Ground plane (GL_REPEAT, 4× tiling)
- **metal.jpeg** ? Road surface, watchtower posts (GL_REPEAT)

### Special Textures
- **box.png** ? Medical box on quarters table (GL_REPEAT)
- **OIP.jpeg** ? Radar base cubes, general-use (GL_REPEAT)

---

This documentation clearly shows **each object ? primitives ? texture with wrapping mode**. Perfect for viva/exam reference! ??

