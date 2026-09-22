#include <iostream>
#include <vector>
#include <algorithm>

struct Point {
    int x;
    int y;
};

std::vector<Point> points;
bool firstOutput = true;

// Print a point, taking care of spaces between numbers.
void outputPoint(const Point &p) {
    if (!firstOutput) {
        std::cout << ' ';
    }
    std::cout << p.x << ' ' << p.y;
    firstOutput = false;
}

// Build the kd-tree conceptually on points[left, right),
// printing the point for each node in *pre-order*.
// axis = 0 → split by x, axis = 1 → split by y.
void buildKd(int left, int right, int axis) {
    int n = right - left;
    if (n <= 0) return;

    // Leaf: region has exactly one point – store/print that point.
    if (n == 1) {
        outputPoint(points[left]);
        return;
    }

    // Copy current segment and sort it by the active coordinate
    // (x for axis = 0, y for axis = 1). We use the other
    // coordinate as a tiebreaker just to make the order deterministic
    // (though “generic position” means ties don’t actually happen).
    std::vector<Point> tmp(points.begin() + left, points.begin() + right);
    std::sort(tmp.begin(), tmp.end(),
              [axis](const Point &a, const Point &b) {
                  int aCoord = (axis == 0 ? a.x : a.y);
                  int bCoord = (axis == 0 ? b.x : b.y);
                  if (aCoord != bCoord) return aCoord < bCoord;

                  int aOther = (axis == 0 ? a.y : a.x);
                  int bOther = (axis == 0 ? b.y : b.x);
                  return aOther < bOther;
              });

    // Overwrite this segment with the sorted version.
    std::copy(tmp.begin(), tmp.end(), points.begin() + left);

    // The median point (by the current axis) defines the splitting line
    // and is the point stored at this kd-tree node.
    int mid = left + n / 2;
    outputPoint(points[mid]);

    // Recursively process left (coordinate < median) and
    // right (coordinate ≥ median) subsets, switching axis.
    buildKd(left, mid, 1 - axis);
    buildKd(mid, right, 1 - axis);
}

int main() {
    int n;
    if (!(std::cin >> n)) {
        return 0; // no input
    }

    points.resize(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> points[i].x >> points[i].y;
    }

    // Start with an x-split (as in the Module 8 recipe).
    buildKd(0, n, 0);

    std::cout << '\n';
    return 0;
}
