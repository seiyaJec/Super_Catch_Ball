#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Q�[�����G�t�F�N�g�����^�X�N
//-------------------------------------------------------------------
#include "../../Engine/myGame/myGame.h"
#include "../Effects/Task_Effect2D/Effect2D.h"

namespace  EffectGenerator
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�G�t�F�N�g�����^�X�N");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��


	struct ImageData {
		std::string filePath;
		ML::Size2	size;
		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const ImageData& imgData_);
		friend void from_json(const json& j_, ImageData& imgData_);
	};
	//��������������������������������������������������������������������
	// �Q�[���f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	class Data : public BData
	{
	public:
		using SP = shared_ptr<Data>;

		json LoadDataJson(bool useBinary_)						override;	//�e�L�X�g�f�[�^�ǂݍ���
		Data();			//�R���X�g���N�^�i���ɏ����Ȃ��j
		~Data();		//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static Data::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;			//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
		//�i�����g�p����ꍇ�AmyPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����
		ImageData imageReady;			//�G�t�F�N�gReady�摜���
		float	  readyMoveSpeed;		//�G�t�F�N�gReady�ړ����x
		float	  readyTimeStayCenter;	//�G�t�F�N�gReady������~����
	
		ImageData imageStart;			//�G�t�F�N�gStart���
		float	  startScaleDecSpeed;	//�G�t�F�N�gStart�X�P�[�������l
		float	  startScaleMax;		//�G�t�F�N�gStart�X�P�[���ő�l
		float	  startAlphaDecSpeed;	//�G�t�F�N�gStart���ߓx�����l
		float	  startAlphaWaitDec;	//�G�t�F�N�gStart���ߊJ�n�܂ł̎��ԁi�t���[���P�ʁj
		
		ImageData imageFinish;			//�G�t�F�N�gFinish���
		float	  finishScaleDecSpeed;	//�G�t�F�N�gFinish�X�P�[�������l
		float	  finishScaleMax;		//�G�t�F�N�gFinish�X�P�[���ő�l
		float	  finishAlphaDecSpeed;	//�G�t�F�N�gFinish���ߓx�����l
		float	  finishAlphaWaitDec;	//�G�t�F�N�gFinish���ߊJ�n�܂ł̎��ԁi�t���[���P�ʁj

		ImageData imageGBall;			//�G�t�F�N�gGorgeousBall���
		ImageData imageGBallSub;		//�G�t�F�N�gGorgeousBall�T�u�G�t�F�N�g���
		ML::Vec2  GBallMoveVec;			//�G�t�F�N�gGorgeousBall�ړ�����
		int		  GBallLife;			//�G�t�F�N�gGorgeousBall��������
		Effect2D::InitStd::LoopLt GBallRotateLoop;	//�G�t�F�N�gGorgeousBall��]���
		int		  GBallSubMoveSpeed;	//�G�t�F�N�gGorgeousBall�T�u�G�t�F�N�g�ړ�����
		int		  GBallSubLife;			//�G�t�F�N�gGorgeousBall�T�u�G�t�F�N�g��������
		int		  GBallSubGenerateDistance;	//�G�t�F�N�gGorgeousBall�T�u�G�t�F�N�g�����Ԋu
		//-----------------------------
	};

	//��������������������������������������������������������������������
	// ���\�[�X
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
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
		
	};

	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	class  Object : public  BTask
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
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render3D_L0()		override;//�u3D�`��v�P�t���[�����ɍs�������A���C���[0
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		enum EffectsNum{Ready, Start, Finish, GorgeousBall};
		void GenerateEffect(int effectsNum_, ML::Vec2 pos_ = ML::Vec2()); //�G�t�F�N�g�̐���

	};
}

using EffectsNum = EffectGenerator::Object::EffectsNum;