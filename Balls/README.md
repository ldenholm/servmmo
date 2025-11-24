## Refactoring collision detection with AABB + Quadtree

What I am trying to achieve here:
- Narrow phase collision detection: check if circles overlap and resolve if they do.
- Broad phase collision detection: perform an approximate and cheap check using axis aligned bounding boxes AABBs.
- Space partition using a quadtree so we know the close neighbors for a given ball.