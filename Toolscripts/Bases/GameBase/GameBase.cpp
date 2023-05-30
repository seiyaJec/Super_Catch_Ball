#include "GameBase.h"
//--------------------------------------------------------------------------------------------
// メインゲーム用クラス
//--------------------------------------------------------------------------------------------

void GameBase::UpDate() {
	switch (this->stateOfGame) {
	case StateOfGame::BeforeGame:
		UpDateBeforeGame();
		break;
	case StateOfGame::InGame:
		UpDateGame();
		break;
	case StateOfGame::AfterGame:
		UpDateAfterGame();
		break;
	}
}
