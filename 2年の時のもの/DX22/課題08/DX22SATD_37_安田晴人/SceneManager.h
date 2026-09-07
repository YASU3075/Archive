//-----------------------------------------------
// SceneManager.h
// 制作日：2024/12/10
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

//-----------------------------------------------
// 列挙体宣言
//-----------------------------------------------
enum SCENE
{
    SCENE_TITLE = 0,
    SCENE_GAME,
    SCENE_RESULT,
    SCENE_MAX,
};

class SCENE_MANAGER
{
private:
    class BASE_SCENE* m_p_current_scene = nullptr;
public:
    SCENE_MANAGER() {}         //コンストラクタ
    ~SCENE_MANAGER() {}        //デストラクタ

    //初期化関数
    void Init(void);
    //終了処理
    void Uninit(void);
    //更新処理
    void Update(void);
    //描画処理
    void Draw(void);

    void ChangeScene(SCENE scene);
};