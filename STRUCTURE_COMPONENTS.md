# Military Base Camp - Structure Components Documentation

## JEEP
**Main Parts:**
- **Lower Body**: Main cabin frame & sides
- **Upper Cabin**: Windshield pillars & roof frame
- **Doors**: Animated double-skin doors (open/close) + attached windows (can drop)
- **Windshield**: Tilted front glass (18° angle) with frame
- **Roof**: Top panel
- **Hood**: Front engine cover with center ridge & trim
- **Seats**: Front-left & front-right interior seating
- **Wheels** (×4): Tires with lug nuts, tread blocks, rim faces, center hub
- **Headlights** (×2): Front lights (toggle on/off)
- **Grille**: Front air intake with horizontal slats
- **Bumpers**: Front & rear protective bars with mounts
- **Mirrors** (×2): Side mirrors
- **Spare Tire**: Mounted on rear + rim detail
- **Exhaust**: Rear tailpipe
- **Details**: Military markings, antenna mast

---

## TRUCK
**Main Parts:**
- **Chassis**: Long frame with 6 wheels (front axle + dual rear axles)
- **Cab**: Main driver cabin with sloped windshield
- **Front Grille**: Metal lattice grill
- **Windshields** (×2): Windshield + side windows (glass panels)
- **Mirrors** (×2): Extended side mirrors
- **Cargo Bed**: Open cargo platform (5.3m long)
- **Cargo Cover**: Half-cylinder tarp cover over bed
- **Hide Panels**: Side/front/rear panels for cargo concealment
- **Flaps** (×2): Front & rear cargo flaps
- **Bumper**: Front protective bumper
- **Lights**: Headlights & turn signal indicators
- **Wheels** (×6): Large duty tires on 3 axles (front, mid, rear)
- **Render Modes**: Color-only, Texture-only, or Blended (50/50)

---

## BARRACK
**Main Parts:**
- **Walls**: Wooden panel exterior on 4 sides
- **Roof**: Pitched roof (military style)
- **Door**: Main entrance (single animated door)
- **Windows**: Side windows (openable)
- **Interior**: Bunk beds, basic furniture
- **Foundation**: Ground-level platform
- **Tube Lights** (×3): Interior ceiling lights for illumination

---

## GARAGE PLATFORM (Jeep & Truck Versions)
**Main Parts:**
- **Floor**: Raised concrete platform
- **Walls** (×3): Left, right, back walls (open front)
- **Roof**: Overhead cover structure
- **Ramp**: Sloped entry/exit for vehicles
- **Dimensions**: 
  - **Jeep Platform**: 14W × 18D × 7H interior
  - **Truck Platform**: 16W × 20D × 7H interior (wider/longer)

---

## BUILD (Military Building)
**Main Parts:**
- **Walls**: Concrete exterior panels (all 4 sides)
- **Floor**: Ground floor with thickness parameter
- **Door**: Single main entrance (animated open/close)
- **Windows** (×2): Left & right side windows (animated drop/raise)
- **Roof**: Top covering
- **Interior Furniture**:
  - **Beds** (×2): Left & right rear bunks
  - **Table**: Center furniture piece
  - **Medical Box**: On table (supplies container)
- **Dimensions**: 14W × 18D × 6.5H

---

## FURNITURE (Inside Buildings)
**Bed Components:**
- Mattress base
- Side supports (frame)
- Cushioned head/back

**Table Components:**
- Top surface
- Support legs (×4)

**Medical Box:**
- Container body
- Hinged lid

---

## BOUNDARY (Military Fence)
**Main Parts:**
- **Perimeter Wall**: Continuous concrete/metal wall around base
- **Gate**: Animated entrance gate (open/close)
- **Gate Posts** (×2): Support columns on either side of gate opening
- **Wall Height**: 14 units
- **Wall Thickness**: 0.85 units
- **Gate Opening**: 26 units wide
- **Gate Height**: 10.5 units
- **Dimensions**: 160W × 150L perimeter

---

## WATCHPOWER (×4 Corners)
**Main Parts:**
- **Base Pillars** (×4): Corner vertical support columns
- **Platform**: Elevated observation floor at top
- **Railing**: Safety barrier around platform
- **Roof**: Peaked roof (military style)
- **Stairs**: Access staircase (with 8 steps, tilted entry)
- **Antenna**: Vertical mast/cross on roof peak
- **Height**: ~11 units tall

---

## RADAR STATION
**Main Parts:**
- **Base Platform**: Large concrete foundation pad
- **Support Cylinder**: Central vertical support
- **Radar Dish** (Spherical): Rotating detection dish (animated)
- **Support Frame**: Metal framework holding dish
- **Walls** (×2): Surrounding protective walls
- **Control Unit**: Equipment box (if present)
- **Position**: X=40, Z=22-31 area

---

## CAMPFIRE
**Main Parts:**
- **Logs** (×4): Arranged in cross pattern on ground
- **Ash/Embers**: Ground circle base
- **Flames**: Procedural particle effect (visual only)
- **Pebbles**: Border ring around fire
- **Light Source**: Point light for illumination

---

## MILITARY ROAD
**Main Parts:**
- **Asphalt Surface**: Main road top
- **Road Markings**: Center line or edge lines
- **Width**: 16 units
- **Length**: 260 units (full base runway)
- **Elevation**: Slightly raised above ground plane

---

## STRUCTURES SUMMARY TABLE

| Structure | Parts Count | Key Features | Interactive |
|-----------|------------|--------------|-------------|
| Jeep | 15+ | Animated doors/windows, 4 wheels, lights | Yes (door, window, headlight) |
| Truck | 12+ | 6 wheels, 3 render modes, cargo cover | Yes (render mode toggle) |
| Barrack | 6+ | Double doors, windows, tube lights | Yes (door) |
| Garage | 4 | Platform, ramp, 3 walls | No |
| Build | 7+ | Door, windows, interior furniture | Yes (door, windows) |
| Boundary | 3 | Gate, walls, perimeter | Yes (gate open/close) |
| WatchTower | 6+ | Stairs, platform, antenna, roof | No |
| Radar | 5+ | Rotating dish, framework, walls | Yes (dish rotation) |
| Campfire | 5 | Logs, fire, light, pebbles | No (dynamic light) |
| Road | 2 | Surface, markings | No |

