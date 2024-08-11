#pragma once
#include "ISubsystem.h"
#include "Resource/IResource.h"
#include <filesystem>		// ���ϰ�� ����(2017�� �߰���)

// Assets�� �ִ� �ڿ� ����
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
	// ��� ��Ͽ뵵
	void RegisterDirectory(const AssetType& type, const std::string& directory);

private:
	using  resource_group_t = std::vector<std::shared_ptr<IResource>>;
	std::map<ResourceType, resource_group_t> resource_groups;

	std::mutex resource_mutex;
	std::map<AssetType, std::string> assert_directories;
};

// template�� .cpp�� �Ѱ�ٴ� ���� ��������� ���ø� Ư��ȭ�� �ϱ� ���ؼ���
template<typename T>
inline const std::shared_ptr<T> ResourceManager::Load(const std::string& path)
{
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	// ������ ��ΰ� �����ϴ���
	if (std::filesystem::exists(path) == false)
	{
		assert(false);
		return nullptr;
	}

	/*
		��� ��� : Assets/Texture/metalslug.png
		���� ��� : C:\Users\user\Desktop\C++\D2D11Game\Assets\Texture\metalslug.png
	*/

	/*
		find_last_of() -> �� �ڿ������� ���� �Ǵ� ���ڿ� �ȿ� ����ִ� ���ڸ� ã���� �� ��ġ�� ��������

		substr(a, b) -> a�� ��ġ���� b�� ��ġ���� ©�� �־���
	*/

	auto last_index = path.find_last_of("\\/");		// \�� Escape Sequence�� 2���� �߰��������
	auto file_name = path.substr(last_index + 1, path.length());	// path.length() : (���/����)����� ����

	last_index = file_name.find_last_of('.');
	auto resource_name = file_name.substr(0, last_index);	// index�� 0���� �����ϱ� ������ ���̸� -1���ָ� �ȵ�

	// ������ �̸��� �ؽ�ó�� ����ϰ� �ִٸ�, �� ������� �ʿ���� ��������ִ� ���� ����
	if (HasResource(resource_name, IResource::DeduceResourceType<T>()))		// boolean�̶� ���� true �ȳ־��൵ ��
		return GetResourceFromName<T>(resource_name);

	// else -> ������ �̸��� �ؽ��Ͱ� ���ٸ�, ���� �����Ͽ� ����
	std::shared_ptr<T> resource = std::make_shared<T>(context);
	resource->SetResourceName(resource_name);
	resource->SetResourcePath(path);

	if (resource->LoadFromFile(path) == false)
	{
		assert(false);
		return nullptr;
	}

	// ���� ���
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

	return nullptr;		// ��ȯ ���� �ֳ� ���� ����(return�� return nullptr�� ����)
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
