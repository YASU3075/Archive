//-----------------------------------------------
// ColliderManager.h
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#include "ColliderManager.h"
#include "Primitive3D.h"

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void COLLIDER_MANAGER::Init(void)
{
	for (int i = 0; i < MAX_COLLIDER; i++)
	{
		m_p_collider[i] = nullptr;
	}
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void COLLIDER_MANAGER::Uninit(void)
{
	delete[] m_p_collider;

	for (int i = 0; i < MAX_COLLIDER; i++)
	{
		m_p_collider[i] = nullptr;
	}
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void COLLIDER_MANAGER::Update(void)
{
	for (int i = 0; i < MAX_COLLIDER; i++)
	{
		if (m_p_collider[i] != nullptr)
		{//ヌルじゃなかったら
			m_p_collider[i]->Update();
		}
	}
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void COLLIDER_MANAGER::Draw(void)
{
	for (int i = 0; i < MAX_COLLIDER; i++)
	{
		if (m_p_collider[i] != nullptr)
		{//ヌルじゃなかったら
			m_p_collider[i]->Draw();
		}
	}
}

//-----------------------------------------------
// 当たり判定登録
//-----------------------------------------------
int COLLIDER_MANAGER::Register(BASE_COLLIDER* p_base_collider)
{
	for (int i = 0; i < MAX_COLLIDER; i++)
	{
		if (m_p_collider[i] == nullptr)
		{//ヌルだったら
			m_p_collider[i] = p_base_collider;

			//登録したIDを返す
			return i;
		}
	}

	//登録失敗
	return -1;
}

//-----------------------------------------------
// 当たり判定削除
//-----------------------------------------------
void COLLIDER_MANAGER::Destroy(int index)
{
	if (m_p_collider[index] != nullptr)
	{
		delete m_p_collider[index];
		m_p_collider[index] = nullptr;
	}
}

//-----------------------------------------------
// 指定したIDと一致した当たり判定取得
//-----------------------------------------------
BASE_COLLIDER* COLLIDER_MANAGER::GetCollider(int id)
{
	return m_p_collider[id];
}

//-----------------------------------------------
// 指定したタグと一致する当たり判定取得
//-----------------------------------------------
BASE_COLLIDER_CONTAINER COLLIDER_MANAGER::GetColliderContainer(const std::string tag)
{
	int count = 0;

	for (int i = 0; i < MAX_COLLIDER; i++)
	{
		if (m_p_collider[i] && m_p_collider[i]->GetTag() == tag)
		{
			count++;
		}
	}
	BASE_COLLIDER_CONTAINER ret(count);

	count = 0;

	for (int i = 0; i < MAX_COLLIDER; i++)
	{
		if (m_p_collider[i] && m_p_collider[i]->GetTag() == tag)
		{
			ret.SetCollider(count++, m_p_collider[i]);
		}
	}

	return ret;
}

//-----------------------------------------------
// 指定した当たり判定と当たってる当たり判定取得
//-----------------------------------------------
BASE_COLLIDER_CONTAINER COLLIDER_MANAGER::GetColliderContainer(const BASE_COLLIDER& collider)
{
	int count = 0;

	for (int i = 0; i < MAX_COLLIDER; i++)
	{
		if (m_p_collider[i])
		{
			if (GetCollision(*m_p_collider[i], collider))
			{//当たってたらカウントプラス
				count++;
			}
		}
	}
	BASE_COLLIDER_CONTAINER ret(count);

	count = 0;

	for (int i = 0; i < MAX_COLLIDER; i++)
	{
		if (m_p_collider[i])
		{
			if (GetCollision(*m_p_collider[i], collider))
			{//当たってたらコンテナに詰める
				ret.SetCollider(count++, m_p_collider[i]);
			}
		}
	}

	return ret;
}

//-----------------------------------------------
// 当たり判定処理
//-----------------------------------------------
bool COLLIDER_MANAGER::GetCollision(const BASE_COLLIDER& collider_1, const BASE_COLLIDER& collider_2)
{
	switch (collider_1.GetType())
	{
	case COLLIDER_TYPE_SPHERE:
		switch (collider_2.GetType())
		{
		case COLLIDER_TYPE_SPHERE:
		{//球同士の当たり判定
			//座標取得
			FLOAT3 pos_1 = collider_1.GetGameObject()->GetPos();
			FLOAT3 pos_2 = collider_2.GetGameObject()->GetPos();

			//半径取得
			float rad_1 = collider_1.GetGameObject()->GetRad();
			float rad_2 = collider_2.GetGameObject()->GetRad();

			//半径同士を足した物の2乗を取得
			float rad_sq = powf(rad_1 + rad_2, 2.0f);

			//長さの2乗取得
			float length = (pos_1 - pos_2).LengthSq();

			//当たってるかどうか返す
			return (rad_sq > length);

		}
			break;
		case COLLIDER_TYPE_BOX:
		{//球と立方体の当たり判定
			XMFLOAT3 vec = XMFLOAT3(0.0f, 0.0f, 0.0f);

			float length_x = collider_2.GetGameObject()->GetSca().x * 0.5f;
			float length_y = collider_2.GetGameObject()->GetSca().y * 0.5f;
			float length_z = collider_2.GetGameObject()->GetSca().z * 0.5f;

			XMFLOAT3 pos1 = collider_1.GetGameObject()->GetPos();		//球
			XMFLOAT3 pos2 = collider_2.GetGameObject()->GetPos();		//立方体

			XMFLOAT3 pos_div = XMFLOAT3(pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z);

			FLOAT3 pos_vec(pos_div);

			XMFLOAT3 x = XMFLOAT3(1.0f, 0.0f, 0.0f);
			XMFLOAT3 y = XMFLOAT3(0.0f, 1.0f, 0.0f);
			XMFLOAT3 z = XMFLOAT3(0.0f, 0.0f, 1.0f);

			//ベクトル計算用の領域へベクトルデータを転送
			XMVECTOR v_x;
			XMVECTOR v_y;
			XMVECTOR v_z;
			v_x = XMLoadFloat3(&x);
			v_y = XMLoadFloat3(&y);
			v_z = XMLoadFloat3(&z);

			//回転行列を作成
			XMMATRIX rotm = XMMatrixRotationRollPitchYaw(XMConvertToRadians(collider_2.GetGameObject()->GetRot().x), XMConvertToRadians(collider_2.GetGameObject()->GetRot().y), XMConvertToRadians(collider_2.GetGameObject()->GetRot().z));

			//ベクトルを回転
			v_x = XMVector3TransformCoord(v_x, rotm);
			v_y = XMVector3TransformCoord(v_y, rotm);
			v_z = XMVector3TransformCoord(v_z, rotm);

			//ベクトルの回転結果を通常の変数へ格納する
			XMStoreFloat3(&x, v_x);
			XMStoreFloat3(&y, v_y);
			XMStoreFloat3(&z, v_z);

			if (length_x > 0)
			{
				float s = pos_vec.Dot(x) / length_x;

				s = fabsf(s);
				if (s > 1)
				{
					vec.x += (1 - s) * length_x * x.x;
					vec.y += (1 - s) * length_x * x.y;
					vec.z += (1 - s) * length_x * x.z;
				}
			}

			if (length_y > 0)
			{
				float s = pos_vec.Dot(y) / length_y;

				s = fabsf(s);
				if (s > 1)
				{
					vec.x += (1 - s) * length_y * y.x;
					vec.y += (1 - s) * length_y * y.y;
					vec.z += (1 - s) * length_y * y.z;
				}
			}

			if (length_z > 0)
			{
				float s = pos_vec.Dot(z) / length_z;

				s = fabsf(s);
				if (s > 1)
				{
					vec.x += (1 - s) * length_z * z.x;
					vec.y += (1 - s) * length_z * z.y;
					vec.z += (1 - s) * length_z * z.z;
				}
			}

			FLOAT3 ret = vec;
			if (ret.Length() <= collider_1.GetGameObject()->GetRad())
			{
				return true;
			}

			else
			{
				return false;
			}
		}
			break;
		default:
			break;
		}
		break;

	case COLLIDER_TYPE_BOX:
		switch (collider_2.GetType())
		{
		case COLLIDER_TYPE_SPHERE:
		{
			XMFLOAT3 vec = XMFLOAT3(0.0f, 0.0f, 0.0f);

			float length_x = collider_1.GetGameObject()->GetSca().x * 0.5f;
			float length_y = collider_1.GetGameObject()->GetSca().y * 0.5f;
			float length_z = collider_1.GetGameObject()->GetSca().z * 0.5f;

			XMFLOAT3 pos1 = collider_2.GetGameObject()->GetPos();		//球
			XMFLOAT3 pos2 = collider_1.GetGameObject()->GetPos();		//立方体

			XMFLOAT3 pos_div = XMFLOAT3(pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z);

			FLOAT3 pos_vec(pos_div);

			XMFLOAT3 x = XMFLOAT3(1.0f, 0.0f, 0.0f);
			XMFLOAT3 y = XMFLOAT3(0.0f, 1.0f, 0.0f);
			XMFLOAT3 z = XMFLOAT3(0.0f, 0.0f, 1.0f);

			//ベクトル計算用の領域へベクトルデータを転送
			XMVECTOR v_x;
			XMVECTOR v_y;
			XMVECTOR v_z;
			v_x = XMLoadFloat3(&x);
			v_y = XMLoadFloat3(&y);
			v_z = XMLoadFloat3(&z);

			//回転行列を作成
			XMMATRIX rotm = XMMatrixRotationRollPitchYaw(XMConvertToRadians(collider_1.GetGameObject()->GetRot().x), XMConvertToRadians(collider_1.GetGameObject()->GetRot().y), XMConvertToRadians(collider_1.GetGameObject()->GetRot().z));

			//ベクトルを回転
			v_x = XMVector3TransformCoord(v_x, rotm);
			v_y = XMVector3TransformCoord(v_y, rotm);
			v_z = XMVector3TransformCoord(v_z, rotm);

			//ベクトルの回転結果を通常の変数へ格納する
			XMStoreFloat3(&x, v_x);
			XMStoreFloat3(&y, v_y);
			XMStoreFloat3(&z, v_z);

			if (length_x > 0)
			{
				float s = pos_vec.Dot(x) / length_x;

				s = fabsf(s);
				if (s > 1)
				{
					vec.x += (1 - s) * length_x * x.x;
					vec.y += (1 - s) * length_x * x.y;
					vec.z += (1 - s) * length_x * x.z;
				}
			}

			if (length_y > 0)
			{
				float s = pos_vec.Dot(y) / length_y;

				s = fabsf(s);
				if (s > 1)
				{
					vec.x += (1 - s) * length_y * y.x;
					vec.y += (1 - s) * length_y * y.y;
					vec.z += (1 - s) * length_y * y.z;
				}
			}

			if (length_z > 0)
			{
				float s = pos_vec.Dot(z) / length_z;

				s = fabsf(s);
				if (s > 1)
				{
					vec.x += (1 - s) * length_z * z.x;
					vec.y += (1 - s) * length_z * z.y;
					vec.z += (1 - s) * length_z * z.z;
				}
			}

			FLOAT3 ret = vec;
			if (ret.Length() <= collider_2.GetGameObject()->GetRad())
			{
				return true;
			}

			else
			{
				return false;
			}
		}
			break;
		case COLLIDER_TYPE_BOX:
			break;
		default:
			break;
		}
		break;
		break;
	default:
		break;
	}
}