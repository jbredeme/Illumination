# Illumination

This program reads in a collection of objects from a scene formatted using JSON (JavaScript Object Notation). These objects consisting of mathematical primitives, sphere(s) and plane(s) are stored into system memory where vector (ray) intersections calculations are then performed to determine an object's location in 3D space. This intersection data is used to construct an image of scene by coloring individual pixels on a pixel map with an object’s corresponding color property. Lastly this image data is fed into an image buffer and written out to a P6 portable pixmap format (PPM) for viewing.

## Usage
```c
raycast width height input.json output.ppm
```

## Example json scene data
TODO

## Example Screenshots
TODO

## Resources
TODO