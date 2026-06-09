# Enhanced Jeep Wheel Design - Dark Gray Realistic Wheels

## Overview

The wheel design has been completely revamped to look like a realistic car wheel with distinct components:
- **Tire** - Black rubber outer covering
- **Rim** - Dark gray metallic circular part
- **Spokes** - Multiple connecting lines from center to rim (for realistic appearance)
- **Hub** - Center component where wheel attaches to axle

## Wheel Structure Breakdown

### 1. **Tire (Rubber Outer Layer)**
```
Color: (0.02, 0.02, 0.02) - Pure black
Purpose: Rubber tread
Thickness: 2.0 × WW (wheel width)
Radius: WR (wheel radius = 0.65)
```

### 2. **Rim (Metal Circular Part)**

#### Outer Rim Ring
```
Color: (0.30, 0.30, 0.32) - Dark gray metallic
Radius: WR × 0.58
Width: 0.08 × WW
Position: X-offset = 0.72 × WW (inside tire)
Shininess: 20.0
```

#### Inner Rim Ring
```
Color: (0.22, 0.22, 0.24) - Darker gray shade
Radius: WR × 0.35
Width: 0.05 × WW
Position: X-offset = 0.68 × WW
Shininess: 16.0
```

### 3. **Spokes (5 on main wheels, 4 on spare)**

```
Main Wheels: 5 spokes (evenly distributed at 72° intervals)
Spare Wheel: 4 spokes (evenly distributed at 90° intervals)

Each Spoke:
  Color: (0.25, 0.25, 0.27) - Medium-dark gray
  Width: 0.04 × WR
  Thickness: 0.06 × WW
  Length: 0.80 × rim_radius
  Shininess: 14.0
  Rotation: Dynamic around Y-axis for distribution
```

### 4. **Hub (Center Component)**

#### Outer Hub Ring
```
Color: (0.28, 0.28, 0.30) - Medium gray
Radius: 1.2 × hubR (hubR = 0.12 × WR)
Width: 0.10 × WW
Position: X-offset = 0.90 × WW
Shininess: 18.0
```

#### Center Darkened Area
```
Color: (0.18, 0.18, 0.20) - Dark gray (for depth)
Radius: 0.85 × rimInnerR
Width: 0.03 × WW
Position: X-offset = 0.55 × WW
Shininess: 12.0
```

#### Hub Center Cap
```
Color: (0.15, 0.15, 0.17) - Very dark gray
Radius: 0.8 × hubR
Width: 0.035 × WW
Position: X-offset = 0.94 × WW
Shininess: 16.0
```

#### Hub Shadow Detail
```
Color: (0.12, 0.12, 0.14) - Almost black
Radius: 1.0 × hubR
Width: 0.04 × WW
Position: X-offset = 0.88 × WW
Shininess: 10.0
```

## Color Palette

| Component | RGB Values | Description |
|-----------|-----------|-------------|
| Tire | (0.02, 0.02, 0.02) | Pure black rubber |
| Hub Shadow | (0.12, 0.12, 0.14) | Darkest element |
| Hub Center Cap | (0.15, 0.15, 0.17) | Very dark gray |
| Center Darkened | (0.18, 0.18, 0.20) | Dark gray shading |
| Inner Rim | (0.22, 0.22, 0.24) | Dark metallic |
| Spokes | (0.25, 0.25, 0.27) | Medium-dark gray |
| Outer Hub | (0.28, 0.28, 0.30) | Medium gray |
| Outer Rim | (0.30, 0.30, 0.32) | Lighter metallic |

## Shading Strategy

The wheel uses **graduated shading** to create depth:

```
Darkest ???????????????????? Lightest
(0.02)  ?  (0.12)  ?  (0.18)  ?  (0.25)  ?  (0.30)
  ?         ?         ?         ?         ?
 Tire    Hub Shadow Center   Spokes   Rim
         Detail    Darken
```

This creates a **3D appearance** with:
- Dark outer tire
- Increasing brightness towards center
- Prominent rim edges for definition
- Shadow details in hub for realism

## Wheel Radius Calculations

```
WR (Wheel Radius) = 0.65

Rim Outer Radius = 0.58 × WR = 0.377
Rim Inner Radius = 0.35 × WR = 0.2275
Hub Radius = 0.12 × WR = 0.078
Hub Outer = 1.2 × hubR = 0.0936
Hub Inner = 0.8 × hubR = 0.0624
```

## Key Improvements Over Previous Design

| Aspect | Before | After |
|--------|--------|-------|
| Components | 4 simple rings | 10+ layered components |
| Realism | Basic gray rings | Full rim, spokes, hub |
| Depth | Flat appearance | Multi-layer shading |
| Shine Variation | Uniform | Graduated (8-20) |
| Spokes | None | 5 main / 4 spare |
| Color Range | 2 colors | 8 shades of gray |

## Implementation Details

### Main Wheels (4 wheels on vehicle)
- **5 Spokes** for realistic appearance
- Spokes rotated around **Y-axis** in 72° increments
- Standard size proportions

### Spare Tire (mounted on back)
- **4 Spokes** for variety
- Spokes rotated around **X-axis** (perpendicular mounting)
- Slightly larger overall (0.78 × WR radius)

### Rendering Method
All wheel components use the `drawColor()` function with proper material settings:
- **Ambient**: 0.28 × base color
- **Diffuse**: Full base color
- **Specular**: 0.6 gray for metallic shine
- **Shininess**: Varies per component (8-20)

## Visual Hierarchy

```
???????????????????????????????????
?  TIRE (Black Rubber Outer)      ?
?  ????????????????????????????   ?
?  ?  RIM OUTER (Bright Gray) ?   ?
?  ?  ??????????????????????  ?   ?
?  ?  ? RIM INNER (Dark)   ?  ?   ?
?  ?  ? ?????????????????? ?  ?   ?
?  ?  ? ?  5 SPOKES      ? ?  ?   ?
?  ?  ? ? (Med-Dark Gray)? ?  ?   ?
?  ?  ? ? ?????????????? ? ?  ?   ?
?  ?  ? ? ? HUB CENTER ? ? ?  ?   ?
?  ?  ? ? ? (Very Dark)? ? ?  ?   ?
?  ?  ? ? ?????????????? ? ?  ?   ?
?  ?  ? ?????????????????? ?  ?   ?
?  ?  ??????????????????????  ?   ?
?  ????????????????????????????   ?
???????????????????????????????????
```

## Result

The wheels now have:
? **Dark gray metallic appearance** - Professional look
? **Visible spokes** - Realistic spoke pattern
? **Layered depth** - Multiple shades create 3D effect
? **Hub detail** - Center component clearly defined
? **Consistent styling** - Both main and spare wheels match design language
? **Proper shininess** - Metallic appearance with appropriate reflections
