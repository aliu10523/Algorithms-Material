#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <utility>

using namespace std;
using Mat = array<array<double, 3>, 3>;

#include <cmath>

// const double PI = std::acos(-1.0);
const double PI = M_PI;

const Mat I = {{
    {{1.0, 0.0, 0.0}},
    {{0.0, 1.0, 0.0}},
    {{0.0, 0.0, 1.0}}
}};

void printMatrix(Mat& m) {
   cout << " PRINT MATRIX: " << endl;
   for (int i=0;i<3;i++) {
	 cout << "          " << m[i][0] << ",  " << m[i][1] << ",  " << m[i][2] << endl;
   }
}

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

array<double,3> apply(const Mat &M, const array<double,3> &v) {
    array<double,3> r{};
    for(int i=0;i<3;i++){
        r[i]=0.0;
	
        for(int j=0;j<3;j++) {
            double result =M[i][j]*v[j];
            r[i]=result + r[i];
            // r[i]+=M[i][j]*v[j];
	}
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
        double rad = op.a * PI / 180.0L;
        double c = cos(rad), s = sin(rad);
        M[0][0] =  c; M[0][1] = 0.0-s;
        M[1][0] =  s; M[1][1] =  c;
    }
    return M;
}

Mat mul(const Mat &A, const Mat &B) {
        Mat C{};
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++){
                C[i][j]=0.0L;
                for(int k=0;k<3;k++) {
                    double result =A[i][k]*B[k][j];
                    C[i][j]+=result;
                    // C[i][j]+=A[i][k]*B[k][j];
		}
            }
        return C;
}

vector<pair<double,double>> solve(int n, int q, vector<Op>& trans, vector<Query>& queries) {

    vector<Mat> mats;
    mats.reserve(trans.size());
    for (const Op& op: trans) {
        mats.push_back(toMat(op));
    }
    
    for (const Query& query : queries) {
        if (query.type == 'U') {
	    // cout << "UPDATE" << endl;
	    Mat m = toMat(query.newOp);
	    for (int i=query.l; i<=query.r; i++) {
		mats[i]= m;
	    }
        } else {
	    // cout << "QUERY" << endl;

	    auto mat = mats[query.l];
	    // printMatrix(mat);
	    for (int i=query.l+1; i<=query.r; i++) {
	       mat = mul(mats[i],mat);
	    }

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
            // cin >> queries[i].idx >> queries[i].newOp.type;
            cin >> queries[i].l >> queries[i].r >> queries[i].newOp.type;
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
    
