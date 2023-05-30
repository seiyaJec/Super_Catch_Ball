#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//クリエイター
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"

namespace  Creator
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("クリエイター");	//グループ名
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
		void  Render3D_L0()		override;//「3D描画」１フレーム毎に行う処理、レイヤー0
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理、3D描画後に行われる
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		ML::Model3D::SP		targetSampleSideR;				//的サンプル左
		ML::Model3DClone::SP	targetSampleSideL;			//的サンプル右
		ML::Model3DClone::SP	targetSampleMoveRot;	//的サンプル初期位置
		ML::Color			sampleColorSide;	//サンプル端の色
		ML::Color			sampleColorRot;	//サンプル左の色
		ML::Vec3 pos;				//座標
		ML::Vec3		ptPos;		//ポインターの座標
		float			   ptHeight;		//ポインターのy軸の高さ（設置する的の高さになる）
		float	 moveSpeed;			//カメラ移動速度
		ML::Rect2		moveArea;	//的を移動させる範囲
		ML::Obb3				hitBaseMs;				//マウスの当たり判定直方体
		ML::Camera::SP		myCamera;			//自身が持つカメラ
		Target::Object::SP movingTarget;		//移動中の的

		int tgMoveTime;					//的が動かせるようになる長押し時間
		int msBtOnCnt;					//マウスボタン長押し時間計測

		float defaultTgMoveSpeed;		//初期の移動速度
		float defaultTgMoveScale;		//初期の移動幅
		float defaultTgAddRot;			//初期の回転量



		ML::Obb3 CallHitBoxMs() const;	//当たり判定呼び出し
		bool IsMouseInMoveArea();		//的移動可能範囲にカーソルがあるか
		void Move();					//移動
		void TargetSetting();			//的の設定
		void TargetLinkToMouse();		//的の座標をマウスと同期させる
		void SetCamera();				//カメラの位置・標的・上座標を設定
		void LinkToInfo();				//情報タスクと同期
		void SaveToJsonFile();			//情報タスクと同期
		void DrawSample();				//回転分移動させた的を表示
	};
}