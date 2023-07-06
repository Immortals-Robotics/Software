#pragma once

#include <map>
#include <string>
#include "Vector.h"

struct DataNode
{
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
	std::string   _string ;
};

typedef std::map<std::string,DataNode>		ParameterSet;
typedef std::map<std::string,ParameterSet>	ParameterBook;

void LoadParameters ( const std::string& add , ParameterBook& parameterBook );
void ParseParameters ( const std::string& data , ParameterBook& parameterBook );