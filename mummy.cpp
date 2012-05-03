#include <cstdio>
#include <algorithm>
#include <vector>
#define INF 0x3f3f3f3f
#define MAX_R 2000000

using namespace std;

class TournamentTree {
private:
    vector <int> minimum, addToEveryChild;
    int size;

    // [, >
    int _getMinimum(int p, int from, int to, int left, int right) {
        if (to <= left || from >= right) return INF;
        if (from <= left && to >= right) return minimum[p] + addToEveryChild[p];
        int lmin = _getMinimum(2*p, from, to, left, (right+left)/2);
        int rmin = _getMinimum(2*p+1, from, to, (right+left)/2, right);
        return min(lmin, rmin) + addToEveryChild[p];
    }

    // returns minimum in [left, right>
    int _addToRange(int p, int from, int to, int x, int left, int right) {
        if (to <= left || from >= right) return minimum[p] + addToEveryChild[p];
        if (from <= left && to >= right) {
            addToEveryChild[p] += x;
        } else {
            int lmin = _addToRange(2*p, from, to, x, left, (right+left)/2);
            int rmin = _addToRange(2*p+1, from, to, x, (right+left)/2, right);
            minimum[p] = min(lmin, rmin);
        }
        return minimum[p] + addToEveryChild[p];
    }

public:
    // [, ]
    int getMinimum(int from, int to) {
        return _getMinimum(1, from, to + 1, 0, size);
    }

    // [, ]
    void addToRange(int from, int to, int x) {
        _addToRange(1, from, to + 1, x, 0, size);
    }

    void init(int n) {
        size = 1;
        for ( ; size < n; size *= 2);
        minimum.resize(2*size + 1);
        addToEveryChild.resize(2*size + 1);
        fill(minimum.begin(), minimum.end(), 0);
        fill(addToEveryChild.begin(), addToEveryChild.end(), 0);
    }

    TournamentTree() {}
};

vector <pair <int, int> > mummies;

bool load() {
    int N;
    scanf("%d", &N);
    if (N == -1) return false;
    mummies.resize(N);
    for (int i = 0; i < N; ++i) {
        scanf("%d%d", &mummies[i].first, &mummies[i].second);
    }
    return true;
}

class Event {
public:
    int x;
    int type;
    int yTop, yBottom;

    Event() {}
    Event(int _x, int _type, int _yTop, int _yBottom) :
        x(_x), type(_type), yTop(_yTop), yBottom(_yBottom) {}
};

bool operator < (const Event &a, const Event &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.type < b.type;
}

TournamentTree tournament;
vector <Event> events;

void output_tt(int s) {
    for (int i = 0; i <= 2*s; ++i) {
        printf("%d ", tournament.getMinimum(i, i));
    }
    printf("\n");
}

bool can_catch(int s) {
    events.clear();
    
    for (int i = 0; i < mummies.size(); ++i) {
        //printf("from (%d %d) I'm creating (%d -> %d), (%d, %d)\n", mummies[i].first, mummies[i].second, mummies[i].first - s, mummies[i].first + s + 1, mummies[i].second + s, mummies[i].second -s );
        events.push_back(Event(mummies[i].first - s, 1, mummies[i].second + s, mummies[i].second - s));
        events.push_back(Event(mummies[i].first + s + 1, -1, mummies[i].second + s, mummies[i].second - s));
    }
    events.push_back(Event(-s, 2, 0, 0));
    events.push_back(Event(-s + 1, 2, 0, 0));
    events.push_back(Event(s, 2, 0, 0));
    events.push_back(Event(s - 1, 2, 0, 0));

    sort(events.begin(), events.end());

    tournament.init(2*s + 10);

    for (int i = 0; i < events.size(); ++i) {
        //printf("processing event at x %d, y (%d, %d) type %d\n", events[i].x, events[i].yBottom, events[i].yTop, events[i].type);
        //output_tt(s);
        if (i && events[i - 1].x >= -s) {
            if (i && events[i].x != events[i-1].x) {
                if (tournament.getMinimum(0, 2*s) == 0) {
                    //output_tt(s);
                    return false;
                }
            }
        }
        if (events[i].x > s) break;

        if (events[i].type != 2) {
            int from = min(max(events[i].yBottom + s, 0), 2*s + 10);
            int to = min(max(events[i].yTop + s, 0), 2*s + 10);
            tournament.addToRange(from, to, events[i].type);
        }

    }

    return true;
}

int solve() {
    // [l, r>
    int l = 0, r = MAX_R;

    while (r - l > 1) {
        int p = (l+r)/2;

        if (can_catch(p)) {
            r = p;
        } else {
            l = p;
        }
    }

    if (r == MAX_R) return -1;

    return l;
}

int main() {
    int tt = 0;
    while (load()) {
        int sol = solve();
        if (sol == -1) printf("Case %d: never\n", ++tt);
        else printf("Case %d: %d\n", ++tt, sol + 1);
    }

    return 0;
}
