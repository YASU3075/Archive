//-----------------------------------------------
// JointManager3D.cpp
// 制作日：2026/03/02
// 制作者：安田晴人
//-----------------------------------------------
#include <algorithm>
#include "physics/joint/JointManager3D.h"
#include "physics/RigidBody.h"
#include "physics/joint/DistanceJoint3D.h"

//-----------------------------------------------
// 剛体に関連するジョイントを削除
//-----------------------------------------------
void JOINT_MANAGER_3D::RemoveJointsForRigidBody(const RIGID_BODY* rb)
{
	if (!rb)
	{
		return;
	}

	m_joints.erase(std::remove_if(m_joints.begin(), m_joints.end(),
		[rb](const std::unique_ptr<JOINT_3D>& joint)
		{
			return joint->GetBodyA() == rb || joint->GetBodyB() == rb;
		}), m_joints.end());
}

//-----------------------------------------------
// 距離ジョイントの作成
//-----------------------------------------------
DISTANCE_JOINT_3D* JOINT_MANAGER_3D::CreateDistanceJoint(RIGID_BODY* a, RIGID_BODY* b, const VECTOR3& world_anchor_a, const VECTOR3& world_anchor_b, float rest_length)
{
	if (!a || !b)
	{
		return nullptr;
	}
	auto joint = std::make_unique<DISTANCE_JOINT_3D>(a, b);
	joint->SetAnchorsWorld(world_anchor_a, world_anchor_b);
	joint->SetRestLength(rest_length);

	DISTANCE_JOINT_3D* joint_ptr = joint.get();
	m_joints.push_back(std::move(joint));
	return joint_ptr;
}

//-----------------------------------------------
// ロープジョイントの作成（最大距離のみ）
//-----------------------------------------------
DISTANCE_JOINT_3D* JOINT_MANAGER_3D::CreateRopeJointMax(RIGID_BODY* a, RIGID_BODY* b, const VECTOR3& world_anchor_a, const VECTOR3& world_anchor_b, float max_length)
{
	if (!a || !b) return nullptr;

	auto joint = std::make_unique<DISTANCE_JOINT_3D>(a, b);
	joint->SetAnchorsWorld(world_anchor_a, world_anchor_b);
	joint->SetRopeLength(max_length);

	auto* ptr = joint.get();
	m_joints.emplace_back(std::move(joint));
	return ptr;
}

//-----------------------------------------------
// 最小距離と最大距離の両方を設定する距離ジョイントの作成
//-----------------------------------------------
DISTANCE_JOINT_3D* JOINT_MANAGER_3D::CreateLimitsJoint(RIGID_BODY* a, RIGID_BODY* b, const VECTOR3& world_anchor_a, const VECTOR3& world_anchor_b, float min_length, float max_length)
{
	if (!a || !b) return nullptr;

	auto joint = std::make_unique<DISTANCE_JOINT_3D>(a, b);
	joint->SetAnchorsWorld(world_anchor_a, world_anchor_b);
	joint->SetLimits(min_length, max_length);

	auto* ptr = joint.get();
	m_joints.emplace_back(std::move(joint));
	return ptr;
}

