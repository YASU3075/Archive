//-----------------------------------------------
// ComUniquePtr.h
// 制作日：2026/01/31
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <memory>

template <typename T>
struct ComReleaser {
	void operator()(T* ptr) const noexcept
	{
		if (ptr) 
		{
			ptr->Release();
		}
	}
};

template <typename T>
using ComUniquePtr = std::unique_ptr<T, ComReleaser<T>>;