# 🧭 Convex Hull by Jarvis March (Gift Wrapping Algorithm)

**Author:** Alexandre Cottier — UCBL 2025  
**Course:** Advanced C++ / Computational Geometry Project  
**Assignment:** Convex Hull — Gift Wrapping / Jarvis March Implementation

---

## 🎯 Objective

The goal of this project was to **implement the Jarvis March algorithm** (also known as the *Gift Wrapping algorithm*) to compute the **convex hull** of a set of points in 2D — and optionally in 3D.

This README summarizes both the original assignment and what was implemented in this solution.

---

## 🧩 Project Overview

### Provided base code (assignment skeleton)
The original framework included:
- `sampling` → random generation of 2D/3D points.  
- `svg` → simple SVG image export for visualization.  
- `main` → starting point with the `orient` predicate for geometric orientation tests.

The `orient` predicate returns whether a point lies **to the left**, **to the right**, or **on** a segment (2D), or above/below a plane (3D).  
All convex hull logic had to be added manually by the student.

---

## 💻 Implementation Summary

This project implements all requested tasks from the assignment and validates them through visual or data output.

### 1️⃣ Finding an initial hull point (`hull_point`)
- Implemented a search for the point with **minimum x-coordinate** (and minimum y in case of tie).  
- This guarantees a point that lies on the convex hull’s boundary.  
- A visual test (`HULL_POINT_TEST`) generates an SVG (`hull_point.svg`) showing this point.

✅ **Status:** Implemented and tested successfully.  

---

### 2️⃣ Implementing the 2D “wrap” step (`wrap`)
- Added a function that, given a current hull point `p0`, iterates over all other points to find the next hull vertex `p1`.  
- Uses `orient(p0, p1, p2)` to determine which point lies most “to the left” of all others (counterclockwise).  
- Produces one valid edge of the convex hull.

✅ **Status:** Fully implemented, producing SVG (`wrap_2d.svg`) confirming correct geometric orientation.  

---

### 3️⃣ Completing the 2D convex hull (`jarvis2d`)
- Implemented the full Jarvis March algorithm:  
  1. Start from `hull_point`.  
  2. Repeatedly “wrap” around to find next hull vertices.  
  3. Stop when the algorithm returns to the starting point.  
- Results are saved into a vector of vertex indices and exported visually.  
- The output image shows the convex hull polygon connecting all boundary points in correct order.

✅ **Status:** Working and validated (`hull_2d.svg`).  

---

### 4️⃣ Sampling utilities (`sampling.*`)
- Implemented random generation of points in 1D, 2D, and 3D using C++ `<random>`.  
- Supports uniform sampling for tests and visualization.  
- Used to create input data for the convex hull computation.

✅ **Status:** Complete, integrated, and reusable.  

---

### 5️⃣ Optional 3D extension (outline only)
The 3D part was analyzed and partially implemented conceptually, including:
- Reuse of the 3D orientation predicate `orient(p0,p1,p2,p3)`.
- Outline of the `wrap` function for triangular face detection.  
- Edge tracking and propagation planned using `std::set<std::pair<size_t,size_t>>`.  

🟡 **Status:** Partial conceptual setup (not fully implemented).  

---

## 🧠 Algorithmic Concepts

| Concept | Description |
|----------|-------------|
| **Orientation predicate** | Determines left/right (2D) or above/below (3D) relationship between points. |
| **Gift Wrapping principle** | Incrementally builds hull by always choosing the next extreme point. |
| **Convexity** | Ensures that all points of the dataset lie on or inside the constructed polygon/polyhedron. |
| **Termination condition** | Algorithm ends when it returns to the first vertex. |
| **Complexity** | O(n·h), where *h* = number of hull points. |

---

## 🧪 Example Run (2D)

Example of usage in `main.cpp`:

```cpp
#include "sampling.h"
#include <vector>
#include <iostream>

int main() {
    std::vector<Point2D> points = generate_random_points(200);

    std::vector<int> hull_indices;
    jarvis2d(points, hull_indices);

    std::cout << "Convex hull points: ";
    for (int i : hull_indices)
        std::cout << i << " ";
    std::cout << std::endl;

    export_svg(points, hull_indices, "hull_2d.svg");
}
```

Running this produces `hull_2d.svg`, a visualization of the convex hull.

---

## 📂 File Structure

```
.
├── main.cpp           # Implementation of hull_point, wrap, jarvis2d
├── sampling.cpp/.h    # Random generation utilities
├── svg.h              # SVG export helper
└── README.md          # This documentation
```

---

## 🧩 How It Differs from the Provided Skeleton

| Component | Provided | Implemented |
|------------|-----------|-------------|
| Random sampling | Basic | Extended, cleaned-up utilities |
| Orientation predicate | Provided | Used for 2D/3D wrapping |
| Hull point selection | Missing | Implemented (`hull_point`) |
| Wrapping step | Missing | Implemented (`wrap`) |
| Full convex hull | Missing | Implemented (`jarvis2d`) |
| 3D Hull extension | Optional | Conceptually prepared |

---

## 🧑‍💻 Author

**Alexandre Cottier**  
Master’s Student — Université Claude Bernard Lyon 1 (UCBL)  
Year: 2025  
Course: *Advanced C++ / Geometry Processing*

---

© 2025 — Educational implementation of the Jarvis March algorithm for convex hull computation.
