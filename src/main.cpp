#include "header.h"

int main(int argc, char** argv) {
	// Declare variables
	ifstream		i1, i2;
	partitions		subHull;
	vector<coord>	ConvexHull;
	vector<coord>	hull;
	vector<coord>	P;
	coord		    tangent, p0 = { -9, 3 }, p1 = { 7, -9 };
    string          fileName1 = "points\\";
	bool			success;
	int m, k;

	// Make sure to put your sets in the points directory or alter the code lol
	if (argc != 3) {
		cout << "Invalid input\n" << "Usage: main.exe <file1> <file2>";
		exit(3);
	}

	fileName1 += static_cast<string>(argv[1]);

    i1.open(fileName1);

    if (!i1.is_open()){
        cout << fileName1 << " failed to open.\n";
        exit(1);
    }

	// testing jarvis march shit
	P = readPoints(i1);

	hull = GrahamsScan(P);

	m = int(hull.size());
	
	k = computeK(int(P.size()), m);

	subHull = subConvexHulls(P, k, m);

	success = JarvisMarch(ConvexHull, subHull, P,  k, m);


    i1.close();

	return 0;
}

// function definitions go here
stack<coord> ChansAlgorithm(vector<coord> P) {
	stack<coord>	tempS;

	return tempS;
}



int computeK(int size, int m) {
	double k0 = double(size) / double(m);
	int k = 0;
	k = int(std::ceil(k0));

	return k;
}



double distance(coord p1, coord p2) {
	return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}



double findAngle(Vec2 v1, Vec2 v2) {
	double m1, m2, dot;
	
	m1 = v1.findMagnitude();
	m2 = v2.findMagnitude();

	dot = findDotProduct(v1, v2);

	return acos(dot / (m1 * m2)) * 180 / PI;
}



int findBottomMost(vector<coord> P) {
	int tmp = 0;

	for (int i = 1; i < int(P.size()); i++) {
		if (P.at(i).y < P.at(tmp).y)
			tmp = i;
	}

	return tmp;
}



void findDistance(vector<coord>& P) {
	for (int i = 1; i < int(P.size()); i++) {
		P.at(i).DISTANCE = distance(P.at(0), P.at(i));
	}
}



double findDotProduct(Vec2 v1, Vec2 v2) {
	return ((v1.x * v2.x) + (v1.y * v2.y));
}



int findLeftMost(vector<coord> P) {
	int	tmp = 0;

	for (int i = 1; i < int(P.size()); i++) {
		if (P.at(i).x < P.at(tmp).x)
			tmp = i;
	}

	return tmp;
}



void findPolarAngle(vector<coord>& P) {
	for (int i = 1; i < int(P.size()); i++) {
		P.at(i).POLAR_ANGLE = polarAngle(P.at(0), P.at(i));

		if (P.at(i).POLAR_ANGLE < 0.0)
			P.at(i).POLAR_ANGLE = 180 + P.at(i).POLAR_ANGLE;
	}
}



coord findTangentPoint(vector<coord> Q, coord p0, int low, int high) {
	coord				nullPt;
	nullPt.DISTANCE =	-INFINITY;
	int					prev, next;
	
	int					mid = static_cast<int>(std::round(
							(double(high) + low) / 2));

	prev = mid - 1;
	next = mid + 1;

	// out of scope
	if (mid < 0) {
		mid = int(Q.size()) - 1;
		prev = mid - 1;
	}
	else if (mid >= int(Q.size())) {
		mid = 0;
		next = mid + 1;
	}
	else if (next >= int(Q.size()))
		next = 0;
	else if (prev < 0)
		prev = int(Q.size()) - 1;
	
	// current point on hull is our mid point for subhull
	if (Q.at(mid) == p0 && mid == 0)
		return Q.at(mid);

	if (orientation(Q.at(mid), p0, Q.at(prev)) == 0
		&& orientation(Q.at(mid), p0, Q.at(next)) == 0)
		return findTangentPoint(Q, p0, mid + 1, high);

	// go into upper interval if mid was the "wrong" tangent point
	// can only get upper tangent point on the first pass if it is mid
	if (orientation(Q.at(mid), p0, Q.at(prev)) == 2
		&& orientation(Q.at(mid), p0, Q.at(next)) == 2)
		return findTangentPoint(Q, p0, mid + 1, high);

	// TODO: Binary search ig?
	if (orientation(Q.at(mid), p0, Q.at(prev)) == 1
		&& orientation(Q.at(mid), p0, Q.at(next)) == 1)
		return Q.at(mid);
	if (orientation(Q.at(mid), p0, Q.at(prev)) == 0
		&& orientation(Q.at(mid), p0, Q.at(next)) == 1)
		return Q.at(mid);
	if (orientation(Q.at(mid), p0, Q.at(prev)) == 1
		&& orientation(Q.at(mid), p0, Q.at(next)) == 0)
		return Q.at(mid);


	// Other conditions that result in needing to choose an 
	// interval [low, mid - 1] [mid + 1, high]
	if (orientation(Q.at(mid), p0, Q.at(prev)) == 0
		&& orientation(Q.at(mid), p0, Q.at(next)) == 2)
		return findTangentPoint(Q, p0, mid + 1, high);
	if (orientation(Q.at(mid), p0, Q.at(prev)) == 2
		&& orientation(Q.at(mid), p0, Q.at(next)) == 0)
		return findTangentPoint(Q, p0, low, mid - 1);

	if (orientation(Q.at(mid), p0, Q.at(prev)) == 1
		&& orientation(Q.at(mid), p0, Q.at(next)) == 2)
		return findTangentPoint(Q, p0, mid + 1, high);
	if (orientation(Q.at(mid), p0, Q.at(prev)) == 2
		&& orientation(Q.at(mid), p0, Q.at(next)) == 1)
		return findTangentPoint(Q, p0, low, mid - 1);
	else
		return nullPt;
}



vector<coord> GrahamsScan(vector<coord> P) {
	coord			temp;
	stack<coord>	stack;
	int				tmp = 0, i = 0;

	tmp = findBottomMost(P);

	// Move p0 to the front
	temp = P.at(tmp);
	P.at(tmp) = P.at(0);
	P.at(0) = temp;

	findPolarAngle(P);

	findDistance(P);

	P = removeDupes(P);

	sortByPolarAngle(P);

	// Main loop of Grahams 
	for (int i = 0; i < int(P.size()); i++) {
		while (stack.size() > 1 && orientation(nextToTop(stack), stack.top(), P.at(i)) != 2) {
			stack.pop();
		}

		stack.push(P.at(i));
	}

	return stackToVector(stack);
}



bool JarvisMarch(vector<coord>& convexHull, partitions Q, vector<coord> P, int k, int m) {
	// TODO: All of this bullshit ig
	Vec2			v1, v2, n1, n2;
	coord			tan, currTan, p = { -10000000000, 0 }, p0;
	int				b = findBottomMost(P);
	double			angle = -1000;

	p0 = P.at(b);
	convexHull.push_back(p0);

	for (int j = 0; j < m; j++) {
		for (int i = 0; i < k; i++) {
			tan = findTangentPoint(Q.at(i), p0, 0, int(Q.at(i).size()) - 1);

			v1 = { tan.x - p0.x, tan.y - p0.y };
			v2 = { p.x - p0.x, p.y - p0.y };
	
			n1 = v1.normalize();
			n2 = v2.normalize();

			// Compute angle between those two vectors
			if (angle < findAngle(n1, n2)) {
				angle = findAngle(n1, n2);
				currTan = tan;
			}
		}

		// TODO: Update stuff for next iteration
		p = p0;
		p0 = currTan;
		convexHull.push_back(p0);
		angle = -1000;

		if (convexHull.front() == convexHull.back()) {
			convexHull.pop_back();
			return true;
		}
	}

	return false;
}



coord nextToTop(stack<coord> P) {
	coord	tmp;

	P.pop();

	tmp = P.top();
	return tmp;
}



int orientation(coord x, coord y, coord z) {
	double value = (y.y - x.y) * (z.x - y.x) -
		(y.x - x.x) * (z.y - y.y);

	if (value == 0) return 0;	// Collinear

	return (value > 0) ? 1 : 2; // CW vs CCW
}



partitions partition(vector<coord> P, int k, int m) {
	partitions 				Q;
	vector<coord>::iterator it = P.begin();
	int						size;

	for (int i = 0; i < k; i++) {
		size = std::min(m, int(std::distance(it, P.end())));

		vector<coord> temp(it, it + size);
		Q.push_back(temp);
		
		it += size;

		if (it == P.end())
			break;
	}

	// TODO: Make sure each parition is at least 3 points


	return Q;
}



double polarAngle(coord p1, coord p2) {
	return atan2((p2.y - p1.y), (p2.x - p1.x)) * 180 / PI;
}



vector<coord> readPoints(ifstream& fin) {
	vector<coord>	P;
	coord			c;

	while (fin >> c.x >> c.y) {
		P.push_back(c);
	}

	return P;
}



vector<coord> removeDupes(vector<coord> P) {
	vector<coord>::iterator pt;

	sort(P.begin() + 1, P.end(),
		[](coord& a, coord& b) {
			return a.DISTANCE < b.DISTANCE;
		});

	P.erase(std::unique(P.begin(), P.end()), P.end());

	return P;
}



void sortByPolarAngle(vector<coord>& P) {
	sort(P.begin() + 1, P.end(),
		[](coord& a, coord& b) {
			return a.POLAR_ANGLE < b.POLAR_ANGLE;
		});
}



vector<coord> stackToVector(stack<coord> S) {
	vector<coord>	tmp;
	tmp.resize(int(S.size()));

	for (int i = int(S.size()) - 1; i >= 0; i--) {
		tmp.at(i) = S.top();
		S.pop();
	}

	return tmp;
}



partitions subConvexHulls(vector<coord> P, int k, int m) {
	partitions		Q, tmp;
	vector<coord>	subHull;
	Q = partition(P, k, m);

	for (int i = 0; i < int(Q.size()); i++) {
		subHull = GrahamsScan(Q.at(i));

		tmp.push_back(subHull);

		subHull.clear();
	}

	return tmp;
}