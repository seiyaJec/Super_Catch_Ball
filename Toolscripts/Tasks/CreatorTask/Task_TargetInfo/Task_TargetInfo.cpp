//-------------------------------------------------------------------
//的設定画面
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_TargetInfo.h"

namespace  TargetInfo
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
		this->font = ML::Font::Create("MS UI Gothic", 80);

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->font.reset();
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
		int inputCharMax = 10;
		ML::Vec2 viewDistance(-100, 0);
		this->viewPosX.Create(inputCharMax, true, viewDistance);
		this->viewPosZ.Create(inputCharMax, true, viewDistance);
		this->viewMoveSpeed.Create(inputCharMax, true, viewDistance);
		this->viewMoveScale.Create(inputCharMax, true, viewDistance);
		this->viewAddRot.Create(inputCharMax, true, viewDistance);



		this->viewPosX.SetData(ML::Vec2(1100, 200), "X：");
		this->viewPosZ.SetData(			ML::Vec2(1100, 350), "Z：");
		this->viewMoveSpeed.SetData(	ML::Vec2(1100, 500), "MoveSpeed：");
		this->viewMoveScale.SetData(	ML::Vec2(1100, 650), "MoveScale：");
		this->viewAddRot.SetData(		ML::Vec2(1100, 800), "AddRot：");
		this->viewingTarget.reset();

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
		this->LinkTargetToView();
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
	//関数定義
	//-------------------------------------------------------------------
	//---------------------------------
	//ビューの値を選択中の的とリンクさせる
	void Object::LinkTargetToView() {
		//選択中の的がある場合のみ行う
		auto target = this->viewingTarget.lock();
		if (target) {
			this->viewPosX.LinkValue(target->basePos.x);
			this->viewPosZ.LinkValue(target->basePos.z);
			this->viewMoveSpeed.LinkValue(target->moveSpeed);
			this->viewMoveScale.LinkValue(target->moveScaleW);
			this->viewAddRot.LinkValue(target->addRotate);
		}
	}
	//表示する的を設定する
	void Object::SetViewingTarget(Target::Object::SP target_) {
		this->viewingTarget = target_;
	}
	//-------------------------------------------------------------------
	//ビュークラスメソッド定義
	//-------------------------------------------------------------------
	//生成
	void Object::View::Create(int inputCharMax_, bool flagGameEnginePushBack_, const ML::Vec2& posDistance_) {
		this->info = Infomation::Object::Create(flagGameEnginePushBack_);
		this->input = KeyInput::Object::Create(inputCharMax_, true, true, true, flagGameEnginePushBack_);
		this->posDistance = posDistance_;
	}
	//座標・説明文章設定
	void Object::View::SetData(const ML::Vec2& pos_, const std::string& data_) {
		this->info->SetData(pos_, data_);
		ML::Vec2 inputPos = pos_ + this->posDistance;
		this->input->SetPos(inputPos);
	}
	//指定した変数と同期させる
	void Object::View::LinkValue(float& value_) {
		//入力があれば取得
		float newValue;
		if (this->input->GetInputNum(newValue) == true) {
			value_ = newValue;
		}
		//値を表示させる
		this->info->SetValue(value_);
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