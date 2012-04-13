/***************************
 *     Number theory 
 * *************************/
// WA library.
// CRT NOT TESTED
// extended gcd works
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <algorithm>

using namespace std;

ldiv_t div_correct(long y, long x) {
	ldiv_t v = ldiv(y, x);
	if (y < 0 && v.rem != 0) {
		v.quot -= 1;
		v.rem += labs(x);
	}
	return v;
}

pair<long, long> extended_gcd(long a, long b) {
	if (a % b == 0) 
		return pair<long, long>(0, 1);
	else {
		ldiv_t v = div_correct(a, b);
		pair<long, long> t = extended_gcd(b, v.rem);
		return pair<long,long>(t.second, t.first - t.second * v.quot);
	}
}

long crt(long *a, long *n, long r)
{
	long N = 1;
	for (int k = 0; k < r; k++)
		N *= n[k];

	long s = 0;
	for (int k = 0; k < r; k++)
	{
		long p = N / n[k];
		long x = extended_gcd(p, n[k]).first;
		s += a[k] * p * x;
		s %= N;
	}
	return s;
}

int main() {
	long A, B;
	while (scanf("%ld%ld", &A, &B) != EOF) {
		pair<long,long> xy = extended_gcd(A, B);
		printf("%ld %ld %ld\n", xy.first, xy.second, A * xy.first + B * xy.second);
	}
}
