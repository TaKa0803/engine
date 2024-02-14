#include "MT4Scene.h"
#include<imgui.h>

void DrawQuaternion(const char* name,const Quaternion&q) {

	ImGui::Begin(name);
	ImGui::Text("%4.2f,%4.2f,%4.2f,%4.2f",q.x,q.y,q.z,q.w);
	ImGui::End();
}



void MT4Scene::Initialize() {

}

void MT4Scene::Update() {

	

	DrawQuaternion("rotation", rotation);
	ImGui::Begin("math");
	ImGui::Text("rotateMatrix");
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f",rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);

	
	ImGui::Text("%4.2f %4.2f %4.2f : rotateByQuaternion", rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z);
	ImGui::Text("%4.2f %4.2f %4.2f : rotatebyMatrix", rotateByMatrix.x, rotateByMatrix.y, rotateByMatrix.z);

	ImGui::End();

}

void MT4Scene::Draw() {

}

void MT4Scene::Finalize() {

}
