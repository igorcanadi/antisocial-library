#include <cstdio>
#include <cstring>
#include <vector>
#define MAX 100000

using namespace std;

vector <vector <int> > E;
int on_stack[MAX], visited[MAX], component[MAX];
int num_components;
int global_time;
vector <int> node_stack;

void load() {
    int n, m;
    scanf("%d%d", &n, &m);
    E.clear();
    E.resize(n);

    for (int i = 0; i < m; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        E[a-1].push_back(b-1);
    }
}

int dfs(int s) {
    int lowlink = visited[s] = global_time++;
    node_stack.push_back(s);
    on_stack[s] = 1;

    for (int i = 0; i < E[s].size(); ++i) {
        if (!visited[E[s][i]]) {
            lowlink = min(lowlink, dfs(E[s][i]));
        } else if (on_stack[E[s][i]]) {
            lowlink = min(lowlink, visited[E[s][i]]);
        }
    }

    if (lowlink == visited[s]) {
        // s defines new component consisting of nodes on stack
        ++num_components;
        while (true) {
            int t = node_stack.back();
            component[node_stack.back()] = num_components;
            on_stack[node_stack.back()] = 0;
            node_stack.pop_back();
            if (t == s) break;
        }
    }

    return lowlink;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        load();
        memset(visited, 0, sizeof visited);
        memset(on_stack, 0, sizeof on_stack);
        memset(component, 0, sizeof component);
        global_time = 1;
        num_components = 0;
        for (int i = 0; i < E.size(); ++i) {
            if (!visited[i]) {
                dfs(i);
            }
        }

        // solution is number of componenets with in-degree == 0
        vector <int> knock_down(num_components, 1);
        int sol = num_components;

        for (int i = 0; i < E.size(); ++i) {
            for (int j = 0; j < E[i].size(); ++j) {
                if (component[i] != component[E[i][j]] && knock_down[component[E[i][j]]]) {
                    knock_down[component[E[i][j]]] = 0;
                    --sol;
                }
            }
        }

        printf("%d\n", sol);
    }
    return 0;
}
