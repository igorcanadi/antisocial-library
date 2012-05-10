#include <cmath>
#include <complex>
#include <vector>
#include <map>

using namespace std;

#define EPS 1e-9
#define INF 1000000000

typedef complex<double> pt;
typedef pair<pt, double> circle;
typedef pair<pt, pt> line;
typedef vector<pt> polygon;
typedef line seg;

#define det(a, b) imag(conj(a)*(b))
#define dot(a, b) real(conj(a)*(b))
#define sign(a) (abs(a) < EPS ? 0 : a > 0 ? 1 : -1)
#define signstar(a) (sign(a) == -1 ? -1 : 1)

pt xLineLine(line a, line b)
{
	return
		( det(a.first, a.second) * (b.first - b.second) - det(b.first, b.second) * (a.first - a.second) ) / det(a.first - a.second, b.first - b.second) ;
}

bool xPtSeg(pt p, seg l)
{
	return abs(abs(p - l.first) + abs(p - l.second) - abs(l.first - l.second)) < EPS;
}

bool xPtSeg_open(pt p, seg l)
{
	return
		abs(p - l.first) > EPS && 
		abs(p - l.second) > EPS && 
		xPtSeg(p, l);
}

bool parallel(line a, line b)
{
	return abs(det(a.first - a.second, b.first - b.second)) < EPS;
}

bool xLineSeg(line a, seg b, pt &x)
{
	x = xLineLine(a, b);
	return !parallel(a, b) && xPtSeg(x, b);
}

bool xLineSeg_open(line a, seg b, pt &x)
{
	x = xLineLine(a, b);
	return !parallel(a, b) && xPtSeg_open(x, b);
}

bool xPtLine(pt p, line l)
{
	double
		da = abs(p - l.first),
		db = abs(p - l.second),
		dc = abs(l.first - l.second);

	return abs(2 * (da + db + dc) - max(da, max(db, dc))) < EPS;
}

double ccw(pt a, pt b, pt c)
{
	return det(a - b, c - a);
}

bool comp_pt(const pt a, const pt b)
{
	if (abs(real(a - b)) < EPS)
		return imag(b - a) > EPS;
	return real(b - a) > EPS;
}

/*
 * Assume p[0] == p[-1] 
 * Tested: UVA 11460
 */
vector<polygon> xLinePoly(line l, polygon p)
{
	vector<polygon> x;
	vector<vector<int> > s(2, vector<int>());
	vector<int> z;
	map<int, int> z_map, pos;
	pt u;
	double c;

	for (int i = 0; i < p.size(); i++)
	{
		if (i > 0 && xLineSeg_open(l, line(p[i - 1], p[i]), u))
			p.insert(p.begin() + i, u);

		c = ccw(l.first, l.second, p[i]);
		if (c > -EPS)
			s[0].push_back(i);
		if (c < EPS)
			s[1].push_back(i);

		if (abs(c) < EPS)
			if (z.size() == 0 || comp_pt(p[z.back()], p[i]))
				z.push_back(i);
			else
			{
				int lo = 0, hi = z.size(), mid;
				while (hi - lo > 0)
					if (comp_pt(p[i], p[z[mid = (hi + lo - 1) / 2]]))
						hi = mid;
					else 
						lo = mid + 1;
				z.insert(z.begin() + lo, i);
			}
	}

	for (int i = 0; i < z.size(); i++)
		z_map[z[i]] = i;

	for (int k = 0; k < s.size(); k++)
	{
		if (s[k].front() != s[k].back())
			s[k].push_back(s[k].front());

		for (int i = 1; i < s[k].size(); i++)
			if (z_map.count(s[k][i - 1]) > 0 && z_map.count(s[k][i]) > 0)
				for (int j = z_map[s[k][i - 1]] + sign(z_map[s[k][i]] - z_map[s[k][i - 1]]); j != z_map[s[k][i]]; j += sign(z_map[s[k][i]] - z_map[s[k][i - 1]]))
					s[k].insert(s[k].begin() + i++, z[j]);

		pos.clear();
		for (int i = 0; i < s[k].size(); i++)
		{
			if (pos.count(s[k][i]) != 0)
			{
				x.push_back(polygon());
				for (int j = pos[s[k][i]]; j < i; j++)
					x.back().push_back(p[s[k][j = pos[s[k][j]]]]);
			}
			pos[s[k][i]] = i;
		}
	}

	for (int i = x.size() - 1; i >= 0; i--)
		if (x[i].size() < 3)
			x.erase(x.begin() + i);
		else
			x[i].push_back(x[i].front());

	return x;
}

/* True if p is on segment a-b.
 *  - Assume a != b
 *  - True at endpoints      */
bool xPtSeg(pt p, pt a, pt b)
{
    return
        abs(det(p-a, b-a)) < EPS &&
        dot(p-a, b-a) > -EPS &&
        dot(p-b, a-b) > -EPS ;
}

/* True if segment a-b intersects segment c-d 
 *  -- True at endpoints. */
bool xSegSeg(pt a, pt b, pt c, pt d) 
{
    double
        ta = det(c-a, d-a),
        tb = det(d-b, c-b),
        tc = det(a-c, b-c),
        td = det(b-d, a-d) ;
    return 
        xPtSeg(a, c, d) ||
        xPtSeg(b, d, c) ||
        xPtSeg(c, a, b) ||
        xPtSeg(d, b, a) ||
        sign(ta) && sign(ta) == sign(tb) &&
        sign(tc) && sign(tc) == sign(td) ;
}

/* True if segment a-b intersects segment c-d 
 *  -- False at endpoints.
 *  -- False if segments are parallel. */
bool xSegSeg_open(pt a, pt b, pt c, pt d) 
{
    double 
        ta = det(c-a, d-a),
        tb = det(d-b, c-b),
        tc = det(a-c, b-c),
        td = det(b-d, a-d) ;
    return 
        sign(ta) && sign(ta) == sign(tb) &&
        sign(tc) && sign(tc) == sign(td) ;
}

/* True if segment a-b intersects segment c-d 
 *  -- Assumes that colinear and corner cases never occur. */
bool xSegSeg_simple(pt a, pt b, pt c, pt d) 
{
    return 
        det(c-a, d-a) > EPS == det(d-b, c-b) > EPS &&
        det(a-c, b-c) > EPS == det(b-d, a-d) > EPS ;
}

/* True if segment a-b intersects segment c-d 
 *  -- Assumes that colinear and corner cases never occur.
 *  -- Cheesy method using xLineLine                        
 *     This also applies to closed corners, xSegLine, etc, but
 *          colinear cases need to be a special case           */
// NOT TESTED
bool xSegSeg_simple2(pt a, pt b, pt c, pt d) 
{
    pt x = 
        ( det(a, b) * (c - d) - det(c, d) * (a - b) )
            / det(a-b, c-d) ;
    double s = real((x-a)/(b-a)), 
           t = real((x-c)/(d-c)) ;
    return 
        EPS < s && s < 1-EPS &&
        EPS < t && t < 1-EPS   ;
}

/* True if segment a-b intersects line --c-d-- 
 *  -- Assumes that colinear and corner cases never occur. */
// Tested by ICPC 2005 Finals - GSM
bool xSegLine_simple(pt a, pt b, pt c, pt d) 
{
    return 
        det(c-a,d-a) > EPS == det(d-b,c-b) > EPS ;
}

/* Intersection of line a-b and line c-d
 *  -- Returns an "invalid" complex if a-b c-d parallel. 
 */
pt xLineLine(pt a, pt b, pt c, pt d)
{
    //assert( abs(det(a-b, c-d)) > EPS );

    return 
        ( det(a, b) * (c - d) - det(c, d) * (a - b) )
            / det(a-b, c-d) ;
}

void perp_bisector(pt a, pt b, pt &m, pt &d)
{
	m = (a + b) / pt(2.0, 0.0);
	d = (b - a) * pt(0.0, 1.0);
}

/* Intersection of a line and a circle
 *  -- Returns the number of points of intersection, 0, 1 or 2
 *  -- Populates points a and b with the points of intersection
 *  Tested: UVA 11037
 */
int xLineCircle(line x, circle y, pt &a, pt &b)
{
	double dpl = det(x.second - x.first, y.first - x.first) / abs(x.second - x.first);
	pt m, d;
	perp_bisector(x.first, x.second, m, d);
	pt i = y.first - d * dpl / abs(d);

	if (abs(abs(dpl) - y.second) < EPS)
	{
		a = i;
		return 1;
	}
	else if (abs(dpl) < y.second - EPS)
	{
		double h = sqrt(y.second * y.second - dpl * dpl);
		a = i + h * (x.second - x.first) / abs(x.second - x.first);
		b = i - h * (x.second - x.first) / abs(x.second - x.first);
		return 2;
	}
	else
	{
		return 0;
	}
}

/* Intersection of two circles
 *  -- Returns the number of points of intersection, 0, 1 or 2 or INF
 *  -- Populates points m and n with the points of intersection
 *  Tested: UVA 11037
 */
int xCircleCircle(circle x, circle y, pt &m, pt &n)
{
	double d = abs(x.first - y.first);
	if (abs(x.second - y.second) < EPS && abs(x.first - y.first) < EPS)
	{
		return INF;
	}
	else if (abs(x.second + y.second - d) < EPS)
	{
		m = (x.first + y.first) / pt(2.0, 0.0);
		return 1;
	}
	else if (d < x.second + y.second - EPS && d > abs(x.second - y.second) + EPS)
	{
		double a = (x.second * x.second - y.second * y.second + d * d) / (2 * d);
		double h = sqrt(x.second * x.second - a * a);
		pt p = x.first + a * (y.first - x.first) / d;
		m = pt(real(p) + h * (imag(y.first) - imag(x.first)) / d, imag(p) - h * (real(y.first) - real(x.first)) / d);
		n = pt(real(p) - h * (imag(y.first) - imag(x.first)) / d, imag(p) + h * (real(y.first) - real(x.first)) / d);
		return 2;
	}
	else
	{
		return 0;
	}
}

/*$*/
int main()
{
	return 0;
}
/*$*/
