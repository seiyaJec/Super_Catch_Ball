#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//シーン遷移
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"

namespace  SceneTransition
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("シーン遷移");	//グループ名
	const  string  defName("NoName");	//タスク名
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// ゲームデータ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	class Data : public BData
	{
	public:
		using SP = shared_ptr<Data>;

		json LoadDataJson(bool useBinary_)		override;	//テキストデータ読み込み
		Data();			//コンストラクタ（特に処理なし）
		~Data();		//デストラクタ  （特に処理なし）
		static Data::SP UseData();	//ゲームエンジンからデータを受け取る
		static int myNum;			//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
		//（★★使用する場合、myPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 
		//-----------------------------
		//データ変数は以下に追加する


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
	// シーン遷移基底クラス
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	class  SceneTransitionStd : public  BTask
	{
		//生成・処理系◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~SceneTransitionStd();
		typedef  shared_ptr<SceneTransitionStd>		SP;
		typedef  weak_ptr<SceneTransitionStd>		WP;
		Resource::SP	res;

	protected:

		SceneTransitionStd();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render3D_L0()		override;//「3D描画」１フレーム毎に行う処理、レイヤー0
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理、3D描画後に行われる
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理

		virtual bool  InitializeCustom();				//追加の初期化処理
		virtual void  AppearUpDate() = 0;						//出現処理
		virtual void  DisappearUpDate() = 0;					//消滅処理
		virtual bool  CanDisappear();						//消滅判定

		//メインのメンバ・メソッド◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇

		//ーー非公開ーー//
	protected:
		float		transitionSpeed;	//移動方向
		ML::Color	color;				//色情報
		bool		appearFlag;			//出現フラグ
		bool		finishAppearFlag;	//出現終了フラグ
		bool		disappearFlag;		//消滅フラグ
		//ーー公開ーー//
	public:
		void SetColor(ML::Color color_) { this->color = color_; }
		void Appear() { this->appearFlag = true; }
		bool CheckFinishAppear() { return this->finishAppearFlag; }
		void Disappear() { this->disappearFlag = true; }
	};










	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//横分割　逆ピラミッド型
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// データ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	class DataRevPyramid :public BData
	{
	public:
		using SP = shared_ptr<DataRevPyramid>;

		json LoadDataJson(bool useBinary_)						override;	//テキストデータ読み込み
		DataRevPyramid();			//コンストラクタ（特に処理なし）
		~DataRevPyramid();		//デストラクタ  （特に処理なし）
		static DataRevPyramid::SP UseData();	//ゲームエンジンからデータを受け取る
		static int myNum;			//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
		//（★★使用する場合、myPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 
		//-----------------------------
		//データ変数は以下に追加する
		int cuts;			//分割数
		float distance;		//分割の間隔
		float moveSpeed;	//1フレームごと移動速度
		//-----------------------------
	};
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

	class RevPyramid :public SceneTransitionStd
	{
		//生成・実行関連◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		~RevPyramid();
		typedef shared_ptr<RevPyramid>		SP;
		typedef weak_ptr<RevPyramid>			WP;
		static RevPyramid::SP Create(bool flagGameEnginePushBack_);
	protected:
		RevPyramid();
		bool InitializeCustom() override;	//初期化処理
		void AppearUpDate() override;		//出現処理
		void DisappearUpDate() override;	//消滅処理
		void Render2D_AF() override;		//描画処理
	private:
		DataRevPyramid::SP data;

		//追加メンバ・メソッド◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	protected:
		int			cuts;		//分割数
		float		distance;	//分割の間隔
		float		moveSpeed;	//移動速度
		vector<ML::Box2D> cutBoxes;		//分割した箱
		float		disappearCnt;	//消滅時間計測
	public:
		void Set(int cuts_, float distance_, float moveSpeed_);
	};





	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//円拡大
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// データ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	class DataCircleScaleUp :public BData
	{
	public:
		using SP = shared_ptr<DataCircleScaleUp>;

		json LoadDataJson(bool useBinary_)						override;	//テキストデータ読み込み
		DataCircleScaleUp();			//コンストラクタ（特に処理なし）
		~DataCircleScaleUp();		//デストラクタ  （特に処理なし）
		static DataCircleScaleUp::SP UseData();	//ゲームエンジンからデータを受け取る
		static int myNum;			//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
		//（★★使用する場合、myPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 
		//-----------------------------
		//データ変数は以下に追加する
		float scaleUpSpeed;		//拡大速度
		//-----------------------------
	};
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

	class CircleScaleUp :public SceneTransitionStd
	{
		//生成・実行関連◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		~CircleScaleUp();
		typedef shared_ptr<CircleScaleUp>		SP;
		typedef weak_ptr<CircleScaleUp>			WP;
		static CircleScaleUp::SP Create(bool flagGameEnginePushBack_);
	protected:
		CircleScaleUp();
		bool InitializeCustom() override;	//初期化処理
		void AppearUpDate() override;		//出現処理
		void DisappearUpDate() override;	//消滅処理
		void Render2D_AF() override;		//描画処理
	private:
		DataCircleScaleUp::SP data;

		//追加メンバ・メソッド◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	protected:
		int scaleUpSpeed;
		int radius;
		int lineThickness;			//線の太さ
		int moveCnt;
	public:
		void Set(int scaleUpSpeed_);
	};












	/*
	
	-----エフェクトひな形-----


	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//円拡大
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// データ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	class DataSampleScTr :public BData
	{
	public:
		using SP = shared_ptr<DataSampleScTr>;

		json LoadDataJson(bool useBinary_)						override;	//テキストデータ読み込み
		DataSampleScTr();			//コンストラクタ（特に処理なし）
		~DataSampleScTr();		//デストラクタ  （特に処理なし）
		static DataSampleScTr::SP UseData();	//ゲームエンジンからデータを受け取る
		static int myNum;			//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
		//（★★使用する場合、myPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★）
		//-----------------------------
		//データ変数は以下に追加する

		//-----------------------------
	};
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

	class SampleScTr :public SceneTransitionStd
	{
		//生成・実行関連◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		~SampleScTr();
		typedef shared_ptr<SampleScTr>		SP;
		typedef weak_ptr<SampleScTr>			WP;
		static SampleScTr::SP Create(bool flagGameEnginePushBack_);
	protected:
		SampleScTr();
		bool InitializeCustom() override;	//初期化処理
		void AppearUpDate() override;		//出現処理
		void DisappearUpDate() override;	//消滅処理
		void Render2D_AF() override;		//描画処理
	private:
		DataSampleScTr::SP data;

		//追加メンバ・メソッド◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	protected:

	public:

	};

	
	*/
}