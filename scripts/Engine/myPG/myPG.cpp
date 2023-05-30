#include "myPG.h"
#define	REFRESHRATE  60


namespace myPG
{

	void myGameEngine::InitCounter() {
		counterMap.clear();
	}
	void myGameEngine::ActCounter() {
		for (auto& c : counterMap) {
			if (c.second.flag == COUNTER_FLAGS::LIMIT) {
				c.second.flag = COUNTER_FLAGS::END;
			}
			if (c.second.flag == COUNTER_FLAGS::ACTIVE) {
				c.second.count--;
				if (c.second.count <= 0) {
					c.second.flag = COUNTER_FLAGS::LIMIT;
				}
			}
		}
	}
	//すべてのカウンタを強制的に止める。
	void myGameEngine::AllEndCounter() {
		for (auto& c : counterMap) {
			if (c.second.flag == COUNTER_FLAGS::ACTIVE) {
				c.second.flag = COUNTER_FLAGS::END;
			}
		}
	}
	//指定の名前のカウンタの状態を取得する。
	myGameEngine::COUNTER_FLAGS myGameEngine::getCounterFlag(string str)
	{
		return counterMap[str].flag;
	}
	//指定の名前をカウンタを作成し、指定フレームのカウントを始める。
	void myGameEngine::StartCounter(string str, int count) {
		counterMap[str].count = count;
		counterMap[str].flag = COUNTER_FLAGS::ACTIVE;
	}
	myGameEngine::myGameEngine()
		:myEngine(
			//	環境情報
			1920,							//	画面サイズX
			1080,							//	画面サイズY
			//2,								//	表示倍率
			//1,								//	マルチサンプル(1or2or4)
			//true,							//	フルスクリーンモード
			1920,							//	2D描画用解像度X(現在非対応）
			1080,							//	2D描画用解像度Y(現在非対応）
			//"進級制作",	//	ウインドウタイトル
			//"GPG",							//	ウィンドウクラス名
			1,//UpDate呼び出し頻度　↓のRender呼び出し頻度との比率で決まる１以上を指定すること
			1 //Render呼び出し頻度　↑のUpDate呼び出し頻度との比率で決まる１以上を指定すること
			//例えば、120Hz固定のPCで、60Hzと同等の状態で動かす場合、1U/2Rで設定すれば、UpDateの呼び出しが1/2になる
		),
		camerasActive(camerasMax,false),
		cameras(new ML::Camera::SP[camerasMax])
	{
		//seting.txtによるリフレッシュレート調整の為追加
		ifstream  fin("./data/Shader/seting.txt");
		if (fin) {
			fin >> this->updateCall >> this->renderCall;
			fin.close();
		}
	}

	//ゲームエンジンに追加したものの初期化と開放
	bool myGameEngine::Initialize()//(HWND wnd_)
	{
		//FPS計測
		{
			c = new FPSCounter(REFRESHRATE);
		}

		//ゲームデータ読み込み
		this->CreateGameData();
		this->LoadGameDataJson();

		//デバッグ用フォントの準備
		this->debugFont = ML::Font::Create();

		//デバッグファイルの初期化
		ofstream ofs("./data/debug/Debug.txt");
		ofs << "----------DebugData----------" << endl;
		ofs.close();

		//エフェクト生成用のタスク生成
		this->effectGenerator = EffectGenerator::Object::Create(false);
		//シーン遷移用のタスク生成
		this->scTrGenerator = SceneTransitionGenerator::Object::Create(false);

		////カメラ1
		ge->cameras[0] = ML::Camera::Create(
			ML::Vec3(1.0f, 0.0f, 0.0f),	//被写体の位置
			ML::Vec3(0.0f, 0.0f, 0.0f),	//カメラの位置
			ML::Vec3(0.0f, 1.0f, 0.0f),	//カメラの上方向を示すベクトル
			ML::ToRadian(70.0f),		//視野角
			10.0f,						//どのくらい手前のものを映すか
			1000000.0f,						//どのくらい奥のものを映すか
			1,							//	画面比率（自動で画面サイズのアスペクト比にしてくれるので、画面分割する際のみ使用）
			ML::Box2D(0, 0, (float)ge->screenWidth / 2, (float)ge->screenHeight));	//画面に描画する範囲
		//カメラ2
		ge->cameras[1] = ML::Camera::Create(
			ML::Vec3(1.0f, 0.0f, 0.0f),	//被写体の位置
			ML::Vec3(0.0f, 0.0f, 0.0f),	//カメラの位置
			ML::Vec3(0.0f, 1.0f, 0.0f),	//カメラの上方向を示すベクトル
			ML::ToRadian(70.0f),		//視野角
			10.0f,						//どのくらい手前のものを映すか
			1000000.0f,						//どのくらい奥のものを映すか
			1,							//	画面比率（自動で画面サイズのアスペクト比にしてくれるので、画面分割する際のみ使用）
			ML::Box2D((float)ge->screenWidth / 2, 0, (float)ge->screenWidth / 2, (float)ge->screenHeight));		//画面に描画する範囲
		//使わないカメラの初期化
		this->cameras[3] = this->cameras[2] = this->cameras[0];

		//スクリーンハンドル初期化
		this->screenHandle = -1;

		//デバッグ用情報の表示ON/OFF
		DebugMode(false);

		//背景色の設定（デフォルト）
		SetBackgroundColor(180, 200, 200);
		ClearDrawScreen();

		//Zバッファを使用する
		DxLib::SetUseZBuffer3D(TRUE);
		DxLib::SetWriteZBuffer3D(TRUE);

		//初期実行タスク生成＆ゲームエンジンに登録
		auto  ft = TitleManager::Object::Create(true);

		//----------------------------------------
		//レイヤー毎の描画のON/OFF
		//----------------------------------------
		Layer_3D(true, true, false, false);
		Layer_2D(true, true);
		return true;
	}

	//解放処理
	myGameEngine::~myGameEngine()
	{
		//fpsカウンター解放
		free(c);
		//カメラマン解放
		delete[] cameras;
	}

	//ゲームエンジンに追加したもののステップ処理
	void myGameEngine::UpDate()
	{
		//入力情報の更新
		this->UpDateInput();
		//エフェクトアニメーションの更新
		UpdateEffekseer3D();
		//カウンタ動作
		ActCounter();

	}

	//入力情報の更新処理
	void myGameEngine::UpDateInput()
	{
		inpms.UpDate();
		inpkey.UpDate();
	}

	//2D描画環境のデフォルトパラメータ設定
	void myGameEngine::Set2DRenderState(DWORD l_)
	{
		//現在処理なし
		//if (l_ == 0) {
		//}
		//if (l_ == 1) {
		//}
	}
	//3D描画環境のデフォルトパラメータ設定
	void myGameEngine::Set3DRenderState(DWORD l_)
	{
		//レイヤー0番に対する設定
		if (l_ == 0) {
			this->screenHandle = MakeScreen(
				(int)this->cameras[0]->screenArea.w,
				(int)this->cameras[0]->screenArea.h,
				FALSE
			);
			DxLib::SetDrawScreen(this->screenHandle);
			cameras[0]->update();

		}
		if (l_ == 1) {
			this->screenHandle = MakeScreen(
				(int)this->cameras[0]->screenArea.w,
				(int)this->cameras[0]->screenArea.h,
				FALSE
			);
			DxLib::SetDrawScreen(this->screenHandle);
			cameras[1]->update();
		}
		if (l_ == 2) {
		}
		if (l_ == 3) {
		}
		ClearDrawScreen();
		Effekseer_Sync3DSetting();
	}

	//3D描画終了設定
	void myGameEngine::End3DRenderState(DWORD l_) {
		DrawEffekseer3D();
		DxLib::SetDrawScreen(DX_SCREEN_BACK);
		if (l_ == 0) {
			DxLib::DrawGraph(
				(int)this->cameras[0]->screenArea.x,
				(int)this->cameras[0]->screenArea.y,
				this->screenHandle,
				false
			);
		}
		if (l_ == 1) {
			DxLib::DrawGraph(
				(int)this->cameras[1]->screenArea.x,
				(int)this->cameras[1]->screenArea.y,
				this->screenHandle,
				false
			);
		}
		if (l_ == 2) {
		}
		if (l_ == 3) {
		}
		DxLib::DeleteGraph(this->screenHandle);
		//DxLib::SetDrawArea(0, 0, ge->screenWidth, ge->screenHeight);
		//DxLib::SetCameraScreenCenter(ge->screenWidth / 2.0f, ge->screenHeight / 2.0f);
	}

	//デバッグ関数
	void myGameEngine::printToDebugFile(string debugstr_) {
		ofstream ofs("./data/debug/Debug.txt",ofstream::app);
		ofs << debugstr_ << endl;
		ofs.close();
	}

	//スクリーンの中心を返す
	ML::Vec2 myGameEngine::screenCenter() {
		ML::Vec2 ct;
		ct.x = (float)this->screenWidth / 2.0f;
		ct.y = (float)this->screenHeight / 2.0f;
		return ct;
	}

	//スクリーンの対角線の長さを返す
	float myGameEngine::screenDiagonalLength() {
		return fabsf(sqrtf(this->screenWidth * this->screenWidth + this->screenHeight * this->screenHeight));
	}
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	//ゲームデータ追加
	//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
	void myGameEngine::CreateGameData() {

		BData::SP preData[] = {
			//追加したデータはこの下に書く
			//ーーーーーーーーーーーーーーーーーーーー
			make_shared<PlayerBase::Data>(),
			make_shared<Ground::Data>(),
			make_shared<Ball::Data>(),
			make_shared<GameManager::Data>(),
			make_shared<Border::Data>(),
			make_shared<ScoreJudge::Data>(),
			make_shared<PowerBar::Data>(),
			make_shared<NumberGUI::Data>(),
			make_shared<Target::Data>(),
			make_shared<TargetGenerator::Data>(),
			make_shared<TimeCount::Data>(),
			make_shared<Frame::Data>(),
			make_shared<ScoreAddNum::Data>(),
			make_shared<TitleImage::Data>(),
			make_shared<ResultFont::Data>(),
			make_shared<ResultManager::Data>(),
			make_shared<ResultBGImage::Data>(),
			make_shared<EffectGenerator::Data>(),
			make_shared<Effect2D::DataL_C_R>(),
			make_shared<Effect2D::DataEffectStamp>(),
			make_shared<Effect2D::DataGenerateSubRandom>(),
			make_shared<Effect2D::DataMoveStraight>(),
			make_shared<SceneTransition::DataRevPyramid>(),
			make_shared<SceneTransition::DataCircleScaleUp>(),
			make_shared<SceneTransitionGenerator::Data>(),
			//ーーーーーーーーーーーーーーーーーーーー
			//追加したデータはこの上に書く
		};

		//配列にあるデータをすべて追加
		for (auto& it : preData) {
			this->data.push_back(it);
		}
	}

	void myGameEngine::LoadGameDataJson() {
		//データ読み込み・書き出し
		{
			ofstream ofs("gameDataLatest.bin", ofstream::binary);
			//data配列の各要素で読み込み・書き出し処理を行う
			for (auto& it : this->data) {
				this->gameData["data"].push_back(it->LoadDataJson(false));
			}
			//BSONデータをファイルに変換
			vector<uint8_t> gameDataBin = json::to_bson(this->gameData);
			for (auto& val : gameDataBin) {
				ofs.write((const char*)&val, sizeof(uint8_t));
			}

			ofs.close();
		}
	}

	void myGameEngine::LoadGameDataBson() {

		//BSONファイル読み込み
		ifstream ifs("gameDataLatest.bin", ifstream::binary);
		vector<uint8_t> gameDataBin;
		while (ifs) {
			uint8_t gameDataVal;
			ifs.read((char*)&gameDataVal, sizeof(uint8_t));
			if (!ifs) { break; }
			gameDataBin.push_back(gameDataVal);
		}
		ifs.close();
		this->gameData = json::from_bson(gameDataBin);

		//data配列の各要素で読み込み・書き出し処理を行う
		for (auto& it : this->data) {
			//読み込んだ最新のJSONデータを、一つのBSONデータに保存しておく
			it->LoadDataJson(true);
		}

	}
	//------------------------------------------------------------
}

myPG::myGameEngine* ge;
