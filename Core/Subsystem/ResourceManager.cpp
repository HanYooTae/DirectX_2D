#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(Context* const context)
	:ISubsystem(context)
{
	std::string root_directory = GetAssetRootDirectory();

	RegisterDirectory(AssetType::Animation, root_directory + "Animation/");
	RegisterDirectory(AssetType::Shader, root_directory + "Shader/");
	RegisterDirectory(AssetType::Texture, root_directory + "Texture/");
}

bool ResourceManager::Initialize()
{
	return true;
}

void ResourceManager::Update()
{
}

bool ResourceManager::HasResource(const std::string& resource_name, const ResourceType& resource_type)
{
	// resource_groups에서 resource_type들을 다 꺼내서 이름을 비교해보고,
	// 맞다면 true 리턴, 아니라면 false 리턴
	for (const auto& resource : resource_groups[resource_type])
	{
		if (resource->GetResourceName() == resource_name)
			return true;
	}
	return false;
}

const std::string ResourceManager::GetAssetDirectory(const AssetType& type)
{
	return assert_directories[type];
}

void ResourceManager::RegisterDirectory(const AssetType& type, const std::string& directory)
{
	assert_directories[type] = directory;
}