# Chan's Algorithm in C++
This is a project I was assigned last semester that I was not able to finish by the deadline (whoops) but I took a solid chunk of my break to get it up and running. All this program does is take an input file of points (formatted "x y") and then computes the convex hull in clockwise order using Chan's Algorithm.

## Getting Started
You can simply clone this repository and use it for whatever reasons you want :) my hope is that someone struggling in the same spot I was can look at this and hopefully use it to get their work done on time.
```
git clone https://github.com/Spezzdidly/ChansAlgorithm
```
It's important to note the proper command line arguments and format for the file containing the set of points.
```
Usage: main.exe <file1>
```
```
Format:
x y
x y
x y
...
```
## Compiling
I used visual studio as my IDE of choice for this project so that handles the compiling and linking of the program. If you want to use something like g++ it is fairly simple, in the ChansAlgorithm directory type
```
g++ -I src src/main.cpp -o main.exe
```
You will need to alter the code so that the slash ("\\") is correct for linux systems ("/") since it is currently designed to run on windows which uses the back slash.

## Authors
* **Spezzdidly**

## License
This project is licensed under the MIT License

## Acknowledgements
Thank you to **Tom Switzer** (tixxit) specifically for his tangent function which can be found [here](https://gist.github.com/tixxit/252229)
<br>Another HUGE thank you to my professor who didn't fail me even though I was unable to complete this project on time!
## Links
* [Chan's Algorithm Wiki](https://en.wikipedia.org/wiki/Chan%27s_algorithm)
* [This Term Paper](https://www.cs.umd.edu/~patras/patra2012_convexhull_report.pdf)
* [Stack Exchange Post](https://cs.stackexchange.com/questions/138901/upper-and-lower-tangent-line-to-convex-hull-from-a-point)
