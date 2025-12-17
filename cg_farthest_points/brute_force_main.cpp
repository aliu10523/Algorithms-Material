#include <iostream>
#include <vector>
#include <bits/stdc++.h>
//#include <stack>
#include <cmath>

using namespace std;

struct Point{
    int x;
    int y;
    bool del;

    Point operator-(const Point &p) {
        return {x - p.x, y - p.y};
    }
};


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

    if (totalPoints < 3) {
        cout << "It is required to provide at least 3 points!" << endl;
        return 0;
    }  

    long max_dist_square = 0;

    vector<array<int,2>> max_index;

    for (int i=0;i<totalPoints;i++) {
        for (int j=i+1;j<totalPoints;j++) {
            auto dist_square = pow(points[i].x - points[j].x,2) + pow(points[i].y - points[j].y, 2);

            if (dist_square > max_dist_square) {
                max_dist_square = dist_square;  

                max_index.clear();
                max_index.push_back({i,j});
           } 

           if (dist_square == max_dist_square) {
               bool exist_flag = false;
               for (auto pp : max_index)
               {

                   if (pp[0] == i && pp[1] == j)
                   {
                       exist_flag = true;
                       break;
                   }
                   if (pp[0] == j && pp[1] == i)
                   {
                       exist_flag = true;
                       break;
                   }
               }
               if (!exist_flag)
               {

                   max_index.push_back({i, j});
               }
           } 
        }

    }

    cout << "Max Distance square = " << max_dist_square << endl;

    for (auto pp : max_index) {
      cout << "(" << points[pp[0]].x << " , " << points[pp[0]].y << ")  and (" 
          << points[pp[1]].x << " , " << points[pp[1]].y << ")" << endl;      
    }

    return 0;
}