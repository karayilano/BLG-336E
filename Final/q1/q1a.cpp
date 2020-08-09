//O. Kürşat Karayılan
//150140011

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;
int recursionCounter = 0;
void print_arr(vector<int>& arr, int first, int last)
{
	for (int i = first; i <= last; i++)
	{
		string if_eol = (i == last) ? "\n" : ", ";

		cout << arr[i] << if_eol;
	}
}

void merge(vector<int>& arr, int first, int mid, int midPlus, int last)
{
	//Initialize
	int i = first;
	int j = midPlus;
	int k = 0;
	vector<int> temp;

	//Step2
	while(i<=mid and j<=last)
	{
		if(arr[i]<arr[j])
		{
			temp.push_back(arr[i++]);
			k++;
		}
		else
		{
			temp.push_back(arr[j++]);
			k++;
		}
	}

	//Step3
	while(i<=mid)
	{
		temp.push_back(arr[i++]);
		k++;
	}

	//Step4
	while(j<=last)
	{
		temp.push_back(arr[j++]);
		k++;
	}

	//Step5
	i = first;
	j = 0;
	while(i<=last)
	{
		arr[i] = temp[j];
		i = i+1;
		j = j+1;
	}
}

void func(vector<int>& arr, int first, int last)
{
	//Initialize
	int m=0;
	int k = first;
	vector<int> A;

	//Step 2
	while(k<last)
	{
		if(arr[k]>arr[k+1])
		{
			int temp = arr[k];
			arr[k] = arr[k+1];
			arr[k+1] = temp;
		}
		k = k+2;
	}

	//Step3
	k = first+1;
	while(k<=last)
	{
		A.push_back(arr[k]);
		m = m+1;
		k=k+2;
	}
	
	//Step4
	int x = first;
	k = first;
	while(k<=last)
	{
		arr[x] = arr[k];
		x = x+1;
		k = k+2;
	}
	
	//Step5
	k = 0;
	while(k<m)
	{
		arr[x] = A[k];
		x = x+1;
		k = k+1;
	}
}

void printParts(vector<int> arr)
{
	vector<int> temp;
	vector<int> temp2;
	if(arr.size()<=2)
	{
		;
	}
	else
	{	
		for(int i=0; i<arr.size(); i++)
		{
			if(i%2==0)
			{
				cout << arr[i] << ", ";
				temp.push_back(arr[i]);
			}
		}
		cout << endl;
		printParts(temp);
		for(int i=0; i<arr.size(); i++)
		{
			if(i%2==1)
			{
				cout << arr[i] << ", ";
				temp2.push_back(arr[i]);
			}
		}
		cout << endl;
		printParts(temp2);
	}
}

void sort(vector<int>& arr, int first, int last)
{
	int n = last-first+1;
	if(n<=1)
	{
		;
	}
	else if(first<last)
	{
		func(arr, first, last);
		int mid = (first+last)/2;
		if(recursionCounter<1)
		{
			vector<int> temp;
			vector<int> temp2;
			for(int i=0; i<=mid; i++)
			{
				cout << arr[i] << ", ";
				temp.push_back(arr[i]);
			}
			cout << endl;
			printParts(temp);

			for(int i=mid+1; i<=last; i++)
			{
				cout << arr[i] << ", ";
				temp2.push_back(arr[i]);
			}
			cout << endl;
			printParts(temp2);
			recursionCounter++;
		}
		sort(arr, first, mid);
		sort(arr, mid+1, last);
		merge(arr, first, mid, mid+1, last);
	}
	
}

int main(int argc, char *argv[])
{

	vector<int> all_values;


	try {
		
		string filename = argv[1];

		ifstream myfile(filename);

		
		if (myfile.is_open())
		{
			string line;
			while (getline(myfile, line))
			{
				int number = stoi(line);
				all_values.push_back(number);
			}
			print_arr(all_values, 0, all_values.size() - 1);

			sort(all_values, 0, all_values.size() - 1);

			print_arr(all_values, 0, all_values.size() - 1);
		}
		else
		{
			cout << "File is not available." << endl;
			throw;
		}

	}
	catch (const ifstream::failure& e)
	{
		cerr << "Exception opening the file!" << endl;
		return -1;
	}
	catch (...)
	{
		cerr << "Something's wrong!" << endl;
		return -1;
	}



	return 0;
}