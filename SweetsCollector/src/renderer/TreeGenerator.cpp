//-----------------------------------------------
// TreeGenerator.cpp
// 制作日：2025/12/17
// 制作者：安田晴人
//-----------------------------------------------
#include "main.h"
#include "renderer/TreeGenerator.h"
#include "renderer/MeshFactory.h"
#include "math/PerlinNoise3D.h"
#include <stack>

//-----------------------------------------------
// メッシュに変換行列を適用する関数
//-----------------------------------------------
void TREE_GENERATOR::ApplyTransform(MESH_RENDERER_DATA& mesh, const XMMATRIX& transform)
{
    for (auto& v : mesh.vertices)
    {
        XMVECTOR pos = XMLoadFloat3(&v.Position);
        pos = XMVector3Transform(pos, transform);
        XMStoreFloat3(&v.Position, pos);

        XMVECTOR norm = XMLoadFloat3(&v.Normal);
        norm = XMVector3TransformNormal(norm, transform);
        norm = XMVector3Normalize(norm);
        XMStoreFloat3(&v.Normal, norm);
    }
}

//-----------------------------------------------
// 木を生成する関数
//-----------------------------------------------
MESH_RENDERER_DATA TREE_GENERATOR::GenerateTree(int n)
{
    std::string str = m_lsystem.Generate(n); // n回生成
    std::stack<TURTLE> stack;

    float branch_length = 0.5f;
    float branch_radius = 0.1f;

    TURTLE turtle;
    turtle.transform = XMMatrixIdentity();
    turtle.position = XMVectorZero();
    turtle.branch_length = branch_length;
    turtle.branch_radius = branch_radius;

    m_branches.clear();
    m_leaves.clear();

    MESH_RENDERER_DATA result;
    unsigned int offset = 0;

    PERLIN_NOISE_3D perlin(12345);

    for (char c : str)
    {
        if (c == 'F')
        {
            // ローカルY軸を向きベクトルに変換（長さは1.0のまま）
            XMVECTOR dir = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), turtle.transform);
            dir = XMVector3Normalize(dir);

            // 始点と終点
            XMVECTOR start = turtle.position;
            XMVECTOR end = XMVectorAdd(turtle.position, XMVectorScale(dir, turtle.branch_length));

            // 中点
            XMVECTOR mid = XMVectorScale(XMVectorAdd(start, end), 0.5f);

            // 回転行列 (Y軸 → dir)
            XMVECTOR up = XMVectorSet(0, 1, 0, 0);
            float dot = XMVectorGetX(XMVector3Dot(up, dir));
            XMVECTOR axis = XMVector3Cross(up, dir);
            float len = XMVectorGetX(XMVector3Length(axis));
            XMMATRIX rot = XMMatrixIdentity();
            if (len > 1e-6f)
            {
                rot = XMMatrixRotationAxis(axis, acosf(dot));
            }

            // スケーリング（高さ branch_length）
            XMMATRIX scale = XMMatrixScaling(turtle.branch_radius, turtle.branch_length, turtle.branch_radius);

            // 平行移動（中心ではなく start → end に揃える）
            XMMATRIX trans = XMMatrixTranslationFromVector(XMVectorAdd(start, XMVectorScale(dir, turtle.branch_length * 0.5f)));

            // 合成
            XMMATRIX world = scale * rot * trans;

            // Cylinder生成（高さ1.0を想定）
            MESH_RENDERER_DATA cyl = MESH_FACTORY::CreateCylinderMesh(1.0f, 1.0f, 8);
            for (auto& v : cyl.vertices)
            {
                v.Diffuse = XMFLOAT4(0.55f, 0.27f, 0.07f, 1.0f);
            }
            ApplyTransform(cyl, world);

            result.vertices.insert(result.vertices.end(), cyl.vertices.begin(), cyl.vertices.end());
            for (auto idx : cyl.indices)
                result.indices.push_back(idx + offset);
            offset += static_cast<unsigned int>(cyl.vertices.size());

            // 次の位置
            turtle.position = end;
        }
        else if (c == 'L')
        {
            // 葉
            MESH_RENDERER_DATA leaf = MESH_FACTORY::CreateSphereMesh (0.5f, 6, 6);
            for (auto& v : leaf.vertices)
            {
                // 乱数で葉の形を少し変える
                XMVECTOR pos = XMLoadFloat3(&v.Position);
                float noise = perlin.Noise(XMVectorGetX(pos) * 0.5f, XMVectorGetY(pos) * 0.5f, XMVectorGetZ(pos) * 0.5f);
                pos = XMVectorAdd(pos, XMVectorSet(noise * 0.1f, noise * 0.1f, noise * 0.1f, 0.0f));
                XMStoreFloat3(&v.Position, pos);

                v.Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
            }
            
            XMMATRIX world = turtle.transform * XMMatrixTranslationFromVector(turtle.position);
            ApplyTransform(leaf, world);

            result.vertices.insert(result.vertices.end(), leaf.vertices.begin(), leaf.vertices.end());
            for (auto idx : leaf.indices)
                result.indices.push_back(idx + offset);
            offset += static_cast<unsigned int>(leaf.vertices.size());
        }
        else if (c == '+')
        {
            turtle.transform *= XMMatrixRotationY(XMConvertToRadians(25.0f));
        }
        else if (c == '-')
        {
            turtle.transform *= XMMatrixRotationY(XMConvertToRadians(-25.0f));
        }
        else if (c == '&')
        {
            turtle.transform *= XMMatrixRotationX(XMConvertToRadians(25.0f));
        }
        else if (c == '^')
        {
            turtle.transform *= XMMatrixRotationX(XMConvertToRadians(-25.0f));
        }
        else if (c == '\\')
        {
            turtle.transform *= XMMatrixRotationZ(XMConvertToRadians(25.0f));
        }
        else if (c == '/')
        {
            turtle.transform *= XMMatrixRotationZ(XMConvertToRadians(-25.0f));
        }
        else if (c == '[')
        {
            // 状態を保存
            stack.push(turtle);
        }
        else if (c == ']')
        {
            if (!stack.empty())
            {
                // 葉クラスタ
				MESH_RENDERER_DATA leafCluster = MESH_FACTORY::CreateSphereMesh(0.3f, 6, 6);
                for (auto& v : leafCluster.vertices)
                {
                    // 乱数で葉の形を少し変える
                    XMVECTOR pos = XMLoadFloat3(&v.Position);
                    float noise = perlin.Noise(XMVectorGetX(pos) * 0.5f, XMVectorGetY(pos) * 0.5f, XMVectorGetZ(pos) * 0.5f);
                    pos = XMVectorAdd(pos, XMVectorSet(noise, noise, noise, 0.0f));
                    XMStoreFloat3(&v.Position, pos);

                    v.Diffuse = XMFLOAT4(0.0f, 0.8f + 0.2f * (rand() % 100 / 100.0f), 0.0f, 1.0f);
                }

                XMMATRIX world = turtle.transform * XMMatrixTranslationFromVector(turtle.position);
                ApplyTransform(leafCluster, world);

                result.vertices.insert(result.vertices.end(), leafCluster.vertices.begin(), leafCluster.vertices.end());
                for (auto idx : leafCluster.indices)
                    result.indices.push_back(idx + offset);
                offset += static_cast<unsigned int>(leafCluster.vertices.size());

                // 復元
                turtle = stack.top();
                stack.pop();

                // 枝を細く
                turtle.branch_radius *= 0.8f;
            }
        }
    }

    return result;
}