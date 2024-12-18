#include "header.h"

int main(int argc, char** argv) {
	// Declare variables
	ifstream		i1, i2;
	partitions		Partition;
	stack<coord>	ConvexHull;
	vector<coord>	hull;
	vector<coord>	P = { {0, 3}, {2, 2}, {1, 1}, {2, 1},
						{3, 0}, {0, 0}, {3, 3} };
	coord		    p1 = { -7, 2 }, p2 = { -INFINITY, 0 }, p3 = { 5, 0 };

	if (argc != 3) {
		cout << "Invalid input\n" << "Usage: main.exe <path-to-file> <path-to-file2>";
		exit(3);
	}



	return 0;
}

// function definitions go here
stack<coord> ChansAlgorithm(vector<coord> P) {
	int				t, m;
	bool			foundHull = false;
	stack<coord>	tempS;

	return tempS;
}



void duplicateAngles(vector<coord>& P) {
	int		tmp = 1;
	double		DISTANCE_ONE, DISTANCE_TWO;

	for (int i = 2; i < int(P.size()); i++){
		if (P.at(i) == P.at(tmp))
			P.erase(P.begin() + tmp);
		else if (P.at(i).POLAR_ANGLE == P.at(tmp).POLAR_ANGLE) {
			DISTANCE_ONE = findDistance(P.at(0), P.at(i));
			DISTANCE_TWO = findDistance(P.at(0), P.at(tmp));

			// Remove closest point
			if (DISTANCE_ONE > DISTANCE_TWO)
				P.erase(P.begin() + tmp);
			else
				P.erase(P.begin() + i);
		}
	}
}
	





double findDistance(coord p1, coord p2) {
	return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}



int findLeftMost(vector<coord> P) {
	int	tmp = 0;

	for (int i = 1; i < int(P.size()); i++) {
		if (P.at(i).x < P.at(tmp).x)
			tmp = i;
	}

	return tmp;
}



double findPolarAngle(coord p1, coord p2) {
	return atan2((p2.y - p1.y), (p2.x - p1.x)) * 180 / PI;
}



vector<coord> GrahamsScan(vector<coord> P) {
	coord			temp;
	stack<coord>	stack;
	int				tmp = 0, i = 0;
	int				np = int(P.size());

	// Find the index of the lowest point in P
	for (i = 1; i < np; i++) {
		if (P.at(i).y < P.at(tmp).y)
			tmp = i;
	}

	// Move p0 to the front
	temp = P.at(tmp);
	P.at(tmp) = P.at(0);
	P.at(0) = temp;

	for (int i = 1; i < np; i++) {
		P.at(i).POLAR_ANGLE = findPolarAngle(P.at(0), P.at(i));


		if (P.at(i).POLAR_ANGLE < 0.0)
			P.at(i).POLAR_ANGLE = 180 + P.at(i).POLAR_ANGLE;
	}

	sort(P.begin() + 1, P.end(),
		[](coord& a, coord& b) {
			return a.POLAR_ANGLE < b.POLAR_ANGLE;
		});

	// Going to need to check for duplicate angles in the newly sorted list (skipping p0)
	// Then if any are found we need to remove the one that is closer to p0
	

	// Main loop of Grahams 
	for (int i = 0; i < np; i++) {
		while (stack.size() > 1 && orientation(nextToTop(stack), stack.top(), P.at(i)) != 2) {
			stack.pop();
		}

		stack.push(P.at(i));
	}

	return stackToVector(stack);
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

	return (value > 0) ? 1 : 2; // Clock vs Counterclock wise
}



partitions partition(vector<coord> P, int k, int m) {
	partitions Q;
	int n = int(P.size());
	vector<coord>::iterator it = P.begin();

	for (int i = 0; i < k; i++) {
		int currSize = min(m, int(distance(it, P.end())));
		vector<coord> tmp(it, it + currSize);
		Q.push_back(tmp);
		it += currSize;

		// Break out when all points distributed
		if (it == P.end()) 
			break;
	}

	return Q;
}



vector<coord> readPoints(ifstream& fin) {
	vector<coord>	P;
	coord			c;

	while (!fin.eof()) {
		fin >> c.x >> c.y;
		P.push_back(c);
	}

	return P;
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
