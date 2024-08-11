#pragma once

#define CALLER __FUNCTION__, __FILE__, std::to_string(__LINE__)

// __VA_ARGS__ : ...���� ������ ���� �ش� ��ġ�� �������ش�.
#define LOG_INFO(Message, ...)								Logsystem::SetLog("INFO",	CALLER, Message, __VA_ARGS__)
#define LOG_WARNING(Message, ...)							Logsystem::SetLog("WARING",	CALLER, Message, __VA_ARGS__)
#define LOG_ERROR(Message, ...)								Logsystem::SetLog("ERROR",	CALLER, Message, __VA_ARGS__)

#define LOG_TARGET_ALL()		Logsystem::SetTarget(Logsystem::TARGET::ALL)
#define LOG_TARGET_CONSOLE()	Logsystem::SetTarget(Logsystem::TARGET::CONSOLE)
#define LOG_TARGET_FILE()		Logsystem::SetTarget(Logsystem::TARGET::FILE)

// �ܺο� ������ ������ ����� ���´�.
// ���� ������ϸ� �����ϱ� ������ ����� �ִ� ����� �ܺο� ������ �ȴ�.
namespace Logsystem
{
	// �α׸� ����ִ� ���� ����
	enum class TARGET
	{
		ALL,
		CONSOLE,
		FILE
	};

	// �α׸� ����� ����� ������ �Լ�
	extern void SetTarget(const TARGET target);



	extern void SetLog
	(
		const std::string type,		// �ش� �α��� Ÿ��(INFO, WARNING, ERROR)
		const std::string function,	// �ش� �α׸� ȣ���� �Լ��� �̸� __FUNCTION__
		const std::string file,		// �ش� �α׸� ȣ���� ������ ��� __FILE__
		const std::string line,		// �ش� �α׸� ȣ���� �� ��ȣ __LINE__
		const std::string format,	// ����ϰ���� �α��� ������ ���� �κ�
		...							// ����ϰ���� �α��� ������ ���� �κ�
	);
}