#include "header.h"

int main(int argc, char** argv) {
	// Declare variables
	ifstream		i1;
	ofstream		fout;
	partitions		subHull;
	vector<coord>	ConvexHull, hull;
	vector<coord>	P;
	coord		    tangent, p0 = { -9, 3 }, p1 = { 7, -9 };
    string          fileName1 = "points\\", fileName2 = "src\\";

	// Make sure to put your sets in the points directory or alter the code lol
	if (argc != 3) {
		cout << "Invalid input\n" << "Usage: main.exe <outputfile> <file1>";
		exit(3);
	}

	fileName1 += static_cast<string>(argv[2]);
	fileName2 += static_cast<string>(argv[1]);

    i1.open(fileName1);
	fout.open(fileName2);

    if (!i1.is_open()){
        cout << fileName1 << " failed to open.\n";
        exit(1);
    }
	if (!fout.is_open()) {
		cout << fileName2 << " failed to open.\n";
		exit(1);
	}

	// testing jarvis march shit
	P = readPoints(i1);
	
	hull = GrahamsScan(P);

	//TODO: Bug fixes something isn't working... :3
	ConvexHull = ChansAlgorithm(P);

	outputHull(fout, ConvexHull);

	fout.close();
    i1.close();

	return 0;
}

// function definitions go here
vector<coord> ChansAlgorithm(vector<coord> P) {
	partitions		subHulls;
	vector<coord>	hull;
	int				m, k;
	bool			tf;

	// TODO: rapidly increase m and compute the hull
	for (int t = 1; t <= int(P.size()); t++) {
		m = static_cast<int>(std::min(pow(2, pow(2, t)), double(P.size())));
		k = computeK(int(P.size()), m);

		subHulls = subConvexHulls(P, k, m);

		tf = JarvisMarch(hull, subHulls, P, k, m);

		if (tf)
			return hull;

		hull.clear();
	}


	return hull;
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


// Credit to Tom Switzer (tixxit) https://gist.github.com/tixxit/252229
coord findTangentPoint(vector<coord> Q, coord p0) {
	int					n = int(Q.size());
	int					left = 0, right = n - 1;
	int					lPrev = orientation(p0, Q[0], Q[n - 1]);
	int					lNext = orientation(p0, Q[0], Q[(left + 1) % n]);
	int mid, mPrev, mNext, mSide;

	while (left < right) {
		mid = (left + right) / 2;
		mPrev = orientation(p0, Q[mid], Q[(mid - 1 + n) % n]);
		mNext = orientation(p0, Q[mid], Q[(mid + 1) % n]);
		mSide = orientation(p0, Q[left], Q[mid]);

		// Small hull size
		if (n == 1)
			return Q[0];
		else if (n == 2) {
			int o = orientation(p0, Q[0], Q[1]);
			if (o == CCW || o == COLLINEAR)
				return Q[0];
			else
				return Q[1];
		}

		if (mPrev != CCW && mNext != CCW) {
			if (mPrev == 0 && mNext == 0)
				return Q[mid - 1];

			return Q[mid];
		}
		else if (lNext == 0)
			return Q[left];
		else if (mSide == CW && (lNext == CCW || lPrev == lNext)
			|| mSide == CCW && mPrev == CCW)
			right = mid;
		else {
			left = mid + 1;
			lPrev = -mNext;
			lNext = orientation(p0, Q[left], Q[(left + 1) % n]);
		}
	}
	
	return Q[left];
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
		while (stack.size() > 1 && orientation(nextToTop(stack), stack.top(), P.at(i)) != CCW) {
			stack.pop();
		}

		stack.push(P.at(i));
	}

	return stackToVector(stack);
}



bool JarvisMarch(vector<coord>& convexHull, partitions Q, vector<coord> P, int k, int m) {
	// TODO: All of this bullshit ig
	Vec2			v1, v2, n1, n2;
	coord			tan, currTan, p = { 10000000000, 0 }, p0;
	int				b = findBottomMost(P);
	double			angle = -360;

	p0 = P.at(b);
	convexHull.push_back(p0);

	for (int j = 0; j < m; j++) {
		for (int i = 0; i < k; i++) {
			tan = findTangentPoint(Q.at(i), p0);

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
		angle = -360;

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

	return (value > 0) ? 1 : -1; // CW vs CCW
}



void outputHull(ofstream& fout, vector<coord> hull) {
	for (int i = 0; i < int(hull.size()); i++) {
		fout << hull[i].x << "," << hull[i].y << "\n";
	}
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