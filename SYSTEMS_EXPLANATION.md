# ?? CORE SYSTEMS EXPLANATION: Fire Animation & Vehicle Collision

Here is a detailed, descriptive breakdown of the two major systems requested. This is perfect for project documentation, thesis reports, or a viva presentation.

---

## ?? PART 1: THE FIRE ANIMATION LOGIC

The campfire animation in the project is procedurally generated without relying on pre-animated models or sprite sheets. Instead, it uses **mathematical functions (sine waves) and matrix transformations** to simulate the chaotic, dancing nature of fire.

### 1. Multi-Layered Flame Structure
The fire is not a single object; it is built using an array of 6 horizontal "layers" (`FlameLayer`).
*   **Base layers** (bottom) are wide, orange/red, and stable.
*   **Tip layers** (top) are narrow, bright yellow/white, and move more aggressively.
*   By layering multiple semi-transparent cubes (`GL_BLEND` enabled), the overlapping colors naturally create a bright, glowing center.

### 2. Time-Based Animation (`glfwGetTime()`)
All mathematical operations rely on an ever-increasing time variable `t`. This ensures the animation is smooth and continuously flowing regardless of the frame rate.

### 3. The 3 Pillars of the Fire Logic

*   **A. The Sway (Translation)**
    ```cpp
    float sway = 0.08f * sinf(t * 4.5f + fl.phase);
    float yAnim = fl.yOff + 0.03f * sinf(t * 5.0f + fl.phase * 2.0f);
    ```
    The fire "dances" side-to-side using a sine wave. Each layer has a unique `phase` offset. Because of this phase offset, the layers don't move left and right at the exact same time; they follow each other, creating a realistic, wavy snake-like motion upwards.

*   **B. The Pulse & Flicker (Scaling)**
    ```cpp
    float flicker = 0.85f + 0.08f * sinf(t * 7.3f) + 0.07f * sinf(t * 13.1f + 1.2f);
    float pulse = 1.0f + 0.12f * sinf(t * 6.0f + fl.phase) * flicker;
    ```
    To simulate wind hitting the fire, the overall scale of the layers rapidly pulses. The `flicker` effect combines two high-frequency sine waves (7.3f and 13.1f). Adding two distinct sine waves creates a chaotic, non-repeating pattern, mimicking unpredictable wind. This flicker value is also passed directly to the `PointLight` attached to the fire, causing the environment lighting to flicker flawlessly in sync with the flame size!

*   **C. The Twist (Rotation)**
    ```cpp
    float speedFactor = (7.0f - fl.layerIndex) / 6.0f;
    float layerSpeed = 180.0f * speedFactor;
    float rotationAngle = layerSpeed * t;
    ```
    To make it look 3D instead of flat, the layers continuously rotate around the Y-axis. The clever logic here is the `speedFactor`. The base of the fire (Layer 1) rotates slowly and steadily, while the tip of the fire (Layer 6) rotates extremely fast.

### 4. The Rising Embers (Particle System)
Around the main fire, 5 tiny cubes act as "sparks". 
Using mathematical modulo (`fmodf`), the sparks are programmed to rise upward (Y-axis) and gradually fade their alpha out. Once a spark reaches `loop = 1.0f`, it instantly resets to the bottom (`loop = 0.0f`), creating an infinite loop of rising hot embers.

---

## ?? PART 2: THE VEHICLE OBSTACLE (COLLISION) SYSTEM

To stop Jeeps and Trucks from driving through walls, buildings, and each other, the project implements a **2D Axis-Aligned Bounding Box (AABB) Collision Detection System**. 

### 1. Conceptual Approach: 2D over 3D
Though the world is 3D, vehicles only move along the ground (X and Z axes). Therefore, height (Y-axis) can be completely ignored for collision detection. The system projects all 3D objects down into 2D rectangles on the ground plane.

### 2. The Bounding Box Structure
```cpp
struct AABB2D {
    float minX, maxX;
    float minZ, maxZ;
};
```
Every physical object in the world is represented by its Minimum and Maximum coordinates on the X and Z axes. 
For dynamic vehicles, this box is generated on the fly. A Jeep mathematically projects its boundaries outward from its center coordinate:
```cpp
AABB2D makeJeepBox(const glm::vec3& pos) {
    return {
        pos.x - 1.1f, pos.x + 1.1f,  // Width boundaries
        pos.z - 2.0f, pos.z + 2.0f   // Length boundaries
    };
}
```

### 3. The Intersection Logic (The Math)
To check if a vehicle hit a wall, the code checks if their two bounding boxes overlap.
```cpp
bool intersects2D(const AABB2D& a, const AABB2D& b) {
    return !(a.maxX < b.minX || a.minX > b.maxX ||
             a.maxZ < b.minZ || a.minZ > b.maxZ);
}
```
This is an incredibly efficient algorithm. It essentially says:
*"If Box A is completely to the Left, Right, Front, or Back of Box B, they are NOT colliding. Otherwise, they MUST be touching."*

### 4. Look-Ahead Collision Prevention
Instead of waiting for the vehicle to hit the wall and getting stuck, the system uses a **predictive** approach.
```cpp
// 1. Where does the player WANT to go?
glm::vec3 nextPos = j->pos + forward * moveSpeed;

// 2. Will there be an obstacle AT that future position?
if (!collidesAtPosition(nextPos, j)) {
    j->pos = nextPos; // 3. No collision! Move the vehicle.
} else {
    // 3. Collision! Do nothing. The vehicle is blocked.
}
```
This guarantees the vehicle boundaries never actually breach the wall boundaries, preventing physics glitches.

### 5. Static vs. Dynamic Obstacles
*   **Static List (`buildObstacleBoxes`)**: The barracks, bounding walls, radar office, trees, and watchtowers never move. To save CPU power, the game builds a static `std::vector` of these coordinates. The function is called once, and the boundaries are hard-coded mapped to the visual scaling of the 3D models.
*   **Dynamic Checking**: When checking for collisions, a Jeep first iterates through the static world obstacles. If it passes, it then specifically checks the dynamic position of the *other Jeep*. If both checks pass, the movement is permitted.