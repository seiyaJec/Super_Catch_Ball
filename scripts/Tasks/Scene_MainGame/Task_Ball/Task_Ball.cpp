//-------------------------------------------------------------------
//ボール
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_Ball.h"


namespace  Ball
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

		//jsonファイル読み込み
		json j;
		if (useBinary_ == false) {
			//ファイルパス
			std::ifstream fin("./data/gamedata/ball.json");
			if (!fin) { return json(); }
			fin >> j;
			//ファイル読み込み終了
			fin.close();
		}
		else {
			j = ge->gameData["data"][this->myArg];
		}
		//----------------------------------
		//★データ読み込み処理
		this->pos = j["position"];
		this->moveVec = j["moveVec"];
		this->hitBase = j["hitBase"];
		this->modelFilePath = j["modelFilePath"];
		this->comboScoreTable = j.at("comboScoreTable").get<vector<int>>();
		this->modelShadowFilePath = j["modelShadowFilePath"];
		this->shadowHeight = j["shadowHeight"];

		this->effek_move = j["dataEffekseer_move"];
		this->effek_moveRotateDeg = j["dataEffekseerMoveRotateDegree"];
		this->effek_moveDistance = j["dataEffekseerMoveDistance"];

		this->drawHitBase = j["drawHitBase"];
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
		this->model = ML::Model3D::Create(this->data->modelFilePath);
		this->modelShadow = ML::Model3D::Create(this->data->modelShadowFilePath);
		this->effekMove = ML::EffekseerObj::Create(this->data->effek_move);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->model.reset();
		this->modelShadow.reset();
		this->effekMove.reset();
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
		this->state = State::IDLE;
		this->prePos = this->res->data->pos;
		this->pos = this->res->data->pos;
		this->moveVec = this->res->data->moveVec;
		this->hitBase = this->res->data->hitBase;
		this->moveCnt = 0;
		this->flagCarried = false;
		this->moveDistanceCnt = 0;
		//★タスクの生成

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
	void  Object::UpDateGame()
	{
		this->Judge();
		this->Action();

		++moveCnt;
	}
	//-------------------------------------------------------------------
	//「３Ｄ描画」１フレーム毎に行う処理
	void  Object::Render3D_L0()
	{
		this->res->model->DrawStd(this->pos);
		ML::Vec3 shadowPos = this->pos;
		shadowPos.y = this->res->data->shadowHeight;
		this->res->modelShadow->DrawStd(shadowPos);

		//当たり判定描画
		if (this->res->data->drawHitBase) {
			ML::DrawBox3D(this->CallHitBox(), ML::Color(0.5f, 0, 1, 0));
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理（3D描画後に行われる）
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//当たり判定を呼び出す
	ML::Obb3 Object::CallHitBox() const {
		return this->hitBase.OffsetCopy(this->pos);
	}
	//---------------------------------------------
	//場面に応じて状態を変化させる
	void Object::Judge() {
		State nextState = this->state;

		switch (this->state) {
		case  State::NON:
			break;
		case  State::IDLE:
			if (this->flagCarried == true) {
				nextState = State::CARRIED;
			}
			break;
		case  State::THROWN:
			if (this->IsMoving() == false) {
				nextState = State::IDLE;
			}
			if (this->flagCarried == true) {
				nextState = State::CARRIED;
			}
			break;
		case  State::CARRIED:
			if (this->flagCarried == false) {
				nextState = State::IDLE;
				if (this->IsMoving() == true) {
					nextState = State::THROWN;
				}
			}
			break;
		}

		this->UpDateState(nextState);
	}
	//---------------------------------------------
	//状態の更新
	void Object::UpDateState(State state_) {
		if (this->state == state_) {
			return;
		}
		else {
			this->state = state_;
			this->moveCnt = 0;
		}
	}
	//---------------------------------------------
	//状態に合わせて動作させる
	void Object::Action() {
		this->prePos = this->pos;

		switch (this->state) {
		case  State::NON:
			if (this->moveCnt == 0) {
				this->moveVec = ML::Vec3();
				this->combo = 0;
			}
			break;
		case  State::IDLE:
			if (this->moveCnt == 0) {
				this->moveVec = ML::Vec3();
				this->combo = 0;
			}
			break;
		case  State::THROWN:
			this->CreateMoveEffect();
			this->pos += moveVec;
			break;
		case  State::CARRIED:
			if (this->moveCnt == 0) {
				this->moveVec = ML::Vec3();
				this->combo = 0;
			}
			break;
		}

		//フラグをリセット
		this->ResetFlag();
	}
	//---------------------------------------------
	//移動エフェクト生成
	void Object::CreateMoveEffect() {
		if (moveCnt == 0) {
			this->moveDistanceCnt = 0;
		}
		this->moveDistanceCnt += this->moveVec.Length();
		//一定距離移動したらエフェクト生成（設定した間隔が0.0より大きい場合のみ行う）
		if (this->res->data->effek_moveDistance > 0.0f) {
			ML::Vec3 norm = this->moveVec.Norm();
			while (this->moveDistanceCnt >= this->res->data->effek_moveDistance) {
				this->moveDistanceCnt -= this->res->data->effek_moveDistance;
				auto playefk = this->res->effekMove->Play();
				//座標設定
				playefk->SetPos(this->pos - (norm * this->moveDistanceCnt));
				//角度設定
				ML::Vec3 rot(0, atan2f(this->moveVec.z, this->moveVec.x), 0);
				playefk->SetRotateRad(rot + ML::ToRadian(this->res->data->effek_moveRotateDeg));

			}
		}
	}
	//---------------------------------------------
	//フラグのリセット
	void Object::ResetFlag() {
		this->flagCarried = false;
	}
	//---------------------------------------------

	//移動した距離分1ミリごとに当たり判定を行う。
	bool Object::CheckHitPreposToPos(const ML::Obb3& you_) {
		ML::Vec3 est = this->pos - this->prePos;
		//移動がなかった場合の処理
		if (est == 0.0f) {
			ML::Obb3 me = this->CallHitBox();
			return ML::CheckHit(me, you_);
		}
		//X軸
		while (est.x != 0.0f) {
			if (est.x >= 1.0f) { --est.x; }
			else if (est.x <= -1.0f) { ++est.x; }
			else { est.x = 0.0f; }

			//当たり判定
			if (ML::CheckHit(this->hitBase.OffsetCopy(this->pos - est), you_)
				== true) {
				return true;
			}
		}
		//Z軸
		while (est.z != 0.0f) {
			if (est.z >= 1.0f) { --est.z; }
			else if (est.z <= -1.0f) { ++est.z; }
			else { est.z = 0.0f; }

			//当たり判定
			if (ML::CheckHit(this->hitBase.OffsetCopy(this->pos - est), you_)
				== true) {
				return true;
			}
		}
		//Y軸
		while (est.y != 0.0f) {
			if (est.y >= 1.0f) { --est.y; }
			else if (est.y <= -1.0f) { ++est.y; }
			else { est.y = 0.0f; }

			//当たり判定
			if (ML::CheckHit(this->hitBase.OffsetCopy(this->pos - est), you_)
				== true) {
				return true;
			}
		}


		return false;
	}
	//---------------------------------------------
	//持ち運ぶ
	void Object::Carry(const ML::Vec3& carryPos_) {
		this->pos = carryPos_;
		this->flagCarried = true;
	}
	//---------------------------------------------
	//移動中か
	bool Object::IsMoving() {
		return this->moveVec != 0.0f;
	}
	//---------------------------------------------
	//受け取る処理
	void Object::HitToTarget() {
		//連続ヒット数に応じて得点を追加
		auto& cmbScTable = this->res->data->comboScoreTable;
		int addScore = cmbScTable[min(combo, (int)cmbScTable.size() - 1)];
		ge->qa_scoreJudge->addScore(addScore);
		++this->combo;
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