#pragma once
#include"stdafx.h"

class Object
{
public:
	Object() { id = GUID_Generator::Generate(); };
	virtual ~Object() = default;

	uint GetID() const { return id; }
	void SetID(const int& id) { this->id = id; }

private:
	uint id;
};