#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

bool search(vector<int>& , int );
void fillArray(vector<int>& );
void printNumbers(vector<int>& );

int main(void) {

    // CODE
    vector<int> array;
    int number;

    fillArray(array);


    while(number != -1) {
        
        cout<< "Enter Number Which you want to search in the Array(Press -1 for Exit)" <<endl;
        cin>> number;

        cout<<" --------------------------------------------" <<endl;
        bool status = search(array, number);
        if(status == true)
            cout<<" Number Exist in the Array." <<endl;
        else
            cout<<" Number Does Not Exist in the Array." <<endl;
        cout<<" --------------------------------------------" <<endl;
    }

    return 0;
}

void printNumbers(vector<int>& Array) {

    // CODE
    for(int i = 0 ; i< Array.size() ; i++)
        cout<< Array[i] <<" " <<endl;
}

void fillArray(vector<int>& Array) {

    // Variable Declarations
    int number;

    // CODE
    cout<< "Enter the numbers in array (Press -1 to Exit):" ;
    cin>> number;

    while (number != -1)
    {
        Array.push_back(number);
        cin>> number;
    }
}

bool search(vector<int>& Array, int search) {

    // CODE
    for(int i = 0 ; i < Array.size() ; i++)
    {
        if(Array[i] == search)
            return true;
    }
    return false;
}
