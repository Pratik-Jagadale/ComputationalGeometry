#include<iostream>

using namespace std;

int main(void)
{
    pair<int , string> p;
    p = {2, "abcd"};
    pair<int , string> &p1 = p;
    cout<< p.first << " " << p.second <<endl;

    int a[] = { 1 , 2, 3};
    int b[] = {2 , 3, 4};
    pair<int ,int> p_array[3];

    p_array[0] = { 1 , 2};
    p_array[1] = { 2 , 3};
    p_array[2] = { 3 , 4};

    swap(p_array[0] , p_array[2]);
    for(int i; i < 3 ; i++)
        cout<<i <<" : "<<p_array[i].first << " " <<p_array[i].second <<endl;

    return 0;
}