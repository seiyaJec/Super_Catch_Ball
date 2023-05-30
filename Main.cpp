#include "DxLib.h"
#include "scripts/Engine/myPG/myPG.h"
#include <iostream>
#include <map>
#include <string>
#include <tuple>

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowSizeExtendRate(1.0);
	SetGraphMode(1920, 1080, 32);
	ChangeWindowMode(TRUE);
	DxLib::SetUseDirect3D11AdapterIndex(0);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	if (Effekseer_Init(500) == -1)	//Effekseer初期化処理
	{
		return -1;			//エラーが起きたら直ちに終了
	}

	//ーーーーーーーーここからメイン処理を行うーーーーーーーー//
	SetDrawScreen(DX_SCREEN_BACK);


	ge = new myPG::myGameEngine();
	ge->B_Initialize();
	SetGlobalAmbientLight(ML::Color(1, 0.5f, 0.5f, 0.5f));
	SetLightDifColor(ML::Color(1, 1, 1, 1));
	SetLightAmbColor(ML::Color(1, 0, 0, 0));

	//ゲーム内ループ（escキーで終了）
	while (ge->FinishGame() == false) {
		if (ge->c->LoopControl()) {
			ge->B_Step();		//ゲーム処理
			ProcessMessage();
		}

	}

	delete ge;



	//ーーーーーーーーーーーメイン処理終了ーーーーーーーーーー//

	Effkseer_End();				//Effekseer使用の終了処理
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
