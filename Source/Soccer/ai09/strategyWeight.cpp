#include <map>
#include "ai09.h"

int ai09::strategy_weight ( void )
{
	if (!playBook) {
		std::cout<<"strategy_weight__+:  444"<<std::endl;
		return -1;
	}
	
	if (playBook->strategy_size() == 0) {
		std::cout<<"strategy_weight__+:  555"<<std::endl;
		return -1;
	}
	std::cout<<"strategy_weight__+:  333"<<std::endl;

	std::map<int,float> good_strs;
	
	float sigma_w = 0;
	
	std::cout << "	strategy:	" ;
	for (int i = 0; i < playBook->strategy_size(); i++) {
		if (
			(side*ball.Position.X>playBook->strategy(i).minx()) &&
			(side*ball.Position.X<playBook->strategy(i).maxx()) &&
			(fabs(ball.Position.Y)>playBook->strategy(i).miny()) &&
			(fabs(ball.Position.Y)<playBook->strategy(i).maxy()) &&
			(playBook->weight(i)>0)
			)
		{
			std::cout << i << "|" << playBook->weight(i) << "	";
			good_strs[i] = sigma_w + playBook->weight(i);
			sigma_w += playBook->weight(i);
		}
		
	}
	std::cout << std::endl;
	
	if (sigma_w == 0) {
		return -1;
	}
	
	
	int ans_str = -1;
	for (std::map<int,float>::const_iterator it = good_strs.begin(); it!=good_strs.end(); ++it) {
		if (randomParam*sigma_w <= it->second) {
			ans_str = it->first;
			break;
		}
	}
	
	return ans_str;
	
}