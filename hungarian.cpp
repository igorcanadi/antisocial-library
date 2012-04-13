/*********************
 *     Hungarian 
 * *******************/
// tested on ACM ICPC live problem 3198
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>

using namespace std;

#define MAX_R 100 // mora biti >= MAX_C
#define MAX_C 100
#define VELIKO 1000000

bool zero(int x) { return x == 0; }
bool zero(double x) {return fabs(x) < 1e-12; }

template <typename tip>
struct hungarian {
	int n, m;
	tip costs[MAX_R][MAX_C]; // pocente vrijednosti NE OSTAJU ocuvane
	bool ret[MAX_R][MAX_C]; // na kraju, jedinice su matching
	int stars;
	int star_r[MAX_R], star_c[MAX_C];
	int prime_r[MAX_R], prime_c[MAX_C];
	int cover_r[MAX_R], cover_c[MAX_C];
	
	void matching() {
		for ( ; n < m; ++n) 
			for (int c = 0; c < m; ++c) 
				costs[n][c] = 0;
		for (int r = 0; r < n; ++r) { star_r[r] = -1; cover_r[r] = 0; }
		for (int c = 0; c < m; ++c) { star_c[c] = -1; cover_c[c] = 0; }
		stars = 0;
		step1();
	}

	void step1() {
		for (int r = 0; r < n; ++r) {
			tip mini = VELIKO;
			for (int c = 0; c < m; ++c) mini = min(mini, costs[r][c]);
			for (int c = 0; c < m; ++c) costs[r][c] -= mini;
		}
		step2();
	}

	void step2() {
		for (int r = 0; r < n; ++r) {
			for (int c = 0; c < m; ++c) {
				if (star_c[c] != -1) continue;
				if (!zero(costs[r][c])) continue;
				star_r[r] = c;
				star_c[c] = r;
				++stars;
				break;
			}
		}
		step3();
	}

	void step3() {
		if (stars == m) {
			for (int r = 0; r < n; ++r)
				for (int c = 0; c < m; ++c)
					ret[r][c] = (star_r[r] == c);
			return; // zavrsetak algoritma
		}
		for (int r = 0; r < n; ++r) cover_r[r] = 0;
		for (int c = 0; c < m; ++c) cover_c[c] = star_c[c] != -1;

		step4();
	}

	void step4() {
		queue <int> Q;
		for (int c = 0; c < m; ++c) if (!cover_c[c]) Q.push(c);

		for (; !Q.empty(); Q.pop()) {
			int c = Q.front();
			for (int r = 0; r < n; ++r) {
				if (cover_r[r]) continue;
				if (!zero(costs[r][c])) continue;
				if (star_r[r] != -1) {
					cover_c[star_r[r]] = 0;
					cover_r[r] = 1;
					prime_r[r] = c;
					prime_c[c] = r;
					Q.push(star_r[r]);
				} else {
					step5(r, c);
					return;
				}
			}
		}
		tip mini = VELIKO;
		for (int r = 0; r < n; ++r) {
			if (!cover_r[r]) 
				for (int c = 0; c < m; ++c) 
					if (!cover_c[c])
						mini = min(mini, costs[r][c]);
		}
		step6(mini);
	}
	void step5(int r, int c) {
		while (star_c[c] != -1) {
			int tmp_r = star_c[c];
			star_r[r] = c;
			star_c[c] = r;
			c = prime_r[tmp_r];
			r = tmp_r;
		}
		star_r[r] = c;
		star_c[c] = r;
		stars++;
		step3();
	}
	void step6(tip mini) {
		for (int r = 0; r < n; ++r)
			for (int c = 0; c < m; ++c) 
				if (cover_r[r] && cover_c[c]) costs[r][c] += mini;
				else if (!cover_r[r] && !cover_c[c]) costs[r][c] -= mini;
		step4();
	}
};


hungarian <int> H;
char ploca[100][100];
int N, M;

bool load() {
	scanf("%d%d", &N, &M);

	for (int i = 0; i < N; ++i) {
		scanf("%s", ploca[i]);
	}

	return N+M;
}

int my_abs(int x) { return x < 0 ? -x : x; }
int dist(pair <int, int> a, pair <int, int> b) {
	return my_abs(a.first - b.first) + my_abs(a.second - b.second);
}
vector <pair <int, int> > houses, men;

void generate_costs() {
	houses.clear(); men.clear();

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			if (ploca[i][j] == 'H') houses.push_back(make_pair(i, j));
			if (ploca[i][j] == 'm') men.push_back(make_pair(i, j));
		}
	}

	H.n = H.m = houses.size();
	for (int i = 0; i < houses.size(); ++i) {
		for (int j = 0; j < men.size(); ++j) {
			H.costs[i][j] = dist(houses[i], men[j]);
		}
	}
}

int main() {
	while (load()) {
		generate_costs();
		H.matching();
		int sol = 0;
		for (int i = 0; i < houses.size(); ++i) {
			for (int j = 0; j < men.size(); ++j) {
				if (H.ret[i][j]) sol += dist(houses[i], men[j]);
			}
		}
		printf("%d\n", sol);
	}
	return 0;
}

