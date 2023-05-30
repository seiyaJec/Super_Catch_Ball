#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤー管理タスク
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"

namespace  PlayerBase
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("プレイヤー");	//グループ名
	const  string  defName("管理");	//タスク名

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// ゲームデータ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	//ゲームで使用する各データ
	class Data : public BData
	{
	public:
		using SP = shared_ptr<Data>;

		json LoadDataJson(bool useBinary_)						override;	//テキストデータ読み込み
		Data();			//コンストラクタ（特に処理なし）
		~Data();		//デストラクタ  （特に処理なし）
		static Data::SP UseData();	//ゲームエンジンからデータを受け取る
		static int myNum;			//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号

		//（★★myPG.cppの配列にこのクラスを追加するのも忘れずに！★★）
		//-----------------------------
		//データ変数は以下に追加する
		ML::Vec3 handPos;		//手の座標
		ML::Vec3 throwStartPos;	//ボールを投げたときの最初の位置
		ML::Vec3 modelAngle;	//モデルの向き
		ML::Vec3 distance;		//カメラとの距離
		ML::Obb3 hitBase;		//地面との当たり判定矩形
		ML::Obb3 catchBase;		//キャッチ用当たり判定
		ML::Vec3 throwVec;		//投げる向き
		float	 power;			//投げる強さ
		float	 speed;			//移動速度
		int    throwStopFrames;	//投げの硬直時間（フレーム単位）
		int      angleTable[3][3];	//角度テーブル
		int		playersCnt;		//プレイヤー総数
		int		score_catch;	//キャッチ時のスコア加算値
		int		score_miss;		//キャッチ時のスコア減算値

		vector<int>		 key_movefront;	//入力・前進
		vector<int>		 key_moveback;	//入力・後退
		vector<int>		 key_moveleft;	//入力・左に移動
		vector<int>		 key_moveright;	//入力・右に移動
		vector<int>		 key_throwBall;	//入力・ボール投げ

		std::string	modelFilePath;	//モデルへのファイルパス

		bool	drawHitBase;				//trueなら当たり判定描画
		bool	drawCatchBase;				//trueならキャッチ用当たり判定描画
		//-----------------------------
	};

	//-------------------------------------------------------------------
	//同じタスク間で共有する処理
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
		ML::Model3D::SP	model;	//3Dモデル
	};
	//-------------------------------------------------------------------
	//個々で行う処理
	class  Object : public  BTask
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
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render3D_L0()		override;//「3D描画」１フレームごとに行う処理、レイヤー0
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理、3D描画後に行われる
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		int		 num;			//プレイヤー番号
		ML::Vec3 pos;			//基準となる座標
		ML::Vec3 angle;			//Y軸の向き
		ML::Vec3 defaultAngle;		//標準時の向き
		ML::Vec3 distance;		//カメラとの距離
		ML::Obb3 hitBase;		//地面との当たり判定矩形f
		ML::Obb3 catchBase;		//キャッチ用当たり判定
		ML::Vec3 throwVec;		//投げる向き（単位ベクトル化される）
		float	 power;			//投げる強さ
		float	 speed;			//移動速度
		bool	 hasBall;		//ボールを所持しているか
		int		 moveCount;		//移動カウント
		int		 throwStopCount;//投げ硬直カウント
		int		 score;			//スコア
		ML::Model3DClone::SP	modelCl;	//モデルのクローン


		ML::Vec3 addcameraAngle;	//カメラに追加する向き（主にデバッグ用）

		//プレイヤーのモーション
		enum class Motion{IDLE, WALK, GET, CATCH, CARRY, THROW};
		Motion	 motion;		//モーション
		Motion	 preMotion;		//前フレームのモーション

		//当たり判定を呼び出す
		ML::Obb3 CallHitBox() const;

		//次の行動を判断
		void judge();
		//モーション更新
		void UpDateMotion(Motion nm_);

		//プレイヤー行動処理
		void Action();
		
		//プレイヤー移動
		void Move();

		//ボールを投げる
		void ThrowBall();
		
		//ボールをキャッチする
		bool CatchBall();

		//ボールを持つ（自身の座標と連動させる）
		void CarryBall();

		//ボールを拾う
		bool SearchBall();
	};




}