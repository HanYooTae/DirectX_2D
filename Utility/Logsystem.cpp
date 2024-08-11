#include "stdafx.h"
#include "Logsystem.h"
#include <ctime>

//struct tm* t = localtime(&timer);
//time_t timer = time(NULL);

// 외부에 공개하지 않을 변수들
namespace Logsystem
{
	TARGET target = TARGET::ALL;

	// 로그를 저장할 파일의 경로
	std::filesystem::path log_path = "./MyLog.txt";

	// 처음 파일을 여는가?
	/*
		이 부분의 문제점
		- 에러가 났음 -> 로그를 봐야 함 -> 로그확인안하고 실행을 누르면 파일이 덮어씌워지면서 이전 로그가 날라감
	*/
	
	bool first = true;

	// SetLog에서 만들어진 로그를 log에 저장, Target에 따라서 로그를 작성하는 코드를 넣어줌
	// -> 로그를 출력하는 함수
	void Print(LPCSTR log);		// LPCSTR == Long ptr constant string
}

// 미리 컴파일된 헤더에 추가되지 않았기 때문에 공개가 안됨.
// private한 변수로 두고 싶다면 cpp에 변수를 추가해도 됨

// == 소스파일에 작성한 코드는 소스파일을 포함하지 않는 한 외부에 비공개된다.
namespace Logsystem
{
	// 타겟 세팅
	void SetTarget(const TARGET target)
	{
		Logsystem::target = target;
	}

	// 문자열 완성시키는 함수
	// -> 로그를 작성하는 함수
	void SetLog(const std::string type, const std::string function, const std::string file, const std::string line, const std::string format, ...)
	{
		// 완성된 로그를 담아서 아래에 있는 Print함수로 출력
		std::string Log = "";

		// 가변 인자 포인터 생성
		va_list List = nullptr;

		// 가변 인자 포인터의 시작 위치를 받는다
		va_start(List, format);
		{
			// _vscprintf() : string에 size만큼 arg_format(형식)과 arg_list(가변인자)를 합쳐 문자열을 만들었을 때 크기 리턴
			// == format.c_str()의 형식에 List의 인자를 넣었을 때 완성되는 문자열의 크기를 리턴
			size_t Size = _vscprintf(format.c_str(), List) + sizeof('\0');		// 크기를 구해줌

			// 만들어질 크기만큼 동적할당을 받는다.
			char* Message = new char[Size];

			// vsprintf_s() : string(Message)에 Size만큼 arg_format과 arg_list를 합쳐 문자열 데이터를 만들어 넣어준다.
			vsprintf_s(Message, Size, format.c_str(), List);		// 문자열을 완성시킴(Message에 데이터를 담아줌)

			// 해당 로그를 띄운 호출자의 정보를 넣어준다.
			Log += "CALLER\t: ";
			Log += function + " (";
			Log += file + ", ";
			Log += line + ")\n";
			Log += type + "\t: ";

			// 해당 로그의 타입과 로그의 내용을 합쳐준다.
			Log += std::string(Message) + "\n\n";

			// 사용 후 할당 해제
			delete[] Message;
		}
		va_end(List);

		// 완성된 로그 데이터를 출력한다.
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

			// 프로그램 실행 후 첫 파일 출력이라면 원래 파일의 내용을 삭제하고 다시 작성
			if (first == true)
			{
				// open() : 해당 경로의 파일을 오픈, 없다면 만들어줌
				// ios::out : 해당 경로의 파일이 없다면 생성하고, 이미 존재한다면 새로운 파일로 덮어씌운다.(open함수의 기본값)
				// ios::out : 파일이 없다면 안만들어주고 안여는 것도 있긴 함
				file.open(log_path, std::ios::out);
				first = false;
			}

			/*
			app		:	각 쓰기 전에 스트림의 끝으로 검색
			ate		:	열린 직후 스트림의 끝으로 찾습니다.
			binary	:	이진 모드에서 엽니다. (이진 모드에 대한 설명은 참조 fopen 하세요.)
			in		:	읽기용으로 열기
			out		:	쓰기용으로 열기
			trunc	:	열린 후 파일의 내용을 삭제합니다.
			*/

			// ios::app : 해당 경로의 파일이 없다면 생성하고, 이미 존재한다면 파일 끝에서부터 이어 작성한다.
			else
				file.open(log_path, std::ios::app);		// begin

			// 해당 파일에 데이터(로그)를 입력한다.
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