# 🎲 Geometric Sampling in C++

**Author:** Alexandre Cottier — UCBL 2025  
**Course:** Advanced C++ / Geometry Processing Lab  

This project provides a modular C++ framework for **random geometric sampling** in 1D, 2D, and 3D spaces.  
It demonstrates how to uniformly generate random points in geometric domains such as squares, circles, spheres, and color spaces.

---

## 🧩 Project Structure

```
.
├── main.cpp               # Demonstration of random sampling
├── sampling.h             # Header with sampling declarations
├── sampling.cpp           # Implementation of sampling functions
├── svg.h                  # (Optional utility for SVG visualization)
└── README.md
```

---

## ⚙️ Features

### 1. Random Sampling Utilities
All sampling routines use a shared random engine (`std::default_random_engine alea`).

- **SamplingOptions::use_random_device()** — Seeds the generator with hardware randomness.  
- **rand_index(begin, end)** — Returns a random integer within the given bounds.  
- **rand_double()** — Returns a floating-point random value in [0,1].

---

### 2. 2D Sampling
- **rand_in_square()** → Returns a point uniformly distributed in the unit square.  
- **rand_in_circle()** → Returns a point uniformly distributed on the unit circle.  
- **rand_in_disk()** → Returns a point uniformly distributed inside the unit disk (radius scaled correctly for uniform density).

---

### 3. 3D Sampling
- **rand_in_cube()** → Point uniformly distributed in the unit cube.  
- **rand_in_sphere()** → Point uniformly distributed on the unit sphere (normalized Gaussian vector).  
- **rand_in_ball()** → Point uniformly distributed inside the unit ball, using radius^(1/3) scaling for volumetric uniformity.

---

### 4. Random Bright Color Generation

```cpp
Color rand_bright_color();
```

Generates a **bright and saturated random color** using HSV → RGB conversion:

- Hue ∈ [0, 6)
- Saturation ∈ [0.3, 1.0]
- Brightness ∈ [0.8, 1.0]

This ensures vivid colors for rendering or visualization purposes.

---

## 📊 Concepts Illustrated

| Concept | Description |
|----------|-------------|
| Random number generation | Use of `<random>` and seed initialization |
| Uniform spatial sampling | Even probability across geometric domains |
| Normalization | Use of normalized vectors for sphere sampling |
| Volumetric distribution | Proper radius scaling for uniform ball density |
| HSV → RGB conversion | Bright, vivid random color generation |

---

## 🧠 Learning Outcomes

- Understand **spatial uniformity** and **probability density** in geometry.  
- Apply random sampling techniques to geometry, rendering, or Monte Carlo simulation.  
- Learn to combine C++ random utilities with mathematical models for 2D/3D analysis.

---

## 🧑‍💻 Author

**Alexandre Cottier**  
Master’s Student — Université Claude Bernard Lyon 1 (UCBL)  
Academic Year 2025  
Course: *Advanced C++ — Sampling & Geometry Processing*

---

© 2025 — Educational project for learning geometric random sampling.
