//-------------------------------------------------------------------
//�v���C���[�Ǘ��^�X�N
//-------------------------------------------------------------------
#include  "../../../Engine/myPG/myPG.h"
#include  "Task_PlayerBase.h"

namespace  PlayerBase
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
			std::ifstream fin("./data/gamedata/player.json");
			if (!fin) { return json(); }
			//JSON�t�@�C���ǂݍ���
			fin >> j;
			//�t�@�C���ǂݍ��ݏI��
			fin.close();
		}
		else {
			j = ge->gameData["data"][this->myArg];
		}
		//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
		//���f�[�^�ǂݍ��ݏ���
		this->handPos = j["handPos"];
		this->throwStartPos = j["throwStartPos"];
		this->modelAngle = j["modelAngle"];
		this->cameraDistance = j["cameraDistance"];
		this->cameraTargetAdd = j["cameraTargetAdd"];
		this->hitBase = j["hitBase"];
		this->catchBase = j["catchBase"];
		this->throwVec = j["throwVec"];
		this->throwVec = this->throwVec.Norm();
		this->throwStopFrames = j["throwStopFrames"];

		this->powerChargeVol = j["throwPowerChargeVol"];
		vector<float> chTable = j["throwPowerChargeTable"];
		for (int i = 0; i < 3; ++i) {
			this->powerChargeTable[i] = chTable[i];
		}
		
		vector<float> powTable = j["throwPowerTable"];
		for (int i = 0; i < 4; ++i) {
			this->powerTable[i] = powTable[i];
		}

		vector<int> angV = j["angleTable"];
		for (int az = 0; az < 3; ++az) {
			for (int ax = 0; ax < 3; ++ax) {
				this->angleTable[az][ax] = angV[az * 3 + ax];
			}
		}
		
		auto jAnim = j["animationIndexes"];
		this->animIndex[Anims::IDLE] = jAnim["idle"];
		this->animIndex[Anims::WALK] = jAnim["walk"];
		this->animIndex[Anims::CATCH] = jAnim["catch"];
		this->animIndex[Anims::CHARGE] = jAnim["charge"];
		this->animIndex[Anims::THROW] = jAnim["throw"];

		this->powerBarDistance = j["powerBarDistance"];
		this->speed = j["walkSpeed"];
		this->score_catch = j.at("score").at("catch").get<int>();
		this->score_miss = j.at("score").at("miss").get<int>();

		this->key_movefront = j.at("keyconfig").at("move_front").get<vector<int>>();
		this->key_moveback = j.at("keyconfig").at("move_back").get<vector<int>>();
		this->key_moveleft = j.at("keyconfig").at("move_left").get<vector<int>>();
		this->key_moveright = j.at("keyconfig").at("move_right").get<vector<int>>();
		this->key_throwBall = j.at("keyconfig").at("throwBall").get<vector<int>>();


		this->modelFilePath = j["modelFilePath"];

		this->effek_charge = j["dataEffekseer_charge"];
		this->effek_chargeDistance = j["effekseer_chargeDistance"];
		this->effek_throw = j["dataEffekseer_throw"];
		this->effek_throwRotationDeg = j["effekseer_throwRotationDeg"];

		this->drawHitBase = j["drawHitBase"];
		this->drawCatchBase = j["drawCatchBase"];
		//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
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
		this->model = ML::Model3D::Create(this->data->modelFilePath);
		this->effekCharge = ML::EffekseerObj::Create(this->data->effek_charge);
		this->effekThrow = ML::EffekseerObj::Create(this->data->effek_throw);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->model.reset();
		this->effekCharge.reset();
		this->effekThrow.reset();
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
		this->cameraDistance = this->res->data->cameraDistance;
		this->cameraTargetAdd = this->res->data->cameraTargetAdd;
		this->hitBase = this->res->data->hitBase;
		this->throwVec = this->res->data->throwVec;
		this->catchBase = this->res->data->catchBase;
		this->moveCount = 0;
		this->throwStopCount = 0;
		this->chargedPower = 0;
		this->power = 0;
		this->speed = this->res->data->speed;
		this->hasBall = false;
		this->score = 0;
		this->modelCl = ML::Model3DClone::Create(this->res->model);
		this->effekChargePlay = nullptr;
		this->effekThrowPlay = nullptr;
		this->powerBar = nullptr;
		this->motion = Motion::IDLE;
		this->preMotion = Motion::IDLE;
		this->preMove = false;
		this->modelCl->AttachAnim(this->res->data->animIndex[Anims::IDLE]);
		
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
	//�[�[�Q�[���O�[�[//
	void  Object::UpDateBeforeGame() {
		this->CameraPosSet();
	}

	//�[�[���C���Q�[���[�[//
	void  Object::UpDateGame()
	{
		//���͏��
		auto& inp = ge->inpkey;



		//�J�E���g
		++moveCount;
		++animCount;

		//�v���C���[�̈ړ�
		//���̍s���𔻒f
		this->judge();
		//�ړ�����
		this->Action();



		//�f�o�b�O�p
		int cameraspeed = 2;
		if (inp.On(KEY_INPUT_NUMPAD8)) {
			this->addcameraAngle.x += cameraspeed;
		}
		if (inp.On(KEY_INPUT_NUMPAD2)) {
			this->addcameraAngle.x -= cameraspeed;
		}
		if (inp.On(KEY_INPUT_NUMPAD6)) {
			this->addcameraAngle.y += cameraspeed;
		}
		if (inp.On(KEY_INPUT_NUMPAD4)) {
			this->addcameraAngle.y -= cameraspeed;
		}

		this->CameraPosSet();
	}

	//�[�[�Q�[����[�[//
	void Object::UpDateAfterGame(){
		this->CameraPosSet();
	}
	//-------------------------------------------------------------------
	// �u�R�c�`��v�P�t���[���ɍs�������A���C���[0
	void Object::Render3D_L0()
	{
		ML::Vec3 pos2(0, 0, 1000);

		ML::Mat4x4 matTr;
		matTr.Identity();
		matTr.RotationY(this->angle.y);
		pos2 = pos2 * matTr;
		pos2 += this->pos;

		//�A�j���[�V����
		this->modelCl->SetAnimTime(this->animCount);

		//���f���̕`��
		ML::Vec3 mAngle = this->angle + this->res->data->modelAngle;

		this->modelCl->DrawStd(this->pos, mAngle);

		DxLib::DrawLine3D(this->pos, pos2, ML::GetColor(ML::Color(1, 0, 1, 1)));
		
		//�����蔻��`��
		if (this->res->data->drawHitBase) {
			ML::DrawBox3D(this->CallHitBox(), ML::Color(0.5f, 1, 1, 1));
		}
		//�L���b�`�����蔻��`��
		if (this->res->data->drawCatchBase) {		//�L���b�`�����蔻���`��
			if (this->motion == Motion::CATCH) {	//�L���b�`���[�V�����ɂȂ��Ă���Ύ��s
				if (ge->qa_ball) {					//�{�[������������Ă���Ύ��s
					//�W�����̌������s��
					ML::Mat4x4 matR(true);			
					matR.RotationXYZ(this->defaultAngle);

					//�L���b�`�����蔻���p��
					ML::Obb3 catchHit = this->catchBase;
					catchHit.SetPos((catchHit.get_pos() * matR) + this->pos);

					ML::DrawBox3D(catchHit, ML::Color(0.5f, 1, 0.8f, 0.8f));
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�����蔻����Ăяo��
	ML::Obb3 Object::CallHitBox() const {
		return this->hitBase.OffsetCopy(this->pos);
	}
	
	//���̍s���𔻒f
	void Object::judge() {
		auto& inp = ge->inpkey;
		Motion nm = this->motion;

		switch (this->motion) {
		//�Î~
		case Motion::IDLE:
			//�{�[���Q�b�g�ւ̑J��
			if (this->SearchBall()) { nm = Motion::GET; }
			//�ړ��ւ̑J��
			if (inp.On(this->res->data->key_movefront[this->num])) { nm = Motion::WALK; }
			if (inp.On(this->res->data->key_moveback[this->num])) { nm = Motion::WALK; }
			if (inp.On(this->res->data->key_moveright[this->num])) { nm = Motion::WALK; }
			if (inp.On(this->res->data->key_moveleft[this->num])) { nm = Motion::WALK; }
			//�L���b�`�ւ̑J��
			if (this->throwStopCount >= this->res->data->throwStopFrames
			&& ge->inpkey.On(this->res->data->key_throwBall[this->num])) 
			{
				nm = Motion::CATCH; 
			}
			break;
		//���s
		case Motion::WALK:
			//�{�[���Q�b�g�ւ̑J��
			if (SearchBall()) { nm = Motion::GET; }
			//�Î~��Ԃւ̑J��
			if (inp.Off(this->res->data->key_movefront[this->num])
			&& inp.Off(this->res->data->key_moveback[this->num])
			&& inp.Off(this->res->data->key_moveright[this->num])
			&& inp.Off(this->res->data->key_moveleft[this->num])) 
			{
				nm = Motion::IDLE; 
			}
			//�L���b�`�ւ̑J��
			if (this->throwStopCount >= this->res->data->throwStopFrames
				&& ge->inpkey.On(this->res->data->key_throwBall[this->num]))
			{
				nm = Motion::CATCH;
			}
			break;
		//�{�[���Q�b�g
		case Motion::GET:
			nm = Motion::CARRY;
			break;
		//�{�[���L���b�`
		case Motion::CATCH:
			if (ge->inpkey.Off(this->res->data->key_throwBall[this->num])) { nm = Motion::IDLE; }
			if (SearchBall()) { nm = Motion::GET; }
			if (CatchBall()) { nm = Motion::GET; }
			break;
		//�{�[������
		case Motion::CARRY:
			//�����{�^���������ꂽ��{�[���𓊂���
			if (ge->inpkey.Down(this->res->data->key_throwBall[this->num])) {
				nm = Motion::CHARGE;
			}
			break;
		//�����`���[�W
		case Motion::CHARGE:
			//�����{�^���������ꂽ��{�[���𓊂���
			if (this->finishChargeFlag == true) {
				nm = Motion::THROW;
			}
			break;
		//�{�[������
		case Motion::THROW:
			if (moveCount >= 30) {
				nm = Motion::IDLE;
			}
			break;
		}


		//���[�V�������X�V
		this->UpDateMotion(nm);
	}
	//---------------------------------------------
	//���[�V�����X�V
	void Object::UpDateMotion(Motion nm_) {
		this->preMotion = this->motion;
		if(nm_ == this->motion)
		{}
		else{
			this->motion = nm_;
			this->moveCount = 0;
			this->ResetAnimation();
		}
	}
	//---------------------------------------------

	//�v���C���[�ړ������E�����ݒ�
	void Object::Action()
	{
		//�����d�����ԃJ�E���g
		if (this->throwStopCount < this->res->data->throwStopFrames) {
			++this->throwStopCount;
		}


		switch (this->motion) {
		//�Î~
		case Motion::IDLE:
			UpDateAnimationStd(Anims::IDLE);
			break;
		//���s
		case Motion::WALK:
			this->UpDateAnimationStd(Anims::WALK);
			ge->printToDebugFile(to_string(this->modelCl->GetAttachAnimTotalTime()));
			this->Move();
			break;
		//�{�[���Q�b�g
		case Motion::GET:
			//�ړ����̃{�[����߂炦���Ȃ�X�R�A���Z
			if (ge->qa_ball->IsMoving() == true) {
				ge->qa_scoreJudge->addScore(this->res->data->score_catch);
			}
			this->CarryBall();
			this->hasBall = true;
			break;
		//�{�[���L���b�`
		case Motion::CATCH:
			if (this->moveCount == 0) {
				this->angle = this->defaultAngle;
				this->modelCl->AttachAnim(this->res->data->animIndex[Anims::CATCH]);
			}
			break;
		//�{�[������
		case Motion::CARRY:
			//�A�j���[�V�����X�V
			//�ړ���
			if (this->Move() == true) {
				if (this->preMove == false) {
					this->animCount = this->modelCl->GetAttachAnimTotalTime();
				}
				this->UpDateAnimationStd(Anims::WALK);
				this->preMove = true;
			}
			//�Î~��
			else {
				if (this->preMove == true) {
					this->animCount = this->modelCl->GetAttachAnimTotalTime();
				}
				this->UpDateAnimationStd(Anims::IDLE);
				this->preMove = false;
			}

			this->CarryBall();
			break;
		case Motion::CHARGE:
			if (this->moveCount == 0) {
				this->angle = this->defaultAngle;
				this->modelCl->AttachAnim(this->res->data->animIndex[Anims::CHARGE]);
			}
			this->Charge();
			break;
		//�{�[������
		case Motion::THROW:
			if (this->moveCount == 0) {
				this->angle = this->defaultAngle;
				this->modelCl->AttachAnim(this->res->data->animIndex[Anims::THROW]);
				this->ThrowBall();
			}
			break;
		}
	}
	//---------------------------------------------
	//�A�j���[�V�������Z�b�g
	void Object::ResetAnimation() {
		this->animCount = 0;
		this->modelCl->DetachAnimAll();
	}
	//---------------------------------------------
	//�A�j���[�V�����X�V
	void Object::UpDateAnimationStd(Anims anim_) {
		if (this->moveCount == 0) {
			this->modelCl->AttachAnim(this->res->data->animIndex[anim_]);
		}
		if (this->animCount >= this->modelCl->GetAttachAnimTotalTime()) {
			this->modelCl->DetachAnimAll();
			this->modelCl->AttachAnim(this->res->data->animIndex[anim_]);
			this->animCount = 0;
		}
	}
	//---------------------------------------------
	//�v���C���[�ړ�
	bool Object::Move() {
		auto& inp = ge->inpkey;
		//���͑���
		int angleX = 1;
		int angleZ = 1;

		//�O
		if (inp.On(this->res->data->key_movefront[this->num])) {
			angleZ -= ML::ONE;
		}
		//���
		if (inp.On(this->res->data->key_moveback[this->num])) {
			angleZ += ML::ONE;
		}
		//�E
		if (inp.On(this->res->data->key_moveright[this->num])) {
			angleX += ML::ONE;
		}
		//��
		if (inp.On(this->res->data->key_moveleft[this->num])) {
			angleX -= ML::ONE;
		}

		float newAngleY = (float)this->res->data->angleTable[angleZ][angleX];

		//���͂�����ꍇ�̏���
		if (newAngleY >= 0.0f && newAngleY <= 360.0f) {

			//������ύX
			this->angle.y = newAngleY;
			this->angle += defaultAngle;

			//�ړ�����
			ML::Vec3 movePos(0, 0, speed);
			ML::Mat4x4 matRot;
			matRot.Identity();
			matRot.RotationXYZ(this->angle);
			movePos = movePos * matRot;
			this->pos += movePos;

			//�ǂƓ��������Ƃ��̈ړ������i�Ȃ��ꍇ�͏������j
			if (!ge->qa_gameManager) { return true; }
			ge->qa_gameManager->CheckPlayerInArea(this->pos, this->num);
			
			return true;
		}

		return false;
	}
	//---------------------------------------------
	//�����`���[�W
	void Object::Charge() {
		if (this->moveCount == 0) {
			//�`���[�W�ʃ��Z�b�g
			this->chargedPower = 0;
			//�t���O���Z�b�g
			this->finishChargeFlag = false;
			//�`���[�W�ʕ\��
			this->powerBar = PowerBar::Object::Create(true);
			ML::Vec2 barPos = ge->cameras[num]->getScreenCenter() + this->res->data->powerBarDistance;
			this->powerBar->SetPos(barPos);
			//�G�t�F�N�g�o��
			this->effekChargePlay = this->res->effekCharge->Play();
		}
		//�G�t�F�N�g����������ďo��
		if (this->effekChargePlay->IsEndPlay()) {
			this->effekChargePlay = this->res->effekCharge->Play();
		}
		//�G�t�F�N�g���W�ړ�
		this->effekChargePlay->SetPos(this->pos + this->res->data->effek_chargeDistance);
		//�����鋭�����Z
		this->chargedPower += this->res->data->powerChargeVol;
		float chargeVolume = chargedPower / this->res->data->powerChargeTable[2];
		this->powerBar->SetVolume(min(1.0f, chargeVolume));

		//�{�[�����W�ړ�
		this->CarryBall();

		//�{�^���������ꂽ��t���O�؂�ւ�
		if (ge->inpkey.Off(this->res->data->key_throwBall[this->num])) {
			this->finishChargeFlag = true;
			this->ChargeToPower();
			this->powerBar->Kill();
			this->powerBar.reset();
			this->effekChargePlay->Stop();
		}
	}
	//---------------------------------------------
	//�`���[�W�ʂ��p���[�ɕϊ�
	void Object::ChargeToPower() {
		int powerNum = 0;
		for (int i = 0; i < 3; ++i) {
			if (this->chargedPower >= this->res->data->powerChargeTable[i]) {
				++powerNum;
			}
			else {
				break;
			}
		}
		this->power = this->res->data->powerTable[powerNum];
	}
	//---------------------------------------------
	//�{�[���𓊂���
	void Object::ThrowBall() {
		//�{�[���������Ă��Ȃ���Γ����Ȃ�
		if (this->hasBall == false) { return; }

		//���������@�W���̕����֓�����
		ML::Mat4x4 matR(true);
		matR.RotationXYZ(this->defaultAngle);
		//���W�ړ�
		ge->qa_ball->pos = this->pos + (this->res->data->throwStartPos * matR);

		ge->qa_ball->moveVec = this->throwVec * matR;

		//�ړ����x
		ge->qa_ball->moveVec *= this->power;
		

		//�G�t�F�N�g����
		auto effeks = this->res->effekThrow->Play();
		effeks->SetPos(this->pos);
		effeks->SetRotateDeg(this->res->data->effek_throwRotationDeg + this->defaultAngle);

		ge->qa_ball->state = Ball::Object::State::THROWN;
		this->hasBall = false;
		this->throwStopCount = 0;
	}
	//---------------------------------------------
	//�{�[�����L���b�`����
	bool Object::CatchBall() {
		//�{�[�����܂���������Ă��Ȃ���΍s��Ȃ�
		if (!ge->qa_ball) { return false; }
		
		//�W�����̌������s��
		ML::Mat4x4 matR(true);
		matR.RotationXYZ(this->defaultAngle);
		
		//�L���b�`�����蔻���p��
		ML::Obb3 me = this->catchBase;
		me.SetPos((me.get_pos() * matR) + this->pos);

		return ge->qa_ball->CheckHitPreposToPos(me);
	}
	//---------------------------------------------
	//�{�[��������
	void Object::CarryBall() {
		//�{�[���������Ă��Ȃ���΍s��Ȃ�
		if (this->hasBall == false) { return; }
		//�{�[����������Ώ������s��Ȃ�
		if (!ge->qa_ball) { return; }

		ML::Vec3 handPos = DxLib::MV1GetFramePosition(this->modelCl->get_handle(), 12);
		ge->qa_ball->Carry(handPos);

	}
	//---------------------------------------------
	//�{�[�����E��
	bool Object::SearchBall() {
		//�{�[�����܂���������Ă��Ȃ���΍s��Ȃ�
		if (!ge->qa_ball) { return false; }

		//�{�[���Ƃ̓����蔻��
		ML::Obb3 me = this->CallHitBox();		//�v���C���[
		return ge->qa_ball->CheckHitPreposToPos(me);
	}
	//---------------------------------------------
	//�J�����ʒu�ݒ�
	void Object::CameraPosSet() {
		//�J�����ʒu�ݒ�
		ML::Mat4x4 matView;
		matView.Identity();
		matView.RotationX(this->defaultAngle.x + this->addcameraAngle.x);
		matView.RotationY(this->defaultAngle.y + this->addcameraAngle.y);

		ge->cameras[num]->pos = this->pos + this->cameraDistance * matView;
		ge->cameras[num]->target = this->pos + this->cameraTargetAdd * matView;
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