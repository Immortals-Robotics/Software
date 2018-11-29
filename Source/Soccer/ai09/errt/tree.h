#pragma once

#include "../../../Common/Vector.h"
#include <stdio.h>

struct Node
{
	Node *parent;
	unsigned int childs_num;
	TVec2 state;
};

#define MAX_NODES 600

class Tree
{
	Node node[MAX_NODES];

	unsigned int nodes_num;

public:
	Tree ( void );
	void reset ( void );
        Node * AddNode ( TVec2 & s , Node* p );
        Node * NearestNeighbour ( TVec2 & s );
        unsigned int tree_size ( void );
        Node* GetNode ( unsigned int num );
};

