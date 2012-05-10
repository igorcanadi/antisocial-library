#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <queue>
#define MAXN 1000000

using namespace std;

class AhoCorasick {
public:
	int n;
	int nodes[MAXN][26];
	int fail[MAXN];
	int done[MAXN];

	void init(const vector <string>& s) {
		n = 1;
		memset(nodes, -1, sizeof nodes);
		memset(done, -1, sizeof done);
		memset(fail, -1, sizeof fail);

		for (int i = 0; i < s.size(); ++i) {
			int t = 0;
			for (int j = 0; j < s[i].size(); ++j) {
				if (nodes[t][s[i][j]-'a'] == -1) {
					nodes[t][s[i][j]-'a'] = n++;
				}
				t = nodes[t][s[i][j]-'a'];
			}

			done[t] = i;
		}

		do_bfs();
	}

	void output() {
		for (int i = 0; i < n; ++i) {
			printf("%d (fail %d, done %d): ", i, fail[i], done[i]);
			for (int j = 0; j < 26; ++j) {
				if (nodes[i][j]) {
					printf("(%c, %d) ", 'a'+j, nodes[i][j]);
				}
			}
			printf("\n");
		}
	}

private:
	void do_bfs() {
		queue <int> Q;

		for (int i = 0; i < 26; ++i) {
			if (nodes[0][i] != -1) {
				fail[nodes[0][i]] = 0;
				Q.push(nodes[0][i]);
			} else {
				nodes[0][i] = 0;
			}
		}

		while (Q.size()) {
			int s = Q.front(); Q.pop();
			done[s] = max(done[s], done[fail[s]]);

			for (int i = 0; i < 26; ++i) {
				if (nodes[s][i] == -1) {
					nodes[s][i] = nodes[fail[s]][i];
				} else {
					fail[nodes[s][i]] = nodes[fail[s]][i];
					Q.push(nodes[s][i]);
				}
			}
		}
	}
};

AhoCorasick AC;
vector <string> S;
int N;

bool cmp(const string& a, const string& b) {
	return a.size() < b.size();
}

bool load() {
	char buffer[1001];
	scanf("%d", &N);

	S.resize(N);
	for (int i = 0; i < N; ++i) {
		scanf("%s", buffer);
		S[i] = string(buffer);
	}

	sort(S.begin(), S.end(), cmp);

	return N;
}

int dfs(int s, vector <vector <int> >& E, vector <int> &seen) {
	if (seen[s] != -1) return seen[s];

	int sol = 0;

	for (int i = 0; i < E[s].size(); ++i) {
		if (E[s][i] != s) {
			sol = max(sol, dfs(E[s][i], E, seen));
		}
	}

	return seen[s] = sol + 1;
}

int solve() {
	AC.init(S);

	vector <vector <int> > substrings(S.size());
	vector <int> seen(S.size(), -1);

	for (int i = 0; i < S.size(); ++i) {
		int t = 0;
		for (int j = 0; j < S[i].size(); ++j) {
			t = AC.nodes[t][S[i][j]-'a'];
			if (AC.done[t] != -1) {
				substrings[i].push_back(AC.done[t]);
			}
			if (AC.done[AC.fail[t]] != -1) {
				substrings[i].push_back(AC.done[AC.fail[t]]);
			}
		}
	}
	return 1;

	int sol = 0;
	for (int i = 0; i < substrings.size(); ++i) {
		sol = max(sol, dfs(i, substrings, seen));
	}

	return sol;
}

int main() {
	while (load()) {
		printf("%d\n", solve());
	}

	return 0;
}

