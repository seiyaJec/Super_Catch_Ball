#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���[�Ǘ��^�X�N
//-------------------------------------------------------------------
#include "../../../Bases/GameBase/GameBase.h"
#include "../Task_PowerBar/Task_PowerBar.h"

namespace  PlayerBase
{
	//�A�j���[�V����
	enum Anims { IDLE = 0, WALK, CATCH, CHARGE, THROW };		

	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�v���C���[");	//�O���[�v��
	const  string  defName("�Ǘ�");	//�^�X�N��

	//��������������������������������������������������������������������
	// �Q�[���f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�Q�[���Ŏg�p����e�f�[�^
	class Data : public BData
	{
	public:
		using SP = shared_ptr<Data>;

		json LoadDataJson(bool useBinary_)						override;	//�e�L�X�g�f�[�^�ǂݍ���
		Data();			//�R���X�g���N�^�i���ɏ����Ȃ��j
		~Data();		//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static Data::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;			//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�

		//�i����myPG.cpp�̔z��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����
		ML::Vec3 handPos;		//��̍��W
		ML::Vec3 throwStartPos;	//�{�[���𓊂����Ƃ��̍ŏ��̈ʒu
		ML::Vec3 modelAngle;	//���f���̌���
		ML::Vec3 cameraDistance;		//�J�����Ƃ̋���
		ML::Vec3 cameraTargetAdd;		//�J������������
		ML::Obb3 hitBase;		//�n�ʂƂ̓����蔻���`
		ML::Obb3 catchBase;		//�L���b�`�p�����蔻��
		ML::Vec3 throwVec;		//���������
		float	 powerChargeVol;		//1�t���[���̓����`���[�W��
		float	 powerChargeTable[3];	//�����`���[�W
		float	 powerTable[4];			//�����鋭��
		ML::Vec2 powerBarDistance;		//�`���[�W�ʕ\���ʒu�i���S����̊Ԋu�j
		float	 speed;			//�ړ����x
		int    throwStopFrames;	//�����̍d�����ԁi�t���[���P�ʁj
		int      angleTable[3][3];	//�p�x�e�[�u��
		int		playersCnt;		//�v���C���[����
		int		score_catch;	//�L���b�`���̃X�R�A���Z�l
		int		score_miss;		//�L���b�`���̃X�R�A���Z�l

		vector<int>		 key_movefront;	//���́E�O�i
		vector<int>		 key_moveback;	//���́E���
		vector<int>		 key_moveleft;	//���́E���Ɉړ�
		vector<int>		 key_moveright;	//���́E�E�Ɉړ�
		vector<int>		 key_throwBall;	//���́E�{�[������

		std::string	modelFilePath;	//���f���ւ̃t�@�C���p�X

		int animIndex[5];		//�e�A�j���[�V�����̔ԍ�

		ML::EffekseerInitData effek_charge;		//�`���[�W���̃G�t�F�N�g
		ML::Vec3			  effek_chargeDistance;	//�G�t�F�N�g�̃v���C���[����̋���
		ML::EffekseerInitData effek_throw;		//�����̃G�t�F�N�g
		ML::Vec3			  effek_throwRotationDeg;	//�G�t�F�N�g�̃v���C���[����̋���

		bool	drawHitBase;				//true�Ȃ瓖���蔻��`��
		bool	drawCatchBase;				//true�Ȃ�L���b�`�p�����蔻��`��
		//-----------------------------
	};

	//-------------------------------------------------------------------
	//�����^�X�N�Ԃŋ��L���鏈��
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		Data::SP		data;		
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		ML::Model3D::SP	model;	//3D���f��
		ML::EffekseerObj::SP	 effekThrow;		//�����̃G�t�F�N�g�n���h��
		ML::EffekseerObj::SP	 effekCharge;		//�`���[�W�̃G�t�F�N�g�n���h��
	};
	//-------------------------------------------------------------------
	//�X�ōs������
	class  Object : public  GameBase
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDateBeforeGame()	override;//�Q�[���O�X�V
		void  UpDateGame()			override;//�Q�[���X�V
		void  UpDateAfterGame()		override;//�Q�[����X�V
		void  Render3D_L0()		override;//�u3D�`��v�P�t���[�����Ƃɍs�������A���C���[0
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		int		 num;			//�v���C���[�ԍ�
		ML::Vec3 pos;			//��ƂȂ���W
		ML::Vec3 angle;			//Y���̌���
		ML::Vec3 defaultAngle;		//�W�����̌���
		ML::Vec3 cameraDistance;		//�J�����Ƃ̋���
		ML::Vec3 cameraTargetAdd;		//�J�����̌�������
		ML::Obb3 hitBase;		//�n�ʂƂ̓����蔻���`f
		ML::Obb3 catchBase;		//�L���b�`�p�����蔻��
		ML::Vec3 throwVec;		//����������i�P�ʃx�N�g���������j
		float	 chargedPower;	//�����`���[�W��
		float	 power;			//�����鋭��
		float	 speed;			//�ړ����x
		bool	 hasBall;		//�{�[�����������Ă��邩
		int		 moveCount;		//�ړ��J�E���g
		int		 throwStopCount;//�����d���J�E���g
		float	 animCount;			//�A�j���[�V�����J�E���g
		bool	 finishChargeFlag;	//�����`���[�W�I���t���O
		bool  preMove;				//1�t���[���O�̈ړ��t���O
		int		 score;			//�X�R�A
		ML::EffekseerPlayObj::SP		 effekThrowPlay;	//�����̍Đ��n���h��
		ML::EffekseerPlayObj::SP		 effekChargePlay;	//�`���[�W�̍Đ��n���h��

		ML::Model3DClone::SP	modelCl;	//���f���̃N���[��
		PowerBar::Object::SP powerBar;		//�`���[�W�ʕ\���^�X�N


		ML::Vec3 addcameraAngle;	//�J�����ɒǉ���������i��Ƀf�o�b�O�p�j

		//�v���C���[�̃��[�V����
		enum class Motion{IDLE, WALK, GET, CATCH, CARRY, CHARGE, THROW };
		Motion	 motion;		//���[�V����
		Motion	 preMotion;		//�O�t���[���̃��[�V����

		//�����蔻����Ăяo��
		ML::Obb3 CallHitBox() const;

		//���̍s���𔻒f
		void judge();
		//���[�V�����X�V
		void UpDateMotion(Motion nm_);

		//�A�j���[�V�������Z�b�g
		void ResetAnimation();
		//�A�j���[�V�����X�V
		void UpDateAnimationStd(Anims anim_);

		//�v���C���[�s������
		void Action();
		
		//�v���C���[�ړ�
		bool Move();

		//�����`���[�W
		void Charge();

		//�`���[�W�ʂ��p���[�ɕϊ�
		void ChargeToPower();

		//�{�[���𓊂���
		void ThrowBall();
		
		//�{�[�����L���b�`����
		bool CatchBall();

		//�{�[�������i���g�̍��W�ƘA��������j
		void CarryBall();

		//�{�[�����E��
		bool SearchBall();

		//�J�����ʒu�ݒ�
		void CameraPosSet();
	};




}