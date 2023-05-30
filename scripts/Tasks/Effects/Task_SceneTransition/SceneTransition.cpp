//-------------------------------------------------------------------
//�V�[���J��
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "SceneTransition.h"

namespace  SceneTransition
{

	//����������������������������������������������������������������������������������
	//����
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �Q�[���f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
	int Data::myNum = BData::NODATA;
	//-------------------------------------------------------------------

	//�Q�[�����[�h��ǂݍ���
	json Data::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//�t�@�C���p�X
			std::ifstream fin("none");
			if (!fin) { return json(); }
			//JSON�t�@�C���ǂݍ���
			fin >> j;
			//�t�@�C���ǂݍ��ݏI��
			fin.close();
		}
		else {
			j = ge->gameData["data"][this->myArg];
		}
		//----------------------------------
		//���f�[�^�ǂݍ��ݏ���

		//��jpos = j[�L�[�̖��O]

		//----------------------------------
		//�i�����g�p����ꍇ��myPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 

		return j;
	}
	//-------------------------------------------------------------------
	//�Q�[���G���W������f�[�^���󂯎��
	Data::SP Data::UseData() {


		//-------------------------------------
		//�g�p���Ȃ��Ƃ����ݒ�imyNum��NOUSE�j�̏ꍇ
		if (myNum == BData::NOUSE) {
			return nullptr;
		}
		//-------------------------------------
		//myNum���ݒ肳��Ă��Ȃ��ꍇ�̏���
		//�Q�[���G���W���ɂ���f�[�^�̉��Ԗڂ���������������
		if (myNum == BData::NODATA) {
			for (auto& it : ge->data) {
				++myNum;
				//���g�̃f�[�^����������J�E���g�I���E�|�C���^��Ԃ�
				if (typeid(*it) == typeid(Data)) {
					return dynamic_pointer_cast<Data>(it);
				}
			}
			//������Ȃ������ꍇ�A�g�p���Ȃ��ݒ�ɐ؂�ւ���
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//���łɒl�����܂��Ă���ꍇ�̏���
		return dynamic_pointer_cast<Data>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	Data::Data() { }		//�R���X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	Data::~Data() { }		//�f�X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------

	//��������������������������������������������������������������������
	// ���\�[�X
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�f�[�^�N���X������󂯎��
		this->data = Data::UseData();

		//���f�[�^������


		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}			//�R���X�g���N�^
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }		//�f�X�g���N�^






	//����������������������������������������������������������������������������������
	//�W���V�[���J��
	//����������������������������������������������������������������������������������


		//-------------------------------------------------------------------
		//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  SceneTransitionStd::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		//�f�t�H���g�\���D��x�ݒ�
		this->render2D_Priority[1] = 0.1f;

		//�ǉ��̏���������
		return this->InitializeCustom();
	}
	//-------------------------------------------------------------------
	//�ǉ��̏����������i�X�[�p�[�N���X�Ȃ̂œ��ɂȂ��j
	bool  SceneTransitionStd::InitializeCustom() {
		return true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  SceneTransitionStd::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���

		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  SceneTransitionStd::UpDate()
	{
		//�o�����̏���
		if (this->CanDisappear() == false) {
			if (this->appearFlag == true) {
				this->AppearUpDate();
			}
		}
		//���Ŏ��̏���
		else{
			this->DisappearUpDate();
		}
	}
	//-------------------------------------------------------------------
	//�ǉ��̍X�V�����i�X�[�p�[�N���X�Ȃ̂œ��ɂȂ��j
	void  SceneTransitionStd::AppearUpDate() {

	}
	void SceneTransitionStd::DisappearUpDate() {

	}
	//-------------------------------------------------------------------
	//���Ŕ���
	bool SceneTransitionStd::CanDisappear() {
		return
			this->finishAppearFlag == true
			&& this->disappearFlag == true;
	}
	//-------------------------------------------------------------------
	//�u�R�c�`��v�P�t���[�����ɍs�������A���C���[0
	void SceneTransitionStd::Render3D_L0()
	{

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
	void  SceneTransitionStd::Render2D_AF()
	{
	}

	//����������������������������������������������������������������������
	// �����֘A���\�b�h
	//����������������������������������������������������������������������
	//-------------------------------------------------------------------
	bool  SceneTransitionStd::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	SceneTransitionStd::~SceneTransitionStd() { this->B_Finalize(); }
	bool  SceneTransitionStd::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	SceneTransitionStd::SceneTransitionStd()
		:transitionSpeed(0)
		,color()
		,appearFlag(false)
		,disappearFlag(false)
	{	}


	//����������������������������������������������������������������������������������
	//�V�[���J�ډ������t�s���~�b�h
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
	int DataRevPyramid::myNum = DataRevPyramid::NODATA;
	//-------------------------------------------------------------------

	//�Q�[�����[�h��ǂݍ���
	json DataRevPyramid::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//�t�@�C���p�X
			std::ifstream fin("./data/gamedata/sceneTransition/reversePyramid.json");
			if (!fin) { return json(); }
			//JSON�t�@�C���ǂݍ���
			fin >> j;
			//�t�@�C���ǂݍ��ݏI��
			fin.close();
		}
		else {
			j = ge->gameData["data"][this->myArg];
		}
		//----------------------------------
		//���f�[�^�ǂݍ��ݏ���
		this->cuts = j["cuts"];
		this->distance = j["distance"];
		this->moveSpeed = j["moveSpeed"];


		//----------------------------------
		//�i�����g�p����ꍇ��myPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 

		return j;
	}

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//--------------------------------------------------------
	//����������
	bool RevPyramid::InitializeCustom() {
		//�f�[�^�ǂݍ���
		this->data = DataRevPyramid::UseData();

		//������������
		this->Set(this->data->cuts, this->data->distance, this->data->moveSpeed);
		this->disappearCnt = 0;

		return true;
	}
	//--------------------------------------------------------
	//�o������
	void RevPyramid::AppearUpDate() {
		//�������ړ�
		for (auto& box : this->cutBoxes) {
			box.x -= this->moveSpeed;
			if (box.x < 0.0f) {
				box.x = 0.0f;
			}
		}

		//�o���I������
		if (this->cutBoxes[this->cutBoxes.size() - 1].x == 0.0f) {
			this->finishAppearFlag = true;
		}
	}

	//--------------------------------------------------------
	//���ŏ���
	void RevPyramid::DisappearUpDate() {
		//�������ړ�
		for (int c = 0; c < this->cutBoxes.size(); ++c) {
			if (this->disappearCnt * this->moveSpeed > this->distance * c) {
				this->cutBoxes[c].x -= this->moveSpeed;
			}
		}

		//���ŏI������
		if (this->cutBoxes[this->cutBoxes.size() - 1].x <= -(float)ge->screenWidth) {
			this->Kill();
		}
		++this->disappearCnt;
	}
	//--------------------------------------------------------
	//�`�揈��
	void RevPyramid::Render2D_AF() {
		for (const auto& box : this->cutBoxes) {
			ML::DrawBox(box, this->color, true);
		}
	}

	//--------------------------------------------------------
	//--------------------------------------------------
	//���̑����\�b�h
	//--------------------------------------------------
	void RevPyramid::Set(int cuts_, float distance_, float moveSpeed_) {
		this->cuts = cuts_;
		this->distance = distance_;
		this->moveSpeed = moveSpeed_;
		cutBoxes.clear();
		//�V�[���J�ڃ{�b�N�X�̐���
		for (int c = 0; c < this->cuts; ++c) {
			cutBoxes.push_back(ML::Box2D(
				(float)ge->screenWidth + (this->distance * c),	//x
				(float)ge->screenHeight / this->cuts * c,		//y
				(float)ge->screenWidth,							//w
				(float)ge->screenHeight / this->cuts			//h
			));
		}
	}


	//--------------------------------------------------------
	//�R���X�g���N�^
	RevPyramid::RevPyramid()
		:SceneTransitionStd()
	{}
	//--------------------------------------------------------
	//�f�X�g���N�^
	RevPyramid::~RevPyramid() {}

	//--------------------------------------------------------
	//�G�t�F�N�g��������
	RevPyramid::SP RevPyramid::Create(bool flagGameEnginePushBack_) {
		RevPyramid::SP ob = RevPyramid::SP(new RevPyramid());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^

			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//�Q�[���G���W������f�[�^���󂯎��
	DataRevPyramid::SP DataRevPyramid::UseData() {
		//-------------------------------------
		//�g�p���Ȃ��Ƃ����ݒ�imyNum��NOUSE�j�̏ꍇ
		if (myNum == BData::NOUSE) {
			return nullptr;
		}
		//-------------------------------------
		//myNum���ݒ肳��Ă��Ȃ��ꍇ�̏���
		//�Q�[���G���W���ɂ���f�[�^�̉��Ԗڂ���������������
		if (myNum == BData::NODATA) {
			for (auto& it : ge->data) {
				++myNum;
				//���g�̃f�[�^����������J�E���g�I���E�|�C���^��Ԃ�
				if (typeid(*it) == typeid(DataRevPyramid)) {
					return dynamic_pointer_cast<DataRevPyramid>(it);
				}
			}
			//������Ȃ������ꍇ�A�g�p���Ȃ��ݒ�ɐ؂�ւ���
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//���łɒl�����܂��Ă���ꍇ�̏���
		return dynamic_pointer_cast<DataRevPyramid>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataRevPyramid::DataRevPyramid() { }		//�R���X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	DataRevPyramid::~DataRevPyramid() { }		//�f�X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------






	//����������������������������������������������������������������������������������
	//�~�g��
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
	int DataCircleScaleUp::myNum = DataCircleScaleUp::NODATA;
	//-------------------------------------------------------------------

	//�Q�[�����[�h��ǂݍ���
	json DataCircleScaleUp::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//�t�@�C���p�X
			std::ifstream fin("./data/gamedata/sceneTransition/circleScaleUp.json");
			if (!fin) { return json(); }
			//JSON�t�@�C���ǂݍ���
			fin >> j;
			//�t�@�C���ǂݍ��ݏI��
			fin.close();
		}
		else {
			j = ge->gameData["data"][this->myArg];
		}
		//----------------------------------
		//���f�[�^�ǂݍ��ݏ���
		this->scaleUpSpeed = j["scaleUpSpeed"];

		//----------------------------------
		//�i�����g�p����ꍇ��myPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 

		return j;
	}

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//--------------------------------------------------------
	//����������
	bool CircleScaleUp::InitializeCustom() {
		//�f�[�^�ǂݍ���
		this->data = DataCircleScaleUp::UseData();

		//������������
		this->scaleUpSpeed = this->data->scaleUpSpeed;
		this->radius = 0;
		this->moveCnt = 0;
		this->lineThickness = 0;

		return true;
	}
	//--------------------------------------------------------
	//�o������
	void CircleScaleUp::AppearUpDate() {
		
		this->radius += scaleUpSpeed;
		float sdl = ge->screenDiagonalLength();
		if (this->radius >= sdl / 2) {
			this->radius = sdl / 2;
			this->lineThickness = sdl;
			this->finishAppearFlag = true;
		}
		++this->moveCnt;
	}

	//--------------------------------------------------------
	//���ŏ���
	void CircleScaleUp::DisappearUpDate() {
		this->lineThickness -= this->scaleUpSpeed * 2;
		if (this->lineThickness <= 0) {
			this->Kill();
		}
	}
	//--------------------------------------------------------
	//�`�揈��
	void CircleScaleUp::Render2D_AF() {
		ML::Vec2 center = ge->screenCenter();
		if (this->CanDisappear() == false) {
			DxLib::DrawCircle((int)center.x, (int)center.y, this->radius, ML::GetColor(this->color), true);
		}
		else {
			DxLib::DrawCircle((int)center.x, (int)center.y, this->radius, ML::GetColor(this->color), true, this->lineThickness);
		}
		
	}

	//--------------------------------------------------------
	//--------------------------------------------------
	//���̑����\�b�h
	//--------------------------------------------------
	void CircleScaleUp::Set(int scaleUpSpeed_) {
		this->scaleUpSpeed = scaleUpSpeed_;
	}


	//--------------------------------------------------------
	//�R���X�g���N�^
	CircleScaleUp::CircleScaleUp()
		:SceneTransitionStd()
	{}
	//--------------------------------------------------------
	//�f�X�g���N�^
	CircleScaleUp::~CircleScaleUp() {}

	//--------------------------------------------------------
	//�G�t�F�N�g��������
	CircleScaleUp::SP CircleScaleUp::Create(bool flagGameEnginePushBack_) {
		CircleScaleUp::SP ob = CircleScaleUp::SP(new CircleScaleUp());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^

			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//�Q�[���G���W������f�[�^���󂯎��
	DataCircleScaleUp::SP DataCircleScaleUp::UseData() {
		//-------------------------------------
		//�g�p���Ȃ��Ƃ����ݒ�imyNum��NOUSE�j�̏ꍇ
		if (myNum == BData::NOUSE) {
			return nullptr;
		}
		//-------------------------------------
		//myNum���ݒ肳��Ă��Ȃ��ꍇ�̏���
		//�Q�[���G���W���ɂ���f�[�^�̉��Ԗڂ���������������
		if (myNum == BData::NODATA) {
			for (auto& it : ge->data) {
				++myNum;
				//���g�̃f�[�^����������J�E���g�I���E�|�C���^��Ԃ�
				if (typeid(*it) == typeid(DataCircleScaleUp)) {
					return dynamic_pointer_cast<DataCircleScaleUp>(it);
				}
			}
			//������Ȃ������ꍇ�A�g�p���Ȃ��ݒ�ɐ؂�ւ���
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//���łɒl�����܂��Ă���ꍇ�̏���
		return dynamic_pointer_cast<DataCircleScaleUp>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataCircleScaleUp::DataCircleScaleUp() { }		//�R���X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	DataCircleScaleUp::~DataCircleScaleUp() { }		//�f�X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------


	/*
	
	----�G�t�F�N�g�ЂȌ`----

	//����������������������������������������������������������������������������������
	//�~�g��
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
	int DataSampleScTr::myNum = DataSampleScTr::NODATA;
	//-------------------------------------------------------------------

	//�Q�[�����[�h��ǂݍ���
	json DataSampleScTr::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//�t�@�C���p�X
			std::ifstream fin("./data/gamedata/sceneTransition/SampleScTr.json");
			if (!fin) { return json(); }
			//JSON�t�@�C���ǂݍ���
			fin >> j;
			//�t�@�C���ǂݍ��ݏI��
			fin.close();
		}
		else {
			j = ge->gameData["data"][this->myArg];
		}
		//----------------------------------
		//���f�[�^�ǂݍ��ݏ���


		//----------------------------------
		//�i�����g�p����ꍇ��myPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j

		return j;
	}

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//--------------------------------------------------------
	//����������
	bool SampleScTr::InitializeCustom() {
		//�f�[�^�ǂݍ���
		this->data = DataSampleScTr::UseData();

		//������������


		return true;
	}
	//--------------------------------------------------------
	//�o������
	void SampleScTr::AppearUpDate() {

	}

	//--------------------------------------------------------
	//���ŏ���
	void SampleScTr::DisappearUpDate() {

	}
	//--------------------------------------------------------
	//�`�揈��
	void SampleScTr::Render2D_AF() {


	}

	//--------------------------------------------------------
	//--------------------------------------------------
	//���̑����\�b�h
	//--------------------------------------------------
	void SampleScTr::Set(int cuts_, float distance_, float moveSpeed_) {

	}


	//--------------------------------------------------------
	//�R���X�g���N�^
	SampleScTr::SampleScTr()
		:SceneTransitionStd()
	{}
	//--------------------------------------------------------
	//�f�X�g���N�^
	SampleScTr::~SampleScTr() {}

	//--------------------------------------------------------
	//�G�t�F�N�g��������
	SampleScTr::SP SampleScTr::Create(bool flagGameEnginePushBack_) {
		SampleScTr::SP ob = SampleScTr::SP(new SampleScTr());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^

			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//�Q�[���G���W������f�[�^���󂯎��
	DataSampleScTr::SP DataSampleScTr::UseData() {
		//-------------------------------------
		//�g�p���Ȃ��Ƃ����ݒ�imyNum��NOUSE�j�̏ꍇ
		if (myNum == BData::NOUSE) {
			return nullptr;
		}
		//-------------------------------------
		//myNum���ݒ肳��Ă��Ȃ��ꍇ�̏���
		//�Q�[���G���W���ɂ���f�[�^�̉��Ԗڂ���������������
		if (myNum == BData::NODATA) {
			for (auto& it : ge->data) {
				++myNum;
				//���g�̃f�[�^����������J�E���g�I���E�|�C���^��Ԃ�
				if (typeid(*it) == typeid(DataSampleScTr)) {
					return dynamic_pointer_cast<DataSampleScTr>(it);
				}
			}
			//������Ȃ������ꍇ�A�g�p���Ȃ��ݒ�ɐ؂�ւ���
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//���łɒl�����܂��Ă���ꍇ�̏���
		return dynamic_pointer_cast<DataSampleScTr>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataSampleScTr::DataSampleScTr() { }		//�R���X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	DataSampleScTr::~DataSampleScTr() { }		//�f�X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	*/
}

