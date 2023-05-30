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
	//���ׂẴJ�E���^�������I�Ɏ~�߂�B
	void myGameEngine::AllEndCounter() {
		for (auto& c : counterMap) {
			if (c.second.flag == COUNTER_FLAGS::ACTIVE) {
				c.second.flag = COUNTER_FLAGS::END;
			}
		}
	}
	//�w��̖��O�̃J�E���^�̏�Ԃ��擾����B
	myGameEngine::COUNTER_FLAGS myGameEngine::getCounterFlag(string str)
	{
		return counterMap[str].flag;
	}
	//�w��̖��O���J�E���^���쐬���A�w��t���[���̃J�E���g���n�߂�B
	void myGameEngine::StartCounter(string str, int count) {
		counterMap[str].count = count;
		counterMap[str].flag = COUNTER_FLAGS::ACTIVE;
	}
	myGameEngine::myGameEngine()
		:myEngine(
			//	�����
			1920,							//	��ʃT�C�YX
			1080,							//	��ʃT�C�YY
			//2,								//	�\���{��
			//1,								//	�}���`�T���v��(1or2or4)
			//true,							//	�t���X�N���[�����[�h
			1920,							//	2D�`��p�𑜓xX(���ݔ�Ή��j
			1080,							//	2D�`��p�𑜓xY(���ݔ�Ή��j
			//"�i������",	//	�E�C���h�E�^�C�g��
			//"GPG",							//	�E�B���h�E�N���X��
			1,//UpDate�Ăяo���p�x�@����Render�Ăяo���p�x�Ƃ̔䗦�Ō��܂�P�ȏ���w�肷�邱��
			1 //Render�Ăяo���p�x�@����UpDate�Ăяo���p�x�Ƃ̔䗦�Ō��܂�P�ȏ���w�肷�邱��
			//�Ⴆ�΁A120Hz�Œ��PC�ŁA60Hz�Ɠ����̏�Ԃœ������ꍇ�A1U/2R�Őݒ肷��΁AUpDate�̌Ăяo����1/2�ɂȂ�
		),
		camerasActive(camerasMax,false),
		cameras(new ML::Camera::SP[camerasMax])
	{
		//seting.txt�ɂ�郊�t���b�V�����[�g�����̈גǉ�
		ifstream  fin("./data/Shader/seting.txt");
		if (fin) {
			fin >> this->updateCall >> this->renderCall;
			fin.close();
		}
	}

	//�Q�[���G���W���ɒǉ��������̂̏������ƊJ��
	bool myGameEngine::Initialize()//(HWND wnd_)
	{
		//FPS�v��
		{
			c = new FPSCounter(REFRESHRATE);
		}

		//�Q�[���f�[�^�ǂݍ���
		this->CreateGameData();
		this->LoadGameDataJson();

		//�f�o�b�O�p�t�H���g�̏���
		this->debugFont = ML::Font::Create();

		//�f�o�b�O�t�@�C���̏�����
		ofstream ofs("./data/debug/Debug.txt");
		ofs << "----------DebugData----------" << endl;
		ofs.close();

		//�G�t�F�N�g�����p�̃^�X�N����
		this->effectGenerator = EffectGenerator::Object::Create(false);
		//�V�[���J�ڗp�̃^�X�N����
		this->scTrGenerator = SceneTransitionGenerator::Object::Create(false);

		////�J����1
		ge->cameras[0] = ML::Camera::Create(
			ML::Vec3(1.0f, 0.0f, 0.0f),	//��ʑ̂̈ʒu
			ML::Vec3(0.0f, 0.0f, 0.0f),	//�J�����̈ʒu
			ML::Vec3(0.0f, 1.0f, 0.0f),	//�J�����̏�����������x�N�g��
			ML::ToRadian(70.0f),		//����p
			10.0f,						//�ǂ̂��炢��O�̂��̂��f����
			1000000.0f,						//�ǂ̂��炢���̂��̂��f����
			1,							//	��ʔ䗦�i�����ŉ�ʃT�C�Y�̃A�X�y�N�g��ɂ��Ă����̂ŁA��ʕ�������ۂ̂ݎg�p�j
			ML::Box2D(0, 0, (float)ge->screenWidth / 2, (float)ge->screenHeight));	//��ʂɕ`�悷��͈�
		//�J����2
		ge->cameras[1] = ML::Camera::Create(
			ML::Vec3(1.0f, 0.0f, 0.0f),	//��ʑ̂̈ʒu
			ML::Vec3(0.0f, 0.0f, 0.0f),	//�J�����̈ʒu
			ML::Vec3(0.0f, 1.0f, 0.0f),	//�J�����̏�����������x�N�g��
			ML::ToRadian(70.0f),		//����p
			10.0f,						//�ǂ̂��炢��O�̂��̂��f����
			1000000.0f,						//�ǂ̂��炢���̂��̂��f����
			1,							//	��ʔ䗦�i�����ŉ�ʃT�C�Y�̃A�X�y�N�g��ɂ��Ă����̂ŁA��ʕ�������ۂ̂ݎg�p�j
			ML::Box2D((float)ge->screenWidth / 2, 0, (float)ge->screenWidth / 2, (float)ge->screenHeight));		//��ʂɕ`�悷��͈�
		//�g��Ȃ��J�����̏�����
		this->cameras[3] = this->cameras[2] = this->cameras[0];

		//�X�N���[���n���h��������
		this->screenHandle = -1;

		//�f�o�b�O�p���̕\��ON/OFF
		DebugMode(false);

		//�w�i�F�̐ݒ�i�f�t�H���g�j
		SetBackgroundColor(180, 200, 200);
		ClearDrawScreen();

		//Z�o�b�t�@���g�p����
		DxLib::SetUseZBuffer3D(TRUE);
		DxLib::SetWriteZBuffer3D(TRUE);

		//�������s�^�X�N�������Q�[���G���W���ɓo�^
		auto  ft = TitleManager::Object::Create(true);

		//----------------------------------------
		//���C���[���̕`���ON/OFF
		//----------------------------------------
		Layer_3D(true, true, false, false);
		Layer_2D(true, true);
		return true;
	}

	//�������
	myGameEngine::~myGameEngine()
	{
		//fps�J�E���^�[���
		free(c);
		//�J�����}�����
		delete[] cameras;
	}

	//�Q�[���G���W���ɒǉ��������̂̃X�e�b�v����
	void myGameEngine::UpDate()
	{
		//���͏��̍X�V
		this->UpDateInput();
		//�G�t�F�N�g�A�j���[�V�����̍X�V
		UpdateEffekseer3D();
		//�J�E���^����
		ActCounter();

	}

	//���͏��̍X�V����
	void myGameEngine::UpDateInput()
	{
		inpms.UpDate();
		inpkey.UpDate();
	}

	//2D�`����̃f�t�H���g�p�����[�^�ݒ�
	void myGameEngine::Set2DRenderState(DWORD l_)
	{
		//���ݏ����Ȃ�
		//if (l_ == 0) {
		//}
		//if (l_ == 1) {
		//}
	}
	//3D�`����̃f�t�H���g�p�����[�^�ݒ�
	void myGameEngine::Set3DRenderState(DWORD l_)
	{
		//���C���[0�Ԃɑ΂���ݒ�
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

	//3D�`��I���ݒ�
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

	//�f�o�b�O�֐�
	void myGameEngine::printToDebugFile(string debugstr_) {
		ofstream ofs("./data/debug/Debug.txt",ofstream::app);
		ofs << debugstr_ << endl;
		ofs.close();
	}

	//�X�N���[���̒��S��Ԃ�
	ML::Vec2 myGameEngine::screenCenter() {
		ML::Vec2 ct;
		ct.x = (float)this->screenWidth / 2.0f;
		ct.y = (float)this->screenHeight / 2.0f;
		return ct;
	}

	//�X�N���[���̑Ίp���̒�����Ԃ�
	float myGameEngine::screenDiagonalLength() {
		return fabsf(sqrtf(this->screenWidth * this->screenWidth + this->screenHeight * this->screenHeight));
	}
	//������������������������������������������������������������������������
	//�Q�[���f�[�^�ǉ�
	//������������������������������������������������������������������������
	void myGameEngine::CreateGameData() {

		BData::SP preData[] = {
			//�ǉ������f�[�^�͂��̉��ɏ���
			//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
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
			//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
			//�ǉ������f�[�^�͂��̏�ɏ���
		};

		//�z��ɂ���f�[�^�����ׂĒǉ�
		for (auto& it : preData) {
			this->data.push_back(it);
		}
	}

	void myGameEngine::LoadGameDataJson() {
		//�f�[�^�ǂݍ��݁E�����o��
		{
			ofstream ofs("gameDataLatest.bin", ofstream::binary);
			//data�z��̊e�v�f�œǂݍ��݁E�����o���������s��
			for (auto& it : this->data) {
				this->gameData["data"].push_back(it->LoadDataJson(false));
			}
			//BSON�f�[�^���t�@�C���ɕϊ�
			vector<uint8_t> gameDataBin = json::to_bson(this->gameData);
			for (auto& val : gameDataBin) {
				ofs.write((const char*)&val, sizeof(uint8_t));
			}

			ofs.close();
		}
	}

	void myGameEngine::LoadGameDataBson() {

		//BSON�t�@�C���ǂݍ���
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

		//data�z��̊e�v�f�œǂݍ��݁E�����o���������s��
		for (auto& it : this->data) {
			//�ǂݍ��񂾍ŐV��JSON�f�[�^���A���BSON�f�[�^�ɕۑ����Ă���
			it->LoadDataJson(true);
		}

	}
	//------------------------------------------------------------
}

myPG::myGameEngine* ge;
