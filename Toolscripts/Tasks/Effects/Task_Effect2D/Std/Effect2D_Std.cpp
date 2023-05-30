#include "Effect2D_Std.h"
#include "../../../../Engine/myPG/myPG.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 標準エフェクト
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝


namespace Effect2D
{
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  EffectStd::Initialize(string imgFilePath_, ML::Size2 imgSize_)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		auto itr = this->res->images.find(imgFilePath_);

		//画像がまだ作られていない場合
		if (itr == this->res->images.end()) {
			//画像読み込み
			auto newimg = ML::Image::Create(imgFilePath_, imgSize_);
			if (newimg) {
				this->res->images.insert(std::make_pair(imgFilePath_, newimg));
				this->img = newimg;
			}
			//読み込み失敗
			else {
				return false;
			}
		}
		//画像が作られている場合
		else {
			this->img = itr->second;
		}

		//回転の中心を初期化
		this->setImgRotateCenter(this->img->CenterPos());

		//追加の初期化処理
		return this->InitializeCustom();
	}
	//-------------------------------------------------------------------
	//追加の初期化処理（スーパークラスなので特になし）
	bool  EffectStd::InitializeCustom() {
		return true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  EffectStd::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成

		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  EffectStd::UpDate()
	{
		this->pos += moveVec;
		
		//ループ
		this->loopRotate.Addto(this->transform.rotateDeg);
		this->loopScaleX.Addto(this->transform.scaleX);
		this->loopScaleY.Addto(this->transform.scaleY);

		this->loopAddAlpha.Addto(this->color.a);
		this->CountLife();
	}
	//-------------------------------------------------------------------
	//「３Ｄ描画」１フレーム毎に行う処理、レイヤー0
	void EffectStd::Render3D_L0()
	{

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理、3D描画後に行われる
	void  EffectStd::Render2D_AF()
	{
		this->img->DrawTransform(this->pos, this->transform, true, this->color.a);
	}
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//コンストラクタ
	EffectStd::Loop::Loop()
		:adds(0.0f)
		, lpmax(9999.0f)
		, lpmin(-9999.0f)
	{}
	//ループ設定
	void EffectStd::Loop::Set(float adds_, float lpmax_, float lpmin_) {
		adds = adds_;
		lpmax = lpmax_;
		lpmin = lpmin_;
	}
	//生存時間カウント
	void EffectStd::CountLife() {
		--this->life;
		if (this->life <= 0) {
			this->Kill();
		}
	}
	//ループ
	void EffectStd::Loop::Addto(float& addto_) {
		if (this->adds > 0.0f) {
			addto_ = min(addto_ + this->adds, this->lpmax);
		}
		else if (this->adds < 0.0f) {
			addto_ = max(addto_ + this->adds, this->lpmin);
		}
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//エフェクト生成窓口
	EffectStd::SP EffectStd::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		EffectStd::SP ob = EffectStd::SP(new EffectStd());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録

			}
			if (!ob->B_Initialize(imgFilePath_, size_)) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  EffectStd::B_Initialize(string imgFilePath_, ML::Size2 size_)
	{
		return  this->Initialize(imgFilePath_, size_);
	}
	//-------------------------------------------------------------------
	EffectStd::~EffectStd() { this->B_Finalize(); }
	bool  EffectStd::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	EffectStd::EffectStd()
		:img(nullptr)
		, pos()
		, moveVec()
		, life(100)
		, transform()
		, color()
		, loopRotate()
		, loopScaleX()
		, loopScaleY()
		, loopAddAlpha()
	{	}

}