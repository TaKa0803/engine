#include "IScene.h"

//初期シーンの設定
int IScene::sceneNo = TITLE;

IScene::~IScene() {}

int IScene::GetSceneNo() { return sceneNo; }