#include <set>

#include "ai09.h"

void ai09::MarkManager ( bool restart )
{
	if (!isDefending) {
		for (map<int*,int>::const_iterator i=markMap.begin(); i!=markMap.end(); ++i) {
			markMap[i->first]=-1;
		}
		return;
	}
	
	//if ( ( !restart ) && ( markMap[&dmf] == -1 ) )
	//	markMap[&dmf] = findKickerOpp(-1);
	
	
	set<int> crunchingOpps;
	
	for (int i = 0 ; i < MAX_ROBOTS ; i ++ ) {
		if ( OppRobot[i].seenState == CompletelyOut )
			continue;
		
		if ( (DIS(OppRobot[i].Position, ball.Position)<400) && (DIS(OwnRobot[attack].State.Position, ball.Position)<400) ) {
			continue;
		}
		
		if (OppRobot[i].Position.X*side>-1500) {
			crunchingOpps.insert(i);
		}
	}
	
	cout << "	Opps: " << crunchingOpps.size() << endl;
	
	for (map<int*,int>::const_iterator i=markMap.begin(); i!=markMap.end(); ++i) {
		if ( (OwnRobot[*i->first].State.seenState == CompletelyOut) || (crunchingOpps.find(i->second)==crunchingOpps.end()) ) {
			markMap[i->first]=-1;
		}
	}
	
	for (set<int>::const_iterator k = crunchingOpps.begin() ; k!=crunchingOpps.end(); ++k) 
	{
		int opp = *k;
		
		bool shouldMark = true;
		
		for (map<int*,int>::const_iterator i=markMap.begin(); i!=markMap.end(); ++i) {
			if (i->second==opp) {
				shouldMark = false;
				break;
			}
		}
		
		if (shouldMark) {
			for (map<int*,int>::const_iterator i=markMap.begin(); i!=markMap.end(); ++i) {
				if ( (OwnRobot[*i->first].State.seenState != CompletelyOut) && (i->second==-1) ) {
					markMap[i->first] = opp;
					break;
				}
			}
		}
	}
	
	
	/*for (map<int*,int>::const_iterator i=markMap.begin(); i!=markMap.end(); ++i) {
		if (i->second==-1) 
			continue;
		
		if ( 
			(DIS(OppRobot[i->second].Position, ball.Position)<400) && 
			(DIS(OwnRobot[*(i->first)].State.Position, ball.Position)<400) &&
			(DIS(OwnRobot[attack].State.Position, ball.Position)>600) ) {
			swap(attack, *(i->first));
			markMap[i->first] = -1;
			break;
		}
	}*/
}