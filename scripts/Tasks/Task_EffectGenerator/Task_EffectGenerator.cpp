//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../Engine/myPG/myPG.h"
#include  "Task_EffectGenerator.h"

namespace  EffectGenerator
{
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
			std::ifstream fin("./data/gamedata/effectGenerator.json");
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
		auto& jReady = j["effectReady"];
		this->imageReady = jReady["imageData"];
		this->readyMoveSpeed = jReady["moveSpeed"];
		this->readyTimeStayCenter = jReady["timeStayCenter"];

		auto& jStart = j["effectStart"];
		this->imageStart = jStart["imageData"];
		this->startScaleDecSpeed = jStart["scaleDecSpeed"];
		this->startScaleMax = jStart["scaleMax"];
		this->startAlphaDecSpeed = jStart["alphaDecSpeed"];
		this->startAlphaWaitDec = jStart["alphaWaitDec"];
		
		auto& jFinish = j["effectFinish"];
		this->imageFinish = jFinish["imageData"];
		this->finishScaleDecSpeed = jFinish["scaleDecSpeed"];
		this->finishScaleMax = jFinish["scaleMax"];
		this->finishAlphaDecSpeed = jFinish["alphaDecSpeed"];
		this->finishAlphaWaitDec = jFinish["alphaWaitDec"];

		auto& jGBall = j["effectGorgeousBall"];
		this->imageGBall = jGBall["imageData"];
		this->imageGBallSub = jGBall["subImageData"];
		this->GBallMoveVec = jGBall["moveVec"];
		this->GBallLife = jGBall["life"];
		this->GBallRotateLoop = jGBall["rotateLoop"];
		this->GBallSubMoveSpeed = jGBall["subMoveSpeed"];
		this->GBallSubLife = jGBall["subLife"];
		this->GBallSubGenerateDistance = jGBall["subGenerateDistance"];
		//----------------------------------
		//�i�����g�p����ꍇ��myPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 

		return j;
	}

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

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//�u�R�c�`��v�P�t���[�����ɍs�������A���C���[0
	void Object::Render3D_L0()
	{
		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//���̑��֐�
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//json�t�@�C���ւ̏����o��
	void to_json(json& j_, const ImageData& imgData_) {
		j_ = json{
			{"filePath", imgData_.filePath},
			{"size", imgData_.size}
		};
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//json�t�@�C���̓ǂݍ���
	void from_json(const json& j_, ImageData& imgData_) {
		j_.at("filePath").get_to(imgData_.filePath);
		j_.at("size").get_to(imgData_.size);
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�G�t�F�N�g�̐���
	void Object::GenerateEffect(int effectsNum_, ML::Vec2 pos_) {

		auto& data = this->res->data;
		switch (effectsNum_) {
			//Ready
		case EffectsNum::Ready: {
			auto effect = Effect2D::EffectL_C_R::Create(data->imageReady.filePath, data->imageReady.size, true);
			effect->setMoveSpeed(data->readyMoveSpeed);
			effect->setTimeStayCenter(data->readyTimeStayCenter);
			break;
		}
			  //Start
		case EffectsNum::Start: {
			auto effect = Effect2D::EffectStamp::Create(data->imageStart.filePath, data->imageStart.size, true);
			effect->setStamp(data->startScaleDecSpeed, data->startScaleMax);
			effect->setAlpha(data->startAlphaDecSpeed, data->startAlphaWaitDec);
			break;
		}
			 //Finish
		case EffectsNum::Finish: {
			auto effect = Effect2D::EffectStamp::Create(data->imageFinish.filePath, data->imageFinish.size, true);
			effect->setStamp(data->finishScaleDecSpeed, data->finishScaleMax);
			effect->setAlpha(data->finishAlphaDecSpeed, data->finishAlphaWaitDec);
			break;
		}
			 //GorgeousBall
		case EffectsNum::GorgeousBall: {
			auto effect = Effect2D::GenerateSubRandom::Create(data->imageGBall.filePath, data->imageGBall.size, true);
			effect->setPos(pos_);
			effect->setMoveVec(data->GBallMoveVec);
			effect->setLife(data->GBallLife);
			effect->setRotateLoop(data->GBallRotateLoop.adds, data->GBallRotateLoop.lpmax, data->GBallRotateLoop.lpmin);
			effect->SetSubImage(data->imageGBallSub.filePath, data->imageGBallSub.size);
			effect->SetSubMove(data->GBallSubMoveSpeed, data->GBallSubLife, data->GBallSubGenerateDistance);
			break;
		}

		}


	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
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
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
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
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
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


}