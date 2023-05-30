//-------------------------------------------------------------------
//シーン遷移
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "SceneTransition.h"

namespace  SceneTransition
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
	//標準シーン遷移
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□


		//-------------------------------------------------------------------
		//「初期化」タスク生成時に１回だけ行う処理
	bool  SceneTransitionStd::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		//デフォルト表示優先度設定
		this->render2D_Priority[1] = 0.1f;

		//追加の初期化処理
		return this->InitializeCustom();
	}
	//-------------------------------------------------------------------
	//追加の初期化処理（スーパークラスなので特になし）
	bool  SceneTransitionStd::InitializeCustom() {
		return true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  SceneTransitionStd::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成

		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  SceneTransitionStd::UpDate()
	{
		//出現時の処理
		if (this->CanDisappear() == false) {
			if (this->appearFlag == true) {
				this->AppearUpDate();
			}
		}
		//消滅時の処理
		else{
			this->DisappearUpDate();
		}
	}
	//-------------------------------------------------------------------
	//追加の更新処理（スーパークラスなので特になし）
	void  SceneTransitionStd::AppearUpDate() {

	}
	void SceneTransitionStd::DisappearUpDate() {

	}
	//-------------------------------------------------------------------
	//消滅判定
	bool SceneTransitionStd::CanDisappear() {
		return
			this->finishAppearFlag == true
			&& this->disappearFlag == true;
	}
	//-------------------------------------------------------------------
	//「３Ｄ描画」１フレーム毎に行う処理、レイヤー0
	void SceneTransitionStd::Render3D_L0()
	{

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理、3D描画後に行われる
	void  SceneTransitionStd::Render2D_AF()
	{
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	// 生成関連メソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	bool  SceneTransitionStd::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	SceneTransitionStd::~SceneTransitionStd() { this->B_Finalize(); }
	bool  SceneTransitionStd::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	SceneTransitionStd::SceneTransitionStd()
		:transitionSpeed(0)
		,color()
		,appearFlag(false)
		,disappearFlag(false)
	{	}


	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//シーン遷移横分割逆ピラミッド
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// データ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
	int DataRevPyramid::myNum = DataRevPyramid::NODATA;
	//-------------------------------------------------------------------

	//ゲームモードを読み込む
	json DataRevPyramid::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//ファイルパス
			std::ifstream fin("./data/gamedata/sceneTransition/reversePyramid.json");
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
		this->cuts = j["cuts"];
		this->distance = j["distance"];
		this->moveSpeed = j["moveSpeed"];


		//----------------------------------
		//（★★使用する場合はmyPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 

		return j;
	}

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//--------------------------------------------------------
	//初期化処理
	bool RevPyramid::InitializeCustom() {
		//データ読み込み
		this->data = DataRevPyramid::UseData();

		//★初期化処理
		this->Set(this->data->cuts, this->data->distance, this->data->moveSpeed);
		this->disappearCnt = 0;

		return true;
	}
	//--------------------------------------------------------
	//出現処理
	void RevPyramid::AppearUpDate() {
		//分割箱移動
		for (auto& box : this->cutBoxes) {
			box.x -= this->moveSpeed;
			if (box.x < 0.0f) {
				box.x = 0.0f;
			}
		}

		//出現終了判定
		if (this->cutBoxes[this->cutBoxes.size() - 1].x == 0.0f) {
			this->finishAppearFlag = true;
		}
	}

	//--------------------------------------------------------
	//消滅処理
	void RevPyramid::DisappearUpDate() {
		//分割箱移動
		for (int c = 0; c < this->cutBoxes.size(); ++c) {
			if (this->disappearCnt * this->moveSpeed > this->distance * c) {
				this->cutBoxes[c].x -= this->moveSpeed;
			}
		}

		//消滅終了判定
		if (this->cutBoxes[this->cutBoxes.size() - 1].x <= -(float)ge->screenWidth) {
			this->Kill();
		}
		++this->disappearCnt;
	}
	//--------------------------------------------------------
	//描画処理
	void RevPyramid::Render2D_AF() {
		for (const auto& box : this->cutBoxes) {
			ML::DrawBox(box, this->color, true);
		}
	}

	//--------------------------------------------------------
	//--------------------------------------------------
	//その他メソッド
	//--------------------------------------------------
	void RevPyramid::Set(int cuts_, float distance_, float moveSpeed_) {
		this->cuts = cuts_;
		this->distance = distance_;
		this->moveSpeed = moveSpeed_;
		cutBoxes.clear();
		//シーン遷移ボックスの生成
		for (int c = 0; c < this->cuts; ++c) {
			cutBoxes.push_back(ML::Box2D(
				(float)ge->screenWidth + (this->distance * c),	//x
				(float)ge->screenHeight / this->cuts * c,		//y
				(float)ge->screenWidth,							//w
				(float)ge->screenHeight / this->cuts			//h
			));
		}
	}


	//--------------------------------------------------------
	//コンストラクタ
	RevPyramid::RevPyramid()
		:SceneTransitionStd()
	{}
	//--------------------------------------------------------
	//デストラクタ
	RevPyramid::~RevPyramid() {}

	//--------------------------------------------------------
	//エフェクト生成窓口
	RevPyramid::SP RevPyramid::Create(bool flagGameEnginePushBack_) {
		RevPyramid::SP ob = RevPyramid::SP(new RevPyramid());
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
	//ゲームエンジンからデータを受け取る
	DataRevPyramid::SP DataRevPyramid::UseData() {
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
				if (typeid(*it) == typeid(DataRevPyramid)) {
					return dynamic_pointer_cast<DataRevPyramid>(it);
				}
			}
			//見つからなかった場合、使用しない設定に切り替える
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//すでに値が決まっている場合の処理
		return dynamic_pointer_cast<DataRevPyramid>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataRevPyramid::DataRevPyramid() { }		//コンストラクタ（特に処理なし）
	//-------------------------------------------------------------------
	DataRevPyramid::~DataRevPyramid() { }		//デストラクタ（特に処理なし）
	//-------------------------------------------------------------------






	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//円拡大
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// データ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
	int DataCircleScaleUp::myNum = DataCircleScaleUp::NODATA;
	//-------------------------------------------------------------------

	//ゲームモードを読み込む
	json DataCircleScaleUp::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//ファイルパス
			std::ifstream fin("./data/gamedata/sceneTransition/circleScaleUp.json");
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
		this->scaleUpSpeed = j["scaleUpSpeed"];

		//----------------------------------
		//（★★使用する場合はmyPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 

		return j;
	}

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//--------------------------------------------------------
	//初期化処理
	bool CircleScaleUp::InitializeCustom() {
		//データ読み込み
		this->data = DataCircleScaleUp::UseData();

		//★初期化処理
		this->scaleUpSpeed = this->data->scaleUpSpeed;
		this->radius = 0;
		this->moveCnt = 0;
		this->lineThickness = 0;

		return true;
	}
	//--------------------------------------------------------
	//出現処理
	void CircleScaleUp::AppearUpDate() {
		
		this->radius += scaleUpSpeed;
		float sdl = ge->screenDiagonalLength();
		if (this->radius >= sdl / 2) {
			this->radius = sdl / 2;
			this->lineThickness = sdl;
			this->finishAppearFlag = true;
		}
		++this->moveCnt;
	}

	//--------------------------------------------------------
	//消滅処理
	void CircleScaleUp::DisappearUpDate() {
		this->lineThickness -= this->scaleUpSpeed * 2;
		if (this->lineThickness <= 0) {
			this->Kill();
		}
	}
	//--------------------------------------------------------
	//描画処理
	void CircleScaleUp::Render2D_AF() {
		ML::Vec2 center = ge->screenCenter();
		if (this->CanDisappear() == false) {
			DxLib::DrawCircle((int)center.x, (int)center.y, this->radius, ML::GetColor(this->color), true);
		}
		else {
			DxLib::DrawCircle((int)center.x, (int)center.y, this->radius, ML::GetColor(this->color), true, this->lineThickness);
		}
		
	}

	//--------------------------------------------------------
	//--------------------------------------------------
	//その他メソッド
	//--------------------------------------------------
	void CircleScaleUp::Set(int scaleUpSpeed_) {
		this->scaleUpSpeed = scaleUpSpeed_;
	}


	//--------------------------------------------------------
	//コンストラクタ
	CircleScaleUp::CircleScaleUp()
		:SceneTransitionStd()
	{}
	//--------------------------------------------------------
	//デストラクタ
	CircleScaleUp::~CircleScaleUp() {}

	//--------------------------------------------------------
	//エフェクト生成窓口
	CircleScaleUp::SP CircleScaleUp::Create(bool flagGameEnginePushBack_) {
		CircleScaleUp::SP ob = CircleScaleUp::SP(new CircleScaleUp());
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
	//ゲームエンジンからデータを受け取る
	DataCircleScaleUp::SP DataCircleScaleUp::UseData() {
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
				if (typeid(*it) == typeid(DataCircleScaleUp)) {
					return dynamic_pointer_cast<DataCircleScaleUp>(it);
				}
			}
			//見つからなかった場合、使用しない設定に切り替える
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//すでに値が決まっている場合の処理
		return dynamic_pointer_cast<DataCircleScaleUp>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataCircleScaleUp::DataCircleScaleUp() { }		//コンストラクタ（特に処理なし）
	//-------------------------------------------------------------------
	DataCircleScaleUp::~DataCircleScaleUp() { }		//デストラクタ（特に処理なし）
	//-------------------------------------------------------------------


	/*
	
	----エフェクトひな形----

	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//円拡大
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// データ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
	int DataSampleScTr::myNum = DataSampleScTr::NODATA;
	//-------------------------------------------------------------------

	//ゲームモードを読み込む
	json DataSampleScTr::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//ファイルパス
			std::ifstream fin("./data/gamedata/sceneTransition/SampleScTr.json");
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
	bool SampleScTr::InitializeCustom() {
		//データ読み込み
		this->data = DataSampleScTr::UseData();

		//★初期化処理


		return true;
	}
	//--------------------------------------------------------
	//出現処理
	void SampleScTr::AppearUpDate() {

	}

	//--------------------------------------------------------
	//消滅処理
	void SampleScTr::DisappearUpDate() {

	}
	//--------------------------------------------------------
	//描画処理
	void SampleScTr::Render2D_AF() {


	}

	//--------------------------------------------------------
	//--------------------------------------------------
	//その他メソッド
	//--------------------------------------------------
	void SampleScTr::Set(int cuts_, float distance_, float moveSpeed_) {

	}


	//--------------------------------------------------------
	//コンストラクタ
	SampleScTr::SampleScTr()
		:SceneTransitionStd()
	{}
	//--------------------------------------------------------
	//デストラクタ
	SampleScTr::~SampleScTr() {}

	//--------------------------------------------------------
	//エフェクト生成窓口
	SampleScTr::SP SampleScTr::Create(bool flagGameEnginePushBack_) {
		SampleScTr::SP ob = SampleScTr::SP(new SampleScTr());
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
	//ゲームエンジンからデータを受け取る
	DataSampleScTr::SP DataSampleScTr::UseData() {
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
				if (typeid(*it) == typeid(DataSampleScTr)) {
					return dynamic_pointer_cast<DataSampleScTr>(it);
				}
			}
			//見つからなかった場合、使用しない設定に切り替える
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//すでに値が決まっている場合の処理
		return dynamic_pointer_cast<DataSampleScTr>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataSampleScTr::DataSampleScTr() { }		//コンストラクタ（特に処理なし）
	//-------------------------------------------------------------------
	DataSampleScTr::~DataSampleScTr() { }		//デストラクタ（特に処理なし）
	//-------------------------------------------------------------------
	*/
}

