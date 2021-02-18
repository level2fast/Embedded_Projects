// Project1_reverse_bit_string.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout <<"Original string: "<< argv[1]<<"\n";
	std::cout << "reversing bits of: " << argv[1]<< "\n";

	std::string reversedArg = argv[1];
	char temp = '0';
	int size = reversedArg.length()-1;
	int numberOfSwaps = (reversedArg.length()) / 2;
	for (int i = 0; i < numberOfSwaps; i++)
	{
		temp = reversedArg[i];
		reversedArg[i] = reversedArg[size] ;
		reversedArg[size] = temp;
		size--;
	}

	std::cout << "reversed string: " << reversedArg << "\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
