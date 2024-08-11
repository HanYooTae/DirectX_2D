#pragma once
#include "ISubsystem.h"
#include "Resource/IResource.h"
#include <filesystem>		// 파일경로 저장(2017에 추가됨)

// Assets에 있는 자원 관리
enum class AssetType : uint
{
	Texture,
	Shader,
	Animation
};

class ResourceManager final : public ISubsystem
{
public:
	ResourceManager(class Context* const context);
	~ResourceManager() = default;

	// Inherited via ISubsystem
	virtual bool Initialize() override;
	virtual void Update() override;

	//==============================================
	// [Resource]
	//==============================================
	template <typename T>
	const std::shared_ptr<T> Load(const std::string& path);

	template<typename T>
	const std::shared_ptr<T> GetResourceFromName(const std::string& name);

	template<typename T>
	const std::shared_ptr<T> GetResourceFromPath(const std::string& path);

	template<typename T>
	void RegisterResource(const std::shared_ptr<T>& resource);

	bool HasResource(const std::string& resource_name, const ResourceType& resource_type);

	//==============================================
	// [Directory]
	//==============================================
	const std::string GetAssetRootDirectory() const { return "Assets/"; }
	const std::string GetAssetDirectory(const AssetType& type);

private:
	// 경로 등록용도
	void RegisterDirectory(const AssetType& type, const std::string& directory);

private:
	using  resource_group_t = std::vector<std::shared_ptr<IResource>>;
	std::map<ResourceType, resource_group_t> resource_groups;

	std::mutex resource_mutex;
	std::map<AssetType, std::string> assert_directories;
};

// template을 .cpp로 넘겼다는 것은 명시적으로 템플릿 특수화를 하기 위해서임
template<typename T>
inline const std::shared_ptr<T> ResourceManager::Load(const std::string& path)
{
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	// 파일이 경로가 존재하는지
	if (std::filesystem::exists(path) == false)
	{
		assert(false);
		return nullptr;
	}

	/*
		상대 경로 : Assets/Texture/metalslug.png
		절대 경로 : C:\Users\user\Desktop\C++\D2D11Game\Assets\Texture\metalslug.png
	*/

	/*
		find_last_of() -> 맨 뒤에서부터 문자 또는 문자열 안에 들어있는 문자를 찾으면 그 위치를 리턴해줌

		substr(a, b) -> a의 위치부터 b의 위치까지 짤라서 넣어줌
	*/

	auto last_index = path.find_last_of("\\/");		// \는 Escape Sequence라 2개를 추가해줘야함
	auto file_name = path.substr(last_index + 1, path.length());	// path.length() : (상대/절대)경로의 길이

	last_index = file_name.find_last_of('.');
	auto resource_name = file_name.substr(0, last_index);	// index는 0부터 시작하기 때문에 길이를 -1해주면 안됨

	// 동일한 이름의 텍스처를 사용하고 있다면, 또 만들어줄 필요없이 만들어져있는 것을 리턴
	if (HasResource(resource_name, IResource::DeduceResourceType<T>()))		// boolean이라 굳이 true 안넣어줘도 됨
		return GetResourceFromName<T>(resource_name);

	// else -> 동일한 이름의 텍스터가 없다면, 새로 생성하여 리턴
	std::shared_ptr<T> resource = std::make_shared<T>(context);
	resource->SetResourceName(resource_name);
	resource->SetResourcePath(path);

	if (resource->LoadFromFile(path) == false)
	{
		assert(false);
		return nullptr;
	}

	// 파일 등록
	RegisterResource<T>(resource);

	return resource;
}

template<typename T>
inline const std::shared_ptr<T> ResourceManager::GetResourceFromName(const std::string& name)
{
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	for (const auto& resource : resource_groups[IResource::DeduceResourceType<T>()])
	{
		if (resource->GetResourceName() == name)
			return std::static_pointer_cast<T>(resource);
	}

	return nullptr;		// 반환 값이 있냐 없냐 차이(return과 return nullptr의 차이)
}

template<typename T>
inline const std::shared_ptr<T> ResourceManager::GetResourceFromPath(const std::string& path)
{
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	for (const auto& resource : resource_groups[IResource::DeduceResourceType<T>()])
	{
		if (resource->GetResourceName() == path)
			return std::static_pointer_cast<T>(resource);
	}

	return nullptr;
}

template<typename T>
inline void ResourceManager::RegisterResource(const std::shared_ptr<T>& resource)
{
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	if (resource == nullptr)
	{
		assert(false);
		return;
	}

	resource_mutex.lock();

	resource_groups[resource->GetResourceType()].emplace_back(resource);
	
	resource_mutex.unlock();
}
