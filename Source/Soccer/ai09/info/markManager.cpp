#include <vector>
#include <algorithm>

#include "ai09.h"

void ai09::MarkManager(bool restart)
{
	if (!isDefending) {
		for (map<int*, int>::const_iterator i = markMap.begin(); i != markMap.end(); ++i) {
			markMap[i->first] = -1;
		}
		return;
	}

	//if ( ( !restart ) && ( markMap[&dmf] == -1 ) )
	//	markMap[&dmf] = findKickerOpp(-1);


	vector<pair<int, float>> crunchingOpps;

	for (int i = 0; i < MAX_ROBOTS; i++) {
		float threat = calculateOppThreat(i, restart);
		if (threat < 0)
			continue;
		crunchingOpps.push_back(make_pair(i, threat));
	}

	sort(crunchingOpps.begin(), crunchingOpps.end(),
		[](const pair<int, float>& a, pair<int, float>& b) -> bool
	{
		return a.second > b.second;
	});

	cout << "	Opps: " << crunchingOpps.size() << endl;
	for(auto it = crunchingOpps.begin(); it != crunchingOpps.end(); ++it)
	{
		cout << "-- " << it->first << " : " << it->second << endl;
	}

	struct MarkPair
	{
		int own;
		int opp;
		float cost;
	};
	vector<MarkPair> mark_pairs;
	int mpi = 0;
	for (auto it = crunchingOpps.begin(); it != crunchingOpps.end(); ++it)
	{
		for (auto it2 = markMap.begin(); it2 != markMap.end(); ++it2)
		{
			auto own = *it2->first;
			auto opp = it->first;
			auto cost = calculateMarkCost(own, opp);
			if (cost < 0)
				continue;
			mark_pairs.push_back(MarkPair{ own, opp, cost });
		}
		if (++mpi >= markMap.size())
			break;
	}

	struct MarkFormation
	{
		vector<pair<int, int>> pairs;
		float TotalCost;
		MarkFormation()
		{
			TotalCost = 0.0f;
		}
	};
	vector<MarkFormation> valid_formations;

	for (auto it1 = mark_pairs.begin(); it1 != mark_pairs.end(); ++it1)
	{
		MarkFormation new_formation1;
		new_formation1.pairs.push_back(make_pair(it1->own, it1->opp));
		new_formation1.TotalCost += it1->cost;
		valid_formations.push_back(new_formation1);

		for (auto it2 = mark_pairs.begin(); it2 != mark_pairs.end(); ++it2)
		{
			if (it1->own == it2->own || it1->opp == it2->opp)
				continue;
			MarkFormation new_formation2;
			new_formation2.pairs.push_back(make_pair(it1->own, it1->opp));
			new_formation2.TotalCost += it1->cost;
			new_formation2.pairs.push_back(make_pair(it2->own, it2->opp));
			new_formation2.TotalCost += it2->cost;
			valid_formations.push_back(new_formation2);
			for (auto it3 = mark_pairs.begin(); it3 != mark_pairs.end(); ++it3)
			{
				if (it2->own == it3->own || it2->opp == it3->opp ||
					it1->own == it3->own || it1->opp == it3->opp)
					continue;
				MarkFormation new_formation3;
				new_formation3.pairs.push_back(make_pair(it1->own, it1->opp));
				new_formation3.TotalCost += it1->cost;
				new_formation3.pairs.push_back(make_pair(it2->own, it2->opp));
				new_formation3.TotalCost += it2->cost;
				new_formation3.pairs.push_back(make_pair(it3->own, it3->opp));
				new_formation3.TotalCost += it3->cost;
				valid_formations.push_back(new_formation3);
			}
		}
	}
	
	sort(valid_formations.begin(), valid_formations.end(),
		[](const MarkFormation& a, const MarkFormation& b) -> bool
	{
		if (a.pairs.size() == b.pairs.size())
		{
			return a.TotalCost < b.TotalCost;
		}
		else
		{
			return a.pairs.size() > b.pairs.size();
		}
	});


	for(auto it = markMap.begin(); it != markMap.end(); ++it)
		markMap[it->first] = -1;

	if (!valid_formations.empty())
	{
		auto best_pair = valid_formations[0].pairs;
		for (auto it = best_pair.begin(); it != best_pair.end(); ++it)
		{
			cout << " XXXXXX " << it->first << " : " << it -> second << endl;
			for (auto it1 = markMap.begin(); it1 != markMap.end(); ++it1)
			{
				if (*it1->first == it->first)
				{
					it1->second = it->second;
					break;
				}
			}
		}
	}
}