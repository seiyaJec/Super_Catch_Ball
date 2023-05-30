//-------------------------------------------------------------------
//3D�G�t�F�N�g
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Effect3D.h"

namespace  Effect3D
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
	//�W���G�t�F�N�g
	//����������������������������������������������������������������������������������


		//-------------------------------------------------------------------
		//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  EffectStd::Initialize(string imgFilePath_, ML::Size2 imgSize_)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		auto itr = this->res->images.find(imgFilePath_);

		//�摜���܂�����Ă��Ȃ��ꍇ
		if (itr == this->res->images.end()) {
			//�摜�ǂݍ���
			auto newimg = ML::Image::Create(imgFilePath_, imgSize_);
			if (newimg) {
				this->res->images.insert(std::make_pair(imgFilePath_, newimg));
				this->img = newimg;
			}
			//�ǂݍ��ݎ��s
			else {
				return false;
			}
		}
		//�摜������Ă���ꍇ
		else {
			this->img = itr->second;
		}

		//��]�̒��S��������
		this->setImgRotateCenter(this->img->CenterPos());

		//�ǉ��̏���������
		return this->InitializeCustom();
	}
	//-------------------------------------------------------------------
	//�ǉ��̏����������i�X�[�p�[�N���X�Ȃ̂œ��ɂȂ��j
	bool  EffectStd::InitializeCustom() {
		return true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  EffectStd::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���

		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  EffectStd::UpDate()
	{
		this->pos += moveVec;

		//���[�v
		this->loopRotate.Addto(this->transform.rotateDeg);
		this->loopScaleX.Addto(this->transform.scaleX);
		this->loopScaleY.Addto(this->transform.scaleY);

		this->loopAddAlpha.Addto(this->color.a);
		this->CountLife();

		//�ǉ��̍X�V�����i�p�������N���X�p�j
		this->UpDateAdd();
	}
	//-------------------------------------------------------------------
	//�ǉ��̍X�V�����i�X�[�p�[�N���X�Ȃ̂œ��ɂȂ��j
	void  EffectStd::UpDateAdd() {

	}
	//-------------------------------------------------------------------
	//�u�R�c�`��v�P�t���[�����ɍs�������A���C���[0
	void EffectStd::Render3D_L0()
	{

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
	void  EffectStd::Render2D_AF()
	{
		this->img->DrawTransform(this->pos, this->transform, true, this->color.a);
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�R���X�g���N�^
	EffectStd::Loop::Loop()
		:adds(0.0f)
		, lpmax(9999.0f)
		, lpmin(-9999.0f)
	{}
	//���[�v�ݒ�
	void EffectStd::Loop::Set(float adds_, float lpmax_, float lpmin_) {
		adds = adds_;
		lpmax = lpmax_;
		lpmin = lpmin_;
	}
	//�������ԃJ�E���g
	void EffectStd::CountLife() {
		--this->life;
		if (this->life <= 0) {
			this->Kill();
		}
	}
	//���[�v
	void EffectStd::Loop::Addto(float& addto_) {
		if (this->adds > 0.0f) {
			addto_ = min(addto_ + this->adds, this->lpmax);
		}
		else if (this->adds < 0.0f) {
			addto_ = max(addto_ + this->adds, this->lpmin);
		}
	}
	//�������f�[�^����ǂݍ���
	void EffectStd::ReadFromInitStd(const InitStd& initStd_) {
		this->setPos(initStd_.pos);
		this->setMoveVec(initStd_.moveVec);
		this->setLife(initStd_.life);
		this->setImgRotate(initStd_.imgRotDeg);
		this->setImgScale(initStd_.scale.x, initStd_.scale.y);
		this->setImgFlip(initStd_.imgFlipX, initStd_.imgFlipY);
		this->setColor(initStd_.color);
		this->setRotateLoop(initStd_.lpRotate.adds, initStd_.lpRotate.lpmax, initStd_.lpRotate.lpmin);
		this->setScaleXLoop(initStd_.lpScaleX.adds, initStd_.lpScaleX.lpmax, initStd_.lpScaleX.lpmin);
		this->setScaleYLoop(initStd_.lpScaleY.adds, initStd_.lpScaleY.lpmax, initStd_.lpScaleY.lpmin);
		this->setAlphaLoop(initStd_.lpAlpha.adds, initStd_.lpAlpha.lpmax, initStd_.lpAlpha.lpmin);
	}

	//����������������������������������������������������������������������
	// �����֘A���\�b�h
	//����������������������������������������������������������������������
	//-------------------------------------------------------------------
	bool  EffectStd::B_Initialize(string imgFilePath_, ML::Size2 size_)
	{
		return  this->Initialize(imgFilePath_, size_);
	}
	//-------------------------------------------------------------------
	EffectStd::~EffectStd() { this->B_Finalize(); }
	bool  EffectStd::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	EffectStd::EffectStd()
		:img(nullptr)
		, pos()
		, moveVec()
		, life(100)
		, transform()
		, color()
		, loopRotate()
		, loopScaleX()
		, loopScaleY()
		, loopAddAlpha()
	{	}
	//��������������������������������������������������������������������
	// �������p�f�[�^�i�[�N���X���\�b�h
	//��������������������������������������������������������������������
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//json�t�@�C���ւ̏����o���i�y�ʃ��[�v�\���́j
	void to_json(json& j_, const InitStd::LoopLt& loop_) {
		j_ = json{
			{"addvalues", loop_.adds},
			{"loopMax", loop_.lpmax},
			{"loopMin", loop_.lpmin}
		};
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//json�t�@�C���̓ǂݍ��݁i�y�ʃ��[�v�\���́j
	void from_json(const json& j_, InitStd::LoopLt& loop_) {
		j_.at("addvalues").get_to(loop_.adds);
		j_.at("loopMax").get_to(loop_.lpmax);
		j_.at("loopMin").get_to(loop_.lpmin);
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//json�t�@�C���ւ̏����o���i�������p�f�[�^�i�[�N���X�j
	void to_json(json& j_, const InitStd& effStd_) {
		j_ = json{
			{"imageFilePath", effStd_.imgFilePath},
			{"imageSize", effStd_.imgSize},
			{"imageRotationCenter", effStd_.imgRotCt},
			{"imageRotateDegree", effStd_.imgRotDeg},
			{"imageFlipX",effStd_.imgFlipX },
			{"imageFlipY",effStd_.imgFlipY},
			{"position", effStd_.pos},
			{"moveVec", effStd_.moveVec},
			{"color", effStd_.color},
			{"scale", effStd_.scale},
			{"life", effStd_.life},
			{"loopScaleX", effStd_.lpScaleX},
			{"loopScaleY", effStd_.lpScaleY},
			{"loopAlpha", effStd_.lpAlpha},
			{"loopRotate", effStd_.lpRotate},
		};
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//json�t�@�C���̓ǂݍ��݁i�������p�f�[�^�i�[�N���X�j
	void from_json(const json& j_, InitStd& effStd_) {
		j_.at("imageFilePath").get_to(effStd_.imgFilePath);
		j_.at("imageSize").get_to(effStd_.imgSize);
		j_.at("imageRotationCenter").get_to(effStd_.imgRotCt);
		j_.at("imageRotateDegree").get_to(effStd_.imgRotDeg);
		j_.at("imageFlipX").get_to(effStd_.imgFlipX);
		j_.at("imageFlipY").get_to(effStd_.imgFlipY);
		j_.at("position").get_to(effStd_.pos);
		j_.at("moveVec").get_to(effStd_.moveVec);
		j_.at("color").get_to(effStd_.color);
		j_.at("scale").get_to(effStd_.scale);
		j_.at("life").get_to(effStd_.life);
		j_.at("loopScaleX").get_to(effStd_.lpScaleX);
		j_.at("loopScaleY").get_to(effStd_.lpScaleY);
		j_.at("loopAlpha").get_to(effStd_.lpAlpha);
		j_.at("loopRotate").get_to(effStd_.lpRotate);
	}










	/*
	
	----�G�t�F�N�g�ЂȌ`----

	//����������������������������������������������������������������������������������
	//�G�t�F�N�g������
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
	int EffectSampleData::myNum = EffectSampleData::NODATA;
	//-------------------------------------------------------------------

	//�Q�[�����[�h��ǂݍ���
	json EffectSampleData::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//�t�@�C���p�X
			std::ifstream fin("./data/effectData/effectSample.json");
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
	bool EffectSample::InitializeCustom() {
		//�f�[�^�ǂݍ���
		this->data = EffectSampleData::UseData();

		//������������

		return true;
	}
	//--------------------------------------------------
	//���̑����\�b�h
	//--------------------------------------------------



	//--------------------------------------------------------
	//�R���X�g���N�^
	EffectSample::EffectSample()
		:EffectStd()
	{}
	//--------------------------------------------------------
	//�f�X�g���N�^
	EffectSample::~EffectSample() {}

	//--------------------------------------------------------
	//�G�t�F�N�g��������
	EffectSample::SP EffectSample::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		EffectSample::SP ob = EffectSample::SP(new EffectSample());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^

			}
			if (!ob->B_Initialize(imgFilePath_, size_)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//�Q�[���G���W������f�[�^���󂯎��
	EffectSampleData::SP EffectSampleData::UseData() {
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
				if (typeid(*it) == typeid(EffectSampleData)) {
					return dynamic_pointer_cast<EffectSampleData>(it);
				}
			}
			//������Ȃ������ꍇ�A�g�p���Ȃ��ݒ�ɐ؂�ւ���
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//���łɒl�����܂��Ă���ꍇ�̏���
		return dynamic_pointer_cast<EffectSampleData>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	EffectSampleData::EffectSampleData() { }		//�R���X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	EffectSampleData::~EffectSampleData() { }		//�f�X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	*/
}

