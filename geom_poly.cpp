#include <vector>
#include <complex>
#include <cmath>

using namespace std;

#define EPS 1E-9
#define det(a, b) imag(conj(a)*(b))
#define dot(a, b) real(conj(a)*(b))
#define sign(a) (abs(a) < EPS ? 0 : a > 0 ? 1 : -1)
#define signstar(a) (sign(a) == -1 ? -1 : 1)

#define PI (2.0 * acos(0.0))
#define sq(x) ((x) * (x))
#define law_of_cosines(a, b, c) acos(min(max((sq(a) + sq(b) - sq(c)) / (2 * (a) * (b)), -1.0), 1.0))

typedef complex<double> pt;
typedef pair<pt, pt> line;
typedef vector<pt> polygon;
typedef line seg;

/*
 * Assume V[0] == V[-1] 
 */
double signed_area(polygon &V) {
	double A = 0.0;
	for (unsigned i = 1; i < V.size(); i++)
		A += det(V[i - 1], V[i]);
	return A / 2;
}

/*
 * Assume V[0] == V[-1] 
 */
pt centroid(polygon &V) {
	pt c = pt(0.0, 0.0);
	for (unsigned i = 1; i < V.size(); i++)
		c += (V[i - 1] + V[i]) * pt(det(V[i - 1], V[i]), 0.0);
	return c / pt(6 * signed_area(V), 0.0);
}

#define det(a, b) imag(conj(a)*(b))

/* Returns 2 * (area of polygon V)
 *  - Assumes V[0] == V[-1]
 */
double area_polygon(polygon &V) {
    double A = 0.0;
    for (unsigned i = 1; i < V.size(); i++)
        A += det(V[i - 1], V[i]);
    return abs(A);
}

/*  - Assumes convex V in ccw order
 *  - Assumes V[0] == V[-1]
 */
bool inside_convex(pt p, polygon& V) {
    for (unsigned i = 1; i < V.size(); i++)
        if (det(V[i] - V[i - 1], p - V[i - 1]) < -EPS)
            return false;
    return true;
}

/*
 * Tests whether p is in simple polygon V
 *  - Assumes V[0] == V[-1]
 *  - Assumes p does not intersect V
 *  - Assumes segment p-q does not intersect corners
 *  - Assumes q is large enough
 */
bool inside_polygon(pt p, polygon& V) {
    pt q = polar(1e8, 1.2345);
    int s = 0;
    for (int i = 1; i < V.size(); i++)
        s += xSegSeg(p, q, V[i - 1], V[i]);
    return s % 2 == 1;
}

double pack_in_cirlce(vector<int> &side_lengths)
{
	double max_lo = 0.0, max_hi = 0.0, lo, hi, r, alpha;
	int max_i;
	bool outside = true;

	for (int i = 0; i < side_lengths.size(); i++)
	{
		if (0.5 * side_lengths[i] > max_lo)
		{
			lo = max_lo = 0.5 * side_lengths[i];
			max_i = i;
		}
		hi = max_hi += 2.0 * side_lengths[i];
	}

	while (abs(hi - lo) > EPS || (outside && lo < max_lo + EPS))
	{
		if (outside && abs(hi - lo) < EPS && lo < max_lo + EPS)
		{
			lo = max_lo;
			hi = max_hi;
			outside = false;
		}

		r = (hi + lo) / 2.0;
		alpha = 0.0;

		for (int i = 0; i < n; i++)
			alpha += (outside || i != max_i ? 1.0 : -1.0) * law_of_cosines(r, r, L[i]);

		if (outside ? alpha < 2 * PI : alpha > EPS)
			hi = r;
		else
			lo = r;
	}

	double area = 0.0;
	for (int i = 0; i < n; i++)
		area += (outside || i != max_i ? 1.0 : -1.0) * 0.5 * r * r * sin(law_of_cosines(r, r, L[i]));
	return area;
}

/*$*/
int main() {
	return 0;
}
/*$*/
