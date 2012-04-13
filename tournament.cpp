/***************************
 *     Tournament tree 
 * *************************/
// NOT YET TESTED
// igor's new code
// supports:
// * find minimum in a range
// * change an element
#include <cstdio>
#include <algorithm>
#define MAXN 1000000
#define INF 0x3f3f3f3f

using namespace std;

int tournament[2*MAXN + 1];
int tt_size;
int A[MAXN];

void tt_create(int n) {
    for (tt_size = 1; tt_size < n; tt_size *= 2);

    for (int i = tt_size; i < tt_size*2; ++i) {
        if (i-tt_size < n) tournament[i] = A[i-tt_size];
        else tournament[i] = INF;
    }
    for (int i = tt_size - 1; i >= 1; --i) {
        tournament[i] = min(tournament[2*i], tournament[2*i+1]);
    }
}

int tt_change(int index, int new_value) {
    tournament[tt_size + index] = new_value;

    for (int i = tt_size + index; i >= 1; i /= 2) {
        tournament[i] = min(tournament[2*i], tournament[2*i+1]);
    }
}

// [from, to> [lo, hi>
int _tt_query(int from, int to, int p, int lo, int hi) {
    if (to <= lo || from >= hi) return INF;
    if (from <= lo && to >= hi) return tournament[p];

    return min(_tt_query(from, to, 2*p, lo, (lo+hi)/2), _tt_query(from, to, 2*p + 1, (lo+hi)/2, hi));
}

int tt_query(int from, int to) {
    return _tt_query(from, to, 1, 0, tt_size);
}
