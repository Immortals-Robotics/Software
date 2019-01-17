#pragma once

#include <map>
#include <vector>
#include "../../Common/math/Vector.h"
#include "protos/messages_robocup_ssl_detection.pb.h"

class IdGenerator
{
private:
	static constexpr double max_elapsed_t = 3000.f;
	static constexpr double max_merge_dis = 125.f;

	const bool use_constant_elapsed;
	const double constant_elapsed;
	double constant_based_time;

	int id_head;

	struct CachedState
	{
		double t_capture;
		TVec2 position;

		CachedState() = default;
		explicit CachedState(const double t_capture, const TVec2& position) :
			t_capture(t_capture), position(position) {}
	};
	std::map<int, CachedState> mappings;

	int GenerateNewId();
	void ClearExpiredMappings(double t_capture);

public:
	IdGenerator() : use_constant_elapsed(false), constant_elapsed(-1.), constant_based_time(0.), id_head(0) {}
	IdGenerator(const double constant_elapsed) : use_constant_elapsed(true), constant_elapsed(constant_elapsed), constant_based_time(0.), id_head(0) {}

	void Update(std::vector<SSL_DetectionRobot>& robots, int robots_count, double t_capture, bool use_vision_ids);
	void Update(std::vector<SSL_DetectionBall>& balls, int robots_count, double t_capture, std::vector<int>& out);
	void Reset();
};
