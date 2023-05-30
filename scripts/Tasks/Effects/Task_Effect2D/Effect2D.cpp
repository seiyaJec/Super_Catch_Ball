//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Effect2D.h"

namespace  Effect2D
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

		//�f�t�H���g�\���D��x�ݒ�
		this->render2D_Priority[1] = 0.2f;

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



	//����������������������������������������������������������������������������������
	//�����������E�̏��ňړ�����G�t�F�N�g
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �Q�[���f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
	int DataL_C_R::myNum = DataL_C_R::NODATA;
	//-------------------------------------------------------------------

	//�Q�[�����[�h��ǂݍ���
	json DataL_C_R::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//�t�@�C���p�X
			std::ifstream fin("./data/effectData/effectL_C_R");
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

		this->moveSpeed = j["moveSpeedDefault"];
		this->moveSpeedMin = j["moveSpeedMinDefault"];
		this->timeStayCenter = j["timeStayCenterDefault"];

		//----------------------------------
		//�i�����g�p����ꍇ��myPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 

		return j;
	}
	//-------------------------------------------------------------------
	//�Q�[���G���W������f�[�^���󂯎��
	DataL_C_R::SP DataL_C_R::UseData() {
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
				if (typeid(*it) == typeid(DataL_C_R)) {
					return dynamic_pointer_cast<DataL_C_R>(it);
				}
			}
			//������Ȃ������ꍇ�A�g�p���Ȃ��ݒ�ɐ؂�ւ���
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//���łɒl�����܂��Ă���ꍇ�̏���
		return dynamic_pointer_cast<DataL_C_R>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataL_C_R::DataL_C_R() { }		//�R���X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	DataL_C_R::~DataL_C_R() { }		//�f�X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//--------------------------------------------------------
	//����������
	bool EffectL_C_R::InitializeCustom() {
		this->data = DataL_C_R::UseData();

		ML::Size2 imgSize = this->img->get_size();
		this->pos.x = imgSize.w / -2;
		this->pos.y = (float)ge->screenHeight / 2.0f;
		this->moveVec.x = this->data->moveSpeed;
		this->moveSpeedMin = this->data->moveSpeedMin;
		this->timeStayCenter = this->data->timeStayCenter;
		this->timeCnt = 0;
		this->passedCenter = false;

		return true;
	}
	//--------------------------------------------------------
	//�X�V����
	void EffectL_C_R::UpDate() {

		//��ʒ����ɂ��邩
		if (this->IsPassedCenterOfScreen() == false) {
			this->pos += this->moveVec;
		}
		else {
			if (this->timeCnt >= this->timeStayCenter) {
				this->pos += this->moveVec;
			}
			++timeCnt;
		}


		//��ʊO�ɏo����L��
		if (this->IsOutOfScreen() == true) {
			this->Kill();
		}

	}

	//--------------------------------------------------
	//���̑����\�b�h
	//--------------------------------------------------
	//��ʉE���֍s�������Ȃ��Ȃ�����
	bool EffectL_C_R::IsOutOfScreen() {
		return this->pos.x - this->img->get_size().w * this->transform.scaleX >= ge->screenWidth;
	}
	//��ʒ�����ʉ߂�����
	bool EffectL_C_R::IsPassedCenterOfScreen() {
		ML::Vec2 screenCenter = ge->screenCenter();
		return
			this->pos.x >= screenCenter.x
			&& this->pos.y >= screenCenter.y;
	}

	//--------------------------------------------------------
	//�Z�b�^
	void EffectL_C_R::setMoveSpeed(float moveSpeed_) {
		if (moveSpeed_ <= 0) {
			string debug("EffectL_C_R::setMoveSpeed>>�s���ȃf�[�^�l�ł��I");
			ge->printToDebugFile(debug);
		}
		this->moveSpeed = moveSpeed_;
		this->moveVec.x = moveSpeed_;
	}
	void EffectL_C_R::setTimeStayCenter(int timeStayCenter_) {
		if (timeStayCenter_ < 0) {
			string debug("EffectL_C_R::setMoveSpeed>>�s���ȃf�[�^�l�ł��I");
			ge->printToDebugFile(debug);
		}
		this->timeStayCenter = timeStayCenter_;
	}
	//--------------------------------------------------------
	//�R���X�g���N�^
	EffectL_C_R::EffectL_C_R()
		:EffectStd()
	{}
	//--------------------------------------------------------
	//�f�X�g���N�^
	EffectL_C_R::~EffectL_C_R() {}

	//--------------------------------------------------------
	//�G�t�F�N�g��������
	EffectL_C_R::SP EffectL_C_R::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		EffectL_C_R::SP ob = EffectL_C_R::SP(new EffectL_C_R());
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

	//����������������������������������������������������������������������������������
	//�X�^���v�i�T�C�Yn�{�����{���������j
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
	int DataEffectStamp::myNum = DataEffectStamp::NODATA;
	//-------------------------------------------------------------------

	//�Q�[�����[�h��ǂݍ���
	json DataEffectStamp::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//�t�@�C���p�X
			std::ifstream fin("./data/effectData/effectStamp.json");
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
		this->stampDecSpeed = j["stampDecSpeedDefault"];
		this->stampScaleMax = j["stampScaleMaxDefault"];
		this->alphaDecSpeed = j["alphaDecSpeedDefault"];
		this->alphaWaitDec = j["alphaWaitDecDefault"];

		//----------------------------------
		//�i�����g�p����ꍇ��myPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 

		return j;
	}

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//--------------------------------------------------------
	//����������
	bool EffectStamp::InitializeCustom() {
		//�f�[�^�ǂݍ���
		this->data = DataEffectStamp::UseData();

		//������������
		this->setScaleXLoop(0.01f, 2.0f);
		this->setPos(ge->screenCenter());
		this->setStamp(this->data->stampDecSpeed, this->data->stampScaleMax);
		this->setAlpha(this->data->alphaDecSpeed, this->data->alphaWaitDec);

		return true;
	}
	//--------------------------------------------------------
	//�ǉ����čs���X�V����
	void EffectStamp::UpDate() {
		
		//�X�P�[���l������������
		this->loopScaleX.Addto(this->transform.scaleX);
		this->loopScaleY.Addto(this->transform.scaleY);
		
		//�X�P�[���l���ŏ��ɂȂ�����
		if (this->IsScaleMin() == true) {
			//���Ԍv��
			if (this->life > 0.0f) {
				--this->life;
			}
			//0�ɂȂ����瓧�����J�n
			else {
				this->loopAddAlpha.Addto(this->color.a);
				if (this->color.a <= 0.0f) {
					this->Kill();
				}
			}
		}

	}
	//--------------------------------------------------
	//���̑����\�b�h
	//--------------------------------------------------
	//�X�P�[���l���ŏ���
	bool EffectStamp::IsScaleMin() {

		//X�EY�������ŏ��Ȃ�true
		if (this->transform.scaleX <= this->loopScaleX.lpmin) {
			if (this->transform.scaleY <= this->loopScaleY.lpmin) {
				return true;
			}
		}

		//�����łȂ����false
		return false;
	}
	//�T�C�Y�ω��ݒ�i�k���X�s�[�h�A�����T�C�Y�j
	void EffectStamp::setStamp(float decSpeed_, float scaleMax_) {
		setScaleXLoop(-decSpeed_, scaleMax_, 1.0f);
		setScaleYLoop(-decSpeed_, scaleMax_, 1.0f);
		setImgScale(scaleMax_, scaleMax_);
	}

	//�����������ݒ�i�������X�s�[�h�A�������܂ł̎��ԁi�t���[���P�ʁj�j
	void EffectStamp::setAlpha(float decSpeed_, float waitDec_) {
		this->setAlphaLoop(-decSpeed_, 1.0f, 0.0f);
		this->setLife(waitDec_);
		this->color.a = 1.0f;
	}

	//--------------------------------------------------------
	//�R���X�g���N�^
	EffectStamp::EffectStamp()
		:EffectStd()
	{}
	//--------------------------------------------------------
	//�f�X�g���N�^
	EffectStamp::~EffectStamp() {}

	//--------------------------------------------------------
	//�G�t�F�N�g��������
	EffectStamp::SP EffectStamp::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		EffectStamp::SP ob = EffectStamp::SP(new EffectStamp());
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
	DataEffectStamp::SP DataEffectStamp::UseData() {
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
				if (typeid(*it) == typeid(DataEffectStamp)) {
					return dynamic_pointer_cast<DataEffectStamp>(it);
				}
			}
			//������Ȃ������ꍇ�A�g�p���Ȃ��ݒ�ɐ؂�ւ���
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//���łɒl�����܂��Ă���ꍇ�̏���
		return dynamic_pointer_cast<DataEffectStamp>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataEffectStamp::DataEffectStamp() { }		//�R���X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	DataEffectStamp::~DataEffectStamp() { }		//�f�X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------


	//����������������������������������������������������������������������������������
	//�T�u�G�t�F�N�g�����_���o��
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
	int DataGenerateSubRandom::myNum = DataGenerateSubRandom::NODATA;
	//-------------------------------------------------------------------

	//�Q�[�����[�h��ǂݍ���
	json DataGenerateSubRandom::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//�t�@�C���p�X
			std::ifstream fin("./data/gamedata/effect/generateSubRandom.json");
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
		this->subImagePath = j["subImagePathDefault"];
		this->subImageSize = j["subImageSizeDefault"];
		this->subMoveSpeed = j["subMoveSpeedDefault"];
		this->subLife = j["subLifeDefault"];
		this->subGenerateDistance = j["subGenerateDistanceDefault"];
		this->moveVec = j["moveVecDefault"];
		this->life = j["lifeDefault"];
		this->rotateLoop = j["rotateLoopDefault"];
		//----------------------------------
		//�i�����g�p����ꍇ��myPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 

		return j;
	}

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//--------------------------------------------------------
	//����������
	bool GenerateSubRandom::InitializeCustom() {
		//�f�[�^�ǂݍ���
		this->data = DataGenerateSubRandom::UseData();

		//������������
		this->SetSubImage(this->data->subImagePath, this->data->subImageSize);
		this->SetSubMove(this->data->subMoveSpeed, this->data->subLife, this->data->subGenerateDistance);
		this->setMoveVec(this->data->moveVec);
		this->setLife(this->data->life);
		this->setRotateLoop(this->data->rotateLoop.adds, this->data->rotateLoop.lpmax, this->data->rotateLoop.lpmin);
		return true;
	}
	//--------------------------------------------------------
	void GenerateSubRandom::UpDate() {
		//�ړ�
		this->pos += this->moveVec;

		//�T�u�G�t�F�N�g����
		if (this->moveCnt % this->subGenerateDistance == 0) {
			auto effect = MoveStraight::Create(this->subImgFilePath, this->subImgSize, true);
			effect->setPos(this->pos);
			//�ړ������ݒ�
			float angle = ML::ToRadian(rand() % 360);
			ML::Vec2 moveVecS(cosf(angle), sinf(angle));
			moveVecS *= this->subMoveSpeed;
			effect->setMoveVec(moveVecS);
			//�������Ԑݒ�
			effect->setLife(this->subLife);
		}

		//��]
		this->loopRotate.Addto(this->transform.rotateDeg);

		this->CountLife();
		++this->moveCnt;
	}
	//--------------------------------------------------
	//���̑����\�b�h
	//--------------------------------------------------
	//�T�u�G�t�F�N�g�摜�ݒ�
	void GenerateSubRandom::SetSubImage(string filePath_, ML::Size2 size_) {
		this->subImgFilePath = filePath_;
		this->subImgSize = size_;
	}
	//�T�u�G�t�F�N�g�����ݒ�
	void GenerateSubRandom::SetSubMove(float moveSpeed_, int life_, int generateDistance_) {
		this->subMoveSpeed = moveSpeed_;
		this->subLife = life_;
		this->subGenerateDistance = generateDistance_;
	}

	//--------------------------------------------------------
	//�R���X�g���N�^
	GenerateSubRandom::GenerateSubRandom()
		:EffectStd()
	{}
	//--------------------------------------------------------
	//�f�X�g���N�^
	GenerateSubRandom::~GenerateSubRandom() {}

	//--------------------------------------------------------
	//�G�t�F�N�g��������
	GenerateSubRandom::SP GenerateSubRandom::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		GenerateSubRandom::SP ob = GenerateSubRandom::SP(new GenerateSubRandom());
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
	DataGenerateSubRandom::SP DataGenerateSubRandom::UseData() {
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
				if (typeid(*it) == typeid(DataGenerateSubRandom)) {
					return dynamic_pointer_cast<DataGenerateSubRandom>(it);
				}
			}
			//������Ȃ������ꍇ�A�g�p���Ȃ��ݒ�ɐ؂�ւ���
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//���łɒl�����܂��Ă���ꍇ�̏���
		return dynamic_pointer_cast<DataGenerateSubRandom>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataGenerateSubRandom::DataGenerateSubRandom() { }		//�R���X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	DataGenerateSubRandom::~DataGenerateSubRandom() { }		//�f�X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------






	//����������������������������������������������������������������������������������
	//�ړ��݂̂̃G�t�F�N�g
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
	int DataMoveStraight::myNum = DataMoveStraight::NODATA;
	//-------------------------------------------------------------------

	//�Q�[�����[�h��ǂݍ���
	json DataMoveStraight::LoadDataJson(bool useBinary_) {
		json j;

		if (useBinary_ == false) {
			//�t�@�C���p�X
			std::ifstream fin("./data/gamedata/effect/effectMoveStraight.json");
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
		this->moveVec = j["moveVec"];
		this->life = j["life"];
		//----------------------------------
		//�i�����g�p����ꍇ��myPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 

		return j;
	}

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//--------------------------------------------------------
	//����������
	bool MoveStraight::InitializeCustom() {
		//�f�[�^�ǂݍ���
		this->data = DataMoveStraight::UseData();

		//������������
		this->setMoveVec(this->data->moveVec);
		this->setLife(this->data->life);
		return true;
	}
	//--------------------------------------------------
	//���̑����\�b�h
	//--------------------------------------------------



	//--------------------------------------------------------
	//�R���X�g���N�^
	MoveStraight::MoveStraight()
		:EffectStd()
	{}
	//--------------------------------------------------------
	//�f�X�g���N�^
	MoveStraight::~MoveStraight() {}

	//--------------------------------------------------------
	//�G�t�F�N�g��������
	MoveStraight::SP MoveStraight::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		MoveStraight::SP ob = MoveStraight::SP(new MoveStraight());
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
	DataMoveStraight::SP DataMoveStraight::UseData() {
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
				if (typeid(*it) == typeid(DataMoveStraight)) {
					return dynamic_pointer_cast<DataMoveStraight>(it);
				}
			}
			//������Ȃ������ꍇ�A�g�p���Ȃ��ݒ�ɐ؂�ւ���
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//���łɒl�����܂��Ă���ꍇ�̏���
		return dynamic_pointer_cast<DataMoveStraight>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataMoveStraight::DataMoveStraight() { }		//�R���X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	DataMoveStraight::~DataMoveStraight() { }		//�f�X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------

	//����������������������������������������������������������������������������������
	//���S�Ɍ������ē����G�t�F�N�g
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
	int DataMoveToCenter::myNum = DataMoveToCenter::NODATA;
	//-------------------------------------------------------------------

	//�Q�[�����[�h��ǂݍ���
	json DataMoveToCenter::LoadDataJson(bool useBinary_) {
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
		this->radius = j["distanceDefault"];
		this->generateDistance = j["generateDistanceDefault"];
		this->speed = j["speedDefault"];
		this->centerPos = j["centerPosDefault"];
		//----------------------------------
		//�i�����g�p����ꍇ��myPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 

		return j;
	}

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//--------------------------------------------------------
	//����������
	bool MoveToCenter::InitializeCustom() {
		//�f�[�^�ǂݍ���
		this->data = DataMoveToCenter::UseData();

		//������������
		this->setRadius(this->data->radius);
		this->setGenerateDistance(this->data->generateDistance);
		this->setSpeed(this->data->speed);
		this->setPos(this->data->centerPos);
		this->moveCnt = 0;
		return true;
	}
	//--------------------------------------------------------
	//�X�V����
	void MoveToCenter::UpDate() {
		if (this->moveCnt % this->generateDistance == 0) {
			auto sub = MoveStraight::Create(this->img->get_filePath(), this->img->get_size(), true);
			float angle = ML::ToRadian(rand() % 360);
			ML::Vec2 moveVecN(cosf(angle) * this->radius, sinf(angle) * this->radius);
			sub->setPos(ML::Vec2(moveVecN * this->radius));
			sub->setMoveVec(ML::Vec2(moveVecN * this->speed * -1));
		}
		++this->moveCnt;
	}
	//--------------------------------------------------
	//���̑����\�b�h
	//--------------------------------------------------



	//--------------------------------------------------------
	//�R���X�g���N�^
	MoveToCenter::MoveToCenter()
		:EffectStd()
	{}
	//--------------------------------------------------------
	//�f�X�g���N�^
	MoveToCenter::~MoveToCenter() {}

	//--------------------------------------------------------
	//�G�t�F�N�g��������
	MoveToCenter::SP MoveToCenter::Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_) {
		MoveToCenter::SP ob = MoveToCenter::SP(new MoveToCenter());
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
	DataMoveToCenter::SP DataMoveToCenter::UseData() {
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
				if (typeid(*it) == typeid(DataMoveToCenter)) {
					return dynamic_pointer_cast<DataMoveToCenter>(it);
				}
			}
			//������Ȃ������ꍇ�A�g�p���Ȃ��ݒ�ɐ؂�ւ���
			myNum = BData::NOUSE;
			return nullptr;

		}
		//-------------------------------------
		//���łɒl�����܂��Ă���ꍇ�̏���
		return dynamic_pointer_cast<DataMoveToCenter>(ge->data[myNum]);
	}
	//-------------------------------------------------------------------
	DataMoveToCenter::DataMoveToCenter() { }		//�R���X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------
	DataMoveToCenter::~DataMoveToCenter() { }		//�f�X�g���N�^�i���ɏ����Ȃ��j
	//-------------------------------------------------------------------


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

