#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#define MAX 2000001000LL
#define EPS 1e-7

using namespace std;

class Line {
public:
	double x1, y1, x2, y2;
	long long slope, b;

	Line () {}
	Line (double _x1, double _y1, double _x2, double _y2, long long _slope, long long _b) :
		x1(_x1), y1(_y1), x2(_x2), y2(_y2), slope(_slope), b(_b) {}
};

double my_abs(double a) { return a < 0 ? -a : a; }

bool operator < (const Line &a, const Line &b) {
	return a.slope < b.slope;
}

double intersection(double a1, double b1, double a2, double b2) {
	if (my_abs(a1 - MAX) < EPS || my_abs(a2 - MAX) < EPS) {
		return MAX;
	}
	return (b2 - b1) / (a1 - a2);
}

double intersection(double a, double b, const Line &c) {
	if (my_abs(a - MAX) < EPS || c.slope == MAX) {
		return MAX;
	}
	return intersection(a, b, c.slope, c.b);
}

double intersection(const Line &a, const Line &b) {
	if (a.slope == MAX || b.slope == MAX) {
		return MAX;
	}
	return intersection(a.slope, a.b, b.slope, b.b);
}

class Hull {
public:
	// it also deletes all lines left of x
	long long getMax(long long x) {
		for (set <Line>::iterator itr = envelope.begin(); itr != envelope.end(); ++itr) {
			if (x + EPS >= itr->x1 && x <= itr->x2 + EPS) {
				return itr->b + x * itr->slope;
			}

			if (itr != envelope.begin() && itr->slope != MAX) {
				// erase it 
				set <Line>::iterator t = itr;
				--itr;
				envelope.erase(t);
				t = itr;
				++t;
				double xIntersect = intersection(*itr, *t);

				Line a = *itr;
				a.x2 = xIntersect;
				a.y2 = a.y1 + (a.x2 - a.x1) * a.slope;

				envelope.erase(itr);
				itr = envelope.insert(a).first;

				a = *t;
				a.x1 = xIntersect;
				a.y1 = a.y2 - (a.x2 - a.x1) * a.slope;

				envelope.erase(t);
				envelope.insert(a);
			}
		}
	}

	// ax + b
	void put(long long a, long long b) {
		// brisi sve desno koje pokrivam
		for (set <Line>::iterator itr = envelope.lower_bound(Line(0, 0, 0, 0, a, 0)); itr != envelope.end() && itr->slope != MAX; ++itr) {
			bool haveToDelete = false;
			if (a == itr->slope) {
				if (b > itr->b) {
					haveToDelete = true;
				} else {
					return;
				}
			} else {
				double x = intersection(a, b, *itr);
				if (x+EPS >= itr->x2) {
					haveToDelete = true;
				}
			}
			
			if (haveToDelete) {
				set <Line>::iterator t = itr;
				--itr;
				envelope.erase(t);
			} else {
				break;
			}
		}

		// brisi sve lijevo koje pokrivam
		for (set <Line>::iterator itr = envelope.lower_bound(Line(0, 0, 0, 0, a, 0)); itr != envelope.begin(); ) {
			--itr;
			if (itr == envelope.begin()) {
				break;
			}
			if (itr->slope == MAX) {
				continue;
			}

			double x = intersection(a, b, *itr);
			if (x <= itr->x1+EPS) {
				set <Line>::iterator t = itr;
				++itr;
				envelope.erase(t);
			} else {
				break;
			}
		}

		set <Line>::iterator left, right;

		right = envelope.lower_bound(Line(0, 0, 0, 0, a, 0));
		left = right;
		--left;
		
		// t is the new segment
		Line t;

		// set up left
		Line l = *left;
		l.x2 = intersection(a, b, l);
		l.y2 = l.y1 + (l.x2-l.x1) * l.slope;

		t.x1 = l.x2;
		t.y1 = l.y2;

		if (l.x2 > left->x2 || l.x2 < left->x1) {
			// nothing to do here
			return;
		}

		// set up right
		Line r = *right;
		r.x1 = intersection(a, b, r);
		t.x2 = r.x1;
		t.y2 = r.x1 * a + b;
		r.y1 = t.y2;

		if (r.slope != MAX && (r.x1 < right->x1 || r.x1 > right->x2)) {
			// nothing to do here
			return; 
		}

		t.slope = a;
		t.b = b;

		envelope.erase(left);
		envelope.insert(l);

		envelope.erase(right);
		envelope.insert(r);

		envelope.insert(t);
	}

	Hull(long long horizontal) {
		envelope.insert(Line(-MAX, horizontal, MAX, horizontal, 0, horizontal));
		envelope.insert(Line(0, 0, 0, 0, MAX, 0));
	}

	void output() {
		for (set<Line>::iterator itr = envelope.begin(); itr != envelope.end(); ++itr) {
			printf("{(%.1lf, %.1lf) -> (%.1lf, %.1lf), (%lld, %lld)}\n", itr->x1, itr->y1, itr->x2, itr->y2, itr->slope, itr->b);
		}
		printf("\n");
	}

	set <Line> envelope;
private:
};

class Machine {
public:
	long long d, p, r, g;
};

bool operator < (const Machine &a, const Machine &b) {
	return a.d < b.d;
}

vector <Machine> machines;
int N, C, D;

bool load() {
	scanf("%d%d%d", &N, &C, &D);

	machines.resize(N);

	for (int i = 0; i < N; ++i) {
		scanf("%lld%lld%lld%lld", &machines[i].d, &machines[i].p, &machines[i].r, &machines[i].g);
	}

	return N + C + D;
}

int main() {
	int tt = 0;

	while (load()) {
		Hull hull(C);

		sort(machines.begin(), machines.end());
		for (int i = 0; i < machines.size(); ++i) {
			long long money = hull.getMax(machines[i].d);
			if (money < machines[i].p) {
				continue;
			}

			hull.put(machines[i].g, (money - machines[i].p + machines[i].r) - machines[i].d * machines[i].g - machines[i].g);
		}

		printf("Case %d: %lld\n", ++tt, hull.getMax(D+1));
	}

	return 0;
}
