#include "stdafx.h"
#include "Animation.h"

Animation::Animation(Context* const context)
	: IResource(context, ResourceType::Animation)
{
}

Animation::~Animation()
{
	keyframes.clear();
}

bool Animation::SaveToFile(const std::string& path)
{
	// ������ �ۼ��ϱ� ���� ����, xml�� ���ۺκ�
	Xml::XMLDocument doc;

	// XMLDeclaration : xml�� ������ ���ڵ� ����� ������ �ڷ���, ���� ��� �Ǳ� ��
	Xml::XMLDeclaration* decl = doc.NewDeclaration();
	// decl�� pushback���ִ� �Ͱ� ����
	doc.LinkEndChild(decl);

	// XMLElement : ������ ������ ��� ���� ����ϴ� �ڷ���(���� ������)
	Xml::XMLElement* root = doc.NewElement("Animation");
	doc.LinkEndChild(root);

	// TinyXML2�� ���� ���� ���̱� ������ �츮�� ���� �ڷ����� ���� �ʴ´�.(ĳ������ ����)
	// SetAttribute(name, ������) : �ش� ������Ʈ�� ������ �����Ϳ� �����͸� ����ų �̸��� �����Ѵ�.
	root->SetAttribute("Name", resource_name.c_str());
	root->SetAttribute("Type", static_cast<uint>(repeat_type));

	root->SetAttribute("TexturePath", sprite_texture_path.c_str());
	root->SetAttribute("TextureSizeX", sprite_texture_size.x);
	root->SetAttribute("TextureSizeY", sprite_texture_size.y);

	for (const auto& keyframe : keyframes)
	{
		Xml::XMLElement* element = doc.NewElement("Keyframe");
		root->LinkEndChild(element);

		element->SetAttribute("OffsetX", keyframe.offset.x);
		element->SetAttribute("OffsetY", keyframe.offset.y);
		element->SetAttribute("SizeX", keyframe.size.x);
		element->SetAttribute("SizeY", keyframe.size.y);
		element->SetAttribute("Time", keyframe.time);
	}


	/*
		doc
		�� decl(xml ����, ���ڵ� ���)
		�� Animation -> �ϳ��� ������ �ִ� ����(repeat_type, Texture_size)
			�� (�ڽ�) Keyframe0 -> offset, size, time
			�� (�ڽ�) Keyframe1 -> offset, size, time
			�� (�ڽ�) Keyframe2 -> offset, size, time
			�� (�ڽ�) Keyframe3 -> offset, size, time
	*/

	return Xml::XMLError::XML_SUCCESS == doc.SaveFile(path.c_str());
}

bool Animation::LoadFromFile(const std::string& path)
{
	Xml::XMLDocument doc;
	
	Xml::XMLError error = doc.LoadFile(path.c_str());
	if (error != Xml::XMLError::XML_SUCCESS)
	{
		assert(false);
		return false;
	}

	Xml::XMLElement* root = doc.FirstChildElement();
	
	resource_name = root->Attribute("Name");		// Attribute : ������, �̿��� ���������� DoubleAttribute ��
	repeat_type = static_cast<RepeatType>(root->UnsignedAttribute("Type"));

	sprite_texture_path = root->Attribute("TexturePath");
	sprite_texture_size.x = root->FloatAttribute("TextureSizeX");
	sprite_texture_size.y = root->FloatAttribute("TextureSizeY");
	SetSpriteTexture(sprite_texture_path);
	
	Xml::XMLElement* element = root->FirstChildElement();

	for ( ; element != nullptr; element = element->NextSiblingElement())
	{
		D3DXVECTOR2 offset;
		offset.x = element->FloatAttribute("OffsetX");
		offset.y = element->FloatAttribute("OffsetY");

		D3DXVECTOR2 size;
		size.x = element->FloatAttribute("SizeX");
		size.y = element->FloatAttribute("SizeY");

		float time = element->FloatAttribute("Time");

		AddKeyframe(offset, size, time);
	}

	return true;
}

void Animation::SetSpriteTexture(const std::string& path)
{
	sprite_texture_path = path;

	sprite_texture = std::make_shared<D3D11_Texture>(context->GetSubsystem<Graphics>());
	sprite_texture->Create(path);
}

void Animation::AddKeyframe(const Keyframe& keyframe)
{
	keyframes.emplace_back(keyframe);
}

void Animation::AddKeyframe(const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double time)
{
	keyframes.emplace_back(offset, size, time);
}

const Keyframe* const Animation::GetKeyframeFromIndex(const uint& index)
{
	return &keyframes[index];
}
