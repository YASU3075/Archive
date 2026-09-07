#pragma once


#include "main.h"
#include "renderer.h"


//線分と無限平面との交差判定
bool	CollisionLineFace(XMFLOAT3 Lp0, XMFLOAT3 Lp1, XMFLOAT3* FacePos, XMFLOAT3 FaceNormal);

//線分とポリゴン（三角形）との交叉判定
bool	CollisionLinePolygon(XMFLOAT3 Lp0, XMFLOAT3 Lp1, XMFLOAT3* VPos, XMFLOAT3 FaceNormal);

