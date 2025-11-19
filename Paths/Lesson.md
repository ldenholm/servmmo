## Part 1 — Understand a grid as a graph
Step 1: Represent a 2D grid

Create your own grid type.
Make it any size you want and fill it with 0 for walkable and 1 for blocked.

### Task:
Write code that prints the grid to the console in a readable form.

Step 2: Pick a coordinate struct

Make a small struct to store x/y positions.

### Task:
Implement a function that checks whether a coordinate is inside the grid.

Step 3: Neighbour lookup

Define what counts as a “neighbour”:

4-way or 8-way movement.

Forbidden moves (e.g., diagonals through corners).

### Task:
Write a function that returns the valid neighbours of any given cell
(don’t implement movement costs yet).