#pragma once
#pragma warning(disable:4996)
#pragma once
//--------------------------------------------------------------------------------------------
// メインゲーム用クラス
//--------------------------------------------------------------------------------------------
#include "../../Engine/myGame/myGame.h"


class GameBase :public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<GameBase>		SP;
	typedef weak_ptr<GameBase>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	enum class StateOfGame{BeforeGame, InGame, AfterGame};
	StateOfGame stateOfGame;

	GameBase() :
		stateOfGame(StateOfGame::BeforeGame)
	{}

	void UpDate() override;

	virtual void UpDateBeforeGame() {};
	virtual void UpDateGame() {};
	virtual void UpDateAfterGame() {};
};
