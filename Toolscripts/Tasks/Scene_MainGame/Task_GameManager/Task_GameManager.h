#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ゲーム本編管理
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"
#include "../Task_TimeCount/Task_TimeCount.h"

namespace  GameManager
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ゲーム管理");	//グループ名
	const  string  defName("NoName");	//タスク名
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// ゲームデータ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	class Data : public BData
	{
	public:
		using SP = shared_ptr<Data>;

		json LoadDataJson(bool useBinary_)						override;	//テキストデータ読み込み
		Data();			//コンストラクタ（特に処理なし）
		~Data();		//デストラクタ  （特に処理なし）
		static Data::SP UseData();	//ゲームエンジンからデータを受け取る
		static int myNum;			//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
		//（★★使用する場合、myPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 
		//-----------------------------
		//データ変数は以下に追加する
		//プレイヤー1サイド
		ML::Vec3	player1pos;				//プレイヤー1初期座標
		ML::Vec3	player1defaultAngle;	//プレイヤー1の標準の向き
		ML::Vec3	ground1pos;				//地面1の座標
		float		border1posZ;			//場外判定1のz座標
		string		border1checkArea;		//場外判定1の判定エリア（"z+"もしくは"z-"）
		//プレイヤー2サイド
		ML::Vec3	player2pos;				//プレイヤー2初期座標
		ML::Vec3	player2defaultAngle;	//プレイヤー2の標準の向き
		ML::Vec3	ground2pos;				//地面2の座標
		float		border2posZ;			//場外判定2のz座標
		string		border2checkArea;		//場外判定2の判定エリア（"z+"もしくは"z-"）

		//プレイヤー移動可能範囲
		float			playerMoveAreaW;		//プレイヤー移動可能範囲＿横幅
		float			playerMoveAreaD;		//プレイヤー移動可能範囲＿奥行

		float		beforeGameTime;	//ゲーム前時間
		float		gameTime;		//ゲーム制限時間

		//エリア表示（デバッグ）
		bool		drawMoveArea;			//プレイヤー移動可能範囲描画

		//-----------------------------
	};

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// リソース
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		Data::SP		data;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		
	};

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	class  Object : public  GameBase
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDateBeforeGame()	override;//「ゲーム前処理」
		void  UpDateGame()			override;//「メインゲーム」
		void  Render3D_L0()		override;//「3D描画」１フレーム毎に行う処理、レイヤー0
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理、3D描画後に行われる
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		TimeCount::Object::SP	beforeGameTimer;//	ゲーム前時間計測
		TimeCount::Object::SP	timer;		//	メインゲーム時間計測

		ML::Font::SP debugFont;			//デバッグ用フォント

		//プレイヤーが範囲外に出ていたら戻す処理
		void CheckPlayerInArea(ML::Vec3& playerPos_, int playerNum_);
		//メインゲームで使用されているタスクを全て含んだvectorを返す
		std::shared_ptr<std::vector<BTask::SP>> GetGameTask();
		//ゲームベースタスクのステータス切替
		void SetGameBaseTaskMode(StateOfGame state_);
	};
}