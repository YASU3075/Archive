


#include "CollisionEx.h"




//線分と無限平面との当たり
//
//p0:線分の端点0
//p1:線分の端点1
//FacePos:面上の任意の点（頂点座標）
//FaceNormal:面の法線（ワールド変換済）
//
bool	CollisionLineFace(XMFLOAT3 Lp0, XMFLOAT3 Lp1, XMFLOAT3* FacePos, XMFLOAT3 FaceNormal)
{
	//ポリゴンの頂点から線分の端点へのベクトル
	Lp0.x -= FacePos[0].x;
	Lp0.y -= FacePos[0].y;
	Lp0.z -= FacePos[0].z;
	//Lp0.x -= FacePos->x;
	//Lp0.y -= FacePos->y;
	//Lp0.z -= FacePos->z;

	Lp1.x -= FacePos->x;
	Lp1.y -= FacePos->y;
	Lp1.z -= FacePos->z;

	XMVECTOR	pos0 = XMLoadFloat3(&Lp0);
	XMVECTOR	pos1 = XMLoadFloat3(&Lp1);
	XMVECTOR	norm = XMLoadFloat3(&FaceNormal);
	XMVECTOR	dot0, dot1;

	//ベクトルと面の法線との内積を計算
	dot0 = XMVector3Dot(pos0, norm);
	dot1 = XMVector3Dot(pos1, norm);
	XMFLOAT3	d0, d1;
	XMStoreFloat3(&d0, dot0);
	XMStoreFloat3(&d1, dot1);

	//交叉判定
	if ((d0.x * d1.x) <= 0.0f)
	{
		return true;
	}

	return false;
}


//線分とポリゴン平面との当たり
//p0:線分の端点0
//p1:線分の端点1
//VPos:ポリゴンの頂点座標配列
//FaceNormal:面の法線
bool	CollisionLinePolygon(XMFLOAT3 Lp0, XMFLOAT3 Lp1, XMFLOAT3* VPos, XMFLOAT3 FaceNormal)
{
	if (CollisionLineFace( Lp0, Lp1, VPos, FaceNormal))//面と当たっている可能性があるか？
	{
		XMFLOAT3	p;
		XMVECTOR	l;
		XMVECTOR	norm = XMLoadFloat3(&FaceNormal);

		//p0と面との最短距離len0を求める
		p.x = Lp0.x - VPos[0].x;
		p.y = Lp0.y - VPos[0].y;
		p.z = Lp0.z - VPos[0].z;
		XMVECTOR	v = XMLoadFloat3(&p);
		l = XMVector3Dot(v, norm);
		float len0 = fabsf(l.m128_f32[0]);


		//p1と面との最短距離len1を求める
		p.x = Lp1.x - VPos[0].x;
		p.y = Lp1.y - VPos[0].y;
		p.z = Lp1.z - VPos[0].z;
		v = XMLoadFloat3(&p);
		l = XMVector3Dot(v, norm);
		float len1 = fabsf(l.m128_f32[0]);

		//内分比を求める
		float	nai = len0 / (len0 + len1);

		//線分の貫通点の座標を求める
		XMFLOAT3	Pos;
		Pos.x = ((1.0f - nai) * Lp0.x) + (nai * Lp1.x);
		Pos.y = ((1.0f - nai) * Lp0.y) + (nai * Lp1.y);
		Pos.z = ((1.0f - nai) * Lp0.z) + (nai * Lp1.z);


		//ポリゴン頂点同士のベクトル
		XMFLOAT3	vv[3];
		vv[0].x = VPos[1].x - VPos[0].x;//V0->V1
		vv[0].y = VPos[1].y - VPos[0].y;
		vv[0].z = VPos[1].z - VPos[0].z;

		vv[1].x = VPos[2].x - VPos[1].x;//V1->V2
		vv[1].y = VPos[2].y - VPos[1].y;
		vv[1].z = VPos[2].z - VPos[1].z;

		vv[2].x = VPos[0].x - VPos[2].x;//V2->V0
		vv[2].y = VPos[0].y - VPos[2].y;
		vv[2].z = VPos[0].z - VPos[2].z;

		//各頂点から貫通点へのベクトル
		XMFLOAT3	pp[3];
		pp[0].x = Pos.x - VPos[1].x;//V1->Pos
		pp[0].y = Pos.y - VPos[1].y;
		pp[0].z = Pos.z - VPos[1].z;

		pp[1].x = Pos.x - VPos[2].x;//V2->Pos
		pp[1].y = Pos.y - VPos[2].y;
		pp[1].z = Pos.z - VPos[2].z;

		pp[2].x = Pos.x - VPos[0].x;//V0->Pos
		pp[2].y = Pos.y - VPos[0].y;
		pp[2].z = Pos.z - VPos[0].z;

		//外積を3つ求める
		XMVECTOR	dot[3];
		for (int i = 0; i < 3; i++)
		{
			XMVECTOR	cross[3];
			XMVECTOR	v0, v1;
			v0 = XMLoadFloat3(&vv[i]);
			v1 = XMLoadFloat3(&pp[i]);
			cross[i] = XMVector3Normalize( XMVector3Cross(v0, v1) );
			dot[i] = XMVector3Dot(norm, cross[i]);
			//ポリゴンの辺に乗っている場合、
			//外積でエラーが出て内積が0になるので特殊処理
			if ((dot[i].m128_f32[0] >= -0.001f) && (dot[i].m128_f32[0] <= 0.001f))
			{
				dot[i].m128_f32[0] = 1.0f;
			}
		}
		//全ての内積値が1.0(法線と各ベクトルが全て同じ向き)の場合は当たり（誤差考慮版）
		if ((dot[0].m128_f32[0] >= 0.990f) && (dot[1].m128_f32[0] >= 0.990f) && (dot[2].m128_f32[0] >= 0.990f))
		{
			return true;
		}

	}

	return false;


}









