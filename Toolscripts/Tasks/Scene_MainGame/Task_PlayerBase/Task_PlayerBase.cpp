//-------------------------------------------------------------------
//プレイヤー管理タスク
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_PlayerBase.h"

namespace  PlayerBase
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
			std::ifstream fin("./data/gamedata/player.json");
			if (!fin) { return json(); }
			//JSONファイル読み込み
			fin >> j;
			//ファイル読み込み終了
			fin.close();
		}
		else {
			j = ge->gameData["data"][this->myArg];
		}
		//ーーーーーーーーーーーーーーーーー
		//★データ読み込み処理
		this->handPos = j["handPos"];
		this->throwStartPos = j["throwStartPos"];
		this->modelAngle = j["modelAngle"];
		this->distance = j["cameraDistance"];
		this->hitBase = j["hitBase"];
		this->catchBase = j["catchBase"];
		this->throwVec = j["throwVec"];
		this->throwVec = this->throwVec.Norm();
		this->throwStopFrames = j["throwStopFrames"];
		this->power = j["throwPower"];
		this->speed = j["walkSpeed"];
		vector<int> angV = j["angleTable"];
		for (int az = 0; az < 3; ++az) {
			for (int ax = 0; ax < 3; ++ax) {
				this->angleTable[az][ax] = angV[az * 3 + ax];
			}
		}
		this->score_catch = j.at("score").at("catch").get<int>();
		this->score_miss = j.at("score").at("miss").get<int>();

		this->key_movefront = j.at("keyconfig").at("move_front").get<vector<int>>();
		this->key_moveback = j.at("keyconfig").at("move_back").get<vector<int>>();
		this->key_moveleft = j.at("keyconfig").at("move_left").get<vector<int>>();
		this->key_moveright = j.at("keyconfig").at("move_right").get<vector<int>>();
		this->key_throwBall = j.at("keyconfig").at("throwBall").get<vector<int>>();


		this->modelFilePath = j["modelFilePath"];
		this->drawHitBase = j["drawHitBase"];
		this->drawCatchBase = j["drawCatchBase"];
		//ーーーーーーーーーーーーーーーーー
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
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->model.reset();
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
		this->distance = this->res->data->distance;
		this->hitBase = this->res->data->hitBase;
		this->throwVec = this->res->data->throwVec;
		this->catchBase = this->res->data->catchBase;
		this->moveCount = 0;
		this->throwStopCount = 0;
		this->power = this->res->data->power;
		this->speed = this->res->data->speed;
		this->hasBall = false;
		this->score = 0;
		this->modelCl = ML::Model3DClone::Create(this->res->model);
		this->motion = Motion::IDLE;
		this->preMotion = Motion::IDLE;
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
	void  Object::UpDate()
	{
		//入力情報
		auto& inp = ge->inpkey;

		//カウント
		++moveCount;

		//プレイヤーの移動
		//次の行動を判断
		this->judge();
		//移動処理
		this->Action();



		//デバッグ用
		int cameraspeed = 2;
		if (inp.On(KEY_INPUT_NUMPAD8)) {
			this->addcameraAngle.x += cameraspeed;
		}
		if (inp.On(KEY_INPUT_NUMPAD2)) {
			this->addcameraAngle.x -= cameraspeed;
		}
		if (inp.On(KEY_INPUT_NUMPAD6)) {
			this->addcameraAngle.y += cameraspeed;
		}
		if (inp.On(KEY_INPUT_NUMPAD4)) {
			this->addcameraAngle.y -= cameraspeed;
		}
		ML::Mat4x4 matView;
		matView.Identity();
		matView.RotationX(this->defaultAngle.x + this->addcameraAngle.x);
		matView.RotationY(this->defaultAngle.y + this->addcameraAngle.y);

		

	}
	//-------------------------------------------------------------------
	// 「３Ｄ描画」１フレームに行う処理、レイヤー0
	void Object::Render3D_L0()
	{
		ML::Vec3 pos2(0, 0, 1000);

		ML::Mat4x4 matTr;
		matTr.Identity();
		matTr.RotationY(this->angle.y);
		pos2 = pos2 * matTr;
		pos2 += this->pos;


		//モデルの描画
		ML::Vec3 mAngle = this->angle + this->res->data->modelAngle;

		this->modelCl->DrawStd(this->pos, mAngle);

		DxLib::DrawLine3D(this->pos, pos2, ML::GetColor(ML::Color(1, 0, 1, 1)));
		
		//当たり判定描画
		if (this->res->data->drawHitBase) {
			ML::DrawBox3D(this->CallHitBox(), ML::Color(0.5f, 1, 1, 1));
		}
		//キャッチ当たり判定描画
		if (this->res->data->drawCatchBase) {		//キャッチ当たり判定を描画
			if (this->motion == Motion::CATCH) {	//キャッチモーションになっていれば実行
				if (ge->qa_ball) {					//ボールが生成されていれば実行
					//標準時の向きを行列化
					ML::Mat4x4 matR(true);			
					matR.RotationXYZ(this->defaultAngle);

					//キャッチ当たり判定を用意
					ML::Obb3 catchHit = this->catchBase;
					catchHit.SetPos((catchHit.get_pos() * matR) + this->pos);

					ML::DrawBox3D(catchHit, ML::Color(0.5f, 1, 0.8f, 0.8f));
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理、3D描画後に行われる
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
	
	//次の行動を判断
	void Object::judge() {
		auto& inp = ge->inpkey;
		Motion nm = this->motion;

		switch (this->motion) {
		//静止
		case Motion::IDLE:
			//ボールゲットへの遷移
			if (this->SearchBall()) { nm = Motion::GET; }
			//移動への遷移
			if (inp.On(this->res->data->key_movefront[this->num])) { nm = Motion::WALK; }
			if (inp.On(this->res->data->key_moveback[this->num])) { nm = Motion::WALK; }
			if (inp.On(this->res->data->key_moveright[this->num])) { nm = Motion::WALK; }
			if (inp.On(this->res->data->key_moveleft[this->num])) { nm = Motion::WALK; }
			//キャッチへの遷移
			if (this->throwStopCount >= this->res->data->throwStopFrames
			&& ge->inpkey.On(this->res->data->key_throwBall[this->num])) 
			{
				nm = Motion::CATCH; 
			}
			break;
		//歩行
		case Motion::WALK:
			//ボールゲットへの遷移
			if (SearchBall()) { nm = Motion::GET; }
			//静止状態への遷移
			if (inp.Off(this->res->data->key_movefront[this->num])
			&& inp.Off(this->res->data->key_moveback[this->num])
			&& inp.Off(this->res->data->key_moveright[this->num])
			&& inp.Off(this->res->data->key_moveleft[this->num])) 
			{
				nm = Motion::IDLE; 
			}
			//キャッチへの遷移
			if (this->throwStopCount >= this->res->data->throwStopFrames
				&& ge->inpkey.On(this->res->data->key_throwBall[this->num]))
			{
				nm = Motion::CATCH;
			}
			break;
		//ボールゲット
		case Motion::GET:
			nm = Motion::CARRY;
			break;
		//ボールキャッチ
		case Motion::CATCH:
			if (ge->inpkey.Off(this->res->data->key_throwBall[this->num])) { nm = Motion::IDLE; }
			if (SearchBall()) { nm = Motion::GET; }
			if (CatchBall()) { nm = Motion::GET; }
			break;
		//ボール所持
		case Motion::CARRY:
			//左クリックが押されたらボールを投げる
			if (ge->inpkey.Down(this->res->data->key_throwBall[this->num])) {
				nm = Motion::THROW;
			}
			break;
		//ボール投げ
		case Motion::THROW:
			nm = Motion::IDLE;
			break;
		}


		//モーションを更新
		this->UpDateMotion(nm);
	}
	//---------------------------------------------
	//モーション更新
	void Object::UpDateMotion(Motion nm_) {
		this->preMotion = this->motion;
		if(nm_ == this->motion)
		{}
		else{
			this->motion = nm_;
			this->moveCount = 0;
		}
	}
	//---------------------------------------------

	//プレイヤー移動処理・方向設定
	void Object::Action()
	{
		//投げ硬直時間カウント
		if (this->throwStopCount < this->res->data->throwStopFrames) {
			++this->throwStopCount;
		}


		switch (this->motion) {
		//静止
		case Motion::IDLE:
			break;
		//歩行
		case Motion::WALK:
			this->Move();
			break;
		//ボールゲット
		case Motion::GET:
			//移動中のボールを捕らえたならスコア加算
			if (ge->qa_ball->IsMoving() == true) {
				ge->qa_scoreJudge->addScore(this->res->data->score_catch);
			}
			this->CarryBall();
			this->hasBall = true;
			break;
		//ボールキャッチ
		case Motion::CATCH:
			break;
		//ボール所持
		case Motion::CARRY:
			this->Move();
			this->CarryBall();
			break;
		//ボール投げ
		case Motion::THROW:
			this->ThrowBall();
			break;
		}
	}
	//---------------------------------------------
	//プレイヤー移動
	void Object::Move() {
		auto& inp = ge->inpkey;
		//入力総数
		int angleX = 1;
		int angleZ = 1;

		//前
		if (inp.On(this->res->data->key_movefront[this->num])) {
			angleZ -= ML::ONE;
		}
		//後ろ
		if (inp.On(this->res->data->key_moveback[this->num])) {
			angleZ += ML::ONE;
		}
		//右
		if (inp.On(this->res->data->key_moveright[this->num])) {
			angleX += ML::ONE;
		}
		//左
		if (inp.On(this->res->data->key_moveleft[this->num])) {
			angleX -= ML::ONE;
		}

		float newAngleY = (float)this->res->data->angleTable[angleZ][angleX];

		//入力がある場合の処理
		if (newAngleY >= 0.0f && newAngleY <= 360.0f) {

			//向きを変更
			this->angle.y = newAngleY;
			this->angle += defaultAngle;

			//移動処理
			ML::Vec3 movePos(0, 0, speed);
			ML::Mat4x4 matRot;
			matRot.Identity();
			matRot.RotationXYZ(this->angle);
			movePos = movePos * matRot;
			this->pos += movePos;

		}

		//壁と当たったときの移動制限（ない場合は処理無）
		if (!ge->qa_gameManager) { return; }
		ge->qa_gameManager->CheckPlayerInArea(this->pos, this->num);
	}
	//---------------------------------------------
	//ボールを投げる
	void Object::ThrowBall() {
		//ボールを持っていなければ投げない
		if (this->hasBall == false) { return; }

		//投げ処理　標準の方向へ投げる
		ML::Mat4x4 matR(true);
		matR.RotationXYZ(this->defaultAngle);
		//座標移動
		ge->qa_ball->pos = this->pos + (this->res->data->throwStartPos * matR);

		ge->qa_ball->moveVec = this->throwVec * matR;


		//移動速度
		ge->qa_ball->moveVec *= this->power;


		ge->qa_ball->state = Ball::Object::State::THROWN;
		this->hasBall = false;
		this->throwStopCount = 0;
	}
	//---------------------------------------------
	//ボールをキャッチする
	bool Object::CatchBall() {
		//ボールがまだ生成されていなければ行わない
		if (!ge->qa_ball) { return false; }
		
		//標準時の向きを行列化
		ML::Mat4x4 matR(true);
		matR.RotationXYZ(this->defaultAngle);
		
		//キャッチ当たり判定を用意
		ML::Obb3 me = this->catchBase;
		me.SetPos((me.get_pos() * matR) + this->pos);

		return ge->qa_ball->CheckHitPreposToPos(me);
	}
	//---------------------------------------------
	//ボールを持つ
	void Object::CarryBall() {
		//ボールを持っていなければ行わない
		if (this->hasBall == false) { return; }
		//ボールが無ければ処理を行わない
		if (!ge->qa_ball) { return; }

		//手の位置に座標変換
		ML::Vec3 handPos = this->res->data->handPos;

		//プレイヤーの向きに合わせて回転
		ML::Vec3 newAngle = this->angle;
		ML::Mat4x4 matR(true);
		matR.RotationXYZ(newAngle);
		handPos *= matR;

		ge->qa_ball->Carry(this->pos + handPos);


	}
	//---------------------------------------------
	//ボールを拾う
	bool Object::SearchBall() {
		//ボールがまだ生成されていなければ行わない
		if (!ge->qa_ball) { return false; }

		//ボールとの当たり判定
		ML::Obb3 me = this->CallHitBox();		//プレイヤー
		return ge->qa_ball->CheckHitPreposToPos(me);
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