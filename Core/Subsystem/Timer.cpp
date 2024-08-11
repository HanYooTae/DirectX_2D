#include "stdafx.h"
#include "Timer.h"

bool Timer::Initialize()
{
	previous_time = std::chrono::high_resolution_clock::now();
	return true;
}

void Timer::Update()
{
	auto current_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms = current_time - previous_time;		// 현재 시간 - 이전 시간 = 간격 시간
	previous_time = current_time;		// 시간을 현재 시간 기준으로 잡아야 하기 때문에 이전 시간에 현재 시간을 대입해줌. 그러면 업데이트될 때마다 간격 시간이 타이머에 뜸.

	delta_time_ms = ms.count();		// 간격 시간을 델타타임으로 넣어줌
}

Stopwatch::Stopwatch()
{
	Start();
}

void Stopwatch::Start()
{
	start_time = std::chrono::high_resolution_clock::now();
}

const float Stopwatch::GetElapsedTimeSec()
{
	std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;
	return static_cast<float>(ms.count() / 1000.0);
}

const float Stopwatch::GetElapsedTimeMs()
{
	std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;
	return static_cast<float>(ms.count());
}