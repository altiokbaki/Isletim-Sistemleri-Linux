/**
 * @file 152120151055_uyg1A.cpp
 * @author Osman Baki ALTIOK
 * Contact: osmanbakialtiok@gmail.com
 */
#include <iostream>
#include <ctime>

// C++ program to generate random numbers and find their prime factors.
using namespace std;

long int randomNumber(int);                    	//Generates random number using srand() and rand().
void printFormatted1(int, int, int, int);      	//To reduce complexity and simplify code.
void printFormatted2(int, int);                	//To reduce complexity and simplify code.
void exception1(int, int, int);                 	//To reduce complexity and simplify code.

int main() {

    int tempNumber;                            	//For keeping number safe from changes.
    int divisor;                                	//Divisor of division operations.
    int number = 0;                             	//Generated number initialized as 0.
    int prime = 1;                              	//A variable to achieve exceptions.
    int temp = 1;                               	//For keeping number safe from changes and holding some exceptions.
    int counter1 = 0, counter2 = 0;             	//Counters.

    number = randomNumber(number);              	//Number is generated with function below main.
    cout <<endl<< "Number is: " << number << endl;    //Printed to the screen.
    tempNumber = number;                        	//tempNumber variable that will be used in division operations has been initialized.

    while (tempNumber % 2 == 0) {               	//This while loop is just to accelerate process.
        tempNumber = tempNumber / 2;
        counter1++;
    }
    printFormatted1(counter1, temp, tempNumber, number);        //Printed to the screen with a good format using function below main.

    for (divisor = 3; divisor < (number / 2); divisor += 2) {   //Main operation
        counter2 = 0;
        while (tempNumber % divisor == 0) {
            tempNumber = tempNumber / divisor;
            counter2++;
        }
        printFormatted2(counter2, divisor);                     //Printed to the screen with a good format using function below main.
    }
 
    exception1(tempNumber, number, prime);                      //Some exception made to accelerate process using function below main.
    return 0;                                                   //End.
}
long int randomNumber(int _number) {
    srand((unsigned)time(NULL));                                //<c.time>
    for (int i = 0; i < 5; i++) 
        _number = rand();                                       //Number variable has been generated.
    return _number;                                             //Number variable returns from the function.
}
void printFormatted1(int _counter1, int _temp, int _tempNumber, int _number) {
    if (_counter1 >= 1)
        cout << "2^" << _counter1 << "\t";                      //->2^1   5^6     ...

    if (_counter1 != 0) {
        for (int i = 0; i < _counter1; i++)
        {
            _temp = _temp * 2;
        }
        _tempNumber = _number / _temp;
    }
     else {
        _tempNumber = _number;
   }
}
void printFormatted2(int _counter2, int _divisor) {
    if (_counter2 >= 1) {
        cout << _divisor << "^" << _counter2 << "\t";           //2^1   ->5^6     ...
    }
}
void exception1(int _tempNumber, int _number, int _prime) {
    if (_tempNumber == _number)                                 //This function made after error handling.
        _prime = 0;
    if (_prime == 0) {
        cout << _number << "^1\t";                              //Some exceptions.  ->Number has no prime factors except 1 and itself.
    }
    else if (_tempNumber == _number / 2)
        cout << _prime / 2 << "^1\t";                           //Some exceptions.  ->Number has only one prime factor is 2.

}
