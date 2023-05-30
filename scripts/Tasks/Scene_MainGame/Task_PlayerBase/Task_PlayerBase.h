#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤー管理タスク
//-------------------------------------------------------------------
#include "../../../Bases/GameBase/GameBase.h"
#include "../Task_PowerBar/Task_PowerBar.h"

namespace  PlayerBase
{
	//アニメーション
	enum Anims { IDLE = 0, WALK, CATCH, CHARGE, THROW };		

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
		ML::Vec3 cameraDistance;		//カメラとの距離
		ML::Vec3 cameraTargetAdd;		//カメラ向き調整
		ML::Obb3 hitBase;		//地面との当たり判定矩形
		ML::Obb3 catchBase;		//キャッチ用当たり判定
		ML::Vec3 throwVec;		//投げる向き
		float	 powerChargeVol;		//1フレームの投げチャージ量
		float	 powerChargeTable[3];	//投げチャージ
		float	 powerTable[4];			//投げる強さ
		ML::Vec2 powerBarDistance;		//チャージ量表示位置（中心からの間隔）
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

		int animIndex[5];		//各アニメーションの番号

		ML::EffekseerInitData effek_charge;		//チャージ中のエフェクト
		ML::Vec3			  effek_chargeDistance;	//エフェクトのプレイヤーからの距離
		ML::EffekseerInitData effek_throw;		//投げのエフェクト
		ML::Vec3			  effek_throwRotationDeg;	//エフェクトのプレイヤーからの距離

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
		ML::EffekseerObj::SP	 effekThrow;		//投げのエフェクトハンドル
		ML::EffekseerObj::SP	 effekCharge;		//チャージのエフェクトハンドル
	};
	//-------------------------------------------------------------------
	//個々で行う処理
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
		void  UpDateBeforeGame()	override;//ゲーム前更新
		void  UpDateGame()			override;//ゲーム更新
		void  UpDateAfterGame()		override;//ゲーム後更新
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
		ML::Vec3 cameraDistance;		//カメラとの距離
		ML::Vec3 cameraTargetAdd;		//カメラの向き調整
		ML::Obb3 hitBase;		//地面との当たり判定矩形f
		ML::Obb3 catchBase;		//キャッチ用当たり判定
		ML::Vec3 throwVec;		//投げる向き（単位ベクトル化される）
		float	 chargedPower;	//投げチャージ量
		float	 power;			//投げる強さ
		float	 speed;			//移動速度
		bool	 hasBall;		//ボールを所持しているか
		int		 moveCount;		//移動カウント
		int		 throwStopCount;//投げ硬直カウント
		float	 animCount;			//アニメーションカウント
		bool	 finishChargeFlag;	//投げチャージ終了フラグ
		bool  preMove;				//1フレーム前の移動フラグ
		int		 score;			//スコア
		ML::EffekseerPlayObj::SP		 effekThrowPlay;	//投げの再生ハンドル
		ML::EffekseerPlayObj::SP		 effekChargePlay;	//チャージの再生ハンドル

		ML::Model3DClone::SP	modelCl;	//モデルのクローン
		PowerBar::Object::SP powerBar;		//チャージ量表示タスク


		ML::Vec3 addcameraAngle;	//カメラに追加する向き（主にデバッグ用）

		//プレイヤーのモーション
		enum class Motion{IDLE, WALK, GET, CATCH, CARRY, CHARGE, THROW };
		Motion	 motion;		//モーション
		Motion	 preMotion;		//前フレームのモーション

		//当たり判定を呼び出す
		ML::Obb3 CallHitBox() const;

		//次の行動を判断
		void judge();
		//モーション更新
		void UpDateMotion(Motion nm_);

		//アニメーションリセット
		void ResetAnimation();
		//アニメーション更新
		void UpDateAnimationStd(Anims anim_);

		//プレイヤー行動処理
		void Action();
		
		//プレイヤー移動
		bool Move();

		//投げチャージ
		void Charge();

		//チャージ量をパワーに変換
		void ChargeToPower();

		//ボールを投げる
		void ThrowBall();
		
		//ボールをキャッチする
		bool CatchBall();

		//ボールを持つ（自身の座標と連動させる）
		void CarryBall();

		//ボールを拾う
		bool SearchBall();

		//カメラ位置設定
		void CameraPosSet();
	};




}