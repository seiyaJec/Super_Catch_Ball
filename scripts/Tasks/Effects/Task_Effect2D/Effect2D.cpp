//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Effect2D.h"

namespace  Effect2D
{

	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//総合
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
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
	Resource::Resource() {}			//コンストラクタ
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }		//デストラクタ






	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//標準エフェクト
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□


		//-------------------------------------------------------------------
		//「初期化」タスク生成時に１回だけ行う処理
	bool  EffectStd::Initialize(string imgFilePath_, ML::Size2 imgSize_)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		auto itr = this->res->images.find(imgFilePath_);

		//デフォルト表示優先度設定
		this->render2D_Priority[1] = 0.2f;

		//画像がまだ作られていない場合
		if (itr == this->res->images.end()) {
			//画像読み込み
			auto newimg = ML::Image::Create(imgFilePath_, imgSize_);
			if (newimg) {
				this->res->images.insert(std::make_pair(imgFilePath_, newimg));
				this->img = newimg;
			}
			//読み込み失敗
			else {
				return false;
			}
		}
		//画像が作られている場合
		else {
			this->img = itr->second;
		}

		//回転の中心を初期化
		this->setImgRotateCenter(this->img->CenterPos());

		//追加の初期化処理
		return this->InitializeCustom();
	}
	//-------------------------------------------------------------------
	//追加の初期化処理（スーパークラスなので特になし）
	bool  EffectStd::InitializeCustom() {
		return true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  EffectStd::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成

		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  EffectStd::UpDate()
	{
		this->pos += moveVec;

		//ループ
		this->loopRotate.Addto(this->transform.rotateDeg);
		this->loopScaleX.Addto(this->transform.scaleX);
		this->loopScaleY.Addto(this->transform.scaleY);

		this->loopAddAlpha.Addto(this->color.a);
		this->CountLife();

		//追加の更新処理（継承したクラス用）
		this->UpDateAdd();
	}
	//-------------------------------------------------------------------
	//追加の更新処理（スーパークラスなので特になし）
	void  EffectStd::UpDateAdd() {

	}
	//-------------------------------------------------------------------
	//「３Ｄ描画」１フレーム毎に行う処理、レイヤー0
	void EffectStd::Render3D_L0()
	{

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理、3D描画後に行われる
	void  EffectStd::Render2D_AF()
	{
		this->img->DrawTransform(this->pos, this->transform, true, this->color.a);
	}
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//コンストラクタ
	EffectStd::Loop::Loop()
		:adds(0.0f)
		, lpmax(9999.0f)
		, lpmin(-9999.0f)
	{}
	//ループ設定
	void EffectStd::Loop::Set(float adds_, float lpmax_, float lpmin_) {
		adds = adds_;
		lpmax = lpmax_;
		lpmin = lpmin_;
	}
	//生存時間カウント
	void EffectStd::CountLife() {
		--this->life;
		if (this->life <= 0) {
			this->Kill();
		}
	}
	//ループ
	void EffectStd::Loop::Addto(float& addto_) {
		if (this->adds > 0.0f) {
			addto_ = min(addto_ + this->adds, this->lpmax);
		}
		else if (this->adds < 0.0f) {
			addto_ = max(addto_ + this->adds, this->lpmin);
		}
	}
	//初期化データから読み込む
	void EffectStd::ReadFromInitStd(const InitStd& initStd_) {
		this->setPos(initStd_.pos);
		this->setMoveVec(initStd_.moveVec);
		this->setLife(initStd_.life);
		this->setImgRotate(initStd_.imgRotDeg);
		this->setImgScale(initStd_.scale.x, initStd_.scale.y);
		this->setImgFlip(initStd_.imgFlipX, initStd_.imgFlipY);
		this->setColor(initStd_.color);
		this->setRotateLoop(initStd_.lpRotate.adds, initStd_.lpRotate.lpmax, initStd_.lpRotate.lpmin);
		this->setScaleXLoop(initStd_.lpScaleX.adds, initStd_.lpScaleX.lpmax, initStd_.lpScaleX.lpmin);
		this->setScaleYLoop(initStd_.lpScaleY.adds, initStd_.lpScaleY.lpmax, initStd_.lpScaleY.lpmin);
		this->setAlphaLoop(initStd_.lpAlpha.adds, initStd_.lpAlpha.lpmax, initStd_.lpAlpha.lpmin);
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	// 生成関連メソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	bool  EffectStd::B_Initialize(string imgFilePath_, ML::Size2 size_)
	{
		return  this->Initialize(imgFilePath_, size_);
	}
	//-------------------------------------------------------------------
	EffectStd::~EffectStd() { this->B_Finalize(); }
	bool  EffectStd::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	EffectStd::EffectStd()
		:img(nullptr)
		, pos()
		, moveVec()
		, life(100)
		, transform()
		, color()
		, loopRotate()
		, loopScaleX()
		, loopScaleY()
		, loopAddAlpha()
	{	}
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// 初期化用データ格納クラスメソッド
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し（軽量ループ構造体）
	void to_json(json& j_, const InitStd::LoopLt& loop_) {
		j_ = json{
			{"addvalues", loop_.adds},
			{"loopMax", loop_.lpmax},
			{"loopMin", loop_.lpmin}
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み（軽量ループ構造体）
	void from_json(const json& j_, InitStd::LoopLt& loop_) {
		j_.at("addvalues").get_to(loop_.adds);
		j_.at("loopMax").get_to(loop_.lpmax);
		j_.at("loopMin").get_to(loop_.lpmin);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し（初期化用データ格納クラス）
	void to_json(json& j_, const InitStd& effStd_) {
		j_ = json{
			{"imageFilePath", effStd_.imgFilePath},
			{"imageSize", effStd_.imgSize},
			{"imageRotationCenter", effStd_.imgRotCt},
			{"imageRotateDegree", effStd_.imgRotDeg},
			{"imageFlipX",effStd_.imgFlipX },
			{"imageFlipY",effStd_.imgFlipY},
			{"position", effStd_.pos},
			{"moveVec", effStd_.moveVec},
			{"color", effStd_.color},
			{"scale", effStd_.scale},
			{"life", effStd_.life},
			{"loopScaleX", effStd_.lpScaleX},
			{"loopScaleY", effStd_.lpScaleY},
			{"loopAlpha", effStd_.lpAlpha},
			{"loopRotate", effStd_.lpRotate},
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み（初期化用データ格納クラス）
	void from_json(const json& j_, InitStd& effStd_) {
		j_.at("imageFilePath").get_to(effStd_.imgFilePath);
		j_.at("imageSize").get_to(effStd_.imgSize);
		j_.at("imageRotationCenter").get_to(effStd_.imgRotCt);
		j_.at("imageRotateDegree").get_to(effStd_.imgRotDeg);
		j_.at("imageFlipX").get_to(effStd_.imgFlipX);
		j_.at("imageFlipY").get_to(effStd_.imgFlipY);
		j_.at("position").get_to(effStd_.pos);
		j_.at("moveVec").get_to(effStd_.moveVec);
		j_.at("color").get_to(effStd_.color);
		j_.at("scale").get_to(effStd_.scale);
		j_.at("life").get_to(effStd_.life);
		j_.at("loopScaleX").get_to(effStd_.lpScaleX);
		j_.at("loopScaleY").get_to(effStd_.lpScaleY);
		j_.at("loopAlpha").get_to(effStd_.lpAlpha);
		j_.at("loopRotate").get_to(effStd_.lpRotate);
	}



	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//左→中央→右の順で移動するエフェクト
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// ゲームデータ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
	int DataL_C_R::myNum = DataL_C_R::NODATA;
	//-------------------------------------------------------------------

	//ゲームモードを読み込む
	json DataL_C_R::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//ファイルパス
			std::ifstream fin("./data/effectData/effectL_C_R");
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

		this->moveSpeed = j["moveSpeedDefault"];
		this->moveSpeedMin = j["moveSpeedMinDefault"];
		this->timeStayCenter = j["timeStayCenterDefault"];

		//----------------------------------
		//（★★使用する場合はmyPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 

		return j;
	}
	//-------------------------------------------------------------------
	//ゲームエンジンからデータを受け取る
	DataL_C_R::SP DataL_C_R::UseData() {
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
				if (typeid(*it) == typeid(DataL_C_R)) {
					return dynamic_pointer_cast<DataL_C_R>(it);
				}
			}
			//見つからなかった場合、使用しない設定に切り替える
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//すでに値が決まっている場合の処理
		return dynamic_pointer_cast<DataL_C_R>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataL_C_R::DataL_C_R() { }		//コンストラクタ（特に処理なし）
	//-------------------------------------------------------------------
	DataL_C_R::~DataL_C_R() { }		//デストラクタ（特に処理なし）
	//-------------------------------------------------------------------

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//--------------------------------------------------------
	//初期化処理
	bool EffectL_C_R::InitializeCustom() {
		this->data = DataL_C_R::UseData();

		ML::Size2 imgSize = this->img->get_size();
		this->pos.x = imgSize.w / -2;
		this->pos.y = (float)ge->screenHeight / 2.0f;
		this->moveVec.x = this->data->moveSpeed;
		this->moveSpeedMin = this->data->moveSpeedMin;
		this->timeStayCenter = this->data->timeStayCenter;
		this->timeCnt = 0;
		this->passedCenter = false;

		return true;
	}
	//--------------------------------------------------------
	//更新処理
	void EffectL_C_R::UpDate() {

		//画面中央にいるか
		if (this->IsPassedCenterOfScreen() == false) {
			this->pos += this->moveVec;
		}
		else {
			if (this->timeCnt >= this->timeStayCenter) {
				this->pos += this->moveVec;
			}
			++timeCnt;
		}


		//画面外に出たらキル
		if (this->IsOutOfScreen() == true) {
			this->Kill();
		}

	}

	//--------------------------------------------------
	//その他メソッド
	//--------------------------------------------------
	//画面右側へ行き見えなくなったか
	bool EffectL_C_R::IsOutOfScreen() {
		return this->pos.x - this->img->get_size().w * this->transform.scaleX >= ge->screenWidth;
	}
	//画面中央を通過したか
	bool EffectL_C_R::IsPassedCenterOfScreen() {
		ML::Vec2 screenCenter = ge->screenCenter();
		return
			this->pos.x >= screenCenter.x
			&& this->pos.y >= screenCenter.y;
	}

	//--------------------------------------------------------
	//セッタ
	void EffectL_C_R::setMoveSpeed(float moveSpeed_) {
		if (moveSpeed_ <= 0) {
			string debug("EffectL_C_R::setMoveSpeed>>不正なデータ値です！");
			ge->printToDebugFile(debug);
		}
		this->moveSpeed = moveSpeed_;
		this->moveVec.x = moveSpeed_;
	}
	void EffectL_C_R::setTimeStayCenter(int timeStayCenter_) {
		if (timeStayCenter_ < 0) {
			string debug("EffectL_C_R::setMoveSpeed>>不正なデータ値です！");
			ge->printToDebugFile(debug);
		}
		this->timeStayCenter = timeStayCenter_;
	}
	//--------------------------------------------------------
	//コンストラクタ
	EffectL_C_R::EffectL_C_R()
		:EffectStd()
	{}
	//--------------------------------------------------------
	//デストラクタ
	EffectL_C_R::~EffectL_C_R() {}

	//--------------------------------------------------------
	//エフェクト生成窓口
	EffectL_C_R::SP EffectL_C_R::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		EffectL_C_R::SP ob = EffectL_C_R::SP(new EffectL_C_R());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録

			}
			if (!ob->B_Initialize(imgFilePath_, size_)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}

	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//スタンプ（サイズn倍→等倍→透明化）
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// データ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
	int DataEffectStamp::myNum = DataEffectStamp::NODATA;
	//-------------------------------------------------------------------

	//ゲームモードを読み込む
	json DataEffectStamp::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//ファイルパス
			std::ifstream fin("./data/effectData/effectStamp.json");
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
		this->stampDecSpeed = j["stampDecSpeedDefault"];
		this->stampScaleMax = j["stampScaleMaxDefault"];
		this->alphaDecSpeed = j["alphaDecSpeedDefault"];
		this->alphaWaitDec = j["alphaWaitDecDefault"];

		//----------------------------------
		//（★★使用する場合はmyPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 

		return j;
	}

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//--------------------------------------------------------
	//初期化処理
	bool EffectStamp::InitializeCustom() {
		//データ読み込み
		this->data = DataEffectStamp::UseData();

		//★初期化処理
		this->setScaleXLoop(0.01f, 2.0f);
		this->setPos(ge->screenCenter());
		this->setStamp(this->data->stampDecSpeed, this->data->stampScaleMax);
		this->setAlpha(this->data->alphaDecSpeed, this->data->alphaWaitDec);

		return true;
	}
	//--------------------------------------------------------
	//追加して行う更新処理
	void EffectStamp::UpDate() {
		
		//スケール値を減少させる
		this->loopScaleX.Addto(this->transform.scaleX);
		this->loopScaleY.Addto(this->transform.scaleY);
		
		//スケール値が最小になったら
		if (this->IsScaleMin() == true) {
			//時間計測
			if (this->life > 0.0f) {
				--this->life;
			}
			//0になったら透明化開始
			else {
				this->loopAddAlpha.Addto(this->color.a);
				if (this->color.a <= 0.0f) {
					this->Kill();
				}
			}
		}

	}
	//--------------------------------------------------
	//その他メソッド
	//--------------------------------------------------
	//スケール値が最小か
	bool EffectStamp::IsScaleMin() {

		//X・Y両方が最小ならtrue
		if (this->transform.scaleX <= this->loopScaleX.lpmin) {
			if (this->transform.scaleY <= this->loopScaleY.lpmin) {
				return true;
			}
		}

		//そうでなければfalse
		return false;
	}
	//サイズ変化設定（縮小スピード、初期サイズ）
	void EffectStamp::setStamp(float decSpeed_, float scaleMax_) {
		setScaleXLoop(-decSpeed_, scaleMax_, 1.0f);
		setScaleYLoop(-decSpeed_, scaleMax_, 1.0f);
		setImgScale(scaleMax_, scaleMax_);
	}

	//透明化処理設定（透明化スピード、透明化までの時間（フレーム単位））
	void EffectStamp::setAlpha(float decSpeed_, float waitDec_) {
		this->setAlphaLoop(-decSpeed_, 1.0f, 0.0f);
		this->setLife(waitDec_);
		this->color.a = 1.0f;
	}

	//--------------------------------------------------------
	//コンストラクタ
	EffectStamp::EffectStamp()
		:EffectStd()
	{}
	//--------------------------------------------------------
	//デストラクタ
	EffectStamp::~EffectStamp() {}

	//--------------------------------------------------------
	//エフェクト生成窓口
	EffectStamp::SP EffectStamp::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		EffectStamp::SP ob = EffectStamp::SP(new EffectStamp());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録

			}
			if (!ob->B_Initialize(imgFilePath_, size_)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//ゲームエンジンからデータを受け取る
	DataEffectStamp::SP DataEffectStamp::UseData() {
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
				if (typeid(*it) == typeid(DataEffectStamp)) {
					return dynamic_pointer_cast<DataEffectStamp>(it);
				}
			}
			//見つからなかった場合、使用しない設定に切り替える
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//すでに値が決まっている場合の処理
		return dynamic_pointer_cast<DataEffectStamp>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataEffectStamp::DataEffectStamp() { }		//コンストラクタ（特に処理なし）
	//-------------------------------------------------------------------
	DataEffectStamp::~DataEffectStamp() { }		//デストラクタ（特に処理なし）
	//-------------------------------------------------------------------


	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//サブエフェクトランダム出現
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// データ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
	int DataGenerateSubRandom::myNum = DataGenerateSubRandom::NODATA;
	//-------------------------------------------------------------------

	//ゲームモードを読み込む
	json DataGenerateSubRandom::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//ファイルパス
			std::ifstream fin("./data/gamedata/effect/generateSubRandom.json");
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
		this->subImagePath = j["subImagePathDefault"];
		this->subImageSize = j["subImageSizeDefault"];
		this->subMoveSpeed = j["subMoveSpeedDefault"];
		this->subLife = j["subLifeDefault"];
		this->subGenerateDistance = j["subGenerateDistanceDefault"];
		this->moveVec = j["moveVecDefault"];
		this->life = j["lifeDefault"];
		this->rotateLoop = j["rotateLoopDefault"];
		//----------------------------------
		//（★★使用する場合はmyPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 

		return j;
	}

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//--------------------------------------------------------
	//初期化処理
	bool GenerateSubRandom::InitializeCustom() {
		//データ読み込み
		this->data = DataGenerateSubRandom::UseData();

		//★初期化処理
		this->SetSubImage(this->data->subImagePath, this->data->subImageSize);
		this->SetSubMove(this->data->subMoveSpeed, this->data->subLife, this->data->subGenerateDistance);
		this->setMoveVec(this->data->moveVec);
		this->setLife(this->data->life);
		this->setRotateLoop(this->data->rotateLoop.adds, this->data->rotateLoop.lpmax, this->data->rotateLoop.lpmin);
		return true;
	}
	//--------------------------------------------------------
	void GenerateSubRandom::UpDate() {
		//移動
		this->pos += this->moveVec;

		//サブエフェクト生成
		if (this->moveCnt % this->subGenerateDistance == 0) {
			auto effect = MoveStraight::Create(this->subImgFilePath, this->subImgSize, true);
			effect->setPos(this->pos);
			//移動方向設定
			float angle = ML::ToRadian(rand() % 360);
			ML::Vec2 moveVecS(cosf(angle), sinf(angle));
			moveVecS *= this->subMoveSpeed;
			effect->setMoveVec(moveVecS);
			//生存時間設定
			effect->setLife(this->subLife);
		}

		//回転
		this->loopRotate.Addto(this->transform.rotateDeg);

		this->CountLife();
		++this->moveCnt;
	}
	//--------------------------------------------------
	//その他メソッド
	//--------------------------------------------------
	//サブエフェクト画像設定
	void GenerateSubRandom::SetSubImage(string filePath_, ML::Size2 size_) {
		this->subImgFilePath = filePath_;
		this->subImgSize = size_;
	}
	//サブエフェクト挙動設定
	void GenerateSubRandom::SetSubMove(float moveSpeed_, int life_, int generateDistance_) {
		this->subMoveSpeed = moveSpeed_;
		this->subLife = life_;
		this->subGenerateDistance = generateDistance_;
	}

	//--------------------------------------------------------
	//コンストラクタ
	GenerateSubRandom::GenerateSubRandom()
		:EffectStd()
	{}
	//--------------------------------------------------------
	//デストラクタ
	GenerateSubRandom::~GenerateSubRandom() {}

	//--------------------------------------------------------
	//エフェクト生成窓口
	GenerateSubRandom::SP GenerateSubRandom::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		GenerateSubRandom::SP ob = GenerateSubRandom::SP(new GenerateSubRandom());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録

			}
			if (!ob->B_Initialize(imgFilePath_, size_)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//ゲームエンジンからデータを受け取る
	DataGenerateSubRandom::SP DataGenerateSubRandom::UseData() {
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
				if (typeid(*it) == typeid(DataGenerateSubRandom)) {
					return dynamic_pointer_cast<DataGenerateSubRandom>(it);
				}
			}
			//見つからなかった場合、使用しない設定に切り替える
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//すでに値が決まっている場合の処理
		return dynamic_pointer_cast<DataGenerateSubRandom>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataGenerateSubRandom::DataGenerateSubRandom() { }		//コンストラクタ（特に処理なし）
	//-------------------------------------------------------------------
	DataGenerateSubRandom::~DataGenerateSubRandom() { }		//デストラクタ（特に処理なし）
	//-------------------------------------------------------------------






	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//移動のみのエフェクト
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// データ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
	int DataMoveStraight::myNum = DataMoveStraight::NODATA;
	//-------------------------------------------------------------------

	//ゲームモードを読み込む
	json DataMoveStraight::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//ファイルパス
			std::ifstream fin("./data/gamedata/effect/effectMoveStraight.json");
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
		this->moveVec = j["moveVec"];
		this->life = j["life"];
		//----------------------------------
		//（★★使用する場合はmyPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 

		return j;
	}

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//--------------------------------------------------------
	//初期化処理
	bool MoveStraight::InitializeCustom() {
		//データ読み込み
		this->data = DataMoveStraight::UseData();

		//★初期化処理
		this->setMoveVec(this->data->moveVec);
		this->setLife(this->data->life);
		return true;
	}
	//--------------------------------------------------
	//その他メソッド
	//--------------------------------------------------



	//--------------------------------------------------------
	//コンストラクタ
	MoveStraight::MoveStraight()
		:EffectStd()
	{}
	//--------------------------------------------------------
	//デストラクタ
	MoveStraight::~MoveStraight() {}

	//--------------------------------------------------------
	//エフェクト生成窓口
	MoveStraight::SP MoveStraight::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		MoveStraight::SP ob = MoveStraight::SP(new MoveStraight());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録

			}
			if (!ob->B_Initialize(imgFilePath_, size_)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//ゲームエンジンからデータを受け取る
	DataMoveStraight::SP DataMoveStraight::UseData() {
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
				if (typeid(*it) == typeid(DataMoveStraight)) {
					return dynamic_pointer_cast<DataMoveStraight>(it);
				}
			}
			//見つからなかった場合、使用しない設定に切り替える
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//すでに値が決まっている場合の処理
		return dynamic_pointer_cast<DataMoveStraight>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataMoveStraight::DataMoveStraight() { }		//コンストラクタ（特に処理なし）
	//-------------------------------------------------------------------
	DataMoveStraight::~DataMoveStraight() { }		//デストラクタ（特に処理なし）
	//-------------------------------------------------------------------

	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//中心に向かって動くエフェクト
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// データ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
	int DataMoveToCenter::myNum = DataMoveToCenter::NODATA;
	//-------------------------------------------------------------------

	//ゲームモードを読み込む
	json DataMoveToCenter::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//ファイルパス
			std::ifstream fin("./data/effectData/effectSample.json");
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
		this->radius = j["distanceDefault"];
		this->generateDistance = j["generateDistanceDefault"];
		this->speed = j["speedDefault"];
		this->centerPos = j["centerPosDefault"];
		//----------------------------------
		//（★★使用する場合はmyPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 

		return j;
	}

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//--------------------------------------------------------
	//初期化処理
	bool MoveToCenter::InitializeCustom() {
		//データ読み込み
		this->data = DataMoveToCenter::UseData();

		//★初期化処理
		this->setRadius(this->data->radius);
		this->setGenerateDistance(this->data->generateDistance);
		this->setSpeed(this->data->speed);
		this->setPos(this->data->centerPos);
		this->moveCnt = 0;
		return true;
	}
	//--------------------------------------------------------
	//更新処理
	void MoveToCenter::UpDate() {
		if (this->moveCnt % this->generateDistance == 0) {
			auto sub = MoveStraight::Create(this->img->get_filePath(), this->img->get_size(), true);
			float angle = ML::ToRadian(rand() % 360);
			ML::Vec2 moveVecN(cosf(angle) * this->radius, sinf(angle) * this->radius);
			sub->setPos(ML::Vec2(moveVecN * this->radius));
			sub->setMoveVec(ML::Vec2(moveVecN * this->speed * -1));
		}
		++this->moveCnt;
	}
	//--------------------------------------------------
	//その他メソッド
	//--------------------------------------------------



	//--------------------------------------------------------
	//コンストラクタ
	MoveToCenter::MoveToCenter()
		:EffectStd()
	{}
	//--------------------------------------------------------
	//デストラクタ
	MoveToCenter::~MoveToCenter() {}

	//--------------------------------------------------------
	//エフェクト生成窓口
	MoveToCenter::SP MoveToCenter::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		MoveToCenter::SP ob = MoveToCenter::SP(new MoveToCenter());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録

			}
			if (!ob->B_Initialize(imgFilePath_, size_)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//ゲームエンジンからデータを受け取る
	DataMoveToCenter::SP DataMoveToCenter::UseData() {
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
				if (typeid(*it) == typeid(DataMoveToCenter)) {
					return dynamic_pointer_cast<DataMoveToCenter>(it);
				}
			}
			//見つからなかった場合、使用しない設定に切り替える
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//すでに値が決まっている場合の処理
		return dynamic_pointer_cast<DataMoveToCenter>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataMoveToCenter::DataMoveToCenter() { }		//コンストラクタ（特に処理なし）
	//-------------------------------------------------------------------
	DataMoveToCenter::~DataMoveToCenter() { }		//デストラクタ（特に処理なし）
	//-------------------------------------------------------------------


	/*
	
	----エフェクトひな形----

	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//エフェクト説明欄
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// データ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
	int EffectSampleData::myNum = EffectSampleData::NODATA;
	//-------------------------------------------------------------------

	//ゲームモードを読み込む
	json EffectSampleData::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//ファイルパス
			std::ifstream fin("./data/effectData/effectSample.json");
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

		//----------------------------------
		//（★★使用する場合はmyPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 

		return j;
	}

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//--------------------------------------------------------
	//初期化処理
	bool EffectSample::InitializeCustom() {
		//データ読み込み
		this->data = EffectSampleData::UseData();

		//★初期化処理

		return true;
	}
	//--------------------------------------------------
	//その他メソッド
	//--------------------------------------------------



	//--------------------------------------------------------
	//コンストラクタ
	EffectSample::EffectSample()
		:EffectStd()
	{}
	//--------------------------------------------------------
	//デストラクタ
	EffectSample::~EffectSample() {}

	//--------------------------------------------------------
	//エフェクト生成窓口
	EffectSample::SP EffectSample::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		EffectSample::SP ob = EffectSample::SP(new EffectSample());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録

			}
			if (!ob->B_Initialize(imgFilePath_, size_)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//ゲームエンジンからデータを受け取る
	EffectSampleData::SP EffectSampleData::UseData() {
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
				if (typeid(*it) == typeid(EffectSampleData)) {
					return dynamic_pointer_cast<EffectSampleData>(it);
				}
			}
			//見つからなかった場合、使用しない設定に切り替える
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//すでに値が決まっている場合の処理
		return dynamic_pointer_cast<EffectSampleData>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	EffectSampleData::EffectSampleData() { }		//コンストラクタ（特に処理なし）
	//-------------------------------------------------------------------
	EffectSampleData::~EffectSampleData() { }		//デストラクタ（特に処理なし）
	//-------------------------------------------------------------------
	*/
}

