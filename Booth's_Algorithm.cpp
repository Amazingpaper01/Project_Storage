#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <bitset>
#include <fstream>
#include <string>
using namespace std; 

int ALU_1_bit(int a, int b, int cin, string OP, bitset<16> &result, int &pos) {
    int Cout = 0;       //carryout

    if (OP == "10") {               // addition operation by adding MD bit to AC bit
        result[pos] = (a ^ b ^ cin);
        pos++;

        Cout = (cin & (a ^ b)) | (a & b);
        return Cout;
    }
    else if (OP == "11") {          // subtraction by adding complement of MD bit
        if (b == 0)
            b = 1;
        else
            b = 0;

        result[pos] = (a ^ b ^ cin);
        pos++;
        
        Cout = (cin & (a ^ b)) | (a & b);
        return Cout;
    }
}

bitset<16> ALU_16_bit(bitset<16> AC, bitset<16> MD, int Binv, string OP) {
    int Cout = 0;
    int pos = 0;            //position for each individual bit
    bitset<16> result = 0000000000000000;

    if (OP == "11") {
        Cout = 1;
    }

    for (int i = 0; i < 16; i++) {
        Cout = ALU_1_bit(AC[i], MD[i], Cout, OP, result, pos);          //calls the 1 bit ALU function 16 times 
    }

    return result;                                                      //result is put into AC 
}

bitset<16> Booth_Algo(bitset<16> MD, bitset<16> &MQ) {
    int CC = 15;                                 //CC stands for cycle_counter which is initailized to '1111' initially
    bitset<16> AC = 0000000000000000;            // Accumulator
    int MQ_1 = 0;
    int counter = 0;                             // To keep track of # of iterations
    
    cout << "\nCycle counter\t\t\t MD\t\t\t AC\t\t\t MQ\t\t MQ_1" << endl;
    cout << "-------------------------------------------------------------------------------------------------------------------------------\n";
    cout << "   " << bitset<4>(CC) << "\t\t\t " << MD << "\t   " << AC << "\t   " << MQ << "\t   " << MQ_1 << endl;     //initial values

    for (CC; CC >= 0; CC--) {
        if (MQ[0] == 0 && MQ_1 == 0) {               // 0 0, none (AC = AC + 0)
            cout << "   " << bitset<4>(CC) << "\t\t\t " << MD << "\t   " << AC << "\t   " << MQ << "\t   " << MQ_1 << "\tStep 1 of itr " << counter + 1 << endl;
            MQ_1 = MQ[0];
            MQ = MQ >> 1;
            MQ[15] = AC[0];
            AC = AC >> 1;
            AC[15] = AC[14];            //simulate an arithmetic shift (cant find function part of bitset library to perform arithmetic right shift)
            cout << "   " << bitset<4>(CC) << "\t\t\t " << MD << "\t   " << AC << "\t   " << MQ << "\t   " << MQ_1 << "\tStep 2 of itr " << counter + 1 << endl << endl;
        }
        else if (MQ[0] == 0 && MQ_1 == 1) {     // 0 1, add (AC = AC + MD)
            AC = ALU_16_bit(AC, MD, 0, "10");        // Add operation
            cout << "   " << bitset<4>(CC) << "\t\t\t " << MD << "\t   " << AC << "\t   " << MQ << "\t   " << MQ_1 << "\tStep 1 of itr " << counter + 1 << endl;
            MQ_1 = MQ[0];
            MQ = MQ >> 1;
            MQ[15] = AC[0];
            AC = AC >> 1;
            AC[15] = AC[14];
            cout << "   " << bitset<4>(CC) << "\t\t\t " << MD << "\t   " << AC << "\t   " << MQ << "\t   " << MQ_1 << "\tStep 2 of itr " << counter + 1 << endl << endl;
        }
        else if (MQ[0] == 1 && MQ_1 == 0) {     // 1 0, sub (AC = AC - MD)
            AC = ALU_16_bit(AC, MD, 1, "11");        // subtraction operation
            cout << "   " << bitset<4>(CC) << "\t\t\t " << MD << "\t   " << AC << "\t   " << MQ << "\t   " << MQ_1 << "\tStep 1 of itr " << counter + 1 << endl;
            MQ_1 = MQ[0];
            MQ = MQ >> 1;
            MQ[15] = AC[0];
            AC = AC >> 1;
            AC[15] = AC[14];
            cout << "   " << bitset<4>(CC) << "\t\t\t " << MD << "\t   " << AC << "\t   " << MQ << "\t   " << MQ_1 << "\tStep 2 of itr " << counter + 1 << endl << endl;
        }
        else {                                  // 1 1, none (AC = AC + 0)
            cout << "   " << bitset<4>(CC) << "\t\t\t " << MD << "\t   " << AC << "\t   " << MQ << "\t   " << MQ_1 << "\tStep 1 of itr " << counter + 1 << endl;
            MQ_1 = MQ[0];
            MQ = MQ >> 1;
            MQ[15] = AC[0];
            AC = AC >> 1;
            AC[15] = AC[14];
            cout << "   " << bitset<4>(CC) << "\t\t\t " << MD << "\t   " << AC << "\t   " << MQ << "\t   " << MQ_1 << "\tStep 2 of itr " << counter + 1 << endl << endl;
        }
        counter++;
    }

    return AC;
}

int main()
{
    string str;             //User choice to keep program running or not
    bitset<16> MD;          //multiplicand
    bitset<16> MQ;          //multiplier 

    while (str != "q") {
        system("cls");
        cout << "Please enter MD (multiplicand) in 16-bit format: ";
        cin >> MD;

        cout << "\nPlease enter MQ (multiplier) in 16-bit format: ";
        cin >> MQ;

        cout << "Final product: " << Booth_Algo(MD, MQ) << "  " << MQ << " in 32-bit." << endl << endl;
        cout << "Enter 'q' to exit or any other letter/string to try different numbers: ";
        cin >> str;
    }
    return 0;
}