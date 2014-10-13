#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
	srand(time(NULL));

	int size = 4;
	int num = 500;
	
	ofstream myfile ("D:\\txt\\macierze4x4.txt");

	for(int m=0;m<num;m++)
	{
		myfile << "***\n"<<size<<endl;

		for(int j=0;j<size;j++)
		{
			for(int i=0;i<size;i++)
			{
				myfile << rand()%512-256 << " ";
			}
			myfile << "\n";
		}
		if(m==num-1)myfile<<"---";
	}
	myfile.close();
	system("PAUSE");
	return EXIT_SUCCESS;
}