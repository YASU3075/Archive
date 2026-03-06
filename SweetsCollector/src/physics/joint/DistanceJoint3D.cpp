//-----------------------------------------------
// DistanceJoint3D.cpp
// 制作日：2026/02/26
// 制作者：安田晴人
//-----------------------------------------------
#include <cmath>
#include <algorithm>

#include "physics/joint/DistanceJoint3D.h"
#include "physics/RigidBody.h"
#include "entity/GameObject.h"

//-----------------------------------------------
// ローカルアンカーをワールド空間に変換
//-----------------------------------------------
static VECTOR3 CalcArmWorld(const TRANSFORM* t, const VECTOR3& local_anchor)
{
	const VECTOR3 sca = t->GetSca();
	VECTOR3 scaled_local = VECTOR3(local_anchor.x * sca.x, local_anchor.y * sca.y, local_anchor.z * sca.z);

	const MATRIX3x3 rot = t->GetRot().ToMatrix3x3();
	return rot.MultiplyVector(scaled_local);
}

//-----------------------------------------------
// デフォルトコンストラクタ
//-----------------------------------------------
DISTANCE_JOINT_3D::DISTANCE_JOINT_3D(RIGID_BODY* a, RIGID_BODY* b)
	: m_rb_a(a), m_rb_b(b)
{
}

//-----------------------------------------------
// ワールド空間のアンカーをローカル空間に変換して保存
//-----------------------------------------------
void DISTANCE_JOINT_3D::SetAnchorsWorld(const VECTOR3& anchor_a, const VECTOR3& anchor_b)
{
	if (!m_rb_a || !m_rb_b)
	{
		return;
	}

	TRANSFORM* ta = m_rb_a->GetOwner()->GetTransform();
	TRANSFORM* tb = m_rb_b->GetOwner()->GetTransform();

	if (!ta || !tb)
	{
		return;
	}

	m_local_anchor_a = ta->InverseTransformPoint(anchor_a);
	m_local_anchor_b = tb->InverseTransformPoint(anchor_b);
}

//-----------------------------------------------
// 事前準備
//-----------------------------------------------
void DISTANCE_JOINT_3D::PreStep(float dt)
{
    if (!m_rb_a || !m_rb_b) return;
    if (dt <= 0.0f) return;

	m_side = DISTANCE_LIMIT_SIDE::NONE;

    const float invMassA = m_rb_a->GetInvMass();
    const float invMassB = m_rb_b->GetInvMass();
    const MATRIX3x3& invIA = m_rb_a->GetInvInertiaWorld();
    const MATRIX3x3& invIB = m_rb_b->GetInvInertiaWorld();

    TRANSFORM* ta = m_rb_a->GetOwner()->GetTransform();
    TRANSFORM* tb = m_rb_b->GetOwner()->GetTransform();

    const VECTOR3 posA = ta->GetPos();
    const VECTOR3 posB = tb->GetPos();

    m_ra = CalcArmWorld(ta, m_local_anchor_a);
    m_rb = CalcArmWorld(tb, m_local_anchor_b);

    const VECTOR3 pA = posA + m_ra;
    const VECTOR3 pB = posB + m_rb;

    VECTOR3 d = pB - pA;
    float dist2 = d.LengthSquared();

    const float eps2 = 1e-12f;
    float dist = 0.0f;

    if (dist2 > eps2)
    {
        dist = std::sqrt(dist2);
        m_normal = d / dist;
    }
    else
    {
        VECTOR3 centerDir = (posB - posA);
        float c2 = centerDir.LengthSquared();

        if (c2 > eps2)
        {
            m_normal = centerDir.GetNormalized();
        }
        else
        {
            if (m_normal.LengthSquared() < 1e-6f)
                m_normal = VECTOR3(1, 0, 0);
            else
                m_normal = m_normal.GetNormalized();
        }

        dist = 0.0f;
    }

    m_normal = d / dist; // A->B

    m_active = true;
    float target = m_rest_length;

    if (m_mode == DISTANCE_LIMIT_MODE::FIXED)
    {
        target = m_rest_length;
        m_active = true;
    }
    else if (m_mode == DISTANCE_LIMIT_MODE::ROPE_MAX_ONLY)
    {
        if (dist <= m_max_length)
        {
            m_active = false;
        }
        else
        {
            target = m_max_length;
            m_active = true;
			m_side = DISTANCE_LIMIT_SIDE::MAX_SIDE;
        }
    }
    else
    {
        if (dist < m_min_length)
        {
            target = m_min_length;
            m_active = true;
            m_side = DISTANCE_LIMIT_SIDE::MIN_SIDE;
        }
        else if (dist > m_max_length)
        {
            target = m_max_length;
            m_active = true;
            m_side = DISTANCE_LIMIT_SIDE::MAX_SIDE;
        }
        else
        {
            m_active = false;
            m_side = DISTANCE_LIMIT_SIDE::NONE;
        }
    }

    if (!m_active)
    {
        m_effective_mass = 0.0f;
        m_bias = 0.0f;
        return;
    }

    VECTOR3 raXn = VECTOR3::Cross(m_ra, m_normal);
    VECTOR3 rbXn = VECTOR3::Cross(m_rb, m_normal);

    float k = invMassA + invMassB;
    k += VECTOR3::Dot(m_normal, VECTOR3::Cross(invIA.MultiplyVector(raXn), m_ra));
    k += VECTOR3::Dot(m_normal, VECTOR3::Cross(invIB.MultiplyVector(rbXn), m_rb));

    m_effective_mass = (k > 1e-8f) ? (1.0f / k) : 0.0f;

    float C = dist - target;

    const float slop = 0.001f;
    if (std::fabs(C) <= slop)
    {
        m_bias = 0.0f;
        return;
    }
    C = (C > 0.0f) ? (C - slop) : (C + slop);

    const float beta = 0.20f;
    m_bias = (beta / dt) * C;
}

//-----------------------------------------------
// 速度解決
//-----------------------------------------------
void DISTANCE_JOINT_3D::SolveVelocity(float dt)
{
    if (!m_rb_a || !m_rb_b) return;
    if (!m_active) return;
    if (m_effective_mass <= 0.0f) return;

    const float invMassA = m_rb_a->GetInvMass();
    const float invMassB = m_rb_b->GetInvMass();
    const MATRIX3x3& invIA = m_rb_a->GetInvInertiaWorld();
    const MATRIX3x3& invIB = m_rb_b->GetInvInertiaWorld();

    VECTOR3 vA = m_rb_a->GetLinearVelocity();
    VECTOR3 wA = m_rb_a->GetAngularVelocity();
    VECTOR3 vB = m_rb_b->GetLinearVelocity();
    VECTOR3 wB = m_rb_b->GetAngularVelocity();

    VECTOR3 velA = vA + VECTOR3::Cross(wA, m_ra);
    VECTOR3 velB = vB + VECTOR3::Cross(wB, m_rb);

    float Cdot = VECTOR3::Dot(velB - velA, m_normal);

    float lambda = -m_effective_mass * (Cdot + m_bias);

    if (m_mode == DISTANCE_LIMIT_MODE::ROPE_MAX_ONLY)
    {
        if (lambda > 0.0f) lambda = 0.0f;
    }
    else if (m_mode == DISTANCE_LIMIT_MODE::LIMITS_MIN_MAX)
    {
        if (m_side == DISTANCE_LIMIT_SIDE::MAX_SIDE)
        {
            if (lambda > 0.0f) lambda = 0.0f;
        }
        else if (m_side == DISTANCE_LIMIT_SIDE::MIN_SIDE)
        {
            if (lambda < 0.0f) lambda = 0.0f;
        }
    }

    VECTOR3 impulse = m_normal * lambda;

    if (invMassA > 0.0f)
    {
        m_rb_a->SetLinearVelocity(vA + (-impulse) * invMassA);
        m_rb_a->SetAngularVelocity(wA + invIA.MultiplyVector(VECTOR3::Cross(m_ra, -impulse)));
        m_rb_a->WakeUp();
    }
    if (invMassB > 0.0f)
    {
        m_rb_b->SetLinearVelocity(vB + impulse * invMassB);
        m_rb_b->SetAngularVelocity(wB + invIB.MultiplyVector(VECTOR3::Cross(m_rb, impulse)));
        m_rb_b->WakeUp();
    }
}

//-----------------------------------------------
// 位置解決
//-----------------------------------------------
void DISTANCE_JOINT_3D::SolvePosition(float dt)
{
    if (!m_rb_a || !m_rb_b) return;
    if (dt <= 0.0f) return;

    
    TRANSFORM* ta = m_rb_a->GetOwner()->GetTransform();
    TRANSFORM* tb = m_rb_b->GetOwner()->GetTransform();

    const VECTOR3 posA0 = ta->GetPos();
    const VECTOR3 posB0 = tb->GetPos();

    const VECTOR3 ra = CalcArmWorld(ta, m_local_anchor_a);
    const VECTOR3 rb = CalcArmWorld(tb, m_local_anchor_b);

    const VECTOR3 pA = posA0 + ra;
    const VECTOR3 pB = posB0 + rb;

    VECTOR3 d = pB - pA;
    float dist2 = d.LengthSquared();

    const float eps2 = 1e-12f;
    float dist = 0.0f;

    if (dist2 > eps2)
    {
        dist = std::sqrt(dist2);
        m_normal = d / dist;
    }
    else
    {
        VECTOR3 centerDir = (posB0 - posA0);
        float c2 = centerDir.LengthSquared();

        if (c2 > eps2)
        {
            m_normal = centerDir.GetNormalized();
        }
        else
        {
            if (m_normal.LengthSquared() < 1e-6f)
                m_normal = VECTOR3(1, 0, 0);
            else
                m_normal = m_normal.GetNormalized();
        }

        dist = 0.0f;
    }

    VECTOR3 n = d / dist;

    bool active = true;
    float target = m_rest_length;

    if (m_mode == DISTANCE_LIMIT_MODE::FIXED)
    {
        target = m_rest_length;
        active = true;
    }
    else if (m_mode == DISTANCE_LIMIT_MODE::ROPE_MAX_ONLY)
    {
        if (dist <= m_max_length) active = false;
        else { target = m_max_length; active = true; }
    }
    else
    {
        if (dist < m_min_length) { target = m_min_length; active = true; }
        else if (dist > m_max_length) { target = m_max_length; active = true; }
        else active = false;
    }

    if (!active) return;

    float C = dist - target;

    const float slop = 0.001f;
    if (std::fabs(C) <= slop) return;
    C = (C > 0.0f) ? (C - slop) : (C + slop);

    const float invA = m_rb_a->GetInvMass();
    const float invB = m_rb_b->GetInvMass();
    const float invSum = invA + invB;
    if (invSum <= 0.0f) return;

    const float beta_pos = 1.0f;

    VECTOR3 corr = n * (beta_pos * C / invSum);

    VECTOR3 newA = posA0 + corr * invA;
    VECTOR3 newB = posB0 - corr * invB;

    if (m_rb_a->HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_X)) newA.x = posA0.x;
    if (m_rb_a->HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_Y)) newA.y = posA0.y;
    if (m_rb_a->HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_Z)) newA.z = posA0.z;

    if (m_rb_b->HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_X)) newB.x = posB0.x;
    if (m_rb_b->HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_Y)) newB.y = posB0.y;
    if (m_rb_b->HasConstraints(RIGID_BODY_CONSTRAINTS::FREEZE_POS_Z)) newB.z = posB0.z;

    ta->SetPos(newA);
    tb->SetPos(newB);
}