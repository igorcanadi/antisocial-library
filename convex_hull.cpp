// tested on ACM problem 11065
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

class Point {
public:
	int x, y;

	Point () {}
	Point (int _x, int _y) : x(_x), y(_y) {}
};

int ccw(const Point &a, const Point &b, const Point &c) {
	return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
}

double dist(const Point &a, const Point &b) {
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

class PointsCmp {
public:
	Point reference;

	bool operator () (const Point &a, const Point &b) {
		int t = ccw(reference, a, b);
		if (t != 0) return t > 0;
		return dist(reference, a) < dist(reference, b);
	}

	PointsCmp(const Point &_reference) : reference(_reference) {}
};

class Polygon {
public:
	vector <Point> points;

	Polygon convexHull() {
		Polygon tmp = *this;

		for (int i = 1; i < points.size(); ++i) {
			if (tmp.points[i].y < tmp.points[0].y) {
				swap(tmp.points[i], tmp.points[0]);
			}
		}

		sort(tmp.points.begin()+1, tmp.points.end(), PointsCmp(tmp.points[0]));

		Polygon hull;

        if (tmp.size() < 3) {
            return hull;
        }

		hull.points.push_back(tmp.points[0]);
		hull.points.push_back(tmp.points[1]);
		hull.points.push_back(tmp.points[2]);

		int M = hull.points.size();

		for (int i = 3; i < points.size(); ++i) {
			while (ccw(hull.points[M-2], hull.points[M-1], tmp.points[i]) < 0) {
				hull.points.pop_back();
				--M;
			}
			hull.points.push_back(tmp.points[i]);
			++M;
		}

		return hull;
	}

	double area() {
		int retval = 0.0;
		for (int i = 0; i < points.size(); ++i) {
			retval += points[i].x * points[(i+1) % points.size()].y - points[(i+1) % points.size()].x * points[i].y;
		}

		return ((retval < 0) ? -retval : retval) / 2.0;
	}

	void output() {
		for (int i = 0; i < points.size(); ++i) {
			printf("(%d, %d) ", points[i].x, points[i].y);
		}
		printf("\n");
	}
};

Polygon P;

bool load() {
	int n;
	scanf("%d", &n);
	P.points.resize(n);

	for (int i = 0; i < n; ++i) {
		scanf("%d%d", &P.points[i].x, &P.points[i].y);
	}

	return n;
}

int main() {
	int tilenum = 1;
	while (load()) {
		printf("Tile #%d\nWasted Space = %.2lf %%\n\n", tilenum++,  (1.0 - (P.area() / P.convexHull().area())) * 100.0);
	}
	return 0;
}
