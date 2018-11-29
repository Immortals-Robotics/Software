#include "tree.h"
#include "../../../Common/distance.h"

Tree::Tree ( void )
{
        reset ( );
}

void Tree::reset ( void )
{
        nodes_num = 0;
}

Node * Tree::AddNode( TVec2 & s , Node *p )
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

        return &(node[nodes_num-1]);
}

Node * Tree::NearestNeighbour ( TVec2 & s )
{
        if ( nodes_num == 0 )
            return NULL;
        if ( nodes_num == 1)
            return &node[0];
        float d = DISM ( s , node[0].state );
        float tmp_d = d;
        int ans = 0;
        for ( int i = 0 ; i < nodes_num ; i ++ )
        {
            tmp_d = DISM ( s , node[i].state );
                if ( tmp_d < d )
                {
                        ans = i;
                        d = tmp_d;
                }
        }
        return &node[ans];
}

unsigned int Tree::tree_size ( void )
{
    return nodes_num;
}

Node* Tree::GetNode ( unsigned int num )
{
    return &node[num];
}
