//-----------------------------------------------
// GameObject.h
// 制作日：2025/05/21
// 制作者：安田晴人
//-----------------------------------------------
#pragma once
#include "renderer/Renderer.h"
#include <vector>

#include "entity\\Transform.h"
#include "component\\Component.h"
#include "physics\\ICollisionListener.h"
#include "physics\\ITriggerListener.h"

enum class LAYER;

class GAME_OBJECT
{
protected:
	bool m_destroy = false;

	TRANSFORM m_transform;

	std::vector<COMPONENT*> m_model_renderer;

	LAYER m_layer;
public:
	virtual void Init() = 0;

	void Uninit()
	{
		//コンポーネントの解放
		for (auto* component : m_model_renderer)
		{
			component->Uninit();
			delete component;
		}
		m_model_renderer.clear();
	}

	void Update()
	{
		m_transform.UpdateWorldMatrix();

		//コンポーネントの更新
		for (auto* component : m_model_renderer)
		{
			component->Update();
		}
	}

	virtual void Draw() {};

	void SetDestroy(bool destroy) { m_destroy = destroy; }
	bool GetDestroy() const { return m_destroy; }

	TRANSFORM* GetTransform() { return &m_transform; }

	void SetLayer(LAYER layer) { m_layer = layer; }
	LAYER GetLayer() const { return m_layer; }

	bool Destroy()
	{
		if (m_destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	VECTOR3 GetRight(void) const
	{
		XMMATRIX matrix;
		QUATERNION q = m_transform.GetRot();

		XMVECTOR q_m = XMVectorSet(q.x, q.y, q.z, q.w);
		matrix = XMMatrixRotationQuaternion(q_m); // クォータニオンから回転行列を取得

		VECTOR3 right;
		XMStoreFloat3((XMFLOAT3*)&right, matrix.r[0]);

		return right;
	}

	VECTOR3 GetUp(void) const
	{
		XMMATRIX matrix;
		QUATERNION q = m_transform.GetRot();

		XMVECTOR q_m = XMVectorSet(q.x, q.y, q.z, q.w);
		matrix = XMMatrixRotationQuaternion(q_m); // クォータニオンから回転行列を取得

		VECTOR3 up;
		XMStoreFloat3((XMFLOAT3*)&up, matrix.r[1]);

		return up;
	}

	VECTOR3 GetForward(void) const
	{
		XMMATRIX matrix;
		QUATERNION q = m_transform.GetRot();

		XMVECTOR q_m = XMVectorSet(q.x, q.y, q.z, q.w);
		matrix = XMMatrixRotationQuaternion(q_m); // クォータニオンから回転行列を取得
		
		VECTOR3 forward;
		XMStoreFloat3((XMFLOAT3*)&forward, matrix.r[2]);

		return forward;
	}

	template <typename T>
	T* AddComponent()
	{
		T* component = new T();

		component->SetOwner(this);
		component->OnAttach();

		component->Init();

		m_model_renderer.push_back(component);

		return component;
	}

	template <typename T>
	T* GetComponent()
	{
		for (auto component : m_model_renderer)
		{
			T* find = dynamic_cast<T*>(component);
			if (find != nullptr)
			{
				return find;
			}
		}

		return nullptr;
	};

	template <typename T>
	std::vector<T*> GetComponents()
	{
		std::vector<T*> components;
		for (auto component : m_model_renderer)
		{
			T* find = dynamic_cast<T*>(component);
			if (find != nullptr)
			{
				components.push_back(find);
			}
		}
		return components;
	};

	void  DispatchCollisionEnter(COLLIDER_3D* other)
	{
		for (auto component : m_model_renderer)
		{
			if (auto* listner = dynamic_cast<I_COLLISION_LISTENER*>(component))
			{
				listner->OnCollisionEnter(other);
			}
		}
	}
	void DispatchCollisionExit(COLLIDER_3D* other)
	{
		for (auto component : m_model_renderer)
		{
			if (auto* listner = dynamic_cast<I_COLLISION_LISTENER*>(component))
			{
				listner->OnCollisionExit(other);
			}
		}
	}
	void DispatchCollisionStay(COLLIDER_3D* other)
	{
		for (auto component : m_model_renderer)
		{
			if (auto* listner = dynamic_cast<I_COLLISION_LISTENER*>(component))
			{
				listner->OnCollisionStay(other);
			}
		}
	}

	void DispatchTriggerEnter(COLLIDER_3D* other)
	{
		for (auto component : m_model_renderer)
		{
			if (auto* listner = dynamic_cast<I_TRIGGER_LISTENER*>(component))
			{
				listner->OnTriggerEnter(other);
			}
		}
	}
	void DispatchTriggerExit(COLLIDER_3D* other)
	{
		for (auto component : m_model_renderer)
		{
			if (auto* listner = dynamic_cast<I_TRIGGER_LISTENER*>(component))
			{
				listner->OnTriggerExit(other);
			}
		}
	}
	void DispatchTriggerStay(COLLIDER_3D* other)
	{
		for (auto component : m_model_renderer)
		{
			if (auto* listner = dynamic_cast<I_TRIGGER_LISTENER*>(component))
			{
				listner->OnTriggerStay(other);
			}
		}
	}

	float GetDistance(VECTOR3 p)
	{
		return (m_transform.GetPos() - p).Length();
	}

	float GetZ(VECTOR3 p, VECTOR3 forward)
	{
		VECTOR3 dir = m_transform.GetPos() - p;
		return VECTOR3::Dot(dir, forward);
	}
};
