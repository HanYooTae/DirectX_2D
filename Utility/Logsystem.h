#pragma once

#define CALLER __FUNCTION__, __FILE__, std::to_string(__LINE__)

// __VA_ARGS__ : ...으로 들어오는 값을 해당 위치에 대입해준다.
#define LOG_INFO(Message, ...)								Logsystem::SetLog("INFO",	CALLER, Message, __VA_ARGS__)
#define LOG_WARNING(Message, ...)							Logsystem::SetLog("WARING",	CALLER, Message, __VA_ARGS__)
#define LOG_ERROR(Message, ...)								Logsystem::SetLog("ERROR",	CALLER, Message, __VA_ARGS__)

#define LOG_TARGET_ALL()		Logsystem::SetTarget(Logsystem::TARGET::ALL)
#define LOG_TARGET_CONSOLE()	Logsystem::SetTarget(Logsystem::TARGET::CONSOLE)
#define LOG_TARGET_FILE()		Logsystem::SetTarget(Logsystem::TARGET::FILE)

// 외부에 공개할 내용을 헤더에 적는다.
// 보통 헤더파일만 포함하기 때문에 헤더에 있는 멤버만 외부에 공개가 된다.
namespace Logsystem
{
	// 로그를 띄워주는 범위 지정
	enum class TARGET
	{
		ALL,
		CONSOLE,
		FILE
	};

	// 로그를 출력할 대상을 지정할 함수
	extern void SetTarget(const TARGET target);



	extern void SetLog
	(
		const std::string type,		// 해당 로그의 타입(INFO, WARNING, ERROR)
		const std::string function,	// 해당 로그를 호출한 함수의 이름 __FUNCTION__
		const std::string file,		// 해당 로그를 호출한 파일의 경로 __FILE__
		const std::string line,		// 해당 로그를 호출한 줄 번호 __LINE__
		const std::string format,	// 출력하고싶은 로그의 내용을 적는 부분
		...							// 출력하고싶은 로그의 내용을 적는 부분
	);
}