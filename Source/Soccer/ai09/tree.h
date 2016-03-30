#pragma once

#include "../../Common/Vector.h"
#include "../../Common/distance.h"

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
	void AddNode ( TVec2 s , Node* p );
	Node * NearestNeighbour ( TVec2 s );
	unsigned int tree_size ( void ) { return nodes_num; }
	Node* GetNode ( unsigned int num ) { return &node[num]; }
};

Tree::Tree ( void )
{
	reset ( );
}

void Tree::reset ( void )
{
	nodes_num = 0;
}

void Tree::AddNode( TVec2 s , Node *p )
{
	if ( nodes_num < MAX_NODES )
	{
		node[nodes_num].state = s;
		node[nodes_num].parent = p;
		node[nodes_num].childs_num = 0;
		if ( p )
			p->childs_num ++;
		nodes_num ++;
	}
}

Node * Tree::NearestNeighbour ( TVec2 s )
{
	float d = 9999999;
	int ans;
	for ( int i = 0 ; i < nodes_num ; i ++ )
	{
		if ( DIS ( s , node[i].state ) < d )
		{
			ans = i;
			d = DIS ( s , node[i].state );
		}
	}
	return &node[ans];
}