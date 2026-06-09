# Jeep Headlight Controls - Quick Reference

## How to Use

### Step 1: Select a Jeep
Press **J** multiple times to cycle through:
- **None** (normal camera mode)
- **Jeep A** (left jeep)  
- **Jeep B** (right jeep)

You'll see in the console which jeep is selected.

### Step 2: Toggle Headlights
While a jeep is selected, press **L** to toggle headlights:
- Press **L** ? Headlights **ON** ?
  - Jeep model headlight glass becomes bright yellow-white
  - Spotlights cast light in front of jeep
- Press **L** again ? Headlights **OFF** ?
  - Headlight glass dims
  - Spotlights no longer illuminates scene

### Step 3: Move and Control
- **Arrow Keys** to move jeep forward/backward/rotate
- Headlights will follow the jeep's direction
- **V** to change camera view (back/front/left/right)
- Lights illuminate whatever is ahead of the jeep

## What You'll See

### With Headlights ON:
- Bright yellow-white beam illuminating terrain, vehicles, buildings
- Light intensity decreases with distance (realistic falloff)
- Beam follows jeep rotation
- Both left and right headlights create unified cone ahead

### With Headlights OFF:
- No spotlight illumination from jeep
- Scene lit only by ambient/point/directional lights
- Jeep still visible but no forward-facing lights

## Switching Between Jeeps
1. Press **J** to cycle to desired jeep
2. Their headlight state is independent
3. Only selected jeep's headlights respond to **L** key
4. Both jeeps can have lights on simultaneously

## Combined Controls Example

```
1. Press J          ? Select Jeep A
2. Press L          ? Turn Jeep A lights ON
3. Arrow Keys       ? Drive Jeep A around
4. Press J (twice)  ? Select Jeep B  
5. Press L          ? Turn Jeep B lights ON
6. Now both jeeps driving with headlights ON
7. Arrow Keys       ? Control Jeep B
8. Press L          ? Turn Jeep B lights OFF (Jeep A still on)
```

## Technical Specs

| Property | Value |
|----------|-------|
| Light Type | Spotlight (directional cone) |
| Cutoff Angle | 20° |
| Light Color (ON) | Yellow-white: (1.0, 0.95, 0.8) |
| Attenuation | k_c=1.0, k_l=0.09, k_q=0.032 |
| Beam Direction | Forward direction of jeep |
| Left Light Offset | 0.6 units to the left |
| Right Light Offset | 0.6 units to the right |
| Forward Offset | 2.5 units ahead of jeep |
| Height | At jeep headlight position (~0.47 units) |

## Troubleshooting

### Headlights not appearing?
- Make sure you have a jeep selected (press J to cycle)
- Check console to confirm selection
- Press L to toggle lights

### Lights not pointing ahead of jeep?
- Rotate the jeep with arrow keys
- Headlights update each frame to match jeep rotation

### Light looks too bright/dim?
- Light colors are fixed but attenuation can be adjusted
- Look for `k_c`, `k_l`, `k_q` parameters in headlight setup

### Only one light showing?
- Both left and right spotlights should active when on
- They create a unified cone effect
- Check shader configuration if only one appears
