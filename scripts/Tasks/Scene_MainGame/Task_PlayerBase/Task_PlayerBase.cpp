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
		this->cameraDistance = j["cameraDistance"];
		this->cameraTargetAdd = j["cameraTargetAdd"];
		this->hitBase = j["hitBase"];
		this->catchBase = j["catchBase"];
		this->throwVec = j["throwVec"];
		this->throwVec = this->throwVec.Norm();
		this->throwStopFrames = j["throwStopFrames"];

		this->powerChargeVol = j["throwPowerChargeVol"];
		vector<float> chTable = j["throwPowerChargeTable"];
		for (int i = 0; i < 3; ++i) {
			this->powerChargeTable[i] = chTable[i];
		}
		
		vector<float> powTable = j["throwPowerTable"];
		for (int i = 0; i < 4; ++i) {
			this->powerTable[i] = powTable[i];
		}

		vector<int> angV = j["angleTable"];
		for (int az = 0; az < 3; ++az) {
			for (int ax = 0; ax < 3; ++ax) {
				this->angleTable[az][ax] = angV[az * 3 + ax];
			}
		}
		
		auto jAnim = j["animationIndexes"];
		this->animIndex[Anims::IDLE] = jAnim["idle"];
		this->animIndex[Anims::WALK] = jAnim["walk"];
		this->animIndex[Anims::CATCH] = jAnim["catch"];
		this->animIndex[Anims::CHARGE] = jAnim["charge"];
		this->animIndex[Anims::THROW] = jAnim["throw"];

		this->powerBarDistance = j["powerBarDistance"];
		this->speed = j["walkSpeed"];
		this->score_catch = j.at("score").at("catch").get<int>();
		this->score_miss = j.at("score").at("miss").get<int>();

		this->key_movefront = j.at("keyconfig").at("move_front").get<vector<int>>();
		this->key_moveback = j.at("keyconfig").at("move_back").get<vector<int>>();
		this->key_moveleft = j.at("keyconfig").at("move_left").get<vector<int>>();
		this->key_moveright = j.at("keyconfig").at("move_right").get<vector<int>>();
		this->key_throwBall = j.at("keyconfig").at("throwBall").get<vector<int>>();


		this->modelFilePath = j["modelFilePath"];

		this->effek_charge = j["dataEffekseer_charge"];
		this->effek_chargeDistance = j["effekseer_chargeDistance"];
		this->effek_throw = j["dataEffekseer_throw"];
		this->effek_throwRotationDeg = j["effekseer_throwRotationDeg"];

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
		this->effekCharge = ML::EffekseerObj::Create(this->data->effek_charge);
		this->effekThrow = ML::EffekseerObj::Create(this->data->effek_throw);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->model.reset();
		this->effekCharge.reset();
		this->effekThrow.reset();
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
		this->cameraDistance = this->res->data->cameraDistance;
		this->cameraTargetAdd = this->res->data->cameraTargetAdd;
		this->hitBase = this->res->data->hitBase;
		this->throwVec = this->res->data->throwVec;
		this->catchBase = this->res->data->catchBase;
		this->moveCount = 0;
		this->throwStopCount = 0;
		this->chargedPower = 0;
		this->power = 0;
		this->speed = this->res->data->speed;
		this->hasBall = false;
		this->score = 0;
		this->modelCl = ML::Model3DClone::Create(this->res->model);
		this->effekChargePlay = nullptr;
		this->effekThrowPlay = nullptr;
		this->powerBar = nullptr;
		this->motion = Motion::IDLE;
		this->preMotion = Motion::IDLE;
		this->preMove = false;
		this->modelCl->AttachAnim(this->res->data->animIndex[Anims::IDLE]);
		
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
	//ーーゲーム前ーー//
	void  Object::UpDateBeforeGame() {
		this->CameraPosSet();
	}

	//ーーメインゲームーー//
	void  Object::UpDateGame()
	{
		//入力情報
		auto& inp = ge->inpkey;



		//カウント
		++moveCount;
		++animCount;

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

		this->CameraPosSet();
	}

	//ーーゲーム後ーー//
	void Object::UpDateAfterGame(){
		this->CameraPosSet();
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

		//アニメーション
		this->modelCl->SetAnimTime(this->animCount);

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
			//投げボタンが押されたらボールを投げる
			if (ge->inpkey.Down(this->res->data->key_throwBall[this->num])) {
				nm = Motion::CHARGE;
			}
			break;
		//投げチャージ
		case Motion::CHARGE:
			//投げボタンが離されたらボールを投げる
			if (this->finishChargeFlag == true) {
				nm = Motion::THROW;
			}
			break;
		//ボール投げ
		case Motion::THROW:
			if (moveCount >= 30) {
				nm = Motion::IDLE;
			}
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
			this->ResetAnimation();
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
			UpDateAnimationStd(Anims::IDLE);
			break;
		//歩行
		case Motion::WALK:
			this->UpDateAnimationStd(Anims::WALK);
			ge->printToDebugFile(to_string(this->modelCl->GetAttachAnimTotalTime()));
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
			if (this->moveCount == 0) {
				this->angle = this->defaultAngle;
				this->modelCl->AttachAnim(this->res->data->animIndex[Anims::CATCH]);
			}
			break;
		//ボール所持
		case Motion::CARRY:
			//アニメーション更新
			//移動時
			if (this->Move() == true) {
				if (this->preMove == false) {
					this->animCount = this->modelCl->GetAttachAnimTotalTime();
				}
				this->UpDateAnimationStd(Anims::WALK);
				this->preMove = true;
			}
			//静止時
			else {
				if (this->preMove == true) {
					this->animCount = this->modelCl->GetAttachAnimTotalTime();
				}
				this->UpDateAnimationStd(Anims::IDLE);
				this->preMove = false;
			}

			this->CarryBall();
			break;
		case Motion::CHARGE:
			if (this->moveCount == 0) {
				this->angle = this->defaultAngle;
				this->modelCl->AttachAnim(this->res->data->animIndex[Anims::CHARGE]);
			}
			this->Charge();
			break;
		//ボール投げ
		case Motion::THROW:
			if (this->moveCount == 0) {
				this->angle = this->defaultAngle;
				this->modelCl->AttachAnim(this->res->data->animIndex[Anims::THROW]);
				this->ThrowBall();
			}
			break;
		}
	}
	//---------------------------------------------
	//アニメーションリセット
	void Object::ResetAnimation() {
		this->animCount = 0;
		this->modelCl->DetachAnimAll();
	}
	//---------------------------------------------
	//アニメーション更新
	void Object::UpDateAnimationStd(Anims anim_) {
		if (this->moveCount == 0) {
			this->modelCl->AttachAnim(this->res->data->animIndex[anim_]);
		}
		if (this->animCount >= this->modelCl->GetAttachAnimTotalTime()) {
			this->modelCl->DetachAnimAll();
			this->modelCl->AttachAnim(this->res->data->animIndex[anim_]);
			this->animCount = 0;
		}
	}
	//---------------------------------------------
	//プレイヤー移動
	bool Object::Move() {
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

			//壁と当たったときの移動制限（ない場合は処理無）
			if (!ge->qa_gameManager) { return true; }
			ge->qa_gameManager->CheckPlayerInArea(this->pos, this->num);
			
			return true;
		}

		return false;
	}
	//---------------------------------------------
	//投げチャージ
	void Object::Charge() {
		if (this->moveCount == 0) {
			//チャージ量リセット
			this->chargedPower = 0;
			//フラグリセット
			this->finishChargeFlag = false;
			//チャージ量表示
			this->powerBar = PowerBar::Object::Create(true);
			ML::Vec2 barPos = ge->cameras[num]->getScreenCenter() + this->res->data->powerBarDistance;
			this->powerBar->SetPos(barPos);
			//エフェクト出現
			this->effekChargePlay = this->res->effekCharge->Play();
		}
		//エフェクトが消えたら再出現
		if (this->effekChargePlay->IsEndPlay()) {
			this->effekChargePlay = this->res->effekCharge->Play();
		}
		//エフェクト座標移動
		this->effekChargePlay->SetPos(this->pos + this->res->data->effek_chargeDistance);
		//投げる強さ加算
		this->chargedPower += this->res->data->powerChargeVol;
		float chargeVolume = chargedPower / this->res->data->powerChargeTable[2];
		this->powerBar->SetVolume(min(1.0f, chargeVolume));

		//ボール座標移動
		this->CarryBall();

		//ボタンが離されたらフラグ切り替え
		if (ge->inpkey.Off(this->res->data->key_throwBall[this->num])) {
			this->finishChargeFlag = true;
			this->ChargeToPower();
			this->powerBar->Kill();
			this->powerBar.reset();
			this->effekChargePlay->Stop();
		}
	}
	//---------------------------------------------
	//チャージ量をパワーに変換
	void Object::ChargeToPower() {
		int powerNum = 0;
		for (int i = 0; i < 3; ++i) {
			if (this->chargedPower >= this->res->data->powerChargeTable[i]) {
				++powerNum;
			}
			else {
				break;
			}
		}
		this->power = this->res->data->powerTable[powerNum];
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
		

		//エフェクト生成
		auto effeks = this->res->effekThrow->Play();
		effeks->SetPos(this->pos);
		effeks->SetRotateDeg(this->res->data->effek_throwRotationDeg + this->defaultAngle);

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

		ML::Vec3 handPos = DxLib::MV1GetFramePosition(this->modelCl->get_handle(), 12);
		ge->qa_ball->Carry(handPos);

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
	//---------------------------------------------
	//カメラ位置設定
	void Object::CameraPosSet() {
		//カメラ位置設定
		ML::Mat4x4 matView;
		matView.Identity();
		matView.RotationX(this->defaultAngle.x + this->addcameraAngle.x);
		matView.RotationY(this->defaultAngle.y + this->addcameraAngle.y);

		ge->cameras[num]->pos = this->pos + this->cameraDistance * matView;
		ge->cameras[num]->target = this->pos + this->cameraTargetAdd * matView;
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