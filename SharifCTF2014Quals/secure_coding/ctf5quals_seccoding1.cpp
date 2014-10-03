#include <string>
#include <iostream>

#include <stdio.h>
#include <string.h>

using namespace std;
int main()
{
	string str;

	printf("SPLITTER\n");
	printf("--------\n");
	printf("\n");

	while (1)
	{
		string temp;
		cin.width(50);
		cin >> temp;

		if (temp == ".")
			break;

		str += temp + "\n";
	}
	if (str.size() > 1000)
			str.resize(1000);

	cout << "\n" << str << endl;

	return -14;
}