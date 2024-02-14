#include"IScene/IScene.h"
#include"Scenes/Scenes.h"

int IScene::sceneNo = TITLE;

bool IScene::leaveGame = false;

IScene::~IScene() {}

int IScene::GetSceneNo() { return sceneNo; }

void IScene::SetSceneNo(int num) { sceneNo = num; }
