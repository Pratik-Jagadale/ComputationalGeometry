#include<iostream>
#include<vector>

using namespace std;

// Vector is Dynamical Growing Array (realloc)

// GLOBAL FUNCTION DECLARTIONS
void PrintVec(vector<int> &v);

// Entry Point Function
int main(void)
{
    // VARIABLE DECLARTIONS
    vector<int> v;
    vector<int> v2;
    // vector<int, 10> v;
    int size;

    // CODE
    cin>> size;
    PrintVec(v);

    for(int i = 0 ; i < size; i++)
    {
        int temp;
        cin>> temp;
        v.push_back(temp); // Add Element at last
    }
    PrintVec(v);

    v.pop_back(); // Remove Last Element
    v.pop_back(); // Remove Last Element

    v2 = v; // Overloaded Opertor time Complexity:O(n) copy - Deep Copy
    // v2 = &v; // Overloaded Opertor time Complexity:O(1) exact copy - Shallow copy
    v2.push_back(5);
    v2.push_back(6);
    v2.push_back(7);

    return 0;
}

void PrintVec(vector<int> &v)
{
    // CODE
    cout<< "Size of Vector" << v.size() << endl; // time Complexity:O(0)
    for(int i = 0 ; i < v.size() ; i++)
        cout<< v[i] <<" ";
    cout<<endl;
}
