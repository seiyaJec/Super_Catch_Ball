#pragma warning(disable:4996)
#pragma once
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//左→中央→右の順で移動するエフェクト
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#include "../Std/Effect2D_Std.h"

namespace  Effect2D
{
	class EffectL_C_R :public EffectStd
	{
		//生成・実行関連◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		~EffectL_C_R();
		typedef shared_ptr<EffectL_C_R>		SP;
		typedef weak_ptr<EffectStd>			WP;
		static EffectL_C_R::SP Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_);
	protected:
		EffectL_C_R();
		bool InitializeCustom() override;	//初期化処理
		void UpDate() override;	//更新処理


		//追加メンバ・メソッド◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	protected:
		float moveSpeed;		//移動速度
		float moveSpeedMin;		//移動速度最小
		int timeStayCenter;	//中央に滞在する時間
		int timeCnt;		//時間計測
		bool passedCenter;		//中央通過済みか
		
		bool IsOutOfScreen();			//画面右側へ行き、見えなくなったか
		bool IsPassedCenterOfScreen();	//画面中央を通過したか
		
		//設定ミスを避けるために必要のない関数を隠す
		using EffectStd::setLife;
		using EffectStd::setMoveVec;
		using EffectStd::setPos;
	public:
		//セッタ
		void setMoveSpeed(float moveSpeed_);
		void setTimeStayCenter(int timeStayCenter_);
	};
}