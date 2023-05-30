//-------------------------------------------------------------------
//文字列入力
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_KeyInput.h"

namespace  KeyInput
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

		//例）pos = j[キーの名前]

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
		this->Inputing = false;

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
	bool  Object::Initialize(bool useCansel_, bool SingleCharOnly_, bool NumCharOnly_)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->state = State::IDLE;
		this->pos = ML::Vec2();
		this->inputing = false;
		this->inputResult = 0;
		this->handle = 0;
		this->moveCnt = 0;
		this->waitSendFlag = false;
		this->useCansel = useCansel_;
		this->singleCharOnly = SingleCharOnly_;
		this->numberCharOnly = NumCharOnly_;

		this->buttonActive = true;
		this->button.Resize(100, 100);
		this->button.Set(this->pos);
		this->buttonColNormal = ML::Color(1, 1, 1, 1);
		this->buttonColTargeting = ML::Color(1, 1, 1, 0);
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		DxLib::DeleteKeyInput(this->handle);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		this->JudgeState();
		this->Move();

		++this->moveCnt;
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
		//ボタン有効なら描画
		if (this->buttonActive) {
			this->DrawButton();
		}

		switch (this->state) {
		case State::IDLE:
			break;
		case State::INPUTING:
			DrawKeyInputString((int)this->pos.x, (int)this->pos.y, this->handle);
			break;
		case State::WAITSEND:
			break;
		}
	}
	//-------------------------------------------------------------------
	// 関数定義
	//-------------------------------------------------------------------
	//現在の状態に合わせて次の状態を判定する
	void Object::JudgeState() {
		//状態ごとに判定
		State nextState = this->state;

		switch (this->state) {
		case State::INVALID:
			if (this->res->Inputing == false) {
				nextState = State::IDLE;
			}
			break;
		case State::IDLE:
			//ボタンが押されたら入力に切り替え
			if (ge->inpms.Down(MOUSE_INPUT_1)) {
				if (this->ButtonCheckHitMouse()) {
					nextState = State::INPUTING;
				}
			}
			//他のタスクが入力中なら無効に
			if (this->res->Inputing == true) {
				nextState = State::INVALID;
			}
			break;
		case State::INPUTING: 
			//入力中以外なら送信待ちに切り替え
			if (this->inputResult != 0) {
				nextState = State::WAITSEND;
			}
			break;
		case State::WAITSEND:
			if (this->waitSendFlag == false) {
				nextState = State::IDLE;
			}
			break;
		}

		this->UpDateState(nextState);
	}
	//-----------------------------
	//状態を更新する
	void Object::UpDateState(State nextState_) {
		if (nextState_ != this->state) {
			this->state = nextState_;
			this->moveCnt = 0;
		}
	}
	//-----------------------------
	//状態に合わせて行動する
	void Object::Move() {
		//共通の処理
		this->button.Set(this->pos);



		switch (this->state) {
		case State::INVALID:
			this->buttonActive = false;
			break;
		case State::IDLE:
			if (this->moveCnt == 0) {
				this->res->Inputing = false;
				this->buttonActive = true;
			}
			break;
		case State::INPUTING:
			if (this->moveCnt == 0) {
				this->res->Inputing = true;
				this->buttonActive = false;
				this->handle = MakeKeyInput(this->charMax, this->useCansel, this->singleCharOnly, this->numberCharOnly);
				DxLib::SetActiveKeyInput(this->handle);
			}
			this->inputResult = CheckKeyInput(this->handle);

			//マウスのクリックが押されたらキャンセル
			if (this->moveCnt != 0) {
				if (ge->inpms.Down(MOUSE_INPUT_1)) {
					this->inputResult = 2;
				}
			}
			break;
		case State::WAITSEND:
			if (this->moveCnt == 0) {
				this->waitSendFlag = true;
			}
			//送信先が見つからなかった場合は送信待ち終了
			else {
				this->waitSendFlag = false;
			}
			//キャンセルされた場合は送信しない
			if (this->inputResult == 2) {
				this->waitSendFlag = false;
			}
			break;
		}
	}
	//-----------------------------
	//ボタン描画
	void Object::DrawButton() {
		//ボタンが無効なら行わない
		if (this->buttonActive == false) {
			return;
		}

		//カーソル非照準時
		if (this->ButtonCheckHitMouse() == false) {
			ML::DrawBox(this->button, this->buttonColNormal);
		}
		//照準時
		else {
			ML::DrawBox(this->button, this->buttonColTargeting);
		}
	}
	//-----------------------------
	//ボタンへのマウスのクリック判定
	bool Object::ButtonCheckHitMouse() {
		ML::Box2D msHit(0, 0, 1, 1);
		msHit.Set(ge->inpms.getPos());
		return msHit.Hit(this->button);
	}
	//-----------------------------
	//入力情報を送り、Killする（文字）
	bool Object::GetInputStr(string& str_) {
		//送信待ち出なければfalse
		if(this->waitSendFlag == false){
			return false;
		}

		char* buff = new char[this->charMax];
		DxLib::GetKeyInputString(buff, this->handle);
		str_ = buff;
		delete[] buff;
		this->waitSendFlag = true;
		return true;
	}

	//-----------------------------
	//入力情報を送り、Killする（数値）
	bool Object::GetInputNum(float& num_) {
		//送信待ち出なければfalse
		if (this->waitSendFlag == false) {
			return false;
		}
		num_ = DxLib::GetKeyInputNumberToFloat(this->handle);
		this->waitSendFlag = true;
		return true;
	}
	//-----------------------------
	//座標設定
	void Object::SetPos(ML::Vec2 pos_) {
		this->pos = pos_;
	}
	//-----------------------------
	//半角英数の有効・無効設定
	void Object::SetSingleCharOnly(bool flagSingleCharOnly_) {
		this->singleCharOnly = flagSingleCharOnly_;
	}
	//-----------------------------
	//半角数字の有効・無効設定
	void Object::SetNumberCharOnly(bool flagNumberCharOnly_) {
		this->numberCharOnly = flagNumberCharOnly_;
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(int charMax_, bool useCansel_, bool SingleCharOnly_, bool NumCharOnly_, bool flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object(charMax_));
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize(useCansel_, SingleCharOnly_, NumCharOnly_)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(bool useCansel_, bool SingleCharOnly_, bool NumCharOnly_)
	{
		return  this->Initialize(useCansel_, SingleCharOnly_, NumCharOnly_);
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object(int charMax_)
		:charMax(charMax_)
	{}
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