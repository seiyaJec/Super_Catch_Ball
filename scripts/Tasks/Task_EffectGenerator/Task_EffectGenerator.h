#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ゲーム内エフェクト生成タスク
//-------------------------------------------------------------------
#include "../../Engine/myGame/myGame.h"
#include "../Effects/Task_Effect2D/Effect2D.h"

namespace  EffectGenerator
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("エフェクト生成タスク");	//グループ名
	const  string  defName("NoName");	//タスク名


	struct ImageData {
		std::string filePath;
		ML::Size2	size;
		//jsonファイルへの読み書き
		friend void to_json(json& j_, const ImageData& imgData_);
		friend void from_json(const json& j_, ImageData& imgData_);
	};
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
		ImageData imageReady;			//エフェクトReady画像情報
		float	  readyMoveSpeed;		//エフェクトReady移動速度
		float	  readyTimeStayCenter;	//エフェクトReady中央停止時間
	
		ImageData imageStart;			//エフェクトStart情報
		float	  startScaleDecSpeed;	//エフェクトStartスケール減少値
		float	  startScaleMax;		//エフェクトStartスケール最大値
		float	  startAlphaDecSpeed;	//エフェクトStart透過度減少値
		float	  startAlphaWaitDec;	//エフェクトStart透過開始までの時間（フレーム単位）
		
		ImageData imageFinish;			//エフェクトFinish情報
		float	  finishScaleDecSpeed;	//エフェクトFinishスケール減少値
		float	  finishScaleMax;		//エフェクトFinishスケール最大値
		float	  finishAlphaDecSpeed;	//エフェクトFinish透過度減少値
		float	  finishAlphaWaitDec;	//エフェクトFinish透過開始までの時間（フレーム単位）

		ImageData imageGBall;			//エフェクトGorgeousBall情報
		ImageData imageGBallSub;		//エフェクトGorgeousBallサブエフェクト情報
		ML::Vec2  GBallMoveVec;			//エフェクトGorgeousBall移動方向
		int		  GBallLife;			//エフェクトGorgeousBall生存時間
		Effect2D::InitStd::LoopLt GBallRotateLoop;	//エフェクトGorgeousBall回転情報
		int		  GBallSubMoveSpeed;	//エフェクトGorgeousBallサブエフェクト移動方向
		int		  GBallSubLife;			//エフェクトGorgeousBallサブエフェクト生存時間
		int		  GBallSubGenerateDistance;	//エフェクトGorgeousBallサブエフェクト生成間隔
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
		enum EffectsNum{Ready, Start, Finish, GorgeousBall};
		void GenerateEffect(int effectsNum_, ML::Vec2 pos_ = ML::Vec2()); //エフェクトの生成

	};
}

using EffectsNum = EffectGenerator::Object::EffectsNum;