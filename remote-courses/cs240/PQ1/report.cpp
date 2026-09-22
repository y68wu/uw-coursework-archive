#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> A(n);
    for (int i = 0; i < n; ++i) {
        cin >> A[i];
    }

    int c;
    cin >> c;

    if (n == 0) {
        return 0;
    }

    // DFS with an index stack; prune subtrees where A[i] < c.
    vector<int> st;
    st.push_back(0);

    while (!st.empty()) {
        int i = st.back();
        st.pop_back();

        //subtree can't contain values which is greater or equal to c
        if (A[i] < c) continue;      

        // report the output to c out
        cout << A[i] << '\n';        

        // push children if they are within bounds
        long long L = 2LL * i + 1;
        long long R = 2LL * i + 2;

        if (L < n) {
            st.push_back(static_cast<int>(L));
        }

        if (R < n) {
            st.push_back(static_cast<int>(R));
        }
    }

    return 0;
}
