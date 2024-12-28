#include "header.h"

int main(int argc, char** argv) {
	// Declare variables
	ifstream		i1, i2;
	partitions		subHull;
	stack<coord>	ConvexHull;
	vector<coord>	hull;
	vector<coord>	P;
	coord		    tangent, p0 = { -1, -8 };
    string          fileName1 = "points\\";

	if (argc != 3) {
		cout << "Invalid input\n" << "Usage: main.exe <path-to-file> <path-to-file2>";
		exit(3);
	}

	fileName1 += static_cast<string>(argv[1]);

    i1.open(fileName1);

    if (!i1.is_open()){
        cout << fileName1 << " failed to open.\n";
        exit(1);
    }

	/* 
	//testing shit
	P = readPoints(i1);
	hull = GrahamsScan(P);

	int m = int(hull.size());
	int k = computeK(int(P.size()), m);

	subHull = subConvexHulls(P, k, m);
	*/

	// testing tangent shit
	P = readPoints(i1);
	hull = GrahamsScan(P);

	tangent = findTangentPoint(hull, p0, 0, int(hull.size()) - 1);


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
	k = int(k0);

	return k;
}



double distance(coord p1, coord p2) {
	return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}



void findDistance(vector<coord>& P) {
	for (int i = 1; i < int(P.size()); i++) {
		P.at(i).DISTANCE = distance(P.at(0), P.at(i));
	}
}



int findLeftMost(vector<coord> P) {
	int	tmp = 0;

	for (int i = 1; i < int(P.size()); i++) {
		if (P.at(i).x < P.at(tmp).x)
			tmp = i;
	}

	return tmp;
}



int findBottomMost(vector<coord> P) {
	int tmp = 0;

	for (int i = 1; i < int(P.size()); i++) {
		if (P.at(i).y < P.at(tmp).y)
			tmp = i;
	}

	return tmp;
}



coord findTangentPoint(vector<coord> Q, coord p0, int low, int high) {
	coord	tmp, tmpP, tmpN;
	int		prev, next;
	
	int mid = (high + low) / 2; // tangent point hopefully

	prev = mid - 1;
	next = mid + 1;
	
	// TODO: Binary search ig?
	if (orientation(Q.at(mid), p0, Q.at(prev)) == 1
		&& orientation(Q.at(mid), p0, Q.at(next)) == 1)
		return Q.at(mid);



	return tmpP;
}



double polarAngle(coord p1, coord p2) {
	return atan2((p2.y - p1.y), (p2.x - p1.x)) * 180 / PI;
}



void findPolarAngle(vector<coord>& P) {
	for (int i = 1; i < int(P.size()); i++) {
		P.at(i).POLAR_ANGLE = polarAngle(P.at(0), P.at(i));

		if (P.at(i).POLAR_ANGLE < 0.0)
			P.at(i).POLAR_ANGLE = 180 + P.at(i).POLAR_ANGLE;
	}
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

	return Q;
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