# ? JEEP RENDER MODE BUTTONS - QUICK ANSWER

## ? JEEPS DO NOT HAVE RENDER MODE BUTTONS

**Important**: Unlike the **TRUCK**, **JEEPS DO NOT have color/texture/blended render modes**.

---

## ?? TRUCK HAS RENDER MODE (K Key)

| Button | Action | Effect |
|--------|--------|--------|
| **K** | Cycle Render Mode | Color Only ? Texture ? Blended (50/50) ? Color Only |

### Truck Render Modes
1. **Color Only** - Solid olive green (0.43, 0.46, 0.33)
2. **Texture** - Full texture details
3. **Blended** - 50/50 mix of color and texture
4. **Back to Color** - Cycles back

---

## ?? JEEP BUTTONS (FOR REFERENCE)

### Jeep Selection
| Button | Action |
|--------|--------|
| **J** | Cycle: None ? Jeep A ? Jeep B ? None |

### Jeep Controls (When Selected)
| Button | Action | Effect |
|--------|--------|--------|
| **UP ARROW** | Move Forward | Drive jeep ahead |
| **DOWN ARROW** | Move Backward | Drive jeep back |
| **LEFT ARROW** | Turn Left | Rotate jeep left |
| **RIGHT ARROW** | Turn Right | Rotate jeep right |
| **D** | Toggle Door | Open/Close doors |
| **W** | Toggle Window | Open/Close windows |
| **L** | Toggle Headlights | Turn lights ON/OFF |
| **V** | Cycle Camera View | Back ? Front ? Left ? Right ? Back |

---

## ?? GROUND HAS RENDER MODE (M Key)

Like trucks, the GROUND also has render modes:

| Button | Action | Effect |
|--------|--------|--------|
| **M** | Cycle Ground Render | Color Only ? Texture ? Blended (50/50) ? Color Only |

### Ground Render Modes
1. **Color Only** - Solid sandy color (0.8, 0.8, 0.7)
2. **Texture** - Detailed texture with 4x tiling
3. **Blended** - 50% color + 50% texture
4. **Back to Color** - Cycles back

---

## ?? WHAT CAN BE RENDERED IN DIFFERENT MODES

| Object | Render Modes | Button |
|--------|--------------|--------|
| **Truck** | ? YES (Color/Texture/Blended) | **K** |
| **Jeep** | ? NO (Always textured) | None |
| **Ground** | ? YES (Color/Texture/Blended) | **M** |
| **Buildings** | ? NO (Always textured) | None |
| **Objects** | ? NO (Always textured) | None |

---

## ?? WHY JEEPS DON'T HAVE RENDER MODES

- **Jeeps are always rendered with texture** (jeep.png applied)
- **Truck has render mode** because it needs flexibility for visualization
- **Ground has render mode** because it's a large flat surface needing optimization
- **Other objects** are always textured (barracks, buildings, trees, etc.)

---

## ?? HOW TO TEST TRUCK RENDER MODE

```
1. Press P ? Select Truck
2. Press K ? Cycle to Color Only (solid green)
3. Press K ? Cycle to Texture Only (textured details)
4. Press K ? Cycle to Blended (50% green + 50% texture)
5. Press K ? Back to Color Only
```

You'll see the truck change appearance each time!

---

## ?? SUMMARY TABLE

| Feature | Jeep | Truck | Ground |
|---------|------|-------|--------|
| Select | **J** | **P** | N/A (global) |
| Move Forward | **UP** | **UP** | N/A |
| Move Backward | **DOWN** | **DOWN** | N/A |
| Turn Left | **LEFT** | **LEFT** | N/A |
| Turn Right | **RIGHT** | **RIGHT** | N/A |
| Render Mode | ? None | ? **K** | ? **M** |
| Camera View | **V** | **V** | **O** (bird's eye) |
| Door | **D** | ? None | N/A |
| Window | **W** | ? None | N/A |
| Headlights | **L** | ? None | N/A |

---

## ?? QUICK ANSWER

**Question**: What button changes jeep between blended, texture, etc?

**Answer**: **There is NO button for jeeps to change render modes.** Jeeps are always rendered with texture (jeep.png). Only the **TRUCK** has render mode switching with the **K** key, and the **GROUND** with the **M** key.

