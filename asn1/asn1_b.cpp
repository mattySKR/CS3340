//
// Created by Matvey Skripchenko
// Student number: 250899673
//

#include <iostream>
#include <cstdlib>
#include <cstring>

// defining the constant
#define var unsigned long long

using namespace std;


// This function will multiply 2 matrices F and L
// and stores result in F
void matrix_mult(var F[2][2], var L[2][2]){
    var a =  F[0][0] * L[0][0] + F[0][1] * L[1][0];
    var b =  F[0][0] * L[0][1] + F[0][1] * L[1][1];
    var c =  F[1][0] * L[0][0] + F[1][1] * L[1][0];
    var d =  F[1][0] * L[0][1] + F[1][1] * L[1][1];

    F[0][0] = a;
    F[0][1] = b;
    F[1][0] = c;
    F[1][1] = d;

}

// In this function, F is raised to the power of n
// and stores the result in F
void matrix_pow(var F[2][2], int n)
{
    if (n == 0 || n == 1)
        return;
    // initializing
    var L[2][2] = {{1,1},{1,0}};

    // multiply M n-1 times
    for(int i = 2; i <= n; i++){
        matrix_mult(F,L);
    }
}

// final function that will return Fib numbers
var fibonacci(var n)
{
    // initializing
    var F[2][2] = {{1,1},{1,0}};
    if (n == 0)
        return 0;
    matrix_pow(F, n - 1);
    return F[0][0];

}

int main()
{
    for(int i=0;i<25;i++)
        cout<<fibonacci(i*20)<<endl;
    return 0;
}
