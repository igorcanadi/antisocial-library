#include <cstdio>
#include <cstring>
#include <vector>
#define MAX 50002

using namespace std;

int N, M;
int discovery_time[MAX], articulation_point[MAX];
int global_time;

vector < vector <int> > E;

bool load(int tt) {
	scanf("%d", &M);

	E.clear();
	N = 0;
	memset(discovery_time, 0, sizeof discovery_time);
	memset(articulation_point, 0, sizeof articulation_point);
	global_time = 0;

	for (int i = 0; i < M; ++i) {
		int a, b;
		scanf("%d%d", &a, &b);
		--a; --b;

		while (E.size() <= max(a, b)) {
			++N;
			E.push_back(vector <int>());
		}

		E[a].push_back(b);
		E[b].push_back(a);
	}

	return M;
}

int find_articulation_points(int s, int p) {
	discovery_time[s] = ++global_time;

	int min_be = discovery_time[s];
	int children = 0;

	for (int i = 0; i < E[s].size(); ++i) {
		if (E[s][i] == p) {
			continue;
		}

		if (discovery_time[E[s][i]] == 0) {
			// normal edge
			int t = find_articulation_points(E[s][i], s);
			if (p != -1 && t >= discovery_time[s]) {
				articulation_point[s] = 1;
			}
			min_be = min(min_be, t);
			++children;
		} else {
			// back edge
			min_be = min(min_be, discovery_time[E[s][i]]);
		}
	}

	if (p == -1 && children > 1) {
		articulation_point[s] = 1;
	}

	return min_be;
}

int shafts;
long long ways;

// {min_be, nodes_subtree}
pair <int, int> solve(int s, int p) {
	discovery_time[s] = ++global_time;

	int min_be = discovery_time[s];
	int nodesSubtree = 1;

	for (int i = 0; i < E[s].size(); ++i) {
		if (E[s][i] == p) {
			continue;
		}

		if (discovery_time[E[s][i]] == 0) {
			// normal edge
			pair <int, int> t = solve(E[s][i], s);
			if (t.second != 0) {
				// ispod nema shafta
				if (p == -1 || (p != -1 && t.first >= discovery_time[s])) {
					// moram staviti shaft ispod jer inace umrem
					shafts++;
					ways *= t.second;
					nodesSubtree = 0;
				} else if (nodesSubtree) {
					// ne moram staviti shaft, samo zapamti kolko nodeova imam
					nodesSubtree += t.second;
				}
			} else {
				// ispod ima shafta. zamapti to
				nodesSubtree = 0;
			}
			min_be = min(min_be, t.first);
		} else {
			// back edge
			min_be = min(min_be, discovery_time[E[s][i]]);
		}
	}

	return make_pair(min_be, nodesSubtree);
}

int main() {
	int tt = 0;
	while (load(tt)) {
		find_articulation_points(0, -1);
		memset(discovery_time, 0, sizeof discovery_time);
		global_time = 0;
		shafts = 0; ways = 1;

		for (int i = 0; i < N; ++i) {
			if (articulation_point[i]) {
				solve(i, -1);
				break;
			}
		}

		if (shafts == 0) {
			shafts = 2;
			ways = ((long long)N * (N-1)) / 2;
		}

		printf("Case %d: %d %lld\n", ++tt, shafts, ways);
	}

	return 0;
}
