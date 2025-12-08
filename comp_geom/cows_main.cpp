#include <iostream>
#include <vector>
#include <stack>

using namespace std;

struct Point{
    int x;
    int y;
    bool del;
};

struct QueryUpdate{
    char type; // Q or U
    int index; // only valid for Update
    Point pos1;
    Point pos2;
};



int cross(const Point& a, const Point& b, const Point& c, const Point&d) {
    // cross product (b - a) x (d - c)
    return (b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x);
}

// Find the anchor: lowest y (then lowest x)
Point findAnchor(const vector<Point>& pts) {
    return *min_element(pts.begin(), pts.end(),
                        [](const Point& a, const Point& b) {
                            return (a.y < b.y) || (a.y == b.y && a.x < b.x);
                        });
}

void graham_scan(vector<Point>& Q, vector<Point>& S) {
    vector<Point> workQ = Q;
    Point p0 = findAnchor(workQ);

    // sort points
    sort(workQ.begin(), workQ.end(), [&](Point &a, Point &b){
        if (a.x == p0.x && a.y == p0.y) return true;
        if (b.x == p0.x && b.y == p0.y) return false;

        int cr = cross(p0, a, p0, b);
        if (cr == 0) {
            // collinear — nearer first
            int da = (a.x - p0.x)*(a.x - p0.x) + (a.y - p0.y)*(a.y - p0.y);
            int db = (b.x - p0.x)*(b.x - p0.x) + (b.y - p0.y)*(b.y - p0.y);
            if (da < db) {
                a.del = true;
            } else {
                b.del = true;
            }
            return da < db;
        }
        return cr > 0; // CCW
    });

    // remove colinear ones except the farthest one
    workQ.erase(std::remove_if(workQ.begin(), workQ.end(), 
                                 [](Point& a) { return a.del == true; }), 
                  workQ.end());

/*
    cout << "workQ print" << endl;
    for (int i=0;i<workQ.size();i++) {
        cout << workQ[i].x << ' ' << workQ[i].y << endl;
    }
*/
    S.push_back(workQ[0]);
    S.push_back(workQ[1]);
    S.push_back(workQ[2]);

    for (int i=3;i<workQ.size();i++) {
        
        while (cross(S[S.size()-2],S[S.size()-1], S[S.size()-1], workQ[i]) <=0 ) {
            S.pop_back();
        }
        S.push_back(workQ[i]);
    }

}

// Method 2: Turns (slide 30)
bool insideConvexHull(const Point& p, const vector<Point>& cH) {
    for (int i=0;i<cH.size();i++) {
        int next = (i+1) % cH.size();
        int cr = cross(cH[i], cH[next], cH[i], p);
        if (cr < 0) return false;
        if (cr > 0) continue;
        if (cr == 0) {
            if (p.x < min(cH[i].x, cH[next].x) || p.x > max(cH[i].x, cH[next].x)) return false;
            if (p.y < min(cH[i].y, cH[next].y) || p.y > max(cH[i].y, cH[next].y)) return false;
            return true;
        }
    }

    return true;
}

int main()
{
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);

    int n;
    int totalCows, totalPosts;


    cin >> n >> totalCows >> totalPosts;
    vector<Point> cows, posts;
    int x, y;
    for (int i=0;i<totalCows;i++) {
        cin >> x >> y;
        cows.push_back({x,y});
    }

    for (int i=0;i<totalPosts;i++) {
        cin >> x >> y;
        posts.push_back({x,y});
    }

    int totalQueryUpdate;
    cin >> totalQueryUpdate;
    vector<QueryUpdate> actions;
    
    char actionType;

    for (int i=0;i<totalQueryUpdate;i++) {
        cin >> actionType;
        if (actionType == 'Q') {
            int x2,y2;
            cin >> x >> y >> x2 >> y2;
            actions.push_back({actionType, -1, {x,y}, {x2,y2}});
        } else {
            int index;
            cin >> index >> x >> y;
            actions.push_back({actionType, index, {x,y},{-1,-1}});
        }
    }  


    vector<Point> workV;

    vector<Point> convexHull;
    for (int k=0;k<actions.size();k++) {
       if (actions[k].type == 'Q') {
           for (int i = 0; i < totalPosts; i++)
           {
                if (posts[i].x >= actions[k].pos1.x  && posts[i].x <= actions[k].pos2.x 
                   && posts[i].y <= actions[k].pos1.y &&  posts[i].y >= actions[k].pos2.y){
                   workV.push_back(posts[i]);
                }
           }
           
           convexHull.clear();
           graham_scan(workV, convexHull);
/*
           cout << "Convex Hull " << endl;
           for (int i = convexHull.size() - 1; i >= 0; i--)
           {
               std::cout << "(" << convexHull[i].x << "," << convexHull[i].y << ")" << endl; // Print the front element
           }
*/
           int totalInside = 0;
           for (int i=0; i< totalCows;i++) {
               if (insideConvexHull(cows[i], convexHull)) totalInside++;
           }
           // cout << "for k = " << k << ": " << totalInside << ' '  << totalCows - totalInside << endl;
           cout << totalInside << ' '  << totalCows - totalInside << endl;
           workV.clear();
           continue;
        } 
/*
        cout << "Before modify" << endl;
        for (int i=0;i<totalCows;i++) {
            cout << cows[i].x << ' ' << cows[i].y << endl;
        }
*/
        cows[actions[k].index].x = actions[k].pos1.x;
        cows[actions[k].index].y = actions[k].pos1.y;
/*
        cout << "After modify" << endl;
        for (int i=0;i<totalCows;i++) {
            cout << cows[i].x << ' ' << cows[i].y << endl;
        }
*/
    }

    // return 0;
}