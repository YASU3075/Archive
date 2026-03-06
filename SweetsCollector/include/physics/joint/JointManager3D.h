//-----------------------------------------------
// JointManager3D.h
// 制作日：2026/03/02
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <vector>
#include <memory>

#include "math/Vector3.h"
#include "physics/joint/Joint3D.h"

class RIGID_BODY;
class DISTANCE_JOINT_3D;

class JOINT_MANAGER_3D
{
private:
	std::vector<std::unique_ptr<JOINT_3D>> m_joints;

public:
	void Clear() { m_joints.clear(); }

	const std::vector<std::unique_ptr<JOINT_3D>>& GetJoints() const { return m_joints; }
	std::vector<std::unique_ptr<JOINT_3D>>& GetJoints() { return m_joints; }

	void RemoveJointsForRigidBody(const RIGID_BODY* rb);

	DISTANCE_JOINT_3D* CreateDistanceJoint(RIGID_BODY* a, RIGID_BODY* b, const VECTOR3& world_anchor_a, const VECTOR3& world_anchor_b, float rest_length);
	DISTANCE_JOINT_3D* CreateRopeJointMax(RIGID_BODY* a, RIGID_BODY* b, const VECTOR3& world_anchor_a, const VECTOR3& world_anchor_b, float max_length);
	DISTANCE_JOINT_3D* CreateLimitsJoint(RIGID_BODY* a, RIGID_BODY* b, const VECTOR3& world_anchor_a, const VECTOR3& world_anchor_b, float min_length, float max_length);

};
