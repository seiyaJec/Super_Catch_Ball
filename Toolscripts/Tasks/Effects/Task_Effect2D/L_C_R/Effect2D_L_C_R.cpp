//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//左→中央→右の順で移動するエフェクト
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#include  "../../../../Engine/myPG/myPG.h"
#include  "Effect2D_L_C_R.h"

namespace  Effect2D
{
	//コンストラクタ
	EffectL_C_R::EffectL_C_R()
		:EffectStd()
		, moveSpeed(10)
		, moveSpeedMin(1)
		, timeStayCenter(60)
	{}

	//デストラクタ
	EffectL_C_R::~EffectL_C_R() {}

	//--------------------------------------------------------
	//初期化処理
	bool EffectL_C_R::InitializeCustom() {
		ML::Size2 imgSize = this->img->get_size();
		this->pos.x = imgSize.w / -2;
		this->pos.y = (float)ge->screenHeight / 2.0f;
		this->moveVec.x = (float)moveSpeed;

		return true;
	}
	//--------------------------------------------------------
	//更新処理
	void EffectL_C_R::UpDate() {
		
		//画面中央にいるか
		if (this->IsPassedCenterOfScreen() == false) {
			this->pos += this->moveVec;
		}
		else {
			if (this->timeCnt >= this->timeStayCenter) {
				this->pos += this->moveVec;
			}
			++timeCnt;
		}


		//画面外に出たらキル
		if (this->IsOutOfScreen() == true) {
			this->Kill();
		}
		
	}

	//--------------------------------------------------
	//その他メソッド
	//--------------------------------------------------
	//画面右側へ行き見えなくなったか
	bool EffectL_C_R::IsOutOfScreen() {
		return this->pos.x - this->img->get_size().w * this->transform.scaleX >= ge->screenWidth;
	}
	//画面中央を通過したか
	bool EffectL_C_R::IsPassedCenterOfScreen() {
		ML::Vec2 screenCenter = ge->screenCenter();
		return
			this->pos.x >= screenCenter.x
			&& this->pos.y >= screenCenter.y;
	}

	//--------------------------------------------------------
	//セッタ
	void EffectL_C_R::setMoveSpeed(float moveSpeed_) {
		if (moveSpeed_ <= 0) {
			string debug("EffectL_C_R::setMoveSpeed>>不正なデータ値です！");
			ge->printToDebugFile(debug);
		}
		this->moveSpeed = moveSpeed_;
		this->moveVec.x = moveSpeed_;
	}
	void EffectL_C_R::setTimeStayCenter(int timeStayCenter_) {
		if (timeStayCenter_ < 0) {
			string debug("EffectL_C_R::setMoveSpeed>>不正なデータ値です！");
			ge->printToDebugFile(debug);
		}
		this->timeStayCenter = timeStayCenter_;
	}


	//--------------------------------------------------------
	//エフェクト生成窓口
	EffectL_C_R::SP EffectL_C_R::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		EffectL_C_R::SP ob = EffectL_C_R::SP(new EffectL_C_R());
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

}