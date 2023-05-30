#include "myGame.h"
#include <time.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>
//#include "PerformanceCounter.h"	//実行速度を調べる

int BData::argManager = 0;
BData::BData() :
	myArg(argManager)
{
	++argManager;
}

DWORD BTask::uKeyManager = 10000;

bool myEngine::B_Initialize()//(HWND wnd_)
{
	//	タイマー分解能を1ミリ秒に設定
	timeBeginPeriod(1);
	//debugMode = false;
	Initialize();//(wnd_);
	return true;
}

void myEngine::B_Step()//(HWND wnd_)
{
	this->renderCnt += this->renderCall;
	this->updateCnt += this->updateCall;

	if (this->updateCnt >= this->renderCall) {
		this->updateCnt -= this->renderCall;

		//	ESCキーが押された	終了要請フラグをＯＮ
		if (GetAsyncKeyState(VK_ESCAPE)) { quitRequest = true; }

		//	タスクのステップ処理
		this->UpDate();
		//	登録済みタスクのアップデート処理を呼び出す
		this->B_StepSub_CallTaskUpDate();
		//	追加登録されたタスクを反映させる
		this->B_StepSub_Entry();
		//	消滅状態のタスクを取り除く
		this->B_StepSub_DeleteTask();
	}

	if (this->renderCnt >= this->updateCall) {
		this->renderCnt -= this->updateCall;

		//３Ｄの描画プライオリティを更新する
		this->B_UpDateSub_CallTaskUpDate3DPriority();

		////描画開始
		//this->dgi->Begin(this->dgi->EffectState().param.bgColor);
		{
			//2D描画（3D背面）
			this->B_StepSub_CallTaskRender2D_BF();
			//レイヤー毎に描画
			this->B_StepSub_CallTaskRender3D_4();
			//2D描画(3D描画後）
			this->B_StepSub_CallTaskRender2D_AF();
			//デバッグ表示
			this->B_StepSub_DebugOut();
		}
		//フリップ
		ScreenFlip();
		//裏画面リセット
		ClearDrawScreen();
		////描画終了
		//this->dgi->End();
	}

	//	タスクが存在しない場合終了とする
	//	終了要請がある時　ウィンドウの破棄を行う
	if ((tasks_detect.size() == 0 && tasks_undetect.size() == 0 && add_Tasks.size() == 0)
		|| quitRequest == true) {
		//DestroyWindow(wnd_);
		finishGame = true;
	}
	frameCnt++;
}

//タスクの更新処理
void  myEngine::B_StepSub_CallTaskUpDate()
{
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); ++it) {
		(*it).second->T_Step();
	}
	for_each(tasks_undetect.begin(), tasks_undetect.end(), mem_fn(&BTask::T_Step));

}

//3D描画の更新処理
void  myEngine::B_UpDateSub_CallTaskUpDate3DPriority()
{
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); ++it) {
		(*it).second->T_UpDate3DPriority();
	}
	for_each(tasks_undetect.begin(), tasks_undetect.end(), mem_fn(&BTask::T_UpDate3DPriority));

}
//タスクの更新前処理
void  myEngine::B_StepSub_Entry()
{
	//	追加されたタスクの登録
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); ++it) {
		if ((*it)->Detectible()) {
			pair<DWORD, BTask::SP>  d;
			d.first = (*it)->GroupKey();
			d.second = (*it);
			tasks_detect.push_back(d);
		}
		else { tasks_undetect.push_back((*it)); }
		(*it).reset();
	}
	add_Tasks.erase(add_Tasks.begin(), add_Tasks.end());
}

//3D描画前に行われる2D描画
void  myEngine::B_StepSub_CallTaskRender2D_BF()
{
	//2D描画（3D背面）
	if (this->layerEnable2D[0]) {
		//	タスクの描画処理(2D)
		//firstには画像優先度（Priority）、secondにはタスクへのポインタ）
		//同じ優先度の値が許されるように、multimapを使っている。
		using dmap = std::pair<float, BTask*>;
		multimap<float, BTask*> draw2DMM;

		//検出可能なタスク
		for (auto it = tasks_detect.begin(); it != tasks_detect.end(); ++it) {
			//サスペンドでない
			//かつ、render2D_Active[0]がtrueなタスクだけ描画処理に含める
			if ((*it).second->TaskStateCnt_Suspend() == 0) {
				if ((*it).second->render2D_Active[0]) {
					//値が小さいほうが後に描画処理を行うために符号をつけている）
					//.get()はshared_ptrが保有しているポインタを返してくれる
					draw2DMM.insert(dmap(-(*it).second->render2D_Priority[0], (*it).second.get()));
				}
			}
		}
		//検出不可能なタスク
		for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); ++it) {
			if ((*it)->TaskStateCnt_Suspend() == 0) {
				if ((*it)->render2D_Active[0]) {
					draw2DMM.insert(dmap(-(*it)->render2D_Priority[0], (*it).get()));
				}
			}
		}
		//Set2DRenderState(0);
		//this->dgi->Begin2D();
		//画像優先度順に並べ替えたタスクを描画
		for (auto it = draw2DMM.begin(); it != draw2DMM.end(); ++it) {
			(*it).second->T_Draw2D(0);
		}
		//this->dgi->End2D();
	}
}

//レイヤー0から順に3D描画
void  myEngine::B_StepSub_CallTaskRender3D_4()
{

	for (DWORD lc = 0; lc < 4; ++lc) {
		if (this->layerEnable3D[lc]) {
			//	タスクの描画処理(3D)
			using dmap = std::pair<float, BTask::SP>;
			multimap<float, BTask::SP> draw3DMM;
			multimap<float, BTask::SP> draw3DAPMM;
			//描画処理を行うためにタスクを配列に追加する処理
			//検出可能なタスクを描画配列に追加
			for (auto it = tasks_detect.begin(); it != tasks_detect.end(); ++it) {
				if ((*it).second->TaskStateCnt_Suspend() == 0) {
					if ((*it).second->render3D_Active[lc]) {
						if (!(*it).second->render3D_Alpha) {
							//透過なし：手前から奥に並べる
							draw3DMM.insert(dmap((*it).second->render3D_Priority[lc], (*it).second));
						}
						else {
							//透過あり：奥から手前に並べる
							draw3DAPMM.insert(dmap(-(*it).second->render3D_Priority[lc], (*it).second));
						}
					}
				}
			}
			//検出不可能なタスクを描画配列に追加
			for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); ++it) {
				if ((*it)->TaskStateCnt_Suspend() == 0) {
					if ((*it)->render3D_Active[lc]) {
						if (!(*it)->render3D_Alpha) {
							//透過なし：手前から奥に並べる
							draw3DMM.insert(dmap((*it)->render3D_Priority[lc], (*it)));
						}
						else {
							//透過あり：奥から手前に並べる
							draw3DAPMM.insert(dmap(-(*it)->render3D_Priority[lc], (*it)));
						}
					}
				}
			}
			//描画開始処理
			Set3DRenderState(lc);
			//透過なしオブジェクトの描画
			for (auto it = draw3DMM.begin(); it != draw3DMM.end(); ++it) { (*it).second->T_Draw3D(0); }
			//透過ありオブジェクトの描画
			for (auto it = draw3DAPMM.begin(); it != draw3DAPMM.end(); ++it) { (*it).second->T_Draw3D(0); }
			//描画終了処理
			End3DRenderState(lc);
			
		}
	}
}


//3D描画後に行われる2D描画
void  myEngine::B_StepSub_CallTaskRender2D_AF()
{
	if (this->layerEnable2D[1]) {
		//	タスクの描画処理(2D)
		using dmap = std::pair<float, BTask*>;
		multimap<float, BTask*> draw2DMM;
		for (auto it = tasks_detect.begin(); it != tasks_detect.end(); ++it) {
			if ((*it).second->TaskStateCnt_Suspend() == 0) {
				if ((*it).second->render2D_Active[1]) {
					draw2DMM.insert(dmap(-(*it).second->render2D_Priority[1], (*it).second.get()));
				}
			}
		}
		for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); ++it) {
			if ((*it)->TaskStateCnt_Suspend() == 0) {
				if ((*it)->render2D_Active[1]) {
					draw2DMM.insert(dmap(-(*it)->render2D_Priority[1], (*it).get()));
				}
			}
		}
		Set2DRenderState(1);
		//this->dgi->Begin2D();
		for (auto it = draw2DMM.begin(); it != draw2DMM.end(); ++it) {
			(*it).second->T_Draw2D(1);
		}
		//this->dgi->End2D();
	}
}


//デバッグ情報描画
void  myEngine::B_StepSub_DebugOut()
{
	if (debugMode)
	{
		ML::Color c[] = { ML::Color(1,1,0,0),  ML::Color(1,0,0,1),
							ML::Color(1,0,1,1),  ML::Color(1,0,1,0),
							ML::Color(1,1,2,0),  ML::Color(1,1,0,0),
							ML::Color(1,1,0,1),  ML::Color(1,0,0,1), };
		ML::Vec2 dPos(0, 0);
		int display = 30;	//何項目表示させるか
		{

			//最新"display"個のタスクを表示させる
			int  s = max(0, int(tasks_detect.size()) - display);
			int id = s;
			for (auto it = (tasks_detect.begin() + s); it != tasks_detect.end(); ++it) {
				//状況を出力
				stringstream ss;
				ss << id << ">" << (*it).second->groupName << ">" << (*it).second->name << ">" << (*it).second->DebugOut();
				this->debugFont->Draw(dPos, ss.str().c_str(), c[0]);
				//次の文字列表示位置を設定
				dPos.y += debugFont->get_size();
				//画面に収まらなくなったら終了
				if (dPos.y >= (int)screenHeight) { break; }
				++id;
			}
		}
		{
			//最新"display"個のタスクを表示させる
			int  s = max(0, int(tasks_undetect.size()) - display);
			int id = s;
			for (auto it = (tasks_undetect.begin() + s); it != tasks_undetect.end(); ++it) {
				//状況を出力
				stringstream ss;
				ss << id << "検出不可タスク>" << (*it)->groupName << ">" << (*it)->name << ">" << (*it)->DebugOut();
				this->debugFont->Draw(dPos, ss.str().c_str(), c[0]);
				//次の文字列表示位置を設定
				dPos.y += debugFont->get_size();
				//画面に収まらなくなったら終了
				if (dPos.y >= (int)screenHeight) { break; }
				++id;
			}
		}
	}

}

//kill状態のタスクの削除
void  myEngine::B_StepSub_DeleteTask()
{
	class cNullCheck {
	public:
		// タスクが既に解放されているならtrueを返す
		bool operator()(BTask::SP t_) const {
			bool rtv = t_ == nullptr;
			return  rtv;
		}
	};
	class cNullCheckP {
	public:
		// タスクが既に解放されているならtrueを返す
		bool operator()(pair<DWORD, BTask::SP> t_) const {
			bool rtv = t_.second == nullptr;
			return  rtv;
		}
	};

	//検出可能なタスクの削除
	{
		for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill状態の全てを削除する
			if ((*it).second->TaskStateCnt_Kill() > 0) {
				(*it).second.reset();
			}
		}
		//削除処理
		auto endIt = remove_if(tasks_detect.begin(), tasks_detect.end(), cNullCheckP());
		tasks_detect.erase(endIt, tasks_detect.end());	//Kill状態の全てを削除する
	}

	//検出不可能なタスクの削除
	{
		for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill状態の全てを削除する
			if ((*it)->TaskStateCnt_Kill() > 0) {
				(*it).reset();
			}
		}
		//削除処理
		auto endIt = remove_if(tasks_undetect.begin(), tasks_undetect.end(), cNullCheck());
		tasks_undetect.erase(endIt, tasks_undetect.end());	//Kill状態の全てを削除する
	}
}


myEngine::~myEngine()
{
	//全てのタスクを解放する
	for (auto it = this->tasks_detect.begin(); it != this->tasks_detect.end(); ++it) {
		if ((*it).second == nullptr) { continue; }
		(*it).second.reset();
	}
	for (auto it = this->tasks_undetect.begin(); it != this->tasks_undetect.end(); ++it) {
		if ((*it) == nullptr) { continue; }
		(*it).reset();
	}
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		(*it).reset();
	}
	this->debugFont.reset();
	//	タイマー分解能の設定（1ミリ秒単位）を解除する
	timeEndPeriod(1);
}


bool myEngine::KillAll_GN(const  string& gn_, const  string& n_, bool  nt_)
{
	bool rtv = false;
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill状態の全てを削除する
		if ((*it).second == nullptr) { continue; }
		if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it).second->groupName != gn_) { continue; }
		if ((*it).second->name != n_) { continue; }
		(*it).second->Kill(nt_);
		rtv = true;
	}
	for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill状態の全てを削除する
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		if ((*it)->name != n_) { continue; }
		(*it)->Kill(nt_);
		rtv = true;
	}
	//登録待ちタスクに対しても処理を行う(2016/03/22)
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		if ((*it)->name != n_) { continue; }
		(*it)->Kill(nt_);
	}
	return rtv;
}
bool myEngine::KillAll_G(const  string& gn_, bool  nt_)
{
	bool rtv = false;
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill状態の全てを削除する
		if ((*it).second == nullptr) { continue; }
		if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it).second->groupName != gn_) { continue; }
		(*it).second->Kill(nt_);
		rtv = true;
	}
	for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill状態の全てを削除する
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		(*it)->Kill(nt_);
		rtv = true;
	}
	//登録待ちタスクに対しても処理を行う(2016/03/22)
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		(*it)->Kill(nt_);
	}
	return rtv;
}
//	同名のタスクを全て停止する
bool myEngine::StopAll_GN(const  string& gn_, const  string& n_, bool  m_)
{
	bool rtv = false;
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill状態の全てを削除する
		if ((*it).second == nullptr) { continue; }
		if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it).second->groupName != gn_) { continue; }
		if ((*it).second->name != n_) { continue; }
		(*it).second->Stop(m_);
		rtv = true;
	}
	for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill状態の全てを削除する
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		if ((*it)->name != n_) { continue; }
		(*it)->Stop(m_);
		rtv = true;
	}
	//登録待ちタスクに対しても処理を行う(2016/03/22)
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		if ((*it)->name != n_) { continue; }
		(*it)->Stop(m_);
	}
	return rtv;
}
bool myEngine::StopAll_G(const  string& gn_, bool  m_)
{
	bool rtv = false;
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill状態の全てを削除する
		if ((*it).second == nullptr) { continue; }
		if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it).second->groupName != gn_) { continue; }
		(*it).second->Stop(m_);
		rtv = true;
	}
	for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill状態の全てを削除する
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		(*it)->Stop(m_);
		rtv = true;
	}
	//登録待ちタスクに対しても処理を行う(2016/03/22)
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		(*it)->Stop(m_);
	}
	return rtv;
}
//	同名のタスクを全て一時無効にする
bool myEngine::SuspendAll_GN(const  string& gn_, const  string& n_, bool  m_)
{
	bool rtv = false;
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill状態の全てを削除する
		if ((*it).second == nullptr) { continue; }
		if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it).second->groupName != gn_) { continue; }
		if ((*it).second->name != n_) { continue; }
		(*it).second->Suspend(m_);
		rtv = true;
	}
	for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill状態の全てを削除する
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		if ((*it)->name != n_) { continue; }
		(*it)->Suspend(m_);
		rtv = true;
	}
	//登録待ちタスクに対しても処理を行う(2016/03/22)
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		if ((*it)->name != n_) { continue; }
		(*it)->Suspend(m_);
	}
	return rtv;
}
bool myEngine::SuspendAll_G(const  string& gn_, bool  m_)
{
	bool rtv = false;
	for (auto it = tasks_detect.begin(); it != tasks_detect.end(); it++) {	//Kill状態の全てを削除する
		if ((*it).second == nullptr) { continue; }
		if ((*it).second->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it).second->groupName != gn_) { continue; }
		(*it).second->Suspend(m_);
		rtv = true;
	}
	for (auto it = tasks_undetect.begin(); it != tasks_undetect.end(); it++) {	//Kill状態の全てを削除する
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		(*it)->Suspend(m_);
		rtv = true;
	}
	//登録待ちタスクに対しても処理を行う(2016/03/22)
	for (auto it = add_Tasks.begin(); it != add_Tasks.end(); it++) {
		if ((*it) == nullptr) { continue; }
		if ((*it)->TaskStateCnt_Kill() > 0) { continue; }
		if ((*it)->groupName != gn_) { continue; }
		(*it)->Suspend(m_);
	}
	return rtv;
}


void myEngine::PushBack(const BTask::SP& t_)
{
	if (t_) {
		add_Tasks.push_back(t_);
	}
}
