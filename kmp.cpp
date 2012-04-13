/***************************
 *     KMP
 * *************************/
// c/p from Zagreb
#define MAXP 1000
#define MAXT 1000

int pi[MAXP+1];
char T[MAXT+1]; int n;
char P[MAXP+1]; int m;

void compute_prefix_function() {
    pi[1] = 0;
    int k = 0;
    for (int q = 2; q <= m; ++q) {
        while (k > 0 && P[k] != P[q-1]) k = pi[k];
        if (P[k] == P[q-1]) ++k;
        pi[q] = k;
    }
}

void KMP_matcher() {
    int q = 0;

    for (int i = 1; i <= n; ++i) {
        while (q > 0 && P[q] != T[i-1]) q = pi[q];
        if (P[q] == T[i-1]) q++;
        if (q == m) { 
            // we found pattern with shift i-m
            q = pi[q];
        }
    }
}


int main() {
    return 0;
}
