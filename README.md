# Illumination
This is a graphics application that implements a raycaster which creates a PPM (Portable PixMap) image of mathematical primitives drawn from a JSON (JavaScript Object Notation) file. The data is loaded into pixel buffer where specular and diffuse reflection components are added to the scene of objects to produce lighting and coloring effects.
 
## Screenshots
<img src="https://github.com/jbredeme/Illumination/blob/master/example/png/example01.png" width="256"> <img src="https://github.com/jbredeme/Illumination/blob/master/example/png/example02.png" width="256"> <img src="https://github.com/jbredeme/Illumination/blob/master/example/png/example03.png" width="256">

## Usage
```c
raycast width height input.json output.ppm
```

## Example json scene data
```javascript
[
	{
		"type": "camera",
		"width": 2.0,
		"height": 2.0
	},
	{
		"type": "sphere",
		"radius": 2.0,
		"diffuse_color": [1, 0, 0],
		"specular_color": [1, 1, 1],
		"position": [0, 1, 5]
	},
	{
		"type": "plane",
		"normal": [0, 1, 0],
		"diffuse_color": [0, 1, 0],
		"position": [0, -1, 0]
	},
	{
		"type": "light",
		"color": [2, 2, 2],
		"theta": 0,
		"radial-a2": 0.125,
		"radial-a1": 0.125,
		"radial-a0": 0.125,
		"position": [1, 3, 1]
	}
]
```
## Built With
* [Cygwin](https://cygwin.com/index.html) - 64-bit versions of Windows
* gcc (GCC) 5.4.0
* Windows 10 Professional

## Author
* **Jarid Bredemeier** - *Initial work*

## Resources
* [Computer Graphics] - Pace University 
* [Diffuse reflection] - Wikipedia
* [Specular and diffuse reflection] - Khan Academy

[Computer Graphics]: http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter0.htm
[Diffuse reflection]: https://en.wikipedia.org/wiki/Diffuse_reflection
[Specular and diffuse reflection]: https://www.khanacademy.org/science/physics/geometric-optics/reflection-refraction/v/specular-and-diffuse-reflection
