#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
  CS 341 - Written Assignment 3, Question 3(c) "Not Chess"
  --------------------------------------------------------
  This program reads an m x n board with fixed pawns ('P') and empty squares ('-').
  For every empty square, it determines whether the starting player is winning ('1')
  or losing ('2') under optimal play, where the queen may move:
    - left
    - down
    - diagonally down-left
  any positive number of unobstructed squares.

  Key idea (Theta(mn)):
  We scan the board from bottom-left to top-right so every legal destination
  of the current square has already been processed.

  For each current position (i, j), we only need to know whether there exists
  a reachable losing square in each of the three move directions before hitting
  a pawn:
    - current row segment to the left
    - current column segment below
    - current diagonal segment down-left

  We maintain three boolean summaries:
    - rowHasLose
    - colHasLose[j]
    - diagHasLose[d] where d = i - j (shifted to array index)

  If any is true => current cell is winning ('1')
  Otherwise      => current cell is losing  ('2')
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    vector<string> inputTopToBottom(m);
    for (int i = 0; i < m; i++) {
        cin >> inputTopToBottom[i];
    }

    // Convert input to bottom-up indexing:
    // board[1] is bottom row, board[m] is top row.
    // Also use 1-based indexing for columns by prepending a dummy character.
    vector<string> board(m + 1, string(n + 1, '?'));
    for (int i = 1; i <= m; i++) {
        string row = inputTopToBottom[m - i]; // reverse rows
        board[i] = " " + row;                 // 1-based column indexing
    }

    // Output board in the same internal format (bottom-up, 1-based columns).
    vector<string> ans(m + 1, string(n + 1, '?'));

    // colHasLose[j] = whether current visible segment in column j (below current row)
    // contains a losing square.
    vector<char> colHasLose(n + 1, 0);

    // Diagonal index d = i - j ranges from (1 - n) to (m - 1).
    // Shift by OFFSET so we can store in a vector.
    const int OFFSET = n;
    vector<char> diagHasLose(m + n + 5, 0);

    // Scan rows from bottom to top, columns from left to right.
    for (int i = 1; i <= m; i++) {
        // rowHasLose = whether current visible segment in this row (to the left)
        // contains a losing square.
        char rowHasLose = 0;

        for (int j = 1; j <= n; j++) {
            int dIndex = (i - j) + OFFSET;

            if (board[i][j] == 'P') {
                // Pawn squares remain 'P' in the output.
                ans[i][j] = 'P';

                // A pawn blocks future queen moves through this square, so it resets
                // the visible segment summaries in row / column / diagonal.
                rowHasLose = 0;
                colHasLose[j] = 0;
                diagHasLose[dIndex] = 0;
            } else {
                // Current square is winning iff there is at least one reachable losing square
                // in left / down / diagonal down-left directions.
                bool canReachLosing =
                    (rowHasLose != 0) || (colHasLose[j] != 0) || (diagHasLose[dIndex] != 0);

                if (canReachLosing) {
                    ans[i][j] = '1';  // First player can move to a losing position.
                    // Do NOT update *HasLose summaries, because this square is winning.
                } else {
                    ans[i][j] = '2';  // No move to a losing position => losing square.

                    // This losing square becomes visible to future squares in:
                    // - same row to the right
                    // - same column above
                    // - same diagonal up-right
                    rowHasLose = 1;
                    colHasLose[j] = 1;
                    diagHasLose[dIndex] = 1;
                }
            }
        }
    }

    // Print output in the same format as input: top row first.
    for (int i = m; i >= 1; i--) {
        for (int j = 1; j <= n; j++) {
            cout << ans[i][j];
        }
        cout << '\n';
    }

    return 0;
}
