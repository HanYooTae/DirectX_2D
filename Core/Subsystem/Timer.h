#pragma once
#include "ISubsystem.h"
class Timer final : public ISubsystem
{
public:
	using ISubsystem::ISubsystem;
	~Timer() = default;

	// Inherited via ISubsystem
	virtual bool Initialize() override;
	virtual void Update() override;

	/*
		Delta time
		: 프레임간격 시간을 재서 동일하게 처리되게 함
	*/

	const float GetDeltaTimeMS() { return static_cast<float>(delta_time_ms); }
	const float GetDeltaTimeSEC() { return static_cast<float>(delta_time_ms / 1000.0f); }

private:
	double delta_time_ms = 0.0;
	std::chrono::time_point<std::chrono::high_resolution_clock> previous_time;
};


class Stopwatch final
{
public:
	Stopwatch();
	~Stopwatch() = default;

	void Start();

	const float GetElapsedTimeSec();
	const float GetElapsedTimeMs();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};

