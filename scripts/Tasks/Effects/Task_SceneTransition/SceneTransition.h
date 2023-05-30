#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�V�[���J��
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"

namespace  SceneTransition
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�V�[���J��");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
	//��������������������������������������������������������������������
	// �Q�[���f�[�^
	//��������������������������������������������������������������������
	//-------------------------------------------------------------------
	class Data : public BData
	{
	public:
		using SP = shared_ptr<Data>;

		json LoadDataJson(bool useBinary_)		override;	//�e�L�X�g�f�[�^�ǂݍ���
		Data();			//�R���X�g���N�^�i���ɏ����Ȃ��j
		~Data();		//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static Data::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;			//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
		//�i�����g�p����ꍇ�AmyPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����


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
	// �V�[���J�ڊ��N���X
	//��������������������������������������������������������������������
	class  SceneTransitionStd : public  BTask
	{
		//�����E�����n��������������������������������������������������
	public:
		virtual  ~SceneTransitionStd();
		typedef  shared_ptr<SceneTransitionStd>		SP;
		typedef  weak_ptr<SceneTransitionStd>		WP;
		Resource::SP	res;

	protected:

		SceneTransitionStd();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render3D_L0()		override;//�u3D�`��v�P�t���[�����ɍs�������A���C���[0
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������

		virtual bool  InitializeCustom();				//�ǉ��̏���������
		virtual void  AppearUpDate() = 0;						//�o������
		virtual void  DisappearUpDate() = 0;					//���ŏ���
		virtual bool  CanDisappear();						//���Ŕ���

		//���C���̃����o�E���\�b�h������������������������������������������

		//�[�[����J�[�[//
	protected:
		float		transitionSpeed;	//�ړ�����
		ML::Color	color;				//�F���
		bool		appearFlag;			//�o���t���O
		bool		finishAppearFlag;	//�o���I���t���O
		bool		disappearFlag;		//���Ńt���O
		//�[�[���J�[�[//
	public:
		void SetColor(ML::Color color_) { this->color = color_; }
		void Appear() { this->appearFlag = true; }
		bool CheckFinishAppear() { return this->finishAppearFlag; }
		void Disappear() { this->disappearFlag = true; }
	};










	//����������������������������������������������������������������������������������
	//�������@�t�s���~�b�h�^
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	class DataRevPyramid :public BData
	{
	public:
		using SP = shared_ptr<DataRevPyramid>;

		json LoadDataJson(bool useBinary_)						override;	//�e�L�X�g�f�[�^�ǂݍ���
		DataRevPyramid();			//�R���X�g���N�^�i���ɏ����Ȃ��j
		~DataRevPyramid();		//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static DataRevPyramid::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;			//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
		//�i�����g�p����ꍇ�AmyPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����
		int cuts;			//������
		float distance;		//�����̊Ԋu
		float moveSpeed;	//1�t���[�����ƈړ����x
		//-----------------------------
	};
	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������

	class RevPyramid :public SceneTransitionStd
	{
		//�����E���s�֘A����������������������������������������������������
	public:
		~RevPyramid();
		typedef shared_ptr<RevPyramid>		SP;
		typedef weak_ptr<RevPyramid>			WP;
		static RevPyramid::SP Create(bool flagGameEnginePushBack_);
	protected:
		RevPyramid();
		bool InitializeCustom() override;	//����������
		void AppearUpDate() override;		//�o������
		void DisappearUpDate() override;	//���ŏ���
		void Render2D_AF() override;		//�`�揈��
	private:
		DataRevPyramid::SP data;

		//�ǉ������o�E���\�b�h����������������������������������������������
	protected:
		int			cuts;		//������
		float		distance;	//�����̊Ԋu
		float		moveSpeed;	//�ړ����x
		vector<ML::Box2D> cutBoxes;		//����������
		float		disappearCnt;	//���Ŏ��Ԍv��
	public:
		void Set(int cuts_, float distance_, float moveSpeed_);
	};





	//����������������������������������������������������������������������������������
	//�~�g��
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	class DataCircleScaleUp :public BData
	{
	public:
		using SP = shared_ptr<DataCircleScaleUp>;

		json LoadDataJson(bool useBinary_)						override;	//�e�L�X�g�f�[�^�ǂݍ���
		DataCircleScaleUp();			//�R���X�g���N�^�i���ɏ����Ȃ��j
		~DataCircleScaleUp();		//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static DataCircleScaleUp::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;			//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
		//�i�����g�p����ꍇ�AmyPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����
		float scaleUpSpeed;		//�g�呬�x
		//-----------------------------
	};
	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������

	class CircleScaleUp :public SceneTransitionStd
	{
		//�����E���s�֘A����������������������������������������������������
	public:
		~CircleScaleUp();
		typedef shared_ptr<CircleScaleUp>		SP;
		typedef weak_ptr<CircleScaleUp>			WP;
		static CircleScaleUp::SP Create(bool flagGameEnginePushBack_);
	protected:
		CircleScaleUp();
		bool InitializeCustom() override;	//����������
		void AppearUpDate() override;		//�o������
		void DisappearUpDate() override;	//���ŏ���
		void Render2D_AF() override;		//�`�揈��
	private:
		DataCircleScaleUp::SP data;

		//�ǉ������o�E���\�b�h����������������������������������������������
	protected:
		int scaleUpSpeed;
		int radius;
		int lineThickness;			//���̑���
		int moveCnt;
	public:
		void Set(int scaleUpSpeed_);
	};












	/*
	
	-----�G�t�F�N�g�ЂȌ`-----


	//����������������������������������������������������������������������������������
	//�~�g��
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	class DataSampleScTr :public BData
	{
	public:
		using SP = shared_ptr<DataSampleScTr>;

		json LoadDataJson(bool useBinary_)						override;	//�e�L�X�g�f�[�^�ǂݍ���
		DataSampleScTr();			//�R���X�g���N�^�i���ɏ����Ȃ��j
		~DataSampleScTr();		//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static DataSampleScTr::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;			//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
		//�i�����g�p����ꍇ�AmyPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����

		//-----------------------------
	};
	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������

	class SampleScTr :public SceneTransitionStd
	{
		//�����E���s�֘A����������������������������������������������������
	public:
		~SampleScTr();
		typedef shared_ptr<SampleScTr>		SP;
		typedef weak_ptr<SampleScTr>			WP;
		static SampleScTr::SP Create(bool flagGameEnginePushBack_);
	protected:
		SampleScTr();
		bool InitializeCustom() override;	//����������
		void AppearUpDate() override;		//�o������
		void DisappearUpDate() override;	//���ŏ���
		void Render2D_AF() override;		//�`�揈��
	private:
		DataSampleScTr::SP data;

		//�ǉ������o�E���\�b�h����������������������������������������������
	protected:

	public:

	};

	
	*/
}