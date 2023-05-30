#pragma once
#include "../myGame/myGame.h"
#include "useTaskInMYPG.h"
#include "../MyLibrary/MyLibrary.h"
#include <fstream>
#include <sstream>

//ゲーム内全域で使用する構造体などを定義する
//-----------------------------------------------------------------------------------------------

#include "../fpscounter/fpscounter.h"
#include "../GameData/GameData.h"

//-----------------------------------------------------------------------------------------------
namespace  myPG
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	class myGameEngine : public myEngine
	{
	public:
		myGameEngine();
		//ゲームエンジンに追加したものの初期化と開放
		bool Initialize();//(HWND wnd_);
		~myGameEngine();
		//ゲームエンジンに追加したもののステップ処理
		void UpDate();
		//入力情報の更新処理
		void UpDateInput();
		//3DPG1対応により追加
		//2D描画環境のデフォルトパラメータ設定
		void Set2DRenderState(DWORD l_);
		//3D描画環境のデフォルトパラメータ設定
		void Set3DRenderState(DWORD l_);
		void End3DRenderState(DWORD l_) override;


		//ゲームエンジンに追加したいものは下に加える
		//----------------------------------------------
		//□□□□□□□□□□□□
		//変数
		//□□□□□□□□□□□□
		const int camerasMax = 4;
		vector<bool> camerasActive;
		ML::Camera::SP*		cameras;		//	カメラ
		EffectGenerator::Object::SP effectGenerator;
		SceneTransitionGenerator::Object::SP scTrGenerator;


		GameManager::Object::SP		qa_gameManager;		//ゲームマネージャータスク
		Ball::Object::SP			qa_ball;			//ボールタスク
		NumberGUI::Object::SP		qa_numberGUI;		//数字描画タスク
		ScoreJudge::Object::SP		qa_scoreJudge;		//スコアタスク
		SceneTransition::SceneTransitionStd::WP		sceneTransition;	//シーン遷移タスク

		vector<BData::SP>		data;		//ゲームデータ
		const float				gravity = 0.3f;	//ゲーム全体の重力

		ML::InMouse					inpms;			//マウス入力情報			
		ML::InKeyBoard				inpkey;			//キーボード入力情報

	private:
		int		screenHandle;	//3D描画用スクリーンハンドル

	public:
		//□□□□□□□□□□□□
		//関数
		//□□□□□□□□□□□□

		//ゲームデータ読み込み
		void CreateGameData();
		//スクリーンの中心を返す
		ML::Vec2 screenCenter();
		//スクリーンの対角線の長さを返す
		float    screenDiagonalLength();

		//----------------------------------------------

		//ゲームデータ追加
		json gameData;

		void LoadGameDataJson();
		void LoadGameDataBson();
		

		//デバッグ関数
		void printToDebugFile(std::string debugstr_);


		//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
		//以下FPSCounter関係（基本変更不要）
		//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
		FPSCounter* c;


		int stage;

		enum COUNTER_FLAGS {
			NONE,
			ACTIVE,//稼働中
			LIMIT,//終了直後
			END,//終了
		};

		struct COUNTER {
			COUNTER_FLAGS flag;
			int count;
		};
		std::map < std::string, COUNTER > counterMap;


		void InitCounter();
		void ActCounter();
		void AllEndCounter();
		COUNTER_FLAGS getCounterFlag(string str);
		void StartCounter(string str, int count);


	};
}
extern myPG::myGameEngine* ge;


