//-------------------------------------------------------------------
//的
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_Target.h"

namespace  Target
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
			std::ifstream fin("./data/gamedata/target.json");
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
		this->hitBase = j["hitBase"];
		this->modelAngleBase = j["modelAngleBase"];
		this->modelFilePath = j["modelFilePath"];
		this->drawHitBase = j["drawHitBase"];
		this->modelShadowFilePath = j["modelShadowFilePath"];
		this->shadowHeight = j["shadowHeight"];
		this->effekExplosion = j["dataEffekseer_explosion"];
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
		this->effekExplosion = ML::EffekseerObj::Create(this->data->effekExplosion);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->model.reset();
		this->modelShadow.reset();
		this->effekExplosion.reset();
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
		this->pos = ML::Vec3();
		this->modelCl = ML::Model3DClone::Create(this->res->model);
		this->modelShadowCl = ML::Model3DClone::Create(this->res->model);
		this->hitbase = this->res->data->hitBase;
		this->addRotate = 0;
		this->moveScaleW = 0;
		this->moveSpeed = 1;
		this->state = State::ALIVE;
		this->stateOfGame = StateOfGame::InGame;

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
	//「３Ｄ描画」１フレーム毎に行う処理、レイヤー0
	void Object::Render3D_L0()
	{
		this->modelCl->DrawStd(this->pos, this->angle + this->res->data->modelAngleBase);
		//影の描画
		ML::Vec3 shadowPos = this->pos;
		shadowPos.y = this->res->data->shadowHeight;
		this->res->modelShadow->DrawStd(shadowPos);

		if (this->res->data->drawHitBase) {
			ML::DrawBox3D(this->CallHitBox(), ML::Color(0.5f, 0, 0, 1));
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理、3D描画後に行われる
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//関数定義
	//当たり判定呼び出し
	ML::Obb3 Object::CallHitBox() const {
		return this->hitbase.OffsetCopy(this->pos);
	}
	//モーション変更
	void Object::Judge() {
		State nextState = this->state;

		switch (this->state) {
		case  State::ALIVE:
			if (ge->qa_ball->CheckHitPreposToPos(this->CallHitBox())) {
				nextState = State::HIT;
			}
			break;
		case  State::HIT:
			break;
		}

		//モーション更新
		this->UpDateState(nextState);
	}

	//モーション更新
	void Object::UpDateState(State ns_) {
		if (ns_ != this->state) {
			this->moveCnt = 0;
			this->state = ns_;
		}
	}

	//モーションに応じた移動
	void Object::Action() {
		switch (this->state) {
		case  State::ALIVE:
			this->MoveLR();
			break;
		case  State::HIT:
			ge->qa_ball->HitToTarget();
			this->res->effekExplosion->Play()->SetPos(this->pos);
			this->Kill();
			break;
		}
	}

	//横移動
	void Object::MoveLR() {
		this->pos = this->basePos;
		float movePos = this->moveCnt * this->moveSpeed;
		this->pos.x += cosf(ML::ToRadian(movePos + this->addRotate)) * this->moveScaleW;

		if (fabsf(movePos) > 360) {
			this->moveCnt = 0;
		}
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