/***************************
 *     Matching
 * *************************/
#include <cstdio>
#include <vector>

using namespace std;


vector <vector <int> > E;
vector <int> connectedF, bio;

int dfs(int s) {
	if (bio[s]) return 0;
	bio[s] = 1;

	for (int i = 0; i < E[s].size(); ++i) {
		if (connectedF[E[s][i]] == s) continue;
		if (connectedF[E[s][i]] == -1 || dfs(connectedF[E[s][i]])) {
			connectedF[E[s][i]] = 1;
			return 1;
		}
	}

	return 0;
}

int matching() {
	int sol = 0;
	bio.resize(hor.size());
	connectedF.resize(vert.size());
	fill(connectedF.begin(), connectedF.end(), -1);

	for (int i = 0; i < hor.size(); ++i) {
		fill(bio.begin(), bio.end(), 0);
		sol += dfs(i);
	}

	return sol;
}

