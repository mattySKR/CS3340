//
// Created by Matvey Skripchenko.
// Student number: 250899673
//

#include <iostream>

using namespace std;

int fibonacci(int n) {
    if (n <= 1)
        return n;
    return fibonacci(n-1) + fibonacci(n-2);
}

int main ()
{
    for (int i=0;i<=10;i++)
        cout<<fibonacci(5*i)<<endl;
    return 0;
}

