// NOT TESTED YET
#include <cstdio>
#define MAXN 1000000
#define NOT_CONNECTED 0
#define CONNECTED 1
#define ALREADY_CONNECTED 2

int dad[MAXN], rank[MAXN];
// int kids[MAXN]; // if we want to find largest componenet

int union_find(int a, int b, bool connect = true) {
    int topa, topb;
    int newtop;

    for (topa = a; topa != dad[topa]; topa = dad[topa]);
    for (topb = b; topb != dad[topb]; topb = dad[topb]);
    dad[a] = topa; dad[b] = topb;

    if (topa != topb && connect) {
        if (rank[topa] > rank[topb]) {
            // kids[topa] += kids[topb];
            dad[topb] = newtop = topa;
        } else {
            // kids[topb] += kids[topa];
            dad[topa] = newtop = topb;
            if (rank[topa] == rank[topb]) rank[topb]++;
        }

        int x;
        for ( ; a != topa; ) x = dad[a], dad[a] = newtop, a = x;
        for ( ; b != topb; ) x = dad[b], dad[b] = newtop, b = x;

        return CONNECTED;
    } else {
        int x;
        for ( ; a != topa; ) x = dad[a], dad[a] = topa, a = x;
        for ( ; b != topb; ) x = dad[b], dad[b] = topb, b = x;

        return connect || topa == topb ? ALREADY_CONNECTED : NOT_CONNECTED;
    }
}

void union_find_init(int n) {
    for (int i = 0; i < n; ++i) {
        dad[i] = i;
        rank[i] = 0;
        // kids[i] = 1;
    }
}

int main() {
    return 0;
}
