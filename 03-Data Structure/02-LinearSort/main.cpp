#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

bool sort(vector<int>& Array);
void fillArray(vector<int>& );
void printNumbers(vector<int>& );

int main(void) {

    // CODE
    vector<int> array;
    int number;

    fillArray(array);


    cout<<" --------------------------------------------" <<endl;
    cout<<" Before Sort" <<endl;
    printNumbers(array);
    cout<<" --------------------------------------------" <<endl;
    
    bool status = sort(array);
    cout<<" --------------------------------------------" <<endl;
    cout<<"After Sort" <<endl;
    printNumbers(array);
    cout<<" --------------------------------------------" <<endl;
   

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

bool sort(vector<int>& Array) {

    // CODE
    int minIndex;

    for(int i = 0 ; i < Array.size() ; i++) {

        int key = Array[i];
        int j = i - 1;
        for(j; j > -1 ; j--) {

            if(Array[j] >  key){
                Array[j] = 
            }
        }

        int temp = Array[i];
        Array[i] = Array[minIndex];
        Array[j] = temp;
    }

    return true;
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
