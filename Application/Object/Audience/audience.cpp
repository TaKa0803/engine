#include "audience.h"

Audience::Audience()
{
	InstancingGameObject::Initialize("parrot");

#pragma region 観客
	h_ = std::make_unique<InstancingGameObject>();
	B_ = std::make_unique<InstancingGameObject>();
	LA_ = std::make_unique<InstancingGameObject>();
	RA_ = std::make_unique<InstancingGameObject>();

	
	
#pragma endregion
	h_->Initialize("aH");
	B_->Initialize("aB");
	LA_->Initialize("aLA");
	RA_->Initialize("aRA");

}

Audience::~Audience()
{
}

void Audience::Initialize()
{

	animeData[NORMAL].parts[HEAD].pos.st = { 0,0.57f,0 };
	animeData[NORMAL].parts[HEAD].pos.ed = { 0,0.57f,0 };
	animeData[NORMAL].parts[BODY].pos.st;
	animeData[NORMAL].parts[BODY].pos.ed;
	animeData[NORMAL].parts[LARM].pos.st = { -1.0f,0.1f,0, };
	animeData[NORMAL].parts[LARM].pos.ed = { -1.0f,-0.1f,0, };
	animeData[NORMAL].parts[RARM].pos.st = { 1.0f,0.1f,0, };
	animeData[NORMAL].parts[RARM].pos.ed = { 1.0f,-0.1f,0, };

	animeData[GOOD].parts[HEAD].pos.st = { 0,0.57f,0 };
	animeData[GOOD].parts[HEAD].pos.ed = { 0,0.57f,0 };
	animeData[GOOD].parts[BODY].pos.st = { 0,0.1f,0 };
	animeData[GOOD].parts[BODY].pos.ed = { 0,0.1f,0 };
	animeData[GOOD].parts[BODY].rotate.st = { 0,0,0.2f };
	animeData[GOOD].parts[BODY].rotate.ed = { 0,0,-0.2f };
	animeData[GOOD].parts[LARM].pos.st = { -1.0f,1.2f,0, };
	animeData[GOOD].parts[LARM].pos.ed = { -1.0f,1.2f,0, };
	animeData[GOOD].parts[RARM].pos.st = { 1.0f,1.2f,0, };
	animeData[GOOD].parts[RARM].pos.ed = { 1.0f,1.2f,0, };

	animeData[BAD].parts[HEAD].pos.st = { 0,0.57f,0 };
	animeData[BAD].parts[HEAD].pos.ed = { 0,0.57f,0 };
	animeData[BAD].parts[HEAD].rotate.st = { 0.5f,0,0 };
	animeData[BAD].parts[HEAD].rotate.ed = { 0.4f,0,0 };
	animeData[BAD].parts[BODY].pos.st;
	animeData[BAD].parts[BODY].pos.ed;
	animeData[BAD].parts[BODY].rotate.st;
	animeData[BAD].parts[BODY].rotate.ed;
	animeData[BAD].parts[LARM].pos.st = { -0.6f,-0.22f,0, };
	animeData[BAD].parts[LARM].pos.ed = { -0.7f,-0.07f,0, };
	animeData[BAD].parts[RARM].pos.st = { 0.6f,-0.22f,0, };
	animeData[BAD].parts[RARM].pos.ed = { 0.7f,-0.07f,0, };

	nowAnime = animeData[NORMAL];
}

void Audience::Update()
{
	animeData[NORMAL].parts[HEAD].pos.st = { 0,0.57f,0 };
	animeData[NORMAL].parts[HEAD].pos.ed = { 0,0.57f,0 };
	animeData[NORMAL].parts[BODY].pos.st;
	animeData[NORMAL].parts[BODY].pos.ed;
	animeData[NORMAL].parts[LARM].pos.st = { -1.0f,0.1f,0, };
	animeData[NORMAL].parts[LARM].pos.ed = { -1.0f,-0.1f,0, };
	animeData[NORMAL].parts[RARM].pos.st = { 1.0f,0.1f,0, };
	animeData[NORMAL].parts[RARM].pos.ed = { 1.0f,-0.1f,0, };

	animeData[GOOD].parts[HEAD].pos.st = { 0,0.57f,0 };
	animeData[GOOD].parts[HEAD].pos.ed = { 0,0.57f,0 };
	animeData[GOOD].parts[BODY].pos.st = { 0,0.1f,0 };
	animeData[GOOD].parts[BODY].pos.ed = { 0,0.1f,0 };
	animeData[GOOD].parts[BODY].rotate.st = { 0,0,0.2f };
	animeData[GOOD].parts[BODY].rotate.ed = { 0,0,-0.2f };
	animeData[GOOD].parts[LARM].pos.st = { -1.0f,1.2f,0, };
	animeData[GOOD].parts[LARM].pos.ed = { -1.0f,1.2f,0, };
	animeData[GOOD].parts[RARM].pos.st = { 1.0f,1.2f,0, };
	animeData[GOOD].parts[RARM].pos.ed = { 1.0f,1.2f,0, };

	animeData[BAD].parts[HEAD].pos.st = { 0,0.57f,0 };
	animeData[BAD].parts[HEAD].pos.ed = { 0,0.57f,0 };
	animeData[BAD].parts[HEAD].rotate.st = { 0.5f,0,0 };
	animeData[BAD].parts[HEAD].rotate.ed = { 0.4f,0,0 };
	animeData[BAD].parts[BODY].pos.st;
	animeData[BAD].parts[BODY].pos.ed;
	animeData[BAD].parts[BODY].rotate.st;
	animeData[BAD].parts[BODY].rotate.ed;
	animeData[BAD].parts[LARM].pos.st = { -0.6f,-0.22f,0, };
	animeData[BAD].parts[LARM].pos.ed = { -0.7f,-0.07f,0, };
	animeData[BAD].parts[RARM].pos.st = { 0.6f,-0.22f,0, };
	animeData[BAD].parts[RARM].pos.ed = { 0.7f,-0.07f,0, };


	B_->SetParent(&world_);
	h_->SetParent(&B_->GetWorld());
	LA_->SetParent(&B_->GetWorld());
	RA_->SetParent(&B_->GetWorld());

	B_->SetTranslate({});
	h_->SetTranslate({ 0,0.57f,0 });
	LA_->SetTranslate({ -1.0f,0,0, });
	RA_->SetTranslate({ 1.0f,0,0, });

	Animation();

	h_->SetTranslate(nowAnime.pos[HEAD]);
	h_->SetRotate(nowAnime.rotate[HEAD]);

	B_->SetTranslate(nowAnime.pos[BODY]);
	B_->SetRotate(nowAnime.rotate[BODY]);

	LA_->SetTranslate(nowAnime.pos[LARM]);
	LA_->SetRotate(nowAnime.rotate[LARM]);

	RA_->SetTranslate(nowAnime.pos[RARM]);
	RA_->SetRotate(nowAnime.rotate[RARM]);


	world_.UpdateMatrix();

	B_->Update();
	h_->Update();
	LA_->Update();
	RA_->Update();
}

void Audience::Draw()
{
	h_->Draw();
	B_->Draw();
	LA_->Draw();
	RA_->Draw();
}

void Audience::Debug(const char* name)
{
	//world_.DrawDebug(name); 
	h_->Debug("aH");
	B_->Debug("aB");
	LA_->Debug("aLA");
	RA_->Debug("aRA");

}

void Audience::Animation()
{
	float t = float(nowAnime.count) / float(nowAnime.maxCount);
	if (t > 1.0f) {
		t = 1.0f;
	}

	//すべてのパーツのデータを代入した
	int count = 0;
	for (auto& p : nowAnime.pos) {

		p = Esing(nowAnime.parts[count].pos.st, nowAnime.parts[count].pos.ed, t);
		nowAnime.rotate[count] = Esing(nowAnime.parts[count].rotate.st, nowAnime.parts[count].rotate.ed, t);
		count++;
	}


	if (nowAnime.count++ >= nowAnime.maxCount) {
		nowAnime.count = 0;
		for (int i = 0; i < _count; i++) {
			std::swap(nowAnime.parts[i].pos.st, nowAnime.parts[i].pos.ed);
			std::swap(nowAnime.parts[i].rotate.st, nowAnime.parts[i].rotate.ed);
		}
	}

}
