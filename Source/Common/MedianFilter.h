#pragma once
#include <vector>
#include <algorithm>

using namespace std;

template <class T>
class MedianFilter
{
	vector<T> data;
	vector<T> temp;
	bool index;
	int size;

public:
	MedianFilter( int _size = 10 )
	{
		index = false;
		size = _size;
	}

	void AddData ( T in )
	{
		if ( index == 0 )
		{
			for ( int i = 0 ; i < size ; i ++ )	
				data.push_back(in);
			index = true;
		}
		else
		{
			data.push_back(in);
			data.erase(data.begin());
		}
	}

	T GetCurrent ( void )
	{
		temp = data;
		sort ( temp.begin() , temp.end() );
		if ( size%2 == 0 )
			return ( temp.at(temp.size()/2) + temp.at(1+temp.size()/2) ) / 2;
		return temp.at(temp.size()/2);
	}

	void reset ( void )
	{
		data.clear();
		index = false;
	}
};