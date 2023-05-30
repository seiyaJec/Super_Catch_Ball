#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ボール
//-------------------------------------------------------------------
#include "../../../Bases/GameBase/GameBase.h"

namespace  Ball
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ボール");	//グループ名
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
		ML::Vec3	pos;		//座標
		ML::Vec3	moveVec;	//移動ベクトル
		ML::Obb3	hitBase;	//当たり判定直方体
		vector<int>	comboScoreTable;	//連続ヒットの得点表
		std::string	modelFilePath;	//モデルへのファイルパス

		std::string modelShadowFilePath;	//モデルへのファイルパス（影）
		float		shadowHeight;			//影の高さ

		ML::EffekseerInitData effek_move;	//移動中のエフェクト初期化データ
		ML::Vec3			  effek_moveRotateDeg;	//移動中のエフェクト回転データ（度数法）
		float				  effek_moveDistance;	//移動中のエフェクト生成間隔

		bool  drawHitBase;		//trueなら当たり判定描画
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
		ML::Model3D::SP model;
		ML::Model3D::SP modelShadow;
		ML::EffekseerObj::SP effekMove;		//移動中のエフェクト
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
		void  UpDateGame()		override;//「メインゲーム」１フレーム毎に行う処理
		void  Render3D_L0()		override;//「3D描画」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理（3D描画後に行われる）
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		
		//ボールの状態
		enum class State{NON, IDLE, THROWN, CARRIED};

		State		state;		//ボールの状態
		ML::Vec3    prePos;		//1フレーム前の座標
		ML::Vec3	pos;		//座標
		ML::Vec3	moveVec;	//移動ベクトル
		ML::Obb3	hitBase;	//当たり判定
		int			combo;		//連続ヒット
		int			moveCnt;	//移動カウント
		float		moveDistanceCnt;	//移動距離カウント
		bool		flagCarried;	//運ばれているか


		//当たり判定を呼び出す
		ML::Obb3 CallHitBox() const;

		//エフェクト生成
		void CreateMoveEffect();

		//移動した距離分1ミリごとに当たり判定を行う
		bool CheckHitPreposToPos(const ML::Obb3& you_);

		//場面に応じて状態を変化させる
		void Judge();

		//状態の更新
		void UpDateState(State state_);

		//状態に合わせて動作させる
		void Action();

		//フラグをリセット
		void ResetFlag();

		//持ち運ぶ
		void Carry(const ML::Vec3& carryPos_);

		//移動中か
		bool IsMoving();

		//受け取る処理
		void HitToTarget();
	};
}