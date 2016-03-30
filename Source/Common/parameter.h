#pragma once

#include <map>
#include <string>
#include "Vector.h"
#include "../Reality/Vision/Protobuf/aidebug.pb.h"

using namespace std;

struct DataNode
{
	Data_Type tydpe;
	union {
		bool     _bool   ;
		short    _short  ;
		int      _int    ;
		float    _float  ;
		double   _double ;
		TVec2    _TVec2  ;
		TVec3    _TVec3  ;
		char     c[12]   ;
	};
	string   _string ;
};

typedef map<string,DataNode>		ParameterSet;
typedef map<string,ParameterSet>	ParameterBook;

void LoadParameters ( const string& add , ParameterBook& parameterBook );
void ParseParameters ( const string& data , ParameterBook& parameterBook );