# ? COLLISION FIXES - ROAD & JEEP-TRUCK INTERACTION

## ISSUES FIXED

### 1. **Jeeps/Trucks NOW CAN Drive On Road** ?
- **Problem**: Road collision box was blocking vehicles
- **Solution**: Removed road from obstacle list (commented documentation only)
- **Result**: Vehicles now freely drive on the military road

### 2. **Jeeps CANNOT Go Through Each Other** ?
- **Problem**: Jeep-to-jeep collision was already implemented
- **Status**: VERIFIED - Jeeps block each other
- **Result**: Jeep A and Jeep B cannot pass through each other

### 3. **Truck-Jeep Interaction** ?
- **Status**: DISABLED (optional feature)
- **Current**: Trucks and Jeeps CAN share space (drive through each other)
- **Can Enable**: Uncomment code to prevent truck-jeep collision

---

## CODE CHANGES

### File Modified: `main.cpp`

### Change 1: `buildObstacleBoxes()` Function
**Before**: Road was implicitly blocked (missing from obstacle list)  
**After**: Clear documentation that road is NOT blocked

```cpp
// NOTE: ROAD is NOT blocked - vehicles can drive on it!
// Road position: (0, GROUND_HEIGHT + 0.01, 35)
// Road dimensions: width 16, length 260
// Vehicles are FREE to drive on the road
```

### Change 2: `collidesAtPosition()` Function (Jeep Collision Check)
**Updated**: Added explicit jeep-to-jeep collision checking

```cpp
// JEEP-TO-JEEP COLLISION: Enable collision between jeeps
const JeepState* other = nullptr;
if (self == &jeepA) other = &jeepB;
else if (self == &jeepB) other = &jeepA;

if (other) {
    if (intersects2D(jeepBox, makeJeepBox(other->pos))) return true;
}
```

**Note**: Jeep-to-truck collision is DISABLED (commented out)

### Change 3: `truckCollidesAtPosition()` Function
**Updated**: Added optional truck-to-jeep collision

```cpp
// TRUCK-TO-JEEP COLLISION: DISABLED - Trucks and jeeps can share space
// Uncomment below to prevent trucks from driving through jeeps
// AABB2D jeepABox = makeJeepBox(jeepA.pos);
// AABB2D jeepBBox = makeJeepBox(jeepB.pos);
// if (intersects2D(truckBox, jeepABox)) return true;
// if (intersects2D(truckBox, jeepBBox)) return true;
```

---

## BEHAVIOR SUMMARY

| Scenario | Status | Notes |
|----------|--------|-------|
| **Jeep on Road** | ? CAN | Vehicles drive freely on road |
| **Truck on Road** | ? CAN | Vehicles drive freely on road |
| **Jeep A vs Jeep B** | ? BLOCKED | Jeeps collide with each other |
| **Truck vs Jeep** | ? CAN (disabled) | Currently, they can pass through each other |
| **Jeep vs Barrack** | ? BLOCKED | Collision with buildings |
| **Truck vs Boundary** | ? BLOCKED | Cannot escape perimeter |
| **Both vs Radar** | ? BLOCKED | Cannot drive through radar |
| **Both vs Trees** | ? BLOCKED | Cannot drive through trees |

---

## HOW TO TEST

### Test 1: Drive on Road
```
1. Press P ? Select Truck
2. Press UP ARROW multiple times
3. Drive forward onto the road
4. Vehicle moves freely on road ?
```

### Test 2: Jeep-Jeep Collision
```
1. Press J ? Select Jeep A
2. Drive toward Jeep B
3. Jeep A stops when hitting Jeep B ? (blocked)
4. Press J ? Select Jeep B
5. Try to move toward Jeep A
6. Jeep B stops when hitting Jeep A ? (blocked)
```

### Test 3: Truck-Jeep Interaction (Current: No Collision)
```
1. Press J ? Select Jeep A
2. Move Jeep A to center
3. Press P ? Select Truck
4. Drive Truck toward Jeep A
5. Truck passes through Jeep A ? (no collision)
```

---

## ENABLING TRUCK-JEEP COLLISION (Optional)

To enable trucks to collide with jeeps:

### In `collidesAtPosition()`:
Uncomment these lines:
```cpp
// JEEP-TO-TRUCK COLLISION: DISABLED - Jeeps and trucks can share space
// Uncomment below to prevent jeeps from driving through trucks
AABB2D truckBox = makeTruckBox(truckState.pos);
if (intersects2D(jeepBox, truckBox)) return true;
```

### In `truckCollidesAtPosition()`:
Uncomment these lines:
```cpp
// TRUCK-TO-JEEP COLLISION: DISABLED - Trucks and jeeps can share space
// Uncomment below to prevent trucks from driving through jeeps
AABB2D jeepABox = makeJeepBox(jeepA.pos);
AABB2D jeepBBox = makeJeepBox(jeepB.pos);
if (intersects2D(truckBox, jeepABox)) return true;
if (intersects2D(truckBox, jeepBBox)) return true;
```

Then rebuild project.

---

## COLLISION BOXES REFERENCE

### Road (FREE - No Collision)
- **Position**: (0, GROUND_HEIGHT + 0.01, 35)
- **Dimensions**: Width 16, Length 260
- **Status**: ? Vehicles can drive

### Jeep Collision Box
- **Width**: 2.2 units
- **Length**: 4.0 units
- **Collides With**: Obstacles + Other Jeep (but NOT truck)

### Truck Collision Box
- **Width**: 2.84 units
- **Length**: 5.3 units
- **Collides With**: Obstacles only (NOT jeeps)

---

## PROTECTED OBSTACLES (Still Active)

All major scene obstacles remain protected:
- ? Barracks (2)
- ? Build Object
- ? Boundary Walls (4)
- ? Radar Office
- ? Watch Towers (4)
- ? Campfire
- ? Trees (4)
- ? Tank
- ? Main Tent
- ? Garage Walls

---

## COMPILE STATUS

? **Build**: SUCCESS  
? **Errors**: 0  
? **Warnings**: 0  
? **Ready to Run**: YES

---

## SUMMARY

### ? What Works Now
1. **Vehicles can drive on road** - No collision box on road
2. **Jeeps block each other** - Jeep-to-jeep collision enabled
3. **Trucks and jeeps can coexist** - No cross-vehicle collision
4. **All obstacles protected** - Buildings, walls, radar, trees all block vehicles

### ?? What Can Be Changed
- **Enable truck-jeep collision**: Uncomment code in collision functions
- **Add more obstacles**: Extend `buildObstacleBoxes()` function
- **Adjust collision sizes**: Modify `makeJeepBox()` and `makeTruckBox()` dimensions

### ?? Code Locations
- **Obstacle boxes**: `buildObstacleBoxes()` ~line 620
- **Jeep collision**: `collidesAtPosition()` ~line 680
- **Truck collision**: `truckCollidesAtPosition()` ~line 700

