//O. Kürşat Karayılan
//150140011

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
vector<int> mins;
vector<int> maxs;
int recursionCounter=0;

void print_arr(vector<int>& arr, int first, int last)
{
	for (int i = first; i <= last; i++)
	{
		string if_eol = (i == last) ? "\n" : ", ";

		cout << arr[i] << if_eol;
	}
}

void minMaxMerge()
{
	reverse(maxs.begin(),maxs.end());
	mins.insert(mins.end(), maxs.begin(), maxs.end());
}

void minMaxSort(vector<int> arr, int first, int last)
{
	int n = last-first+1;

	if(n<=1)
	{
		;
	}

	else
	{
		int smallest_element = arr[first]; //let, first element is the smallest one
		int largest_element = arr[first]; //also let, first element is the biggest one
		int i, temp;
	
		for(i = first; i <= last; i++)  //start iterating from the second element
		{
		    if(arr[i] < smallest_element)
		    {
		       smallest_element = arr[i];
		    }
		}
		cout << "Min: " << smallest_element;
	
		for(i = first; i <= last; i++)  //start iterating from the second element
			{
			    if(arr[i] > largest_element)
			    {
			       largest_element = arr[i];
			    }
			}
		cout << " Max: " << largest_element << endl;
		


		mins.push_back(smallest_element);
		maxs.push_back(largest_element);
		arr.erase(find(arr.begin()+first,arr.begin()+last, largest_element));
		arr.erase(find(arr.begin()+first,arr.begin()+last, smallest_element));
		if(n==3)
		{
			mins.push_back(arr[first]);
		}
		minMaxSort(arr, first, last-2);
	}

}

void minMax(vector<int>& arr, int first, int last)
{
	
	int n = last-first+1;
	if(n<=1)
	{
		;
	}
	else if(first<last)
	{
		mins.clear();
		maxs.clear();
		cout << "Min_max unsorted: ";
		for(int j=first; j<=last; j++)
		{
			cout << arr[j] << ", ";
		}
		cout << endl;	
		minMaxSort(arr, first, last);
		minMaxMerge();
		int m=0;
		for(int i=first; i<=last; i++)
		{
			arr[i] = mins[m];
			m++;
		}

		cout << "Min_max sorted: ";
		for(int k=first; k<=last; k++)
		{
			cout << arr[k] << ", ";
		}
		cout << endl;
	}

}

void printParts(vector<int> arr)
{
	vector<int> temp2;
	if(arr.size()<=2)
	{
		;
	}
	else
	{	
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
		m=m+1;
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
		minMax(arr, first, mid);
		if(recursionCounter<1)
		{
			vector<int> temp2;
			for(int i=mid+1; i<=last; i++)
			{
				cout << arr[i] << ", ";
				temp2.push_back(arr[i]);
			}
			cout << endl;
			printParts(temp2);
			recursionCounter++;
		}
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