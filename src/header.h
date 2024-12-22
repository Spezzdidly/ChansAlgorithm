#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stack>
#include <string>
#include <time.h>
#include <vector>

#define PI 3.14159265359

using namespace std;
using namespace chrono;

// Coordinate structure
struct coord {
	double x = 0.0;
	double y = 0.0;

	// variable for sorting points by polar angle
	double POLAR_ANGLE = 0.0;
	double DISTANCE = 0.0;
	
	friend bool operator==(const coord& point1, const coord& point2);
	friend bool operator!=(const coord& point1, const coord& point2);
};

bool operator==(const coord& point1, const coord& point2) {
	return point1.x == point2.x && point1.y == point2.y;
}

bool operator!=(const coord& point1, const coord& point2) {
	return point1.x != point2.x || point1.y != point2.y;
}


// Typedef(s)
typedef vector<vector<coord>> partitions;

// Function declarations go here
stack<coord> 	ChansAlgorithm(vector<coord> P);
void			duplicateAngles(vector<coord>& P);
double 			findDistance(coord p1, coord p2);
int 			findLeftMost(vector<coord> P);
int				findBottomMost(vector<coord> P);
double 			findPolarAngle(coord p1, coord p2);
vector<coord> 	GrahamsScan(vector<coord> P);
coord 			nextToTop(stack<coord> P);
int 			orientation(coord x, coord y, coord z);
partitions 		partition(vector<coord> P, int k, int m);
vector<coord> 	readPoints(ifstream& fin);
vector<coord>	removeDupes(vector<coord> P);
void			findPolarAngle(vector<coord>& P);
vector<coord> 	stackToVector(stack<coord> S);