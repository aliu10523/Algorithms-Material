#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <utility>
#include <set>

using namespace std;

#include <cmath>

struct ElementStruct {
    string data;
    string update;

    // Overload operator< for comparison
    bool operator<(const ElementStruct &other) const
    {
        return data < other.data; // Compare based on 'data'
    }
};

bool isPalindrome(const int& start, const int & len, const string& data) {
    int left = start;
    int right = left + len -1;

    while (left < right) {
        if (data[left] != data[right]) {
            return false;
        }
        left++;
        right--;
    }

    return true;
}

struct ElementStruct applyIncrement(const int& start, const int& len, set<struct ElementStruct> &oldSet, set<struct ElementStruct> &newSet)
{
    struct ElementStruct emptyElement;

    for (const ElementStruct& element: oldSet) {
        int left = start;
        int right = left + len -1;

        struct ElementStruct newElement;

        for (int i=0; i<len;i++) {
            int index = left +i;
            if (element.data[index] == element.data[right-i]) {
                continue;
            }

            newElement = element;
            if (newElement.data[index] < '9') {
                 newElement.data[index] = newElement.data[index] + 1;
                 newElement.update[index] = newElement.update[index] + 1;
        

                 if (isPalindrome(start, len, newElement.data)) {
                    return newElement;
                 }
                 newSet.insert(newElement);
                 continue;
            }

            // for digit 9
            for (auto j = index - 1; j >= 0; j--) {
                if (newElement.data[j] < '9')
                {
                    if (newElement.update[j] > '0' && newElement.data[j] != '0') {
                        newElement.update[j]--;
                    } else {
                        newElement.data[j] = newElement.data[j] + 1;
                    }
                    break;
                }
                else
                {
                    newElement.data[j] = '0';
                }
            }

            newElement.data[index]='0';
            newElement.update[index]++;
            if (isPalindrome(start, len, newElement.data)) {
                return newElement;
            }
            newSet.insert(newElement);
        }
    }

    return emptyElement;

}

struct ElementStruct generatePalindrome(int left, int len, struct ElementStruct& element) {
    struct ElementStruct result;
    set<ElementStruct> oldSet;
    set<ElementStruct> newSet;

    // cout << "processing string : " << element.data.substr(left,len) << endl;

    if (isPalindrome(left, len, element.data)) {
        return element;
    }

    oldSet.insert(element);
    while (result.data.size() == 0) {
        result = applyIncrement(left, len, oldSet, newSet);
        oldSet = newSet;
        newSet.clear();
    }
    // cout << "result string : " << result.data << endl;

    return result;
}

void applyRoundup(int left, int right, struct ElementStruct &element)
{

    int count = 0;
    bool change = true;

    
        int l = left;
        int r = right;
       
        int mid = (right - left) / 2;

        // while (r>= left  && l <= right) {
        while (r > mid) {

            if (element.data[l] == element.data[r])
            {
                r--;
                l++;
                continue;
            }

            int diff = element.data[r] - element.data[l];
            if (diff > 5)
            {
                if (element.data[r] == '9' || (r>0 && element.data[r-1] == '9')) {
                    for (auto i = r - 1; i >= 0; i--)
                    {
                        if (element.data[i] < '9')
                        {
                            element.data[i]++;
                            break;
                        }
                        else
                        {
                            element.data[i] = '0';
                        }
                    }
                    int change = element.data[r] - '0';
                    change = 10 - change;
                    element.data[r] = '0';
                    element.update[r] += change;
                }
            }
            r--;
            l++;
        }

}


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input_string;

    cin >> input_string;
    string data = input_string;

    struct ElementStruct initElement;
    initElement.data = data;
    initElement.update = data;

//    cout << "print out data" << endl;

    for (int i=0; i<data.length();i++)
    {
//        cout << data[i] << " "; 
        initElement.update[i]='0';
    }
//    cout << endl;


//    cout << "generatePalindrome" << endl;

    int actions = 0;
    struct ElementStruct resultElement;

    int calculatedActions = 0;

    if (data.length() > 1) {

        int len = 2;
        int left = data.length() / 2 - 1;

        if (data.length() % 2 != 0)
        {
            len++;
        }

        applyRoundup(0, data.length()-1, initElement);
     
        int actions = 0;
        while (left >= 0)
        {
            calculatedActions = 0;
            resultElement = generatePalindrome(left, len, initElement);
                for (int i=0; i<resultElement.data.length();i++)
                {
             //       cout << resultElement.update[i] << " ";
                    calculatedActions = calculatedActions + resultElement.update[i] - '0';
                }

             //   cout << endl;

             //   cout << "Calculated actions = " << calculatedActions << endl;

            left--;
            len += 2;
            initElement = resultElement;
        }
        cout << calculatedActions << endl;
    } else {
        // cout << "actions = " << actions << endl;
        cout << actions << endl;
    }
    // for (int &v : data)
/*
    for (int i=0; i<resultElement.data.length();i++)
    {
        cout << resultElement.data[i] << " ";
    }
    cout << endl;
*/
    return 0;
}
