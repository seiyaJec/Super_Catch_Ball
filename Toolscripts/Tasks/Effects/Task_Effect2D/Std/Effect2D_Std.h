#pragma once
#include "../Effect2D.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 標準エフェクト
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

namespace Effect2D
{
	class  EffectStd : public  BTask
	{
	//生成・処理系◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~EffectStd();
		typedef  shared_ptr<EffectStd>		SP;
		typedef  weak_ptr<EffectStd>		WP;
		Resource::SP	res;
		//エフェクト生成窓口
		static EffectStd::SP Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_);
	
	protected:

		EffectStd();
		bool  B_Initialize(string imgFilePath_, ML::Size2 imgSize_);
		bool  B_Finalize();
		bool  Initialize(string imgFilePath_, ML::Size2 imgSize_);	//「初期化」タスク生成時に１回だけ行う処理
		virtual bool  InitializeCustom();							//追加の初期化処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render3D_L0()		override;//「3D描画」１フレーム毎に行う処理、レイヤー0
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理、3D描画後に行われる
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	
		
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

	//ーー公開ーー//
	public:
		//セッタ
		//座標
		void setPos(const ML::Vec2& pos_) {
			this->pos = pos_;
		}
		//移動ベクトル
		void setMoveVec(const ML::Vec2& moveVec_){
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

	};
}