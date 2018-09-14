#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include "../../Common/math/Vector.h"
#include "protos/messages_robocup_ssl_detection.pb.h"

class IdGenerator
{
private:
	static constexpr double max_elapsed_t = 1000.f;
	static constexpr double max_merge_dis = 12.5f; // max running speed of human is 45 km/h -> 12.5 mm/ms

	struct CachedState
	{
		double t_capture;
		TVec2 position;

		CachedState() = default;
		explicit CachedState(const double t_capture, const TVec2& position) :
			t_capture(t_capture), position(position) {}
	};
	std::map<int, CachedState> mappings;

	int GenerateNewId() const;

	void ClearExpiredMappings(double t_capture);

public:
	void Update(SSL_DetectionRobot *robots, int robots_count, double t_capture);
};
