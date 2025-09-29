#include <iostream>
#include <string>

#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    cout << n << " " << q << endl;
    string line;
	getline(cin, line);
    for(int i = 0; i < n; i++){
	getline(cin, line);
	cout << line << endl;
    }
    
    srand(time(0));

    for(int i = 0; i < q; i++){
         char type;
         double x, y;
         int l, r;
         int idx;
	 string trans_type;

	string line;
        cin >> type;
        if(type == 'Q'){
	    getline(cin, line);
	    cout << type << line << endl;
        } else {
            cin >> idx >> trans_type;
	    getline(cin, line);

	    if (trans_type == "Scale") {
	      l = rand() % n;
	      r = l+ rand() % 20;
	      if (r > n-1)  r=n-1;
	      
	    } else {
	      l = rand() % n;
	      r = rand() % n;
	      if (l>r) {
	         int tmp = l;
	         l = r;
	         r = l;
	      }
	    }
	    cout << type << " " << l << " " << r << " " << trans_type << line << endl;
	    
        }
    }

    return 0;
}
    
