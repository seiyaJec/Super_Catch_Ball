//-------------------------------------------------------------------
//制作者
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_Creator.h"

namespace  Creator
{
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// ゲームデータ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
	int Data::myNum = BData::NODATA;
	//-------------------------------------------------------------------
	
	//ゲームモードを読み込む
	json Data::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//ファイルパス
			std::ifstream fin("none");
			if (!fin) { return json(); }
			//JSONファイル読み込み
			fin >> j;
			//ファイル読み込み終了
			fin.close();
		}
		else {
			j = ge->gameData["data"][this->myArg];
		}
		//----------------------------------
		//★データ読み込み処理

		//例）pos = j[キーの名前]

		//----------------------------------
		//（★★使用する場合はmyPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 

		return j;
	}

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// リソース
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//データクラスから情報受け取り
		this->data = Data::UseData();

		//★データ初期化


		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		return true;
	}

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->targetSampleSideR = ML::Model3D::Create("./data/mesh/target.mv1");
		this->targetSampleSideL = ML::Model3DClone::Create(this->targetSampleSideR);
		this->targetSampleMoveRot = ML::Model3DClone::Create(this->targetSampleSideR);
		this->sampleColorSide = ML::Color(0.5f, 0.5f, 0.5f, 1);
		this->sampleColorRot = ML::Color(1, 1, 0.5f, 0.5f);
		this->targetSampleSideR->SetDiffuse(this->sampleColorSide);
		this->targetSampleSideL->SetDiffuse(this->sampleColorSide);
		this->targetSampleMoveRot->SetDiffuse(this->sampleColorRot);

		this->ptHeight = 500;
		this->moveSpeed = 100;
		this->movingTarget = nullptr;
		this->myCamera = ge->cameras[0];
		this->hitBaseMs.Resize(ML::Size3(100, 100, 100));
		this->moveArea = ML::Rect2(0, (int)ge->screenWidth / 2, 0, ge->screenHeight);
		this->tgMoveTime = 30;
		this->msBtOnCnt = 0;


		this->defaultTgMoveSpeed = 1;
		this->defaultTgMoveScale = 1000;
		this->defaultTgAddRot = 0;


		this->SetCamera();
		//★タスクの生成
		TargetInfo::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (ge->inpms.On(MOUSE_INPUT_1)) {
			++this->msBtOnCnt;
		}
		else {
			this->msBtOnCnt = 0;
		}


		this->Move();
		this->TargetLinkToMouse();
		this->LinkToInfo();
		this->SetCamera();

		if (ge->inpkey.Down(KEY_INPUT_X)) {
			this->SaveToJsonFile();
		}
	}
	//-------------------------------------------------------------------
	//「３Ｄ描画」１フレーム毎に行う処理、レイヤー0
	void Object::Render3D_L0()
	{
		ML::DrawBox3D(this->CallHitBoxMs(), ML::Color(1, 0, 0, 1));
		this->DrawSample();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理、3D描画後に行われる
	void  Object::Render2D_AF()
	{
		ML::Vec2 msPos = ge->inpms.getPos();	//マウス座標
		ML::Vec3 scPos(msPos.x, msPos.y, 0);	//スクリーン座標

		VECTOR RaySt = DxLib::ConvScreenPosToWorldPos(scPos);
		scPos.z = 1.0f;
		VECTOR RayEn = DxLib::ConvScreenPosToWorldPos(scPos);

		ML::Vec3 RayVec(
			RayEn.x - RaySt.x,
			RayEn.y - RaySt.y,
			RayEn.z - RaySt.z
		);
		RayVec = RayVec.Norm();

		auto newRay = RayVec* (this->pos.y / -RayVec.y);
		ge->debugFont->Draw(ML::Vec2(10, 10), newRay.DebugPrint().c_str());
	}
	//-------------------------------------------------------------------
	// 関数定義
	//-------------------------------------------------------------------
	// 当たり判定呼び出し
	ML::Obb3 Object::CallHitBoxMs() const {
		return this->hitBaseMs.OffsetCopy(this->ptPos);
	}
	//--------------------------------------
	//的移動可能範囲にカーソルがあるか
	bool Object::IsMouseInMoveArea() {
		ML::Vec2 msPos = ge->inpms.getPos();
		return
			msPos.x >= this->moveArea.l
			&& msPos.x <= this->moveArea.r
			&& msPos.y >= this->moveArea.t
			&& msPos.y <= this->moveArea.b;
	}
	//--------------------------------------
	//移動
	void Object::Move() {
		//前
		if (ge->inpkey.On(KEY_INPUT_W)) {
			this->pos.z += this->moveSpeed;
		}
		//後ろ
		if (ge->inpkey.On(KEY_INPUT_S)) {
			this->pos.z -= this->moveSpeed;
		}
		//右
		if (ge->inpkey.On(KEY_INPUT_D)) {
			this->pos.x += this->moveSpeed;
		}
		//左
		if (ge->inpkey.On(KEY_INPUT_A)) {
			this->pos.x -= this->moveSpeed;
		}

		//上下
		this->pos.y += ge->inpms.GetWheelRotVol() * 100;

	}
	//--------------------------------------
	//カメラ設定
	void Object::SetCamera() {
		//位置
		this->myCamera->pos = this->pos;
		//向き
		this->myCamera->target.x = ge->cameras[0]->pos.x;
		this->myCamera->target.y = 0;
		this->myCamera->target.z = ge->cameras[0]->pos.z;
		//上方向
		this->myCamera->up = ML::Vec3(0, 0, 1);
	}
	//--------------------------------------
	// 的を生成する
	void Object::TargetSetting() {
		//マウスが移動可能範囲になければ処理を行わない
		if (this->IsMouseInMoveArea() == false) {
			auto targets = ge->GetTasks<Target::Object>("的");
			for (auto& tg : *targets) {
				tg->SetSelecting(false);
				tg->SetMoving(false);
			}
			return;
		}

		bool isMouseDown = ge->inpms.Down(MOUSE_INPUT_1);
		bool isDelete = ge->inpms.Off(MOUSE_INPUT_1) && ge->inpms.On(MOUSE_INPUT_2);
		
		bool foundTarget = false;
		
		auto targets = ge->GetTasks<Target::Object>("的");
		for (auto& tg : *targets) {
			tg->SetSelecting(false);
			tg->SetMoving(false);

			//的を選択し、マウスのボタンが押されたら
			if (ML::CheckHit(this->CallHitBoxMs(), tg->CallHitBox())) {
				if (isMouseDown == true) {
					//選択する的を更新
					this->movingTarget = tg;
					foundTarget = true;
					break;
				}
				else if (isDelete == true) {
					//的を削除
					this->movingTarget.reset();
					tg->Kill();
					foundTarget = true;
				}
			}
		}
		
		//マウスのボタンが押され、的を選択していなかったら新しく生成
		if (isMouseDown == true) {
			if (foundTarget == false) {
				this->movingTarget = Target::Object::Create(true);
				this->movingTarget->basePos = this->ptPos;
				this->movingTarget->pos = this->ptPos;
				this->movingTarget->moveSpeed = this->defaultTgMoveSpeed;
				this->movingTarget->moveScaleW = this->defaultTgMoveScale;
				this->movingTarget->addRotate = this->defaultTgAddRot;
			}
		}

		//的を選択中なら的のモードを変える
		if (this->movingTarget) {
			this->movingTarget->SetSelecting(true);
		}

	}
	//--------------------------------------
	//的をマウスの位置に合わせる
	void Object::TargetLinkToMouse() {
		ML::Vec3 RayVec = ge->inpms.GetRayNormOfScreen(this->myCamera, ge->screenCenter().y);
		this->ptPos = this->pos + RayVec * ((this->pos.y - this->ptHeight) / -RayVec.y);

		//的の生成・移動
		this->TargetSetting();

		//的を移動中でなければ行わない
		if(!this->movingTarget){
			return;
		}

		//マウスが押されている間は移動
		if (this->msBtOnCnt >= this->tgMoveTime) {
			if (this->IsMouseInMoveArea() == true) {
				this->movingTarget->SetMoving(true);
				this->movingTarget->basePos = this->ptPos;
			}
		}
	}
	//--------------------------------------
	//的の情報を表示用タスクに送る
	void Object::LinkToInfo() {
		auto info = ge->GetTask<TargetInfo::Object>("的設定画面");
		info->SetViewingTarget(this->movingTarget);
	}
	//--------------------------------------
	//回転分移動させた的を表示
	void Object::DrawSample() {
		if (this->movingTarget) {
			ML::Vec3 addPos = this->movingTarget->basePos;
			addPos.x += cosf(ML::ToRadian(0)) * this->movingTarget->moveScaleW;
			this->targetSampleSideR->DrawStd(addPos, this->movingTarget->angle + this->movingTarget->res->data->modelAngleBase, ML::Vec3(0.9f, 0.9f, 0.9f));
			addPos = this->movingTarget->basePos;
			addPos.x += cosf(ML::ToRadian(180)) * this->movingTarget->moveScaleW;
			this->targetSampleSideL->DrawStd(addPos, this->movingTarget->angle + this->movingTarget->res->data->modelAngleBase, ML::Vec3(0.9f, 0.9f, 0.9f));
			addPos = this->movingTarget->basePos;
			addPos.x += cosf(ML::ToRadian(this->movingTarget->addRotate)) * this->movingTarget->moveScaleW;
			this->targetSampleMoveRot->DrawStd(addPos, this->movingTarget->angle + this->movingTarget->res->data->modelAngleBase, ML::Vec3(1.1f, 1.1f, 1.1f));
		}
	}
	//--------------------------------------
	//JSONファイルに保存
	void Object::SaveToJsonFile() {
		json AllgenDatas;	//ファイルに保存するjsonデータ

		//生成されている的を全てjsonに保存
		auto AllTargets = ge->GetTasks<Target::Object>("的");
		for (const auto& target : *AllTargets) {
			json genData;
			genData["basePosition"] = target->basePos;
			genData["moveSpeed"] = target->moveSpeed;
			genData["moveScaleW"] = target->moveScaleW;
			genData["addRotate"] = target->addRotate;
			AllgenDatas["generateData"].push_back(genData);
		}



		int fileNum = 0;		//ファイルの上書きを避けるためのカウント
		bool foundData = true;	//同じ名前のファイルが見つかったか
		string copyName = "";	//名前を重複させないためにつけるコピー名

		//同名のファイルがあるか確認
		while (foundData) {
			if (fileNum > 0) {
				copyName = "(" + to_string(fileNum) + ")";
			}
			ifstream ifs("./data/generateData/targetGenerator" + copyName + ".json");
			if (!ifs) {
				foundData = false;
			}
			else {
				foundData = true;
				++fileNum;
			}
			ifs.close();

		}

		ofstream ofs("./data/generateData/targetGenerator" + copyName + ".json");
		ofs << AllgenDatas.dump(4) << endl;
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
	//-------------------------------------------------------------------
	//ゲームエンジンからデータを受け取る
	Data::SP Data::UseData() {


		//-------------------------------------
		//使用しないという設定（myNumがNOUSE）の場合
		if (myNum == BData::NOUSE) {
			return nullptr;
		}
		//-------------------------------------
		//myNumが設定されていない場合の処理
		//ゲームエンジンにあるデータの何番目が自分かを見つける
		if (myNum == BData::NODATA) {
			for (auto& it : ge->data) {
				++myNum;
				//自身のデータを見つけたらカウント終了・ポインタを返す
				if (typeid(*it) == typeid(Data)) {
					return dynamic_pointer_cast<Data>(it);
				}
			}
			//見つからなかった場合、使用しない設定に切り替える
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//すでに値が決まっている場合の処理
		return dynamic_pointer_cast<Data>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	Data::Data() { }		//コンストラクタ（特に処理なし）
	//-------------------------------------------------------------------
	Data::~Data() { }		//デストラクタ（特に処理なし）
	//-------------------------------------------------------------------


}