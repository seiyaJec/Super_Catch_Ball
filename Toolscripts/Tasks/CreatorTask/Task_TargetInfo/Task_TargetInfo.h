#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//的設定画面
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"
#include "../Task_KeyInput/Task_KeyInput.h"
#include "../Task_Infomation/Task_Infomation.h"

namespace  TargetInfo
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("的設定画面");	//グループ名
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
		ML::Font::SP  font;
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
		//ビュークラス
		struct View {
			Infomation::Object::SP info;
			KeyInput::Object::SP	input;
			ML::Vec2				posDistance;	//infoタスクを原点とした、inputタスクの座標

			//生成
			void Create(int inputCharMax_, bool flagGameEnginePushBack_, const ML::Vec2& posDistance_ = ML::Vec2());

			//座標・説明文章設定
			void SetData(const ML::Vec2& pos, const std::string& data_);

			//指定した変数と同期させる
			void LinkValue(float& value_);
		};


		View	viewPosX;		//的の初期座標X
		View	viewPosZ;		//的の初期座標Z
		View	viewMoveSpeed;	//的の往復の速さ
		View	viewMoveScale;	//的の往復の大きさ
		View	viewAddRot;		//的の追加の回転　標準は0

		Target::Object::WP viewingTarget;	//表示中の的

		void LinkTargetToView();//ビューの値を選択中の的とリンクさせる
		void SetViewingTarget(Target::Object::SP target_);//表示する的を設定する
	};
}