// tested on 11082 ACM problem
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <queue>
#define INF 0x3f3f3f

using namespace std;

int cap[42][42];
int how[42], ff[42];

int bfs(int source, int sink) {
	memset(how, -1, sizeof how);
	memset(ff, 0, sizeof ff);
	how[source] = source;
	ff[source] = INF;
	
	queue <int> Q;
	Q.push(source);

	while (Q.size()) {
		int s = Q.front(); Q.pop();

		if (s == sink) break;

		for (int i = 0; i < 42; ++i) {
			if (cap[s][i] != 0 && how[i] == -1) {
				ff[i] = min(ff[s], cap[s][i]);
				how[i] = s;
				Q.push(i);
			}
		}
	}

	return ff[sink];
}

void flow(int source, int sink) {
	int maxflow = 0;
	for (int f = 0; f = bfs(source, sink); maxflow += f) {
		for (int s = sink; s != source; s = how[s]) {
			cap[how[s]][s] -= f;
			cap[s][how[s]] += f;
		}
	}
}

int main() {
	int T;
	scanf("%d", &T);

	for (int tt = 1; tt <= T; ++tt) {
		memset(cap, 0, sizeof cap);
		int R, C;
		scanf("%d%d", &R, &C);
		int before = 0;
		for (int i = 0; i < R; ++i) {
			int a; scanf("%d", &a);
			cap[0][i+1] = a - C - before;
			before = a;
		}
		before = 0;
		for (int i = 0; i < C; ++i) {
			int a; scanf("%d", &a);
			cap[R+i+1][R+C+1] = a - R - before;
			before = a;
		}
		for (int i = 0; i < R; ++i) {
			for (int j = 0; j < C; ++j) {
				cap[i+1][j+R+1] = 19;
			}
		}

		flow(0, R+C+1);

		printf("Matrix %d\n", tt);
		for (int i = 0; i < R; ++i) {
			for (int j = 0; j < C; ++j) {
				printf("%d ", cap[j+R+1][i+1] + 1);
			}
			printf("\n");
		}
		printf("\n");

	}
	return 0;
}
