#include <iostream>
#include <vector>
//#include <stack>
#include <cmath>
#include <algorithm>
#include <utility> // Required for std::pair
// #include <Set>

using namespace std;

struct Point{
    int x;
    int y;
    bool del;

    Point operator-(const Point &p) {
        return {x - p.x, y - p.y};
    }
};

#define Point_Pair pair<Point, Point>

int cross(const Point& a, const Point& b, const Point& c, const Point&d) {
    // cross product (b - a) x (d - c)
    return (b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x);
}

int sign(int num) {
    if (num < 0) return -1;
    else if (num == 0) return 0;
    else return 1;
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

void calculate_max_distance_square(int& max_dist_square, const Point& p1, const Point& p2, vector<Point_Pair>& max_pairs) {
    auto dist_square = pow(p1.x - p2.x,2) + pow(p1.y - p2.y, 2);
    if (dist_square > max_dist_square) {
            max_dist_square = dist_square;                
            max_pairs.clear();
            Point_Pair new_pair;
            new_pair.first = p1;
            new_pair.second = p2;
            max_pairs.push_back(new_pair);
    }
    if (dist_square == max_dist_square) {       
            bool exist_flag = false;
            for (auto pp: max_pairs) {
                if (pp.first.x == p1.x && pp.first.y == p1.y && pp.second.x == p2.x && pp.second.y == p2.y) {
                    exist_flag = true;
                    break;
                }
                if (pp.first.x == p2.x && pp.first.y == p2.y && pp.second.x == p1.x && pp.second.y == p1.y) {
                    exist_flag = true;
                    break;
                }
            }
            if (!exist_flag)
            {

                max_pairs.push_back({p1, p2});
            }


    }
}


int find_max_distance_using_rotating_capilers(const vector<Point>& p, vector<Point_Pair>& max_pairs) {
    int p1 = 0, p2 = 0; 
    
    auto n = p.size();

    // parallel edges should't be visited twice
    vector<bool> visited(n, false);

    int max_dist_square = 0;

    // for (p1=0; p1 < n; p1++) {
    for (p1=0; p1 < n && visited[p1]==false; p1++) {
        
        auto p1next = (p1+1) % n;
        auto p2next = (p2+1) % n;
        Point base = {p[p1next].x - p[p1].x, p[p1next].y - p[p1].y};

        while (p2 == p1 || p2 == p1next) {
            p2 = (p2+1) % n;
            p2next = (p2+1) % n;
        }
        
        auto c1 = cross(p[p1], p[p1next], p[p2], p[p2next]);
        auto c2 = cross(p[p1], p[p1next], p[(p2-1+n) %n], p[p2]);
        auto s1 = sign(c1);
        auto s2 = sign(c2);


        while (s1 == s2) {
            p2 = (p2+1) % n;
            p2next = (p2+1) % n;

            c1 = cross(p[p1], p[p1next], p[p2], p[p2next]);
            c2 = cross(p[p1], p[p1next], p[(p2-1+n) %n], p[p2]);
            s1 = sign(c1);
            s2 = sign(c2);

        }

        visited[p1] = true;

        calculate_max_distance_square(max_dist_square, p[p1],p[p2], max_pairs);

        calculate_max_distance_square(max_dist_square, p[p1next],p[p2], max_pairs);
       
        // if (p1, p1next) and (p2, p2next) are parallel to each other
        if (cross(p[p1], p[p1next], p[p2],p[p2next]) == 0) {

            calculate_max_distance_square(max_dist_square, p[p1],p[p2next], max_pairs);

            calculate_max_distance_square(max_dist_square, p[p1next],p[p2next], max_pairs);

            visited[p2] = true;
        }
    }
    return max_dist_square;
    
}


int main()
{

    int totalPoints;

    cin >> totalPoints;

    vector<Point> points;
    int x, y;

    for (int i=0;i<totalPoints;i++) {
        cin >> x >> y;
        points.push_back({x,y});
    }
/*
    if (totalPoints == 1) {
        cout << "Max Distance = 0 Between point (" << points[0].x << " , " << points[0].y << ")  and point (" 
          << points[0].x << " , " << points[0].y << ")" << endl;
        return 0;
    }

    if (totalPoints == 2) {
        auto dist_square = pow(points[0].x - points[1].x,2) + pow(points[0].y - points[1].y, 2);
        cout << "Max Distance = " << dist_square << " Between point (" << points[0].x << " , " << points[0].y << ")  and point (" 
          << points[1].x << " , " << points[1].y << ")" << endl;
        return 0;
    }
  */
    if (totalPoints < 3) {
        cout << "It is required to provide at least 3 points!" << endl;
        return 0;
    }     

    vector<Point> convexHull;
    graham_scan(points, convexHull);
/*
    cout << "Print out Convex Hull" << endl;
    for (auto p: convexHull) {
        cout << "point (" << p.x << " , " << p.y << ")" << endl;
    }
*/

    vector<Point_Pair> max_pairs;
    auto max_dist_sq = find_max_distance_using_rotating_capilers(convexHull, max_pairs);

    cout << "Max Distance Square = " << max_dist_sq << endl;
    for (auto pp : max_pairs) {
      cout << "(" << pp.first.x << " , " << pp.first.y << ")  and ("
         << pp.second.x << " , " << pp.second.y << ")" << endl;
    }

    return 0;
}