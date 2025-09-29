#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <utility>

using namespace std;
using Mat = array<array<double, 3>, 3>;

#include <cmath>

const double PI = std::acos(-1.0);

const Mat I = {{
    {{1.0, 0.0, 0.0}},
    {{0.0, 1.0, 0.0}},
    {{0.0, 0.0, 1.0}}
}};

struct Op {
    string type;
    double a, b;
};

struct Query {
    char type;
    double x, y;
    int l, r;
    int idx;
    Op newOp;
};

class SegTree {
    int n;
    vector<Mat> arr;
    vector<Mat> tree;

    public:
        SegTree(const vector<Mat>& a) : arr(a), n(a.size()), tree(4 * n) {
            build(0, 0, n - 1);
        }

    void recalculate(int node) {
        tree[node] = mul(tree[2 * node + 2], tree[2 * node + 1]);
    }

    void build(int node, int left, int right) {
        if (left == right) {
            tree[node] = arr[left];
        } else {
            int middle = (left + right) / 2;
            build(2 * node + 1, left, middle);
            build(2 * node + 2, middle + 1, right);
            recalculate(node);
        }
    }

    void update(int x, Mat y) {
        _update(0, 0, n - 1, x, y);
    }

    void _update(int node, int left, int right, int x, Mat y) {
        if (left == right) {
            tree[node] = y;
        } else {
            int middle = (left + right) / 2;
            if (x <= middle) {
                _update(2 * node + 1, left, middle, x, y);
            } else {
                _update(2 * node + 2, middle + 1, right, x, y);
            }
            recalculate(node);
        }
    }

    Mat query(int x, int y) {
        return _query(0, 0, n - 1, x, y);
    }

    Mat _query(int node, int left, int right, int x, int y) {
        if (x <= left && right <= y) {
            return tree[node];
        } else {
            // identity matrix
            Mat res = I;
            int middle = (left + right) / 2;
            if (x <= middle) {
                res = mul(_query(2 * node + 1, left, middle, x, y), res);
            } 
            
            if (y >= middle + 1) {
                res = mul(_query(2 * node + 2, middle + 1, right, x, y), res);
            }
            return res;
        }
    }

    Mat mul(const Mat &A, const Mat &B) {
        Mat C{};
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++){
                C[i][j]=0;
                for(int k=0;k<3;k++)
                    C[i][j]+=A[i][k]*B[k][j];
            }
        return C;
    }

};

    
array<double,3> apply(const Mat &M, const array<double,3> &v) {
    array<double,3> r{};
    for(int i=0;i<3;i++){
        r[i]=0;
        for(int j=0;j<3;j++)
            r[i]+=M[i][j]*v[j];
    }
    return r;
}



Mat toMat(const Op& op) {
    Mat M = I;
    if (op.type == "Translate") {
        M[0][2] = op.a;
        M[1][2] = op.b;
    } else if (op.type == "Scale") {
        M[0][0] = op.a;
        M[1][1] = op.b;
    } else { // Rotate
        double rad = op.a * PI / 180.0;
        double c = cos(rad), s = sin(rad);
        M[0][0] =  c; M[0][1] = -s;
        M[1][0] =  s; M[1][1] =  c;
    }
    return M;
}
inline double clean(double v, double eps = 1e-9) {
    return std::fabs(v) < eps ? 0.0 : v;
}

vector<pair<double,double>> solve(int n, int q, vector<Op>& trans, vector<Query>& queries) {

    vector<Mat> mats;
    mats.reserve(trans.size());
    for (const Op& op: trans) {
        mats.push_back(toMat(op));
    }

    SegTree st(mats);
    
    for (const Query& query : queries) {
        if (query.type == 'U') {
            st.update(query.idx, toMat(query.newOp));
        } else {
            auto mat = st.query(query.l, query.r);
            auto out = apply(mat, {query.x, query.y, 1.0});
            cout << '(' << int(query.x) << ',' << int(query.y) << "): "
            << fixed << setprecision(6) << out[0] << ' ' << out[1] << '\n';
           
        }
    }

    return {};
}


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Op> trans(n);
    for(int i = 0; i < n; i++){
        cin >> trans[i].type;
        if(trans[i].type == "Translate"){
            cin >> trans[i].a >> trans[i].b;
        } else if(trans[i].type == "Scale"){
            cin >> trans[i].a >> trans[i].b;
        } else {
            cin >> trans[i].a;
            trans[i].b = 0;
        }
    }

    vector<Query> queries(q);
    for(int i = 0; i < q; i++){
        cin >> queries[i].type;
        if(queries[i].type == 'Q'){
            cin >> queries[i].x >> queries[i].y >> queries[i].l >> queries[i].r;
        } else {
            cin >> queries[i].idx >> queries[i].newOp.type;
            if(queries[i].newOp.type == "Translate"){
                cin >> queries[i].newOp.a >> queries[i].newOp.b;
            } else if(queries[i].newOp.type == "Scale"){
                cin >> queries[i].newOp.a >> queries[i].newOp.b;
            } else {
                cin >> queries[i].newOp.a;
                queries[i].newOp.b = 0;
            }
        }
    }

    auto res = solve(n, q, trans, queries);
    for(auto &p : res){
        cout << fixed << setprecision(9) << p.first << " " << p.second << "\n";
    }
    return 0;
}
    