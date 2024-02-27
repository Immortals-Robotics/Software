#include <vector>
#include <algorithm>

#include "ai09.h"

void ai09::MarkManager(bool restart)
{
	if (!isDefending) {
		for (std::map<int*, int>::const_iterator i = markMap.begin(); i != markMap.end(); ++i) {
			markMap[i->first] = -1;
		}
		return;
	}

	auto start_t = timer.time();

	//if ( ( !restart ) && ( markMap[&dmf] == -1 ) )
	//	markMap[&dmf] = findKickerOpp(-1);


	std::vector<std::pair<int, float>> crunchingOpps;

	for (int i = 0; i < Setting::kMaxRobots; i++) {
		float threat = calculateOppThreat(i, restart);
		if (threat < 0)
			continue;
		crunchingOpps.push_back(std::make_pair(i, threat));
	}

	sort(crunchingOpps.begin(), crunchingOpps.end(),
		[](const std::pair<int, float>& a, const std::pair<int, float>& b) -> bool
	{
		return a.second > b.second;
	});

	std::cout << "	Opps: " << crunchingOpps.size() << std::endl;
	for(auto it = crunchingOpps.begin(); it != crunchingOpps.end(); ++it)
	{
		std::cout << "-- " << it->first << " : " << it->second << std::endl;
	}

	struct MarkPair
	{
		int own;
		int opp;
		float cost;
	};
	std::vector<MarkPair> mark_pairs;
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

	int def_count = 0;
	for (std::map<int*, int>::const_iterator i = markMap.begin(); i != markMap.end(); ++i) {
		if (OwnRobot[*i->first].State.seenState != CompletelyOut)
		{
			def_count ++;
		}
	}
	int markings = min(def_count, crunchingOpps.size());

	struct MarkFormation
	{
		std::vector<std::pair<int, int>> pairs;
		float TotalCost;
		MarkFormation()
		{
			pairs.reserve(5);
			TotalCost = 0.0f;
		}
	};
	std::vector<MarkFormation> valid_formations;
	valid_formations.reserve(120);

	for (auto it1 = mark_pairs.begin(); it1 != mark_pairs.end(); ++it1)
	{
		if (markings == 1)
		{
			MarkFormation new_formation1;
			new_formation1.pairs.push_back(std::make_pair(it1->own, it1->opp));
			new_formation1.TotalCost += it1->cost;
			valid_formations.push_back(new_formation1);
		}
		for (auto it2 = it1; it2 != mark_pairs.end(); ++it2)
		{
			if (it1->own == it2->own || it1->opp == it2->opp)
				continue;
			if (markings == 2)
			{
				MarkFormation new_formation2;
				new_formation2.pairs.push_back(std::make_pair(it1->own, it1->opp));
				new_formation2.TotalCost += it1->cost;
				new_formation2.pairs.push_back(std::make_pair(it2->own, it2->opp));
				new_formation2.TotalCost += it2->cost;
				valid_formations.push_back(new_formation2);
			}
			for (auto it3 = it2; it3 != mark_pairs.end(); ++it3)
			{
				if (it2->own == it3->own || it2->opp == it3->opp ||
					it1->own == it3->own || it1->opp == it3->opp)
					continue;
				if (markings == 3)
				{
					MarkFormation new_formation3;
					new_formation3.pairs.push_back(std::make_pair(it1->own, it1->opp));
					new_formation3.TotalCost += it1->cost;
					new_formation3.pairs.push_back(std::make_pair(it2->own, it2->opp));
					new_formation3.TotalCost += it2->cost;
					new_formation3.pairs.push_back(std::make_pair(it3->own, it3->opp));
					new_formation3.TotalCost += it3->cost;
					valid_formations.push_back(new_formation3);
				}
				for (auto it4 = it3; it4 != mark_pairs.end(); ++it4)
				{
					if (it3->own == it4->own || it3->opp == it4->opp ||
						it2->own == it4->own || it2->opp == it4->opp ||
						it1->own == it4->own || it1->opp == it4->opp)
						continue;
					if (markings == 4)
					{
						MarkFormation new_formation4;
						new_formation4.pairs.push_back(std::make_pair(it1->own, it1->opp));
						new_formation4.TotalCost += it1->cost;
						new_formation4.pairs.push_back(std::make_pair(it2->own, it2->opp));
						new_formation4.TotalCost += it2->cost;
						new_formation4.pairs.push_back(std::make_pair(it3->own, it3->opp));
						new_formation4.TotalCost += it3->cost;
						new_formation4.pairs.push_back(std::make_pair(it4->own, it4->opp));
						new_formation4.TotalCost += it4->cost;
						valid_formations.push_back(new_formation4);
					}
					for (auto it5 = it4; it5 != mark_pairs.end(); ++it5)
					{
						if (it4->own == it5->own || it4->opp == it5->opp ||
							it3->own == it5->own || it3->opp == it5->opp ||
							it2->own == it5->own || it2->opp == it5->opp ||
							it1->own == it5->own || it1->opp == it5->opp)
							continue;
						if (markings == 5)
						{
							MarkFormation new_formation5;
							new_formation5.pairs.push_back(std::make_pair(it1->own, it1->opp));
							new_formation5.TotalCost += it1->cost;
							new_formation5.pairs.push_back(std::make_pair(it2->own, it2->opp));
							new_formation5.TotalCost += it2->cost;
							new_formation5.pairs.push_back(std::make_pair(it3->own, it3->opp));
							new_formation5.TotalCost += it3->cost;
							new_formation5.pairs.push_back(std::make_pair(it4->own, it4->opp));
							new_formation5.TotalCost += it4->cost;
							new_formation5.pairs.push_back(std::make_pair(it5->own, it5->opp));
							new_formation5.TotalCost += it5->cost;
							valid_formations.push_back(new_formation5);
						}
					}
				}
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
			std::cout << " XXXXXX " << it->first << " : " << it -> second << std::endl;
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

	auto end_t = timer.time();

	std::cout << "MarkManager execution time: " << (end_t - start_t) * 1000.0 << std::endl;
}
