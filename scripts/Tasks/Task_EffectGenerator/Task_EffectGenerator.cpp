//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../Engine/myPG/myPG.h"
#include  "Task_EffectGenerator.h"

namespace  EffectGenerator
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
			std::ifstream fin("./data/gamedata/effectGenerator.json");
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
		auto& jReady = j["effectReady"];
		this->imageReady = jReady["imageData"];
		this->readyMoveSpeed = jReady["moveSpeed"];
		this->readyTimeStayCenter = jReady["timeStayCenter"];

		auto& jStart = j["effectStart"];
		this->imageStart = jStart["imageData"];
		this->startScaleDecSpeed = jStart["scaleDecSpeed"];
		this->startScaleMax = jStart["scaleMax"];
		this->startAlphaDecSpeed = jStart["alphaDecSpeed"];
		this->startAlphaWaitDec = jStart["alphaWaitDec"];
		
		auto& jFinish = j["effectFinish"];
		this->imageFinish = jFinish["imageData"];
		this->finishScaleDecSpeed = jFinish["scaleDecSpeed"];
		this->finishScaleMax = jFinish["scaleMax"];
		this->finishAlphaDecSpeed = jFinish["alphaDecSpeed"];
		this->finishAlphaWaitDec = jFinish["alphaWaitDec"];

		auto& jGBall = j["effectGorgeousBall"];
		this->imageGBall = jGBall["imageData"];
		this->imageGBallSub = jGBall["subImageData"];
		this->GBallMoveVec = jGBall["moveVec"];
		this->GBallLife = jGBall["life"];
		this->GBallRotateLoop = jGBall["rotateLoop"];
		this->GBallSubMoveSpeed = jGBall["subMoveSpeed"];
		this->GBallSubLife = jGBall["subLife"];
		this->GBallSubGenerateDistance = jGBall["subGenerateDistance"];
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
	}
	//-------------------------------------------------------------------
	//「３Ｄ描画」１フレーム毎に行う処理、レイヤー0
	void Object::Render3D_L0()
	{
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理、3D描画後に行われる
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//その他関数
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const ImageData& imgData_) {
		j_ = json{
			{"filePath", imgData_.filePath},
			{"size", imgData_.size}
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, ImageData& imgData_) {
		j_.at("filePath").get_to(imgData_.filePath);
		j_.at("size").get_to(imgData_.size);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//エフェクトの生成
	void Object::GenerateEffect(int effectsNum_, ML::Vec2 pos_) {

		auto& data = this->res->data;
		switch (effectsNum_) {
			//Ready
		case EffectsNum::Ready: {
			auto effect = Effect2D::EffectL_C_R::Create(data->imageReady.filePath, data->imageReady.size, true);
			effect->setMoveSpeed(data->readyMoveSpeed);
			effect->setTimeStayCenter(data->readyTimeStayCenter);
			break;
		}
			  //Start
		case EffectsNum::Start: {
			auto effect = Effect2D::EffectStamp::Create(data->imageStart.filePath, data->imageStart.size, true);
			effect->setStamp(data->startScaleDecSpeed, data->startScaleMax);
			effect->setAlpha(data->startAlphaDecSpeed, data->startAlphaWaitDec);
			break;
		}
			 //Finish
		case EffectsNum::Finish: {
			auto effect = Effect2D::EffectStamp::Create(data->imageFinish.filePath, data->imageFinish.size, true);
			effect->setStamp(data->finishScaleDecSpeed, data->finishScaleMax);
			effect->setAlpha(data->finishAlphaDecSpeed, data->finishAlphaWaitDec);
			break;
		}
			 //GorgeousBall
		case EffectsNum::GorgeousBall: {
			auto effect = Effect2D::GenerateSubRandom::Create(data->imageGBall.filePath, data->imageGBall.size, true);
			effect->setPos(pos_);
			effect->setMoveVec(data->GBallMoveVec);
			effect->setLife(data->GBallLife);
			effect->setRotateLoop(data->GBallRotateLoop.adds, data->GBallRotateLoop.lpmax, data->GBallRotateLoop.lpmin);
			effect->SetSubImage(data->imageGBallSub.filePath, data->imageGBallSub.size);
			effect->SetSubMove(data->GBallSubMoveSpeed, data->GBallSubLife, data->GBallSubGenerateDistance);
			break;
		}

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