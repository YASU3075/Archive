#pragma once
#include <functional>

// class Tを最大MAXSIZE個確保可能なPoolAllocatorを実装してください
template<class T, size_t MAXSIZE> class PoolAllocator
{
public:
	// コンストラクタ
	PoolAllocator() 
	{
		// TODO: 必要に応じて実装してください
		m_free_list = reinterpret_cast<FreeNode*>(m_pool); // プールの先頭を空きリストの先頭に設定
		FreeNode* current = m_free_list;
		m_pool[MAXSIZE * sizeof(T) - 1] = {}; // プールの初期化

		for (size_t i = 0; i < MAXSIZE - 1; i++)
		{// 空きリストを初期化
			current->next = reinterpret_cast<FreeNode*>(m_pool + (i + 1) * sizeof(T));
			current = current->next;
		}
		current->next = nullptr; // 最後のノードの次はnullptrに設定

		std::fill(std::begin(m_used), std::end(m_used), false); // 使用中のフラグを初期化
	}

	// デストラクタ
	~PoolAllocator() 
	{
		// TODO: 必要に応じて実装してください
		for (size_t i = 0; i < MAXSIZE; i++)
		{
			if (m_used[i])
			{
				T* obj = reinterpret_cast<T*>(m_pool + i * sizeof(T));
				obj->~T(); // オブジェクトのデストラクタを呼び出す
			}
		}
	}

	template<typename... Args>
	T* Alloc(Args&&... args)
	{
		if (!m_free_list)
			return nullptr;

		// アドレス確保
		FreeNode* node = m_free_list;
		m_free_list = m_free_list->next;

		size_t index = (reinterpret_cast<char*>(node) - m_pool) / sizeof(T);
		m_used[index] = true;

		// placement new
		return new (node) T(std::forward<Args>(args)...);
	}

	// Free(nullptr)で誤動作しないようにする事。
	void Free(T* addr) 
	{
		// TODO: 実装してください
		if (addr == nullptr)
		{// nullptrの場合は何もしない
			return;
		}
		addr->~T(); // オブジェクトのデストラクタを呼び出す
		size_t index = (reinterpret_cast<char*>(addr) - m_pool) / sizeof(T); // 返すノードのインデックスを計算
		m_used[index] = false; // 使用中フラグを解除

		FreeNode* node_to_free = reinterpret_cast<FreeNode*>(addr);		// T型ポインタをFreeNode型にキャスト
		node_to_free->next = m_free_list;								// 現在の空きリストの先頭を次に設定
		m_free_list = node_to_free;										// 空きリストの先頭を更新
	}

	void ForEach(std::function<void(T&)> func)
	{
		// プール内のすべてのオブジェクトに対して関数を適用
		for (size_t i = 0; i < MAXSIZE; i++)
		{
			if (m_used[i]) // 使用中のオブジェクトのみ処理
			{
				T* obj = reinterpret_cast<T*>(m_pool + i * sizeof(T)); // オブジェクトのポインタを取得
				func(*obj); // 関数を適用
			}
		}
	}

private:
	// TODO: 実装してください
	alignas(T) char m_pool[MAXSIZE * sizeof(T)]; // プール領域
	struct FreeNode
	{
		FreeNode* next; // 次の空きノードへのポインタ
	};
	FreeNode* m_free_list = nullptr; // 空きノードのリスト
	bool m_used[MAXSIZE]; // 使用中のフラグ
};
