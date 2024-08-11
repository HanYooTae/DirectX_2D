#pragma once
#include "stdafx.h"
#include "Core/Subsystem/ISubsystem.h"
#include "Core/Subsystem/Renderer/Renderer.h"

// subsystem ���� -> �׷��Ƚ�, Ÿ�̸� ��� �����ؼ� ����
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
		// is_base_of<T1, T2> : T2�� T1�� ��ӹ޾��� �� value���� true, �ƴ϶�� false
		static_assert(std::is_base_of<ISubsystem, T>::value == true, "Procided type does not implement ISubsystem");	// �θ� üũ�ϰ�

		// ������ ���� emplace_back�� ���ϵǴ� ���� �ٸ�.
		subsystems.emplace_back(new T(this));		// ������� ������

		return static_cast<T*>(subsystems.back());		// ����

	}

	template<typename T>
	T* const GetSubsystem()
	{
		/*
			typeid�� ����ϸ� RTTI�� ����Ѵٴ� ���̹Ƿ� ��Ÿ����.
			��Ÿ���� �̸� ó���س��� ���� �ƴ� ������ ����Ǵ� ���� ó���ϴ� ��.
			�׷��� ������ �̸� ó���س��� �ͺ��� ���� �� �ۿ� �����Ƿ� ������� ����.(���������� ��Ȥ ���ȴٰ� ��.)
		*/

		static_assert(std::is_base_of<ISubsystem, T>::value == true, "Procided type does not implement ISubsystem");

		// ������Ʈ �Ӽ� -> C/C++ -> ��� -> ��Ÿ�� ���� ���� ���(RTTI�� ����� ���ΰ�) -> �⺻ �� yes
		for (const auto& subsystem : subsystems)
			// ���ø� ���ڸ� �����ͷ� �޾����� typeid�� �����ͷ� �޾ƾߵǴ°� �ƴѰ�?(T*��) �׷��� subsytem�� typeid�� ��� �޾ƾ� �ϴ°���?
			if (typeid(T) == typeid(*subsystem))		// subsystem�� ISubsystem�� ������. �츮�� �˾ƾ� �ϴ� ���� subsystem�� ������ �ִ� �������� ���� Ŭ������ �˾ƾ� �ϱ� ������ �������� �ؾ� ��
														// https://learn.microsoft.com/ko-kr/cpp/cpp/typeid-operator?view=msvc-170
				return static_cast<T*>(subsystem);

		// dynamic_cast�� ����Ѵٸ�
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
			// update �ѹ��� �Ͼ�� �ϴ� ���
			//if (typeid(Renderer) == typeid(*subsystem)) continue;

			if (typeid(*subsystem) == typeid(Renderer)) continue;

			subsystem->Update();

		}
	}

private:
	std::vector<ISubsystem*> subsystems;
};