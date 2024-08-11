#pragma once
// D3D11_Geometry.inl
#include"D3D11_Geometry.h"

template<typename T>
inline D3D11_Geometry<T>::~D3D11_Geometry()
{
	Clear();
}

template<typename T>
inline const std::vector<T> D3D11_Geometry<T>::GetVertices(const uint& offset, const uint& count)
{
	// 사용자가 입력한 범위 값을 복사하여 리턴
	std::vector<T> sub_vertices;

	// 첫번째 원소 + offset으로 복사를 시작할 원소 위치 지정
	auto first = vertices.begin() + offset;
	// 복사 시작 위치에서 읽어야 하는 마지막 원소의 다음 위치
	auto last = first + count;

	// assigne(a, b) : 두 지점을 넣으면 a부터 b전까지의 원소를 복사해주는 함수
	sub_vertices.assign(first, last);

	return sub_vertices;
}

template<typename T>
inline void D3D11_Geometry<T>::AddVertex(const T& vertex)
{
	vertices.emplace_back(vertex);
}

template<typename T>
inline void D3D11_Geometry<T>::AddVertices(const std::vector<T>& vertices)
{
	this->vertices.insert(
		this->vertices.end(),
		vertices.begin(),
		vertices.end()
	);
}

template<typename T>
inline void D3D11_Geometry<T>::SetVertices(const std::vector<T>& vertices)
{
	this->vertices.clear();
	this->vertices.shrink_to_fit();
	this->vertices = vertices;
}

template<typename T>
inline const std::vector<uint> D3D11_Geometry<T>::GetIndices(const uint& offset, const uint& count)
{
	// 사용자가 입력한 범위 값을 복사하여 리턴
	std::vector<uint> sub_indices;

	// 첫번째 원소 + offset으로 복사를 시작할 원소 위치 지정
	auto first = indices.begin() + offset;
	// 복사 시작 위치에서 읽어야 하는 마지막 원소의 다음 위치
	auto last = first + count;

	// assigne(a, b) : 두 지점을 넣으면 a부터 b전까지의 원소를 복사해주는 함수
	sub_indices.assign(first, last);

	return sub_indices;
}

template<typename T>
inline void D3D11_Geometry<T>::AddIndex(const uint& index)
{
	indices.emplace_back(index);
}

template<typename T>
inline void D3D11_Geometry<T>::AddIndices(const std::vector<uint>& indices)
{
	this->indices.insert(
		this->indices.end(),
		indices.begin(),
		indices.end()
	);
}

template<typename T>
inline void D3D11_Geometry<T>::SetIndices(const std::vector<uint>& indices)
{
	this->indices.clear();
	this->indices.shrink_to_fit();
	this->indices = indices;
}

template<typename T>
inline void D3D11_Geometry<T>::Clear()
{
	vertices.clear();
	vertices.shrink_to_fit();

	indices.clear();
	indices.shrink_to_fit();
}
