//-----------------------------------------------
// PhysicsNarrowPhase3D.cpp
// 制作日：2026/01/14
// 制作者：安田晴人
//-----------------------------------------------
#include <algorithm>
#include <cfloat>
#include <cmath>

#include "Main.h"
#include "scene/Manager.h"
#include "renderer/Renderer.h"
#include "entity/GameObject.h"

#include "physics/PhysicsNarrowPhase3D.h"
#include "physics/RigidBody.h"
#include "physics/Collider3D.h"
#include "physics/SphereCollider.h"
#include "physics/BoxCollider3D.h"
#include "physics/ContactManager.h"
#include "math/Matrix3x3.h"

//-----------------------------------------------
// ヘルパー関数群
//-----------------------------------------------
namespace
{
	//-----------------------------------------------
	// 球と球の衝突判定
	//-----------------------------------------------
	bool CheckCollisionSphereSphere(RIGID_BODY* a, RIGID_BODY* b, std::vector<CONTACT_POINT>& out_contacts)
	{
		if (!a || !b)
		{// nullチェック
			return false;
		}

		// コライダー取得
		SPHERE_COLLIDER* col_a = static_cast<SPHERE_COLLIDER*>(a->GetCollider());
		SPHERE_COLLIDER* col_b = static_cast<SPHERE_COLLIDER*>(b->GetCollider());

		if (!col_a || !col_b)
		{// コライダーが無い場合はスキップ
			return false;
		}

		// 中心点と半径の取得
		VECTOR3 center_a = col_a->GetWorldCenter();
		VECTOR3 center_b = col_b->GetWorldCenter();

		float radius_a = col_a->GetWorldRadius();
		float radius_b = col_b->GetWorldRadius();

		VECTOR3 diff = center_b - center_a;
		float dist_sq = diff.LengthSquared();
		float radius_sum = radius_a + radius_b;

		// 衝突しているか判定
		if (dist_sq > radius_sum * radius_sum)
		{
			return false;
		}

		// 衝突情報の計算
		float distance = std::sqrt(dist_sq);

		VECTOR3 normal;
		if (distance > 1e-6f)
		{
			normal = diff / distance; // 正規化
		}
		else
		{// 中心がほぼ同じ位置にある場合、適当な法線を設定
			normal = VECTOR3(1.0f, 0.0f, 0.0f);
		}

		// 侵入深さ
		float penetration = radius_sum - distance;

		// 接触点Aの表面から半分戻した位置
		VECTOR3 contact_point = center_a + normal * (radius_a - penetration * 0.5f);

		// CONTACT_POINT の作成
		CONTACT_POINT cp{};
		cp.pos = contact_point;
		cp.normal = normal;
		cp.penetration = penetration;
		cp.restitution = std::max(a->GetRestitution(), b->GetRestitution());

		cp.static_friction = (a->GetStaticFriction() + b->GetStaticFriction()) * 0.5f;
		cp.dynamic_friction = (a->GetDynamicFriction() + b->GetDynamicFriction()) * 0.5f;

		out_contacts.push_back(cp);

		return true;
	}

	//-----------------------------------------------
	// Sphere vs Box 衝突判定
	//-----------------------------------------------
	bool CheckCollisionSphereBox(RIGID_BODY* sphere, RIGID_BODY* box, std::vector<CONTACT_POINT>& out_contacts)
	{
		if (!box || !sphere)
		{// nullチェック
			return false;
		}

		if (!box->GetCollider() || !sphere->GetCollider())
		{// コライダーが無い場合はスキップ
			return false;
		}

		// コライダー取得
		BOX_COLLIDER_3D* box_col = static_cast<BOX_COLLIDER_3D*>(box->GetCollider());
		SPHERE_COLLIDER* sphere_col = static_cast<SPHERE_COLLIDER*>(sphere->GetCollider());
		if (!box_col || !sphere_col)
		{// コライダーが無い場合はスキップ
			return false;
		}

		// 変換行列取得
		const TRANSFORM* box_tr = box->GetOwner()->GetTransform();
		const TRANSFORM* sphere_tr = sphere->GetOwner()->GetTransform();

		// 各種パラメータ取得
		MATRIX3x3 rb = box_tr->GetRot().ToMatrix3x3();
		VECTOR3   box_pos = box_tr->GetPos();
		VECTOR3   half_ext = box_col->GetWorldSize() * 0.5f;
		VECTOR3   sphere_pos = sphere_tr->GetPos();
		float     radius = sphere_col->GetWorldRadius();

		// Boxローカルに変換
		VECTOR3 local_center = rb.Transpose().MultiplyVector(sphere_pos - box_pos);

		// clamp で最近接点
		VECTOR3 local_closest = local_center.Clamp(-half_ext, half_ext);

		// ワールドの最近接点
		VECTOR3 closest_world = box_pos + rb.MultiplyVector(local_closest);

		// Box上の点からSphere中心へのベクトル
		VECTOR3 diff = sphere_pos - closest_world;
		float   dist2 = diff.LengthSquared();
		float   r2 = radius * radius;

		if (dist2 > r2)
		{// 衝突していない
			return false;
		}

		// 衝突情報の計算
		float dist = std::sqrt(std::max(dist2, 1e-8f));
		VECTOR3 n;

		if (dist > 1e-4f)
		{
			n = diff / dist;
		}
		else
		{
			// Sphereが深くめりこんでるとき：最も近い面方向
			float dx = half_ext.x - std::fabs(local_center.x);
			float dy = half_ext.y - std::fabs(local_center.y);
			float dz = half_ext.z - std::fabs(local_center.z);

			VECTOR3 local_n(0, 0, 0);
			if (dx <= dy && dx <= dz)
			{// x方向が最小
				local_n = VECTOR3((local_center.x >= 0.0f) ? 1.0f : -1.0f, 0, 0);
			}
			else if (dy <= dz)
			{// y方向が最小
				local_n = VECTOR3(0, (local_center.y >= 0.0f) ? 1.0f : -1.0f, 0);
			}
			else
			{// z方向が最小
				local_n = VECTOR3(0, 0, (local_center.z >= 0.0f) ? 1.0f : -1.0f);
			}

			// local_n はBoxローカルでBoxからSphere方向
			n = rb.MultiplyVector(local_n).GetNormalized();
			dist = 0.0f;
		}

		VECTOR3 tw = sphere_pos - box_pos;
		if (VECTOR3::Dot(n, tw) < 0.0f)
		{
			n = -n;
		}

		// 侵入深さ
		float penetration = radius - dist;
		if (penetration <= 0.0f)
		{
			return false;
		}

		const float max_pen = radius * 2.0f;
		if (penetration > max_pen)
		{
			penetration = max_pen;
		}

		// 接触点はSphere表面上の点
		VECTOR3 contactPos = sphere_pos - n * radius;

		// CONTACT_POINT の作成
		CONTACT_POINT cp{};
		cp.pos = contactPos;
		cp.normal = n;
		cp.penetration = penetration;

		cp.restitution = std::max(box->GetRestitution(), sphere->GetRestitution());
		cp.static_friction = 0.5f * (box->GetStaticFriction() + sphere->GetStaticFriction());
		cp.dynamic_friction = 0.5f * (box->GetDynamicFriction() + sphere->GetDynamicFriction());

		out_contacts.push_back(cp);

		return true;
	}


	//-----------------------------------------------
	// ポリゴンを平面でクリップ
	//-----------------------------------------------
	std::vector<VECTOR3> ClipPolygonToPlane(const VECTOR3* vertices, int count, const VECTOR3& plane_point, const VECTOR3& plane_normal)
	{
		std::vector<VECTOR3> result;
		if (count < 2)
		{// 頂点不足
			return result;
		}

		// 法線の正規化
		VECTOR3 n = plane_normal.GetNormalized();
		const float tol = 1e-5f;

		for (int i = 0; i < count; i++)
		{
			VECTOR3 a = vertices[i];
			VECTOR3 b = vertices[(i + 1) % count];
			float da = VECTOR3::Dot(a - plane_point, n);
			float db = VECTOR3::Dot(b - plane_point, n);

			if (fabsf(da) < tol)
			{
				da = 0.0f;
			}

			if (fabsf(db) < tol)
			{
				db = 0.0f;
			}

			// 頂点の内外判定
			bool a_inside = da <= tol;
			bool b_inside = db <= tol;

			if (a_inside && b_inside)
			{
				// 両方内側
				result.push_back(b);
			}
			else if (a_inside && !b_inside)
			{
				// a内側、b外側
				float t = da / (da - db);
				VECTOR3 p = a + (b - a) * t;
				result.push_back(p);
			}
			else if (!a_inside && b_inside)
			{
				// a外側、b内側
				float t = da / (da - db);
				VECTOR3 p = a + (b - a) * t;
				result.push_back(p);
				result.push_back(b);
			}
		}

		return result;
	}

	// ベクトルの idx 成分を取る（x,y,z）
	inline float Comp(const VECTOR3& v, int idx)
	{
		return (idx == 0) ? v.x : (idx == 1 ? v.y : v.z);
	}

	struct SatResult
	{
		bool hit = false;
		VECTOR3 normal = VECTOR3(0, 1, 0);
		float penetration = 0.0f;

		enum class AxisType { FaceA, FaceB, Edge } type = AxisType::FaceA;
		int i = 0; // FaceA or Edge の A側インデックス
		int j = 0; // FaceB or Edge の B側インデックス
	};

	// rot_a と rot_b の列ベクトル同士の内積表を作る
	inline void BuildDotTables(
		const MATRIX3x3& rot_a,
		const MATRIX3x3& rot_b,
		float dot_a_b[3][3],
		float abs_dot_a_b[3][3])
	{
		const float EPS = 1e-5f;

		for (int i = 0; i < 3; ++i)
		{
			const VECTOR3 ai = rot_a.GetColumn(i);
			for (int j = 0; j < 3; ++j)
			{
				const VECTOR3 bj = rot_b.GetColumn(j);
				const float d = VECTOR3::Dot(ai, bj);
				dot_a_b[i][j] = d;
				abs_dot_a_b[i][j] = std::fabs(d) + EPS; // ほぼ平行での数値安定用
			}
		}
	}

	// より小さい貫通を選ぶ
	inline bool BetterPenetration(float cand, float cur)
	{
		const float kRelTol = 0.05f;
		const float kAbsTol = 1e-3f;
		const float th = std::max(kRelTol * cur, kAbsTol);
		return cand < cur - th;
	}

	// SAT で最小貫通の軸を探す
	static SatResult SatObbObb(
		const MATRIX3x3& rot_a,
		const MATRIX3x3& rot_b,
		const VECTOR3& half_a,
		const VECTOR3& half_b,
		const VECTOR3& pos_a,
		const VECTOR3& pos_b,
		const VECTOR3& to_b,
		const VECTOR3& to_b_in_a,
		const float dot_a_b[3][3],
		const float abs_dot_a_b[3][3])
	{
		SatResult res;
		res.hit = false;

		float min_pen = FLT_MAX;
		VECTOR3 best_axis(0, 1, 0);
		SatResult::AxisType best_type = SatResult::AxisType::FaceA;
		int best_i = 0, best_j = 0;

		// Aの3軸
		for (int i = 0; i < 3; i++)
		{
			// A軸
			const float ra = Comp(half_a, i);

			// B軸
			const float rb =
				half_b.x * abs_dot_a_b[i][0] +
				half_b.y * abs_dot_a_b[i][1] +
				half_b.z * abs_dot_a_b[i][2];

			const float dist = std::fabs(Comp(to_b_in_a, i));
			const float overlap = ra + rb - dist;

			if (overlap < 0.0f)
			{// 分離軸発見
				return res;
			}

			if (BetterPenetration(overlap, min_pen))
			{// 最良更新
				min_pen = overlap;
				best_axis = rot_a.GetColumn(i);
				best_type = SatResult::AxisType::FaceA;
				best_i = i;
			}
		}


		// Bの3軸
		for (int j = 0; j < 3; j++)
		{
			// A軸
			const float ra =
				half_a.x * abs_dot_a_b[0][j] +
				half_a.y * abs_dot_a_b[1][j] +
				half_a.z * abs_dot_a_b[2][j];

			// B軸
			const float rb = Comp(half_b, j);

			const float dist = std::fabs(
				to_b_in_a.x * dot_a_b[0][j] +
				to_b_in_a.y * dot_a_b[1][j] +
				to_b_in_a.z * dot_a_b[2][j]);

			const float overlap = ra + rb - dist;

			if (overlap < 0.0f)
			{// 分離軸発見
				return res;
			}

			if (BetterPenetration(overlap, min_pen))
			{// 最良更新
				min_pen = overlap;
				best_axis = rot_b.GetColumn(j);
				best_type = SatResult::AxisType::FaceB;
				best_j = j;
			}
		}


		// edge × edge 9軸
		const float best_face_pen = min_pen; // 面軸の最良値（edgeを少し不利にする）

		for (int i = 0; i < 3; ++i)
		{
			// A軸
			const VECTOR3 axis_a = rot_a.GetColumn(i);

			for (int j = 0; j < 3; ++j)
			{
				// B軸
				const VECTOR3 axis_b = rot_b.GetColumn(j);

				// 外積軸
				VECTOR3 axis = VECTOR3::Cross(axis_a, axis_b);

				const float len2 = axis.LengthSquared();
				if (len2 < 1e-8f)
				{// ほぼ平行で軸が不安定
					continue;
				}

				// 正規化
				axis = axis / std::sqrt(len2);

				// 投影半径計算
				const float ra =
					half_a.x * std::fabs(VECTOR3::Dot(rot_a.GetColumn(0), axis)) +
					half_a.y * std::fabs(VECTOR3::Dot(rot_a.GetColumn(1), axis)) +
					half_a.z * std::fabs(VECTOR3::Dot(rot_a.GetColumn(2), axis));

				const float rb =
					half_b.x * std::fabs(VECTOR3::Dot(rot_b.GetColumn(0), axis)) +
					half_b.y * std::fabs(VECTOR3::Dot(rot_b.GetColumn(1), axis)) +
					half_b.z * std::fabs(VECTOR3::Dot(rot_b.GetColumn(2), axis));

				const float dist = std::fabs(VECTOR3::Dot(to_b, axis));
				const float overlap = ra + rb - dist;

				if (overlap < 0.0f)
				{// 分離軸発見
					return res;
				}

				
				const float th = std::max(0.10f * best_face_pen, 1e-3f);
				if (overlap + th < min_pen)
				{// edge は面よりちょい不利に（面を優先しやすくする）
					min_pen = overlap;
					best_axis = axis;
					best_type = SatResult::AxisType::Edge;
					best_i = i;
					best_j = j;
				}
			}
		}

		if (VECTOR3::Dot(best_axis, to_b) < 0.0f)
		{// 法線を A->B 方向に揃える
			best_axis = -best_axis;
		}

		// 結果セット
		res.hit = true;
		res.normal = best_axis;
		res.penetration = std::max(min_pen, 0.0f);
		res.type = best_type;
		res.i = best_i;
		res.j = best_j;
		return res;
	}

	//----------------------------------------------
	// 法線がほぼ垂直ならスナップ
	//----------------------------------------------
	inline void SnapNormalIfNearlyVertical(VECTOR3& n)
	{
		const float snapThreshold = 0.98f;
		if (std::fabs(n.y) > snapThreshold &&
			std::fabs(n.x) < 0.1f &&
			std::fabs(n.z) < 0.1f)
		{
			n.x = 0.0f;
			n.z = 0.0f;
			n.y = (n.y > 0.0f) ? 1.0f : -1.0f;
		}
	}

	//-----------------------------------------------
	// 参照面 / 入射面 情報
	//-----------------------------------------------
	struct REF_INC
	{
		const TRANSFORM* ref_t = nullptr;
		const TRANSFORM* inc_t = nullptr;

		MATRIX3x3 ref_r, inc_r;
		VECTOR3 ref_half, inc_half;
		VECTOR3 ref_center, inc_center;

		int ref_axis_index = 0;
		VECTOR3 ref_normal = VECTOR3(0, 1, 0); // 参照面の法線（ref boxの面法線）
		VECTOR3 plane_center = VECTOR3(0, 0, 0); // 参照面上の点 C
	};

	//----------------------------------------------
	// 参照面 / 入射面 情報を作る
	//----------------------------------------------
	static REF_INC BuildReferenceIncident(
		const SatResult& sat,
		const TRANSFORM* ta, const MATRIX3x3& rot_a, const VECTOR3& half_a, const VECTOR3& pos_a,
		const TRANSFORM* tb, const MATRIX3x3& rot_b, const VECTOR3& half_b, const VECTOR3& pos_b,
		const VECTOR3& world_normal) // A->B に揃えた法線
	{
		REF_INC ri;

		// EDGE のときは「より法線に近いフェイスを参照面」にする
		bool ref_is_a = true;
		if (sat.type == SatResult::AxisType::FaceA)
		{
			ref_is_a = true;
		}
		else if (sat.type == SatResult::AxisType::FaceB)
		{
			ref_is_a = false;
		}
		else
		{
			const float da =
				std::max(std::fabs(VECTOR3::Dot(world_normal, rot_a.GetColumn(0))),
					std::max(std::fabs(VECTOR3::Dot(world_normal, rot_a.GetColumn(1))),
						std::fabs(VECTOR3::Dot(world_normal, rot_a.GetColumn(2)))));

			const float db =
				std::max(std::fabs(VECTOR3::Dot(world_normal, rot_b.GetColumn(0))),
					std::max(std::fabs(VECTOR3::Dot(world_normal, rot_b.GetColumn(1))),
						std::fabs(VECTOR3::Dot(world_normal, rot_b.GetColumn(2)))));

			ref_is_a = (da >= db);
		}

		if (ref_is_a)
		{
			ri.ref_t = ta; ri.ref_r = rot_a; ri.ref_half = half_a; ri.ref_center = pos_a;
			ri.inc_t = tb; ri.inc_r = rot_b; ri.inc_half = half_b; ri.inc_center = pos_b;
		}
		else
		{
			ri.ref_t = tb; ri.ref_r = rot_b; ri.ref_half = half_b; ri.ref_center = pos_b;
			ri.inc_t = ta; ri.inc_r = rot_a; ri.inc_half = half_a; ri.inc_center = pos_a;
		}

		// 参照面のローカル軸：world_normal に最も平行な ref の軸
		int ref_axis_index = 0;
		float max_dot = std::fabs(VECTOR3::Dot(world_normal, ri.ref_r.GetColumn(0)));
		for (int i = 1; i < 3; ++i)
		{
			const float d = std::fabs(VECTOR3::Dot(world_normal, ri.ref_r.GetColumn(i)));
			if (d > max_dot)
			{
				max_dot = d;
				ref_axis_index = i;
			}
		}

		ri.ref_axis_index = ref_axis_index;

		const VECTOR3 ref_axis = ri.ref_r.GetColumn(ref_axis_index);
		const float s = (VECTOR3::Dot(ref_axis, world_normal) > 0.0f) ? 1.0f : -1.0f;
		ri.ref_normal = ref_axis * s;

		// 参照面上の点 C（参照ボックスの面中心）
		const float ref_e = Comp(ri.ref_half, ref_axis_index);
		ri.plane_center = ri.ref_center + ri.ref_normal * ref_e;

		return ri;
	}

	// incident face（入射面）の4頂点を作る
	static void BuildIncidentFaceVerts(
		const REF_INC& ri,
		VECTOR3 out_inc_verts4[4],
		VECTOR3& out_inc_normal) // incident face の法線（world）
	{
		// incident box で ref_normal と最も平行な軸を取る（面法線候補）
		int inc_axis_index = 0;
		float best_abs_dot = -FLT_MAX;

		for (int i = 0; i < 3; ++i)
		{
			const float d = VECTOR3::Dot(ri.ref_normal, ri.inc_r.GetColumn(i));
			const float ad = std::fabs(d);
			if (ad > best_abs_dot)
			{
				best_abs_dot = ad;
				inc_axis_index = i;
			}
		}

		VECTOR3 inc_normal = ri.inc_r.GetColumn(inc_axis_index);
		// ref_normal と逆向きになるように
		if (VECTOR3::Dot(inc_normal, ri.ref_normal) > 0.0f)
			inc_normal = -inc_normal;

		out_inc_normal = inc_normal;

		const float inc_e = Comp(ri.inc_half, inc_axis_index);
		const VECTOR3 face_center = ri.inc_center + inc_normal * inc_e;

		const int axis_u = (inc_axis_index + 1) % 3;
		const int axis_v = (inc_axis_index + 2) % 3;

		const VECTOR3 side_u = ri.inc_r.GetColumn(axis_u);
		const VECTOR3 side_v = ri.inc_r.GetColumn(axis_v);

		const float eu = Comp(ri.inc_half, axis_u);
		const float ev = Comp(ri.inc_half, axis_v);

		out_inc_verts4[0] = face_center + side_u * eu + side_v * ev;
		out_inc_verts4[1] = face_center - side_u * eu + side_v * ev;
		out_inc_verts4[2] = face_center - side_u * eu - side_v * ev;
		out_inc_verts4[3] = face_center + side_u * eu - side_v * ev;
	}

	// 参照面（矩形）を構成する4つのサイド平面でポリゴンをクリップ
	static std::vector<VECTOR3> ClipIncidentPolygonToReferenceFace(
		const REF_INC& ri,
		const std::vector<VECTOR3>& poly_in)
	{
		const int iu = (ri.ref_axis_index + 1) % 3;
		const int iv = (ri.ref_axis_index + 2) % 3;

		const VECTOR3 u = ri.ref_r.GetColumn(iu);
		const VECTOR3 v = ri.ref_r.GetColumn(iv);

		const float eu = Comp(ri.ref_half, iu);
		const float ev = Comp(ri.ref_half, iv);

		std::vector<VECTOR3> poly = poly_in;

		// 4サイド平面（内側が da<=0 になる向きで渡す想定：既存 ClipPolygonToPlane と同じ）
		poly = ClipPolygonToPlane(poly.data(), (int)poly.size(), ri.plane_center + u * eu, u);
		poly = ClipPolygonToPlane(poly.data(), (int)poly.size(), ri.plane_center - u * eu, u * -1.0f);
		poly = ClipPolygonToPlane(poly.data(), (int)poly.size(), ri.plane_center + v * ev, v);
		poly = ClipPolygonToPlane(poly.data(), (int)poly.size(), ri.plane_center - v * ev, v * -1.0f);

		return poly;
	}

	// クリップ結果から contact point を生成（最大4）
	static void BuildContactPointsFromClippedPoly(
		const std::vector<VECTOR3>& poly,
		const VECTOR3& out_normal,     // A->B（最終法線）
		const REF_INC& ri,
		float out_penetration,
		std::vector<CONTACT_POINT>& out_points)
	{
		out_points.clear();

		const float dup_eps = 1e-3f;

		for (const auto& p : poly)
		{
			// 参照面の法線方向に見た“奥行き”（>0 なら参照面の外側）
			const float dist = VECTOR3::Dot(p - ri.plane_center, ri.ref_normal);
			if (dist > 0.0f)
				continue; // めり込んでない

			CONTACT_POINT cp{};
			cp.pos = p - ri.ref_normal * dist; // 参照面上に投影
			cp.normal = out_normal;

			float pointPen = -dist;
			pointPen = std::max(0.0f, pointPen);
			pointPen = std::min(pointPen, out_penetration * 1.5f);
			cp.penetration = pointPen;

			bool duplicate = false;
			for (auto& ex : out_points)
			{
				if ((ex.pos - cp.pos).LengthSquared() < dup_eps * dup_eps)
				{
					duplicate = true;
					break;
				}
			}
			if (duplicate) continue;

			out_points.push_back(cp);
			if (out_points.size() >= 4) break;
		}
	}

	bool ComputeBoxBoxContacts(RIGID_BODY* a, BOX_COLLIDER_3D* box_a, RIGID_BODY* b, BOX_COLLIDER_3D* box_b, std::vector<CONTACT_POINT>& out_points, VECTOR3& out_normal, float& out_penetration)
	{
		// トランスフォーム取得
		const TRANSFORM* ta = a->GetOwner()->GetTransform();
		const TRANSFORM* tb = b->GetOwner()->GetTransform();

		// 回転行列取得
		const MATRIX3x3 rot_a = ta->GetRot().ToMatrix3x3();
		const MATRIX3x3 rot_b = tb->GetRot().ToMatrix3x3();

		// 半分サイズ取得
		const VECTOR3 half_a = box_a->GetWorldSize() * 0.5f;
		const VECTOR3 half_b = box_b->GetWorldSize() * 0.5f;

		// 位置取得
		const VECTOR3 pos_a = ta->GetPos();
		const VECTOR3 pos_b = tb->GetPos();

		// A->B ベクトル
		const VECTOR3 to_b = pos_b - pos_a;

		// Aローカルで見たB位置
		const VECTOR3 to_b_in_a = {
			VECTOR3::Dot(to_b, rot_a.GetColumn(0)),
			VECTOR3::Dot(to_b, rot_a.GetColumn(1)),
			VECTOR3::Dot(to_b, rot_a.GetColumn(2))
		};

		// 内積表作成
		float dot_a_b[3][3];
		float abs_dot_a_b[3][3];
		BuildDotTables(rot_a, rot_b, dot_a_b, abs_dot_a_b);

		// SAT判定
		const SatResult sat = SatObbObb(rot_a, rot_b, half_a, half_b, pos_a, pos_b, to_b, to_b_in_a, dot_a_b, abs_dot_a_b);
		if (!sat.hit)
		{// 衝突していない
			return false;
		}

		out_normal = sat.normal;
		out_penetration = sat.penetration;

		// ほぼ垂直ならyスナップ
		SnapNormalIfNearlyVertical(out_normal);

		// 参照面・入射面構築
		const REF_INC ri = BuildReferenceIncident(sat, ta, rot_a, half_a, pos_a, tb, rot_b, half_b, pos_b, out_normal);

		// 入射面の4頂点構築
		VECTOR3 inc_verts4[4];
		VECTOR3 inc_face_normal;
		BuildIncidentFaceVerts(ri, inc_verts4, inc_face_normal);

		// 入射ポリゴンを参照面でクリップ
		std::vector<VECTOR3> poly(inc_verts4, inc_verts4 + 4);
		poly = ClipIncidentPolygonToReferenceFace(ri, poly);

		// 接触点生成
		BuildContactPointsFromClippedPoly(poly, out_normal, ri, out_penetration, out_points);

		// 最低1点保証）
		if (out_points.empty())
		{
			CONTACT_POINT cp{};
			cp.pos = (pos_a + pos_b) * 0.5f - out_normal * (out_penetration * 0.5f);
			cp.normal = out_normal;
			cp.penetration = out_penetration;
			out_points.push_back(cp);
		}

		return true;
	}

	//-----------------------------------------------
	// Box vs Box 衝突判定
	//-----------------------------------------------
	bool CheckCollisionBoxBox(RIGID_BODY* a, RIGID_BODY* b, std::vector<CONTACT_POINT>& out_contacts)
	{
		if (!a || !b)
		{// nullチェック
			return false;
		}

		// コライダー取得
		BOX_COLLIDER_3D* box_a = static_cast<BOX_COLLIDER_3D*>(a->GetCollider());
		BOX_COLLIDER_3D* box_b = static_cast<BOX_COLLIDER_3D*>(b->GetCollider());

		if (!box_a || !box_b)
		{// コライダーが無い場合はスキップ
			return false;
		}

		VECTOR3 normal = VECTOR3(0, 0, 0);
		float penetration = 0.0f;
		std::vector<CONTACT_POINT> points;

		if (!ComputeBoxBoxContacts(a, box_a, b, box_b, points, normal, penetration))
		{// 衝突していない
			return false;
		}

		for (auto& p : points)
		{// CONTACT_POINT の作成
			p.normal = normal;
			p.restitution = std::max(a->GetRestitution(), b->GetRestitution());
			p.static_friction = (a->GetStaticFriction() + b->GetStaticFriction()) * 0.5f;
			p.dynamic_friction = (a->GetDynamicFriction() + b->GetDynamicFriction()) * 0.5f;
			out_contacts.push_back(p);
		}

		return !out_contacts.empty();
	}
}

//-----------------------------------------------
// 衝突点の生成
//-----------------------------------------------
bool PHYSICS_NARROW_PHASE_3D::GenerateContacts(RIGID_BODY* a, RIGID_BODY* b, std::vector<CONTACT_POINT>& out_contacts)
{
	out_contacts.clear();
	if (!a || !b)
	{// nullチェック
		return false;
	}

	if (!a->GetCollider() || !b->GetCollider())
	{// コライダーが無い場合はスキップ
		return false;
	}

	// コライダータイプ取得
	COLLIDER_TYPE_3D type_a = a->GetCollider()->GetColliderType();
	COLLIDER_TYPE_3D type_b = b->GetCollider()->GetColliderType();

	bool collided = false;

	if (type_a == COLLIDER_TYPE_3D::SPHERE && type_b == COLLIDER_TYPE_3D::SPHERE)
	{// Sphere vs Sphere
		collided = CheckCollisionSphereSphere(a, b, out_contacts);
	}

	else if (type_a == COLLIDER_TYPE_3D::BOX && type_b == COLLIDER_TYPE_3D::BOX)
	{// Box vs Box
		collided = CheckCollisionBoxBox(a, b, out_contacts);
	}

	else if ((type_a == COLLIDER_TYPE_3D::SPHERE && type_b == COLLIDER_TYPE_3D::BOX) || (type_a == COLLIDER_TYPE_3D::BOX && type_b == COLLIDER_TYPE_3D::SPHERE))
	{// Sphere vs Box
		RIGID_BODY* boxRb = (type_a == COLLIDER_TYPE_3D::BOX) ? a : b;
		RIGID_BODY* sphereRb = (type_a == COLLIDER_TYPE_3D::SPHERE) ? a : b;
		collided = CheckCollisionSphereBox(sphereRb, boxRb, out_contacts);
	}

	return collided && !out_contacts.empty();
}
