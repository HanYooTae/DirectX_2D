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
	// 문서를 작성하기 위한 종이, xml의 시작부분
	Xml::XMLDocument doc;

	// XMLDeclaration : xml의 버전과 인코딩 방식을 가지는 자료형, 굳이 없어도 되긴 함
	Xml::XMLDeclaration* decl = doc.NewDeclaration();
	// decl을 pushback해주는 것과 유사
	doc.LinkEndChild(decl);

	// XMLElement : 문서에 내용을 담기 위해 사용하는 자료형(실제 데이터)
	Xml::XMLElement* root = doc.NewElement("Animation");
	doc.LinkEndChild(root);

	// TinyXML2는 남이 만든 것이기 때문에 우리가 만든 자료형은 들어가지 않는다.(캐스팅은 가능)
	// SetAttribute(name, 데이터) : 해당 엘리먼트에 저장할 데이터와 데이터를 가리킬 이름을 저장한다.
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
		ㄴ decl(xml 버전, 인코딩 방식)
		ㄴ Animation -> 하나만 가지고 있는 정보(repeat_type, Texture_size)
			ㄴ (자식) Keyframe0 -> offset, size, time
			ㄴ (자식) Keyframe1 -> offset, size, time
			ㄴ (자식) Keyframe2 -> offset, size, time
			ㄴ (자식) Keyframe3 -> offset, size, time
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
	
	resource_name = root->Attribute("Name");		// Attribute : 문자형, 이외의 데이터형은 DoubleAttribute 등
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
