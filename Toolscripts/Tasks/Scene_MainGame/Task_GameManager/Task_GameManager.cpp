//-------------------------------------------------------------------
//ゲーム本編管理
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_GameManager.h"
#include  "../Task_Border/Task_Border.h"
#include "../../Effects/Task_Effect2D/Effect2DIncludes.h"

namespace  GameManager
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
			std::ifstream fin("./data/gamedata/gameManager.json");
			if (!fin) { return json(); }
			fin >> j;
			//ファイル読み込み終了
			fin.close();
		}
		else {
			j = ge->gameData["data"][this->myArg];
		}

		//----------------------------------
		//★データ読み込み処理

		this->player1pos = j["player1pos"];
		this->player1defaultAngle = j["player1defaultAngle"];
		this->ground1pos = j["ground1pos"];
		this->border1posZ = j["border1posZ"];
		this->border1checkArea = j["border1checkArea"];

		this->player2pos = j["player2pos"];
		this->player2defaultAngle = j["player2defaultAngle"];
		this->ground2pos = j["ground2pos"];
		this->border2posZ = j["border2posZ"];
		this->border2checkArea = j["border2checkArea"];

		this->playerMoveAreaW = j["playerMoveAreaW"];
		this->playerMoveAreaD = j["playerMoveAreaD"];

		this->beforeGameTime = j["beforeGameTime"];
		this->gameTime = j["gameTime"];

		this->drawMoveArea = j["drawMoveArea"];

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
		ge->qa_gameManager = ge->GetTask<GameManager::Object>("ゲーム管理", "NoName");
		this->debugFont = ML::Font::Create("MS UI Gothic", 50);

		//★タスクの生成
		////プレイヤー
		//auto p1 = PlayerBase::Object::Create(true);
		//p1->pos = this->res->data->player1pos;
		//p1->defaultAngle = this->res->data->player1defaultAngle;
		//p1->num = 0;
		//auto p2 = PlayerBase::Object::Create(true);
		//p2->pos = this->res->data->player2pos;
		//p2->defaultAngle = this->res->data->player2defaultAngle;
		//p2->num = 1;

		//地面
		auto g1 = Ground::Object::Create(true);
		g1->pos = this->res->data->ground1pos;
		auto g2 = Ground::Object::Create(true);
		g1->pos = this->res->data->ground2pos;

		////場外判定
		//auto bd1 = Border::Object::Create(true);
		//bd1->posZ = this->res->data->border1posZ;
		//bd1->checkArea = this->res->data->border1checkArea;
		//bd1->linkingGround = g2;
		//auto bd2 = Border::Object::Create(true);
		//bd2->posZ = this->res->data->border2posZ;
		//bd2->checkArea = this->res->data->border2checkArea;
		//bd2->linkingGround = g1;

		////ボール
		//ge->qa_ball = Ball::Object::Create(true);


		//数字
		ge->qa_numberGUI = NumberGUI::Object::Create(true);

		//画面分割枠
		Frame::Object::Create(true);

		//時間
		this->beforeGameTimer = TimeCount::Object::Create(true);
		this->beforeGameTimer->SetTimeMax(this->res->data->beforeGameTime);
		this->beforeGameTimer->SetDraw(true);
		this->timer = TimeCount::Object::Create(true);
		this->timer->SetTimeMax(this->res->data->gameTime);
		this->timer->SetCounting(false);
		this->timer->SetDraw(false);

		////スコア
		//ge->qa_scoreJudge = ScoreJudge::Object::Create(true);

		//ゲームモードに切り替え（仮）
		this->SetGameBaseTaskMode(GameBase::StateOfGame::BeforeGame);

		//クリエイター
		auto creator = Creator::Object::Create(true);
		creator->pos.x = g1->pos.x;
		creator->pos.y = this->res->data->ground2pos.z - this->res->data->ground1pos.z;
		creator->pos.z = g2->pos.z + g1->pos.z / 2;


		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->qa_ball.reset();
		ge->qa_numberGUI.reset();
		ge->qa_scoreJudge.reset();
		this->timer.reset();

		//ゲームタスクキル
		auto gameTasks = this->GetGameTask();
		for (auto& gameTask : *gameTasks) {
			gameTask->Kill();
		}

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			ResultManager::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	//ーーゲーム前ーー//
	void Object::UpDateBeforeGame()
	{

		//キー押下でモード切替
		if (ge->inpkey.Down(KEY_INPUT_Z)) {
			this->stateOfGame = StateOfGame::InGame;
			this->SetGameBaseTaskMode(StateOfGame::InGame);
		}
			
	}

	//ーーメインゲームーー//
	void  Object::UpDateGame()
	{
		//キー押下でモード切替
		if (ge->inpkey.Down(KEY_INPUT_Z)) {
			this->stateOfGame = StateOfGame::BeforeGame;
			this->SetGameBaseTaskMode(StateOfGame::BeforeGame);
		}
	}
	//-------------------------------------------------------------------
	//「３Ｄ描画」１フレーム毎に行う処理、レイヤー0
	void Object::Render3D_L0()
	{


		//デバッグ時の追加描画処理
		if (this->res->data->drawMoveArea == true) {
			//プレイヤー移動可能範囲設定
			ML::Obb3 drawArea1(
				this->res->data->player1pos,
				ML::Size3(this->res->data->playerMoveAreaW, 100.0f, this->res->data->playerMoveAreaD)
			);
			ML::Obb3 drawArea2 = drawArea1.SetPosCopy(this->res->data->player2pos);

			//描画
			ML::DrawBox3D(drawArea1, ML::Color(1, 1, 0, 0));
			ML::DrawBox3D(drawArea2, ML::Color(1, 1, 0, 0));
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理、3D描画後に行われる
	void  Object::Render2D_AF()
	{
		if (this->stateOfGame == StateOfGame::BeforeGame) {
			this->debugFont->Draw(ML::Vec2(1100, 1000), "EditorMode");
		}
		else {
			this->debugFont->Draw(ML::Vec2(1100, 1000), "GameMode");
		}
	}
	//-------------------------------------------------------------------
	//プレイヤーが範囲外に出ていたら戻す処理
	void Object::CheckPlayerInArea(ML::Vec3& playerPos_, int playerNum_) {
		//中心座標取得
		ML::Vec3 centerPos;
		if (playerNum_ == 0) {
			centerPos = this->res->data->player1pos;
		}
		else {
			centerPos = this->res->data->player2pos;
		}
		//移動可能範囲計算
		ML::Rect2 moveArea(
			centerPos.x - (this->res->data->playerMoveAreaW / 2),
			centerPos.x + (this->res->data->playerMoveAreaW / 2 - 1),
			centerPos.z - (this->res->data->playerMoveAreaD / 2),
			centerPos.z + (this->res->data->playerMoveAreaD / 2 - 1)
		);

		//手前、奥、左右で判定
		//左
		if (playerPos_.x < moveArea.l) {
			playerPos_.x = moveArea.l;
		}
		//手前
		if (playerPos_.z < moveArea.t) {
			playerPos_.z = moveArea.t;
		}
		//右
		if (playerPos_.x > moveArea.r) {
			playerPos_.x = moveArea.r;
		}
		//奥
		if (playerPos_.z > moveArea.b) {
			playerPos_.z = moveArea.b;
		}

	}

	//ゲーム本編タスクを受け取る（消滅処理などの一括設定などに使用する）
	shared_ptr<vector<BTask::SP>> Object::GetGameTask() {
		auto gameTasks = make_shared<vector<BTask::SP>>();	//タスク配列
		shared_ptr<vector<BTask::SP>> oneGroupTasks;
		//ゲームタスクテーブル（タスクが増えたらこの配列も追加する）
		string gameTaskGroups[] = {
			"ボール",
			"場外判定",
			"分割画面用の枠",
			"地面",
			"数字GUI",
			"プレイヤー",
			"スコア加算値",
			"スコア判定",
			"的",
			"的生成",
			"時間計測",
		};
		//テーブルに一致するグループ名のタスクを、タスク配列に追加していく
		for (auto& group : gameTaskGroups) {
			oneGroupTasks = ge->GetTasks<BTask>(group);
			gameTasks->insert(gameTasks->end(), oneGroupTasks->begin(), oneGroupTasks->end());
		}

		return gameTasks;
	}


	//ゲームベースタスクのステータス切り替え
	void Object::SetGameBaseTaskMode(GameBase::StateOfGame state_) {
		auto gameTasks = this->GetGameTask();	//ゲームタスク追加
		for (auto& gameTask : *gameTasks) {
			//ステータスを持つタスクなら切り替え
			GameBase* gameBaseTask = dynamic_cast<GameBase*>(gameTask.get());
			if (gameBaseTask != nullptr) {
				gameBaseTask->stateOfGame = state_;
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