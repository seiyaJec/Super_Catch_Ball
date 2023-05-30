//-------------------------------------------------------------------
//3Dエフェクト
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Effect3D.h"

namespace  Effect3D
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

