//
//  main.cpp
//  HW_02

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;


int main()
{
    
    // Declaring Variables
    string userName;
    string occupation;
    double income;
    double taxMoney = 0;
    int numOfChildren;
    
    //Asking for and setting the user's name
    cout << "Name: ";
    getline(cin, userName);
    if (userName == ""){
        cout << "---\nYou must enter a name" << endl;
        return 1;
    }
    
    //Asking for and setting the use's income
    cout << "Taxable income: ";
    cin  >> income;
    cin.ignore(10000,'\n');
    if(income < 0){
        cout << "---\nThe taxable income must not be negative" << endl;
        return 2;
    }
    
    //Asking for and setting the user's occupation
    cout << "Occupation: ";
    getline(cin, occupation);
    if (occupation == ""){
        cout << "---\nYou must enter an occupation" << endl;
        return 3;
    }
    
    //Asking for and setting the number of children
    cout << "Number of children: ";
    cin  >> numOfChildren;
    if (numOfChildren < 0) {
        cout << "---\nThe number of children must not be negative" << endl;
        return 4;
    }
    
    cout << "---" << endl;

    //Caculating the user's tax money
    //Case 1: User's income <= 55000
    if (income <= 55000) {
        if (numOfChildren > 0) {
            taxMoney = income * 0.04 - 200 * numOfChildren;
        }
        else {
            taxMoney = income * 0.04;
        }
    }
    
    //Case2: User's income <= 125000
    else if (income <= 125000) {
        if (occupation == "nurse" || occupation == "teacher") {
            taxMoney = 2200 + (income - 55000) * 0.05;
        }
        else {
            taxMoney = 2200 + (income - 55000) * 0.07;
        }
        if (numOfChildren > 0) {
            taxMoney = taxMoney - 200 * numOfChildren;
        }
    }
    
    //Case 3: User's income >125000
    else {
        if (occupation == "nurse" || occupation == "teacher") {
            taxMoney = 5700 + (income - 125000) * 0.093;
        }
        else {
            taxMoney = 7100 + (income - 125000) * 0.093;
        }
        
    }
    
    //Tax money must > 0
    if (taxMoney < 0){
        taxMoney = 0;
    }
    
    cout << userName << " would pay $";
    cout << fixed << setprecision(2) << taxMoney << endl;
    return 0;

}

