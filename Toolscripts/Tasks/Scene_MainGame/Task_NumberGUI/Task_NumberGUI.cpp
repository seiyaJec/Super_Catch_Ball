//-------------------------------------------------------------------
//ゲームUI
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_NumberGUI.h"

namespace  NumberGUI
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
			std::ifstream fin("./data/gamedata/numberGUI.json");
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

		this->scorePos = j["scorePos"];
		this->num_oneOfImgSize = j["num_oneOfImageSize"];
		this->num_imagesNumSize = j["num_imagesNumSize"];
		this->num_imagesNumAll = j["num_imagesNumAll"];
		this->num_filePath = j["num_filePath"];

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
		//数字
		numImgs = ML::PartImage::Create(
			this->data->num_filePath		//画像ファイルパス
			, this->data->num_imagesNumAll		//分割総数
			, (int)this->data->num_imagesNumSize.w	//横の分割数
			, (int)this->data->num_imagesNumSize.h	//縦の分割数
			, (int)this->data->num_oneOfImgSize.w		//分割した画像の横幅
			, (int)this->data->num_oneOfImgSize.h		//分割した画像の縦幅
		);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->numImgs.reset();
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
		this->render2D_Priority[1] = 0.3f;
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
		//プレイヤー表示
		//DrawNumber(this->scorePos, ge->qa_playerBase->score);

	}
	//-------------------------------------------------------------------
	//関数定義
	//数字の描画
	ML::Vec2 Object::DrawNumber(const ML::Vec2& pos_, int num_, bool useSign_, int digit_, float alpha_) {
		int digit = 0;
		int checkDigit = 1;		//判定する位
		//桁数を計算
		if (num_ != 0) {
			while (num_ / checkDigit != 0) {
				++digit;
				checkDigit *= 10;
			}
		}
		//数字が0の場合の処理
		else {
			digit = 1;
		}
		//負の数の場合、正に変更（桁数がマイナスになることはないため）
		digit = (int)fabs(digit);

		//引数の桁数のほうが大きければ合わせる
		digit = max(digit, digit_);


		//1桁目（一の位）の左上座標を計算
		ML::Size2 imgSize(
			(float)this->res->numImgs->get_xSize()
			, (float)this->res->numImgs->get_ySize()
		);
		int numSizeW = (int)imgSize.w * digit;
		ML::Vec2 numDrawPos(
			pos_.x + numSizeW / 2 - imgSize.w
			, pos_.y - imgSize.h
		);

		//桁数に合わせた位置に数字を描画
		int drawNum = num_;
		for (int i = 0; i < digit; ++i) {
			this->res->numImgs->Draw(numDrawPos, drawNum % 10, true, alpha_);
			numDrawPos.x -= imgSize.w;
			drawNum /= 10;
		}
		//符号の表示をする場合の処理
		if (useSign_) {
			if (num_ > 0) {
				this->res->numImgs->Draw(numDrawPos, 10, true, alpha_);
			}
			else if (num_ < 0) {
				this->res->numImgs->Draw(numDrawPos, 11, true, alpha_);
			}
		}

		
		return numDrawPos;
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