#pragma once
#include "stdafx.h"
#include "Core/Subsystem/ISubsystem.h"
#include "Core/Subsystem/Renderer/Renderer.h"

// subsystem 관리 -> 그래픽스, 타이머 등등 통합해서 관리
class Context final
{
public:
	Context() = default;
	~Context()
	{
		// reverse begin, reverse end
		for (auto iter = subsystems.rbegin(); iter != subsystems.rend(); iter++)
			SAFE_DELETE(*iter);
	}

	const bool InitializeSubsystems()
	{
		bool result = true;

		for (const auto& subsystem : subsystems)
			if (subsystem->Initialize() == false) result = false;

		return result;
	}

	template<typename T>
	T* const RegisterSubsystem()
	{
		// is_base_of<T1, T2> : T2가 T1을 상속받았을 시 value값이 true, 아니라면 false
		static_assert(std::is_base_of<ISubsystem, T>::value == true, "Procided type does not implement ISubsystem");	// 부모 체크하고

		// 버전에 따라서 emplace_back이 리턴되는 값이 다름.
		subsystems.emplace_back(new T(this));		// 만들어준 다음에

		return static_cast<T*>(subsystems.back());		// 리턴

	}

	template<typename T>
	T* const GetSubsystem()
	{
		/*
			typeid를 사용하면 RTTI를 사용한다는 것이므로 런타임임.
			런타임은 미리 처리해놓는 것이 아닌 게임이 실행되는 동안 처리하는 것.
			그렇기 때문에 미리 처리해놓은 것보다 느릴 수 밖에 없으므로 사용하지 않음.(서버에서는 간혹 사용된다고 함.)
		*/

		static_assert(std::is_base_of<ISubsystem, T>::value == true, "Procided type does not implement ISubsystem");

		// 프로젝트 속성 -> C/C++ -> 언어 -> 런타임 형식 정보 사용(RTTI를 사용할 것인가) -> 기본 값 yes
		for (const auto& subsystem : subsystems)
			// 템플릿 인자를 포인터로 받았으면 typeid도 포인터로 받아야되는거 아닌가?(T*로) 그러면 subsytem의 typeid는 어떻게 받아야 하는거지?
			if (typeid(T) == typeid(*subsystem))		// subsystem은 ISubsystem의 포인터. 우리가 알아야 하는 것은 subsystem이 가지고 있는 포인터의 실제 클래스를 알아야 하기 때문에 역참조를 해야 함
														// https://learn.microsoft.com/ko-kr/cpp/cpp/typeid-operator?view=msvc-170
				return static_cast<T*>(subsystem);

		// dynamic_cast를 사용한다면
		//for (const auto& subsystem : subsystems)
		//{
		//	auto temp = dynamic_cast<T*>(subsystem);	// auto = T*
		//
		//	if (temp != nullptr) return temp;
		//}

		return nullptr;
	}

	void UpdateSubsystems()
	{
		for (const auto& subsystem : subsystems)
		{
			// update 한번만 일어나게 하는 방법
			//if (typeid(Renderer) == typeid(*subsystem)) continue;

			if (typeid(*subsystem) == typeid(Renderer)) continue;

			subsystem->Update();

		}
	}

private:
	std::vector<ISubsystem*> subsystems;
};