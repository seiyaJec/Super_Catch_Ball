#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//3Dエフェクト
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"

namespace  Effect3D
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("2Dエフェクト");	//グループ名
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
		map<string, ML::Image::SP> images;
	};


	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// 初期化用データ格納クラス
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	struct InitStd
	{
		std::string imgFilePath;		//画像ファイルパス
		ML::Size2	imgSize;			//画像サイズ
		ML::Vec2	imgRotCt;			//画像の中心座標
		float		imgRotDeg;		//画像の回転
		bool		imgFlipX;			//画像の左右反転
		bool		imgFlipY;			//画像の上下反転
		ML::Vec2	pos;				//初期座標
		ML::Vec2	moveVec;			//移動方向
		ML::Color	color;				//色
		ML::Vec2	scale;				//スケール
		int			life;				//生存時間

		//軽量版ループ構造体
		struct LoopLt {
			float adds;
			float lpmax;
			float lpmin;
			//JSONファイルへの読み書き
			friend void to_json(json& j_, const LoopLt& loop_);
			friend void from_json(const json& j_, LoopLt& loop_);
		};

		LoopLt lpScaleX;		//スケールXループ
		LoopLt lpScaleY;		//スケールYループ
		LoopLt lpAlpha;			//透過度ループ
		LoopLt lpRotate;		//回転ループ

		//JSONファイルへの読み書き
		friend void to_json(json& j_, const InitStd& effStd_);
		friend void from_json(const json& j_, InitStd& effStd_);
	};



	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// 2Dエフェクト基底クラス
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	class  EffectStd : public  BTask
	{
		//生成・処理系◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~EffectStd();
		typedef  shared_ptr<EffectStd>		SP;
		typedef  weak_ptr<EffectStd>		WP;
		Resource::SP	res;

	protected:

		EffectStd();
		bool  B_Initialize(string imgFilePath_, ML::Size2 size_);
		bool  B_Finalize();
		bool  Initialize(string imgFilePath_, ML::Size2 size_);	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render3D_L0()		override;//「3D描画」１フレーム毎に行う処理、レイヤー0
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理、3D描画後に行われる
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理

		virtual bool  InitializeCustom() = 0;							//追加の初期化処理
		virtual void  UpDateAdd();									//追加して行う更新処理

		//メインのメンバ・メソッド◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇

		//ーー非公開ーー//
	protected:
		ML::Image::SP img;		//自身が使っている画像へのキー

		ML::Vec2 pos;				//座標
		ML::Vec2 moveVec;			//移動方向
		ML::Transform transform;	//変形情報
		int life;					//生存時間
		ML::Color color;			//色情報
		void CountLife();			//生存時間カウント

		//ループ
		struct Loop
		{
			float adds;				//追加する値
			float lpmax;				//最大
			float lpmin;				//最小
			Loop();
			void Set(float adds_, float lpmax_, float lpmin_);	//一括設定
			void Addto(float& addTo_);						//ループ
		};
		Loop	loopRotate;			//追加し続ける値（角度）
		Loop	loopScaleX;			//追加し続ける値（Xスケール）
		Loop	loopScaleY;			//追加し続ける値（Yスケール）
		Loop	loopAddAlpha;		//追加し続ける値（透明度

		//初期化データから読み込む
		void ReadFromInitStd(const InitStd& initStd_);

		//セッタ
		//座標
		void setPos(const ML::Vec2& pos_) {
			this->pos = pos_;
		}
		//移動ベクトル
		void setMoveVec(const ML::Vec2& moveVec_) {
			this->moveVec = moveVec_;
		}
		//生存時間
		void setLife(int life_) {
			this->life = life_;
		}
		//回転
		void setImgRotate(float rotDeg_) {
			this->transform.rotateDeg = rotDeg_;
		}
		//回転の中心
		void setImgRotateCenter(ML::Vec2 rotCtPos_) {
			this->transform.rotateCenterPos = rotCtPos_;
		}
		//スケーリング
		void setImgScale(float scX_, float scY_) {
			this->transform.scaleX = scX_;
			this->transform.scaleY = scY_;
		}
		//反転
		void setImgFlip(bool flipX_, bool flipY_) {
			this->transform.flipX = flipX_;
			this->transform.flipY = flipY_;
		}
		//色情報
		void setColor(const ML::Color& color_) {
			this->color = color_;
		}
		//回転ループ設定
		void setRotateLoop(float adds_, float max_ = 360.0f, float min_ = 0.0f) {
			this->loopRotate.Set(adds_, max_, min_);
		}
		//透明度ループ設定
		void setAlphaLoop(float adds_, float max_ = 1.0f, float min_ = 1.0f) {
			this->loopAddAlpha.Set(adds_, max_, min_);
		}
		//スケールXループ設定
		void setScaleXLoop(float adds_, float max_ = 10.0f, float min_ = 0.0f) {
			this->loopScaleX.Set(adds_, max_, min_);
		}
		//スケールYループ設定
		void setScaleYLoop(float adds_, float max_ = 10.0f, float min_ = 0.0f) {
			this->loopScaleY.Set(adds_, max_, min_);
		}


		//ーー公開ーー//
	public:

	};




















	/*
	
	-----エフェクトひな形-----


	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//エフェクト説明欄
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// データ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	class EffectSampleData :public BData
	{
	public:
		using SP = shared_ptr<EffectSampleData>;

		json LoadDataJson(bool useBinary_)						override;	//テキストデータ読み込み
		EffectSampleData();			//コンストラクタ（特に処理なし）
		~EffectSampleData();		//デストラクタ  （特に処理なし）
		static EffectSampleData::SP UseData();	//ゲームエンジンからデータを受け取る
		static int myNum;			//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
		//（★★使用する場合、myPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★）
		//-----------------------------
		//データ変数は以下に追加する
		//-----------------------------
	};
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	class EffectSample :public EffectStd
	{
		//生成・実行関連◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		~EffectSample();
		typedef shared_ptr<EffectSample>		SP;
		typedef weak_ptr<EffectStd>			WP;
		static EffectSample::SP Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_);
	protected:
		EffectSample();
		bool InitializeCustom() override;	//初期化処理
		//void UpDate() override;			//更新処理　　変更する場合はオーバーライドする
		//void UpDateAdd() override;		//追加して行う更新処理（UpDateをオーバーライドした場合は実行されない）
	private:
		EffectSampleData::SP data;

		//追加メンバ・メソッド◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	protected:

	public:
		//セッタ
	};

	
	*/
}