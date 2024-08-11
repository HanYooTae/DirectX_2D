#include "stdafx.h"
#include "Logsystem.h"
#include <ctime>

//struct tm* t = localtime(&timer);
//time_t timer = time(NULL);

// �ܺο� �������� ���� ������
namespace Logsystem
{
	TARGET target = TARGET::ALL;

	// �α׸� ������ ������ ���
	std::filesystem::path log_path = "./MyLog.txt";

	// ó�� ������ ���°�?
	/*
		�� �κ��� ������
		- ������ ���� -> �α׸� ���� �� -> �α�Ȯ�ξ��ϰ� ������ ������ ������ ��������鼭 ���� �αװ� ����
	*/
	
	bool first = true;

	// SetLog���� ������� �α׸� log�� ����, Target�� ���� �α׸� �ۼ��ϴ� �ڵ带 �־���
	// -> �α׸� ����ϴ� �Լ�
	void Print(LPCSTR log);		// LPCSTR == Long ptr constant string
}

// �̸� �����ϵ� ����� �߰����� �ʾұ� ������ ������ �ȵ�.
// private�� ������ �ΰ� �ʹٸ� cpp�� ������ �߰��ص� ��

// == �ҽ����Ͽ� �ۼ��� �ڵ�� �ҽ������� �������� �ʴ� �� �ܺο� ������ȴ�.
namespace Logsystem
{
	// Ÿ�� ����
	void SetTarget(const TARGET target)
	{
		Logsystem::target = target;
	}

	// ���ڿ� �ϼ���Ű�� �Լ�
	// -> �α׸� �ۼ��ϴ� �Լ�
	void SetLog(const std::string type, const std::string function, const std::string file, const std::string line, const std::string format, ...)
	{
		// �ϼ��� �α׸� ��Ƽ� �Ʒ��� �ִ� Print�Լ��� ���
		std::string Log = "";

		// ���� ���� ������ ����
		va_list List = nullptr;

		// ���� ���� �������� ���� ��ġ�� �޴´�
		va_start(List, format);
		{
			// _vscprintf() : string�� size��ŭ arg_format(����)�� arg_list(��������)�� ���� ���ڿ��� ������� �� ũ�� ����
			// == format.c_str()�� ���Ŀ� List�� ���ڸ� �־��� �� �ϼ��Ǵ� ���ڿ��� ũ�⸦ ����
			size_t Size = _vscprintf(format.c_str(), List) + sizeof('\0');		// ũ�⸦ ������

			// ������� ũ�⸸ŭ �����Ҵ��� �޴´�.
			char* Message = new char[Size];

			// vsprintf_s() : string(Message)�� Size��ŭ arg_format�� arg_list�� ���� ���ڿ� �����͸� ����� �־��ش�.
			vsprintf_s(Message, Size, format.c_str(), List);		// ���ڿ��� �ϼ���Ŵ(Message�� �����͸� �����)

			// �ش� �α׸� ��� ȣ������ ������ �־��ش�.
			Log += "CALLER\t: ";
			Log += function + " (";
			Log += file + ", ";
			Log += line + ")\n";
			Log += type + "\t: ";

			// �ش� �α��� Ÿ�԰� �α��� ������ �����ش�.
			Log += std::string(Message) + "\n\n";

			// ��� �� �Ҵ� ����
			delete[] Message;
		}
		va_end(List);

		// �ϼ��� �α� �����͸� ����Ѵ�.
		Print(Log.c_str());
	}

	void Print(LPCSTR log)
	{
		switch (target)
		{
		case Logsystem::TARGET::ALL:
		case Logsystem::TARGET::CONSOLE:
		{
			printf("%s", log);

			if (target != TARGET::ALL) break;
		}
		case Logsystem::TARGET::FILE:
		{
			std::ofstream file;

			// ���α׷� ���� �� ù ���� ����̶�� ���� ������ ������ �����ϰ� �ٽ� �ۼ�
			if (first == true)
			{
				// open() : �ش� ����� ������ ����, ���ٸ� �������
				// ios::out : �ش� ����� ������ ���ٸ� �����ϰ�, �̹� �����Ѵٸ� ���ο� ���Ϸ� ������.(open�Լ��� �⺻��)
				// ios::out : ������ ���ٸ� �ȸ�����ְ� �ȿ��� �͵� �ֱ� ��
				file.open(log_path, std::ios::out);
				first = false;
			}

			/*
			app		:	�� ���� ���� ��Ʈ���� ������ �˻�
			ate		:	���� ���� ��Ʈ���� ������ ã���ϴ�.
			binary	:	���� ��忡�� ���ϴ�. (���� ��忡 ���� ������ ���� fopen �ϼ���.)
			in		:	�б������ ����
			out		:	��������� ����
			trunc	:	���� �� ������ ������ �����մϴ�.
			*/

			// ios::app : �ش� ����� ������ ���ٸ� �����ϰ�, �̹� �����Ѵٸ� ���� ���������� �̾� �ۼ��Ѵ�.
			else
				file.open(log_path, std::ios::app);		// begin

			// �ش� ���Ͽ� ������(�α�)�� �Է��Ѵ�.
			file << log;

			file.close();			// end

			if (target != TARGET::ALL) break;
		}
			break;
		default:
			break;
		}
	}
}