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

enum {COLLINEAR = 0, CW = 1, CCW = -1};

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

struct Vec2 {
	double x = 0.0;
	double y = 0.0;

	double findMagnitude() {
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	Vec2 normalize() {
		return { x / findMagnitude(), y / findMagnitude() };
	}
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
vector<coord> 	ChansAlgorithm(vector<coord> P);
int				computeK(int size, int m);
double 			distance(coord p1, coord p2);
double			findAngle(Vec2 v1, Vec2 v2);
int				findBottomMost(vector<coord> P);
void			findDistance(vector<coord>& P);
double			findDotProduct(Vec2 v1, Vec2 v2);
int 			findLeftMost(vector<coord> P);
void			findPolarAngle(vector<coord>& P);
coord			findTangentPoint(vector<coord> Q, coord p0);
vector<coord> 	GrahamsScan(vector<coord> P);
bool			JarvisMarch(vector<coord>& convexHull, partitions Q, vector<coord> P, int k, int m);
coord 			nextToTop(stack<coord> P);
int 			orientation(coord x, coord y, coord z);
void			outputHull(ofstream& fout, vector<coord> hull);
partitions 		partition(vector<coord> P, int k, int m);
double 			polarAngle(coord p1, coord p2);
vector<coord> 	readPoints(ifstream& fin);
vector<coord>	removeDupes(vector<coord> P);
void			sortByPolarAngle(vector<coord>& P);
vector<coord> 	stackToVector(stack<coord> S);
partitions		subConvexHulls(vector<coord> P, int k, int m);