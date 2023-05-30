#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//文字列入力
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"

namespace  KeyInput
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("文字列入力");	//グループ名
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
		bool	Inputing;		//同じタスクのどれかが入力中の時trueにする
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
		static  Object::SP  Create(int charMax_, bool useCansel_, bool SingleCharOnly_, bool NumCharOnly_,bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object(int charMax_);
		bool  B_Initialize(bool useCansel_, bool SingleCharOnly_, bool NumCharOnly_);
		bool  B_Finalize();
		bool  Initialize(bool useCansel_, bool SingleCharOnly_, bool NumCharOnly_);	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render3D_L0()		override;//「3D描画」１フレーム毎に行う処理、レイヤー0
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理、3D描画後に行われる
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		//状態
		enum class State { INVALID, IDLE, INPUTING, WAITSEND };
		State state;

		const int charMax;
		bool inputing;			//入力が終了したか
		int inputResult;		//入力結果
		ML::Vec2 pos;			//位置
		int   handle;			//入力ハンドル
		int	  moveCnt;			//行動カウント
		bool  waitSendFlag;			//入力結果を送信したか
		bool useCansel;			//escキャンセル有効/無効
		bool singleCharOnly;	//半角英数限定フラグ
		bool numberCharOnly;	//半角数字限定フラグ



		bool		buttonActive;		//ボタンの有効/無効
		ML::Box2D	button;				//ボタン
		ML::Color	buttonColNormal;	//通常時のボタンの色
		ML::Color	buttonColTargeting;	//照準時のボタンの色


		void JudgeState();						//現在の状態に合わせて次の状態を判定する
		void UpDateState(State nextState_);		//状態を更新する
		void Move();							//状態に合わせて行動する

		//ボタン関連
		void DrawButton();						//ボタン描画
		bool ButtonCheckHitMouse();				//ボタンへのマウスのクリック判定

		//外部からのアクセス用
		bool GetInputStr(std::string& str_);	//入力情報を送り、Killする（文字）
		bool GetInputNum(float& num_);	//入力情報を送り、Killする（数値）
		void SetPos(ML::Vec2 pos_);		//座標設定
		void SetSingleCharOnly(bool flagSingleCharOnly_);		//半角英数限定の有効・無効設定
		void SetNumberCharOnly(bool flagNumberCharOnly_);		//半角数字限定の有効・無効設定
	};
}