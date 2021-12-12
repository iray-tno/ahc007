#include <iostream>
#include <vector>
#include <array>
#include <set>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

constexpr int N=400;
constexpr int M=1995;

constexpr int MAX_LOOP = 28;

typedef double Dist;
struct Point {
    int x; int y;

    // int diff(const Point& rhs) { return abs(x - rhs.x) + abs(y - rhs.y); }
    Dist diff(const Point& rhs) { return pow(abs(x - rhs.x), 2) + pow(abs(y - rhs.y), 2); }
};
struct Edge {
    int src; int dest;
};

struct UnionFind {
    vector<int> par;
    int n;

    UnionFind(const UnionFind& src) {
        n = src.n;
        par = src.par;
    }

    UnionFind(int N) : par(N) {
        for(int i = 0; i < N; i++) par[i] = i;
        n = N;
    }

    int root(int x) {
        if (par[x] == x) return x;
        return par[x] = root(par[x]);
    }

    void unite(int x, int y) {
        int rx = root(x);
        int ry = root(y);
        if (rx == ry) return;
        par[rx] = ry;
    }

    bool same(int x, int y) {
        int rx = root(x);
        int ry = root(y);
        return rx == ry;
    }

    UnionFind clone() {
        return UnionFind(*this);
    }
};


unsigned long xor128() {
  static unsigned long x=123456789, y=362436069, z=521288629, w=88675123;
  unsigned long t=(x^(x<<11));
  x=y; y=z; z=w;
  return ( w=(w^(w>>19))^(t^(t>>8)) );
}

double randCoeff() {
    double to3000 = 10000 + xor128() % (20000 + 1);
    // double to3000 = 11300 + xor128() % (17500 + 1);
    return to3000 * to3000 / 100000000;
}

struct EdgeWithDist {
    int id;
    int src; int dest; Dist dist; Dist minDist;
    bool operator<( const EdgeWithDist& right ) const {
        return dist < right.dist;
    }
};

inline bool use(int counts, int loop) {
    return counts * 2 >= loop;
}

int main() {
    vector<Point> points(N);
    for (int i = 0; i < N; ++i) cin >> points[i].x >> points[i].y;
    vector<Edge> edges(M);
    for (int i = 0; i < M; ++i) cin >> edges[i].src >> edges[i].dest;

    vector<EdgeWithDist> edgesWithDist;
    edgesWithDist.reserve(M);
    for (int i = 0; i < M; ++i) {
        auto edge = edges[i];
        auto dest = points[edge.dest];
        auto src = points[edge.src];

        EdgeWithDist res;
        res.id = i;
        res.dest = edge.dest; res.src = edge.src;
        res.minDist = dest.diff(src);
        edgesWithDist.push_back(res);
    }

    UnionFind uf(N);
    for (int edgeId = 0; edgeId < M; ++edgeId) {
        long long realDistance;
        cin >> realDistance;
        realDistance *= realDistance;

        if (uf.same(edgesWithDist[edgeId].src, edgesWithDist[edgeId].dest)) {
            cout << "0" << endl;
            continue;
        }

        int counts = 0;

        vector<EdgeWithDist> edgesTemp;
        edgesTemp.reserve(M - edgeId);
        EdgeWithDist edge = edgesWithDist[edgeId];
        edge.dist = realDistance;
        edgesTemp.push_back(edge);
        for (int j = edgeId + 1; j < M; ++j) {
            edgesTemp.push_back(edgesWithDist[j]);
        }

        for (int loop = 0; loop < MAX_LOOP; ++loop) {
            UnionFind ufTemp = uf.clone();

            for (int j = 0; j < edgesTemp.size(); ++j) {
                if (edgesTemp[j].id == edgeId) continue;
                edgesTemp[j].dist = edgesTemp[j].minDist * randCoeff();
            }
            sort(edgesTemp.begin(), edgesTemp.end());

            for (int k = 0; k < edgesTemp.size(); ++k) {
                auto edge = edgesTemp[k];

                if (!ufTemp.same(edge.src, edge.dest)) {
                    if (edge.id == edgeId) {
                        counts += 1;

                        if (use(counts, MAX_LOOP)) {
                            // cerr << "edgeId " << edgeId << " " << counts << " " << MAX_LOOP << endl;
                            cout << "1" << endl;
                            uf.unite(edgesWithDist[edgeId].src, edgesWithDist[edgeId].dest);
                            goto CONTINUE_SELECTED;
                        }
                        goto CONTINUE_LOOP;
                    }
                    ufTemp.unite(edge.src, edge.dest);
                }
            }

            CONTINUE_LOOP:
            if (!use(counts + (MAX_LOOP - loop - 1), MAX_LOOP)) { // 最大でも採用されないことが確定したので終わる
                cout << "0" << endl;
                goto CONTINUE_SELECTED;
            }
            continue;
        }


        if (use(counts, MAX_LOOP)) {
            cout << "1" << endl;
            uf.unite(edgesWithDist[edgeId].src, edgesWithDist[edgeId].dest);
        } else {
            cout << "0" << endl;
        }

        CONTINUE_SELECTED:
            continue;
    }
    return 0;
}
