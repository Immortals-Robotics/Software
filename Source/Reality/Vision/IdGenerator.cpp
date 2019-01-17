#include "IdGenerator.h"
#include <unordered_set>
#include <limits>
#include <algorithm>

void IdGenerator::ClearExpiredMappings(const double t_capture)
{
	std::vector<int> to_be_deleted_ids;
	to_be_deleted_ids.reserve(mappings.size());

	for (const auto mapping : mappings)
	{
		const double elapsed = t_capture - mapping.second.t_capture;
		if (elapsed > max_elapsed_t)
		{
			to_be_deleted_ids.push_back(mapping.first);
		}
	}

	for(const auto id : to_be_deleted_ids)
	{
		mappings.erase(id);
	}
}

void IdGenerator::Update(std::vector<SSL_DetectionRobot>& robots, const int robots_count, double t_capture, bool use_vision_ids)
{
	if (use_constant_elapsed)
	{
		t_capture = constant_based_time; 
		constant_based_time += constant_elapsed;
	}

	// Clear expired mappings
	ClearExpiredMappings(t_capture);

	std::unordered_set<int> matched_ids;
	matched_ids.reserve(robots_count);

	if (!use_vision_ids)
	{
		for (int robot_idx = 0; robot_idx < robots_count; ++robot_idx)
		{
			SSL_DetectionRobot& robot = robots[robot_idx];
			robot.clear_robot_id();
		}
	}

	// First pass: try to match new robots to the previously labeled ones
	for (int robot_idx = 0; robot_idx < robots_count; ++robot_idx)
	{
		SSL_DetectionRobot& robot = robots[robot_idx];

		if (robot.has_robot_id())
		{
			continue;
		}

		const TVec2 current_pos{ robot.x(), robot.y() };

		float min_dis = std::numeric_limits<float>::max();
		int min_dis_id = -1;
		for (const auto mapping : mappings)
		{
			const int id = mapping.first;
			if (matched_ids.find(id) != matched_ids.end())
			{
				continue;
			}

			const double elapsed = t_capture - mapping.second.t_capture;
			const auto max_dis = static_cast<float>(elapsed * max_merge_dis);

			const TVec2 prev_pos = mapping.second.position;
			const float dis = Magnitude(current_pos - prev_pos);

			if (dis < max_dis && dis < min_dis)
			{
				min_dis = dis;
				min_dis_id = id;
			}
		}

		if (min_dis_id == -1)
		{
			continue;
		}

		matched_ids.insert(min_dis_id);
		robot.set_robot_id(min_dis_id);
		mappings[min_dis_id] = CachedState{ t_capture, current_pos };
	}

	// Second pass: assign new ids to the remaining unmatched robots
	for (int robot_idx = 0; robot_idx < robots_count; ++robot_idx)
	{
		SSL_DetectionRobot& robot = robots[robot_idx];

		if (robot.has_robot_id())
		{
			continue;
		}

		const int id = GenerateNewId();
		robot.set_robot_id(id);

		const TVec2 current_pos{ robot.x(), robot.y() };
		mappings[id] = CachedState{ t_capture, current_pos };
	}

	// Debug print
	/*std::sort(mappings.begin(), mappings.end(),
		[](const std::unordered_map<int, CachedState>::value_type& a, const std::unordered_map<int, CachedState>::value_type& b)
	{
		return a.first < b.first;
	});*/

	/*for (const auto & mapping : mappings)
	{
		printf(" - %d : (%.2f, %.2f) | [%.2f]\n",
			mapping.first,
			mapping.second.position.X, mapping.second.position.Y,
			static_cast<float>(t_capture - mapping.second.t_capture));
	}*/
}

void IdGenerator::Update(std::vector<SSL_DetectionBall>& balls, int balls_count, double t_capture, std::vector<int>& out)
{
	if (use_constant_elapsed)
	{
		t_capture = constant_based_time;
		constant_based_time += constant_elapsed;
	}

	// Clear expired mappings
	ClearExpiredMappings(t_capture);

	std::unordered_set<int> matched_ids;
	matched_ids.reserve(balls_count);

	out.resize(balls.size(), -1);

	// First pass: try to match new robots to the previously labeled ones
	for (int ball_idx = 0; ball_idx < balls_count; ++ball_idx)
	{
		SSL_DetectionBall& ball = balls[ball_idx];

		const TVec2 current_pos{ ball.x(), ball.y() };

		float min_dis = std::numeric_limits<float>::max();
		int min_dis_id = -1;
		for (const auto mapping : mappings)
		{
			const int id = mapping.first;
			if (matched_ids.find(id) != matched_ids.end())
			{
				continue;
			}

			const double elapsed = t_capture - mapping.second.t_capture;
			const auto max_dis = static_cast<float>(elapsed * max_merge_dis);

			const TVec2 prev_pos = mapping.second.position;
			const float dis = Magnitude(current_pos - prev_pos);

			if (dis < max_dis && dis < min_dis)
			{
				min_dis = dis;
				min_dis_id = id;
			}
		}

		if (min_dis_id == -1)
		{
			continue;
		}

		matched_ids.insert(min_dis_id);
		out[ball_idx] = min_dis_id;
		mappings[min_dis_id] = CachedState{ t_capture, current_pos };
	}

	// Second pass: assign new ids to the remaining unmatched robots
	for (int ball_idx = 0; ball_idx < balls_count; ++ball_idx)
	{
		if (out[ball_idx] != -1)
		{
			continue;
		}

		SSL_DetectionBall& ball = balls[ball_idx];

		const int id = GenerateNewId();
		out[ball_idx] = id;

		const TVec2 current_pos{ ball.x(), ball.y() };
		mappings[id] = CachedState{ t_capture, current_pos };
	}

	// Debug print
	/*std::sort(mappings.begin(), mappings.end(),
		[](const std::unordered_map<int, CachedState>::value_type& a, const std::unordered_map<int, CachedState>::value_type& b)
	{
		return a.first < b.first;
	});*/

	/*for (const auto & mapping : mappings)
	{
		printf(" - %d : (%.2f, %.2f) | [%.2f]\n",
			mapping.first,
			mapping.second.position.X, mapping.second.position.Y,
			static_cast<float>(t_capture - mapping.second.t_capture));
	}*/
}

void IdGenerator::Reset()
{
	mappings.clear();
	id_head = 0;
	constant_based_time = 0.;
}

int IdGenerator::GenerateNewId()
{
	return id_head++;

	/*for (int id = 0;;++id)
	{
		if (mappings.find(id) == mappings.end())
		{
			return id;
		}
	}*/
}
