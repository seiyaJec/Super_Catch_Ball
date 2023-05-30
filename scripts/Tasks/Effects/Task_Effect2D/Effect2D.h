#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//2D�G�t�F�N�g
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"

namespace  Effect2D
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("2D�G�t�F�N�g");	//�O���[�v��
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
		map<string, ML::Image::SP> images;
	};


	//��������������������������������������������������������������������
	// �������p�f�[�^�i�[�N���X
	//��������������������������������������������������������������������
	struct InitStd
	{
		std::string imgFilePath;		//�摜�t�@�C���p�X
		ML::Size2	imgSize;			//�摜�T�C�Y
		ML::Vec2	imgRotCt;			//�摜�̒��S���W
		float		imgRotDeg;		//�摜�̉�]
		bool		imgFlipX;			//�摜�̍��E���]
		bool		imgFlipY;			//�摜�̏㉺���]
		ML::Vec2	pos;				//�������W
		ML::Vec2	moveVec;			//�ړ�����
		ML::Color	color;				//�F
		ML::Vec2	scale;				//�X�P�[��
		int			life;				//��������

		//�y�ʔŃ��[�v�\����
		struct LoopLt {
			float adds;
			float lpmax;
			float lpmin;
			//JSON�t�@�C���ւ̓ǂݏ���
			friend void to_json(json& j_, const LoopLt& loop_);
			friend void from_json(const json& j_, LoopLt& loop_);
		};

		LoopLt lpScaleX;		//�X�P�[��X���[�v
		LoopLt lpScaleY;		//�X�P�[��Y���[�v
		LoopLt lpAlpha;			//���ߓx���[�v
		LoopLt lpRotate;		//��]���[�v

		//JSON�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const InitStd& effStd_);
		friend void from_json(const json& j_, InitStd& effStd_);
	};



	//��������������������������������������������������������������������
	// 2D�G�t�F�N�g���N���X
	//��������������������������������������������������������������������
	class  EffectStd : public  BTask
	{
		//�����E�����n��������������������������������������������������
	public:
		virtual  ~EffectStd();
		typedef  shared_ptr<EffectStd>		SP;
		typedef  weak_ptr<EffectStd>		WP;
		Resource::SP	res;

	protected:

		EffectStd();
		bool  B_Initialize(string imgFilePath_, ML::Size2 size_);
		bool  B_Finalize();
		bool  Initialize(string imgFilePath_, ML::Size2 size_);	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render3D_L0()		override;//�u3D�`��v�P�t���[�����ɍs�������A���C���[0
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs�������A3D�`���ɍs����
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������

		virtual bool  InitializeCustom() = 0;							//�ǉ��̏���������
		virtual void  UpDateAdd();									//�ǉ����čs���X�V����

		//���C���̃����o�E���\�b�h������������������������������������������

		//�[�[����J�[�[//
	protected:
		ML::Image::SP img;		//���g���g���Ă���摜�ւ̃L�[

		ML::Vec2 pos;				//���W
		ML::Vec2 moveVec;			//�ړ�����
		ML::Transform transform;	//�ό`���
		int life;					//��������
		ML::Color color;			//�F���
		void CountLife();			//�������ԃJ�E���g

		//���[�v
		struct Loop
		{
			float adds;				//�ǉ�����l
			float lpmax;				//�ő�
			float lpmin;				//�ŏ�
			Loop();
			void Set(float adds_, float lpmax_, float lpmin_);	//�ꊇ�ݒ�
			void Addto(float& addTo_);						//���[�v
		};
		Loop	loopRotate;			//�ǉ���������l�i�p�x�j
		Loop	loopScaleX;			//�ǉ���������l�iX�X�P�[���j
		Loop	loopScaleY;			//�ǉ���������l�iY�X�P�[���j
		Loop	loopAddAlpha;		//�ǉ���������l�i�����x

		//�������f�[�^����ǂݍ���
		void ReadFromInitStd(const InitStd& initStd_);

		//�Z�b�^
		//���W
		void setPos(const ML::Vec2& pos_) {
			this->pos = pos_;
		}
		//�ړ��x�N�g��
		void setMoveVec(const ML::Vec2& moveVec_) {
			this->moveVec = moveVec_;
		}
		//��������
		void setLife(int life_) {
			this->life = life_;
		}
		//��]
		void setImgRotate(float rotDeg_) {
			this->transform.rotateDeg = rotDeg_;
		}
		//��]�̒��S
		void setImgRotateCenter(ML::Vec2 rotCtPos_) {
			this->transform.rotateCenterPos = rotCtPos_;
		}
		//�X�P�[�����O
		void setImgScale(float scX_, float scY_) {
			this->transform.scaleX = scX_;
			this->transform.scaleY = scY_;
		}
		//���]
		void setImgFlip(bool flipX_, bool flipY_) {
			this->transform.flipX = flipX_;
			this->transform.flipY = flipY_;
		}
		//�F���
		void setColor(const ML::Color& color_) {
			this->color = color_;
		}
		//��]���[�v�ݒ�
		void setRotateLoop(float adds_, float max_ = 360.0f, float min_ = 0.0f) {
			this->loopRotate.Set(adds_, max_, min_);
		}
		//�����x���[�v�ݒ�
		void setAlphaLoop(float adds_, float max_ = 1.0f, float min_ = 1.0f) {
			this->loopAddAlpha.Set(adds_, max_, min_);
		}
		//�X�P�[��X���[�v�ݒ�
		void setScaleXLoop(float adds_, float max_ = 10.0f, float min_ = 0.0f) {
			this->loopScaleX.Set(adds_, max_, min_);
		}
		//�X�P�[��Y���[�v�ݒ�
		void setScaleYLoop(float adds_, float max_ = 10.0f, float min_ = 0.0f) {
			this->loopScaleY.Set(adds_, max_, min_);
		}


		//�[�[���J�[�[//
	public:

	};










	//����������������������������������������������������������������������������������
	//�����������E�̏��ňړ�����G�t�F�N�g
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	class DataL_C_R :public BData
	{
	public:
		using SP = shared_ptr<DataL_C_R>;

		json LoadDataJson(bool useBinary_)						override;	//�e�L�X�g�f�[�^�ǂݍ���
		DataL_C_R();			//�R���X�g���N�^�i���ɏ����Ȃ��j
		~DataL_C_R();		//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static DataL_C_R::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;			//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
		//�i�����g�p����ꍇ�AmyPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j 
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����
		InitStd initStd;
		float moveSpeed;		//�ړ����x
		float moveSpeedMin;		//�ړ����x�ŏ�
		int timeStayCenter;		//�����ɑ؍݂��鎞��
		int timeCnt;			//���Ԍv��
		bool passedCenter;		//�����ʉߍς݂�
		//-----------------------------
	};
	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������

	class EffectL_C_R :public EffectStd
	{
		//�����E���s�֘A����������������������������������������������������
	public:
		~EffectL_C_R();
		typedef shared_ptr<EffectL_C_R>		SP;
		typedef weak_ptr<EffectStd>			WP;
		static EffectL_C_R::SP Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_);
	protected:
		EffectL_C_R();
		bool InitializeCustom() override;	//����������
		void UpDate() override;	//�X�V����
	private:
		DataL_C_R::SP data;

		//�ǉ������o�E���\�b�h����������������������������������������������
	protected:
		float moveSpeed;		//�ړ����x
		float moveSpeedMin;		//�ړ����x�ŏ�
		int timeStayCenter;		//�����ɑ؍݂��鎞��
		int timeCnt;			//���Ԍv��
		bool passedCenter;		//�����ʉߍς݂�

		bool IsOutOfScreen();			//��ʉE���֍s���A�����Ȃ��Ȃ�����
		bool IsPassedCenterOfScreen();	//��ʒ�����ʉ߂�����

	public:
		//�Z�b�^
		void setMoveSpeed(float moveSpeed_);
		void setTimeStayCenter(int timeStayCenter_);

	};










	//����������������������������������������������������������������������������������
	//�X�^���v�i�T�C�Yn�{�����{���������j
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	class DataEffectStamp :public BData
	{
	public:
		using SP = shared_ptr<DataEffectStamp>;

		json LoadDataJson(bool useBinary_)						override;	//�e�L�X�g�f�[�^�ǂݍ���
		DataEffectStamp();			//�R���X�g���N�^�i���ɏ����Ȃ��j
		~DataEffectStamp();		//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static DataEffectStamp::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;			//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
		//�i�����g�p����ꍇ�AmyPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����
		float stampDecSpeed;
		float stampScaleMax;
		float alphaDecSpeed;
		float alphaWaitDec;
		//-----------------------------
	};
	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	class EffectStamp :public EffectStd
	{
		//�����E���s�֘A����������������������������������������������������
	public:
		~EffectStamp();
		typedef shared_ptr<EffectStamp>		SP;
		typedef weak_ptr<EffectStd>			WP;
		static EffectStamp::SP Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_);
	protected:
		EffectStamp();
		bool InitializeCustom() override;	//����������
		void UpDate() override;			//�ǉ����čs���X�V����
	private:
		DataEffectStamp::SP data;

		//�ǉ������o�E���\�b�h����������������������������������������������
	protected:
		bool IsScaleMin();		//�X�P�[���l���ŏ���


	public:
		//�Z�b�^
		using EffectStd::setPos;
		void setStamp(float decSpeed_, float scaleMax_);//�T�C�Y�ω��ݒ�i�k���X�s�[�h�A�����T�C�Y�j
		void setAlpha(float decSpeed_, float waitDec_);//�����������ݒ�i�������X�s�[�h�A�������܂ł̎��ԁi�t���[���P�ʁj�j
	};



	//����������������������������������������������������������������������������������
	//�T�u�G�t�F�N�g�o��
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	class DataGenerateSubRandom :public BData
	{
	public:
		using SP = shared_ptr<DataGenerateSubRandom>;

		json LoadDataJson(bool useBinary_)						override;	//�e�L�X�g�f�[�^�ǂݍ���
		DataGenerateSubRandom();			//�R���X�g���N�^�i���ɏ����Ȃ��j
		~DataGenerateSubRandom();		//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static DataGenerateSubRandom::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;			//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
		//�i�����g�p����ꍇ�AmyPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����
		//-----------------------------
		std::string subImagePath;		//�T�u�G�t�F�N�g�摜�ւ̃p�X
		ML::Size2	subImageSize;		//�T�u�G�t�F�N�g�摜�̃T�C�Y
		float subMoveSpeed;		//�T�u�G�t�F�N�g�ړ����x
		int subLife;			//�T�u�G�t�F�N�g��������
		int   subGenerateDistance;		//�T�u�G�t�F�N�g�o���Ԋu
		ML::Vec2 moveVec;		//�ړ�����
		float	 life;			//��������
		InitStd::LoopLt rotateLoop;		//��]���[�v
	};
	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	class GenerateSubRandom :public EffectStd
	{
		//�����E���s�֘A����������������������������������������������������
	public:
		~GenerateSubRandom();
		typedef shared_ptr<GenerateSubRandom>		SP;
		typedef weak_ptr<EffectStd>			WP;
		static GenerateSubRandom::SP Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_);
	protected:
		GenerateSubRandom();
		bool InitializeCustom() override;	//����������
		void UpDate() override;			//�X�V�����@�@�ύX����ꍇ�̓I�[�o�[���C�h����
	private:
		DataGenerateSubRandom::SP data;

		//�ǉ������o�E���\�b�h����������������������������������������������
	protected:
		std::string subImgFilePath;		//�T�u�G�t�F�N�g�摜�t�@�C���p�X
		ML::Size2	subImgSize;			//�T�u�G�t�F�N�g�摜�T�C�Y
		float subMoveSpeed;		//�T�u�G�t�F�N�g�ړ����x
		int subLife;			//�T�u�G�t�F�N�g��������
		int   subGenerateDistance;		//�T�u�G�t�F�N�g�o���Ԋu
		int   moveCnt;					//�s���J�E���g
	public:
		//�Z�b�^
		using EffectStd::setPos;
		using EffectStd::setMoveVec;
		using EffectStd::setLife;
		using EffectStd::setRotateLoop;
		void SetSubImage(std::string filePath_, ML::Size2 size_);
		void SetSubMove(float moveSpeed_, int life_, int generateDistance_);
	};


	//����������������������������������������������������������������������������������
	//�ړ��݂̂̃G�t�F�N�g
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	class DataMoveStraight :public BData
	{
	public:
		using SP = shared_ptr<DataMoveStraight>;

		json LoadDataJson(bool useBinary_)						override;	//�e�L�X�g�f�[�^�ǂݍ���
		DataMoveStraight();						//�R���X�g���N�^�i���ɏ����Ȃ��j
		~DataMoveStraight();					//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static DataMoveStraight::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;						//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
		//�i�����g�p����ꍇ�AmyPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����
		ML::Vec2 moveVec;		//�ړ�����
		int		 life;			//��������


		//-----------------------------
	};
	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	class MoveStraight :public EffectStd
	{
		//�����E���s�֘A����������������������������������������������������
	public:
		~MoveStraight();
		typedef shared_ptr<MoveStraight>		SP;
		typedef weak_ptr<EffectStd>			WP;
		static MoveStraight::SP Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_);
	protected:
		MoveStraight();
		bool InitializeCustom() override;	//����������
		//void UpDate() override;			//�X�V�����@�@�ύX����ꍇ�̓I�[�o�[���C�h����
		//void UpDateAdd() override;		//�ǉ����čs���X�V�����iUpDate���I�[�o�[���C�h�����ꍇ�͎��s����Ȃ��j
	private:
		DataMoveStraight::SP data;

		//�ǉ������o�E���\�b�h����������������������������������������������
	protected:

	public:
		//�Z�b�^
		using EffectStd::setPos;
		using EffectStd::setMoveVec;
		using EffectStd::setLife;
	};



	//����������������������������������������������������������������������������������
	//���S�Ɍ������G�t�F�N�g
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	class DataMoveToCenter :public BData
	{
	public:
		using SP = shared_ptr<DataMoveToCenter>;

		json LoadDataJson(bool useBinary_)						override;	//�e�L�X�g�f�[�^�ǂݍ���
		DataMoveToCenter();			//�R���X�g���N�^�i���ɏ����Ȃ��j
		~DataMoveToCenter();		//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static DataMoveToCenter::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;			//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
		//�i�����g�p����ꍇ�AmyPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����		
		float	radius;	//���S����̋���
		int	generateDistance;	//��������Ԋu
		float	speed;		//���S�֌��������x
		ML::Vec2 centerPos;	//���S���W

		//-----------------------------
	};
	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	class MoveToCenter :public EffectStd
	{
		//�����E���s�֘A����������������������������������������������������
	public:
		~MoveToCenter();
		typedef shared_ptr<MoveToCenter>		SP;
		typedef weak_ptr<EffectStd>			WP;
		static MoveToCenter::SP Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_);
	protected:
		MoveToCenter();
		bool InitializeCustom() override;	//����������
		void UpDate() override;			//�X�V�����@�@�ύX����ꍇ�̓I�[�o�[���C�h����
	private:
		DataMoveToCenter::SP data;

		//�ǉ������o�E���\�b�h����������������������������������������������
	protected:
		float	radius;		//���S����̋���
		int		generateDistance;	//��������Ԋu
		float	speed;			//���S�֌��������x
		int		moveCnt;		//�ړ��J�E���g
	public:
		//�Z�b�^
		using EffectStd::setPos;
		void setRadius(float radius_) { this->radius = radius_; }
		void setGenerateDistance(int generateDistance_) { this->generateDistance = generateDistance_; }
		void setSpeed(float speed_) { this->speed = speed_; }
	};
















	/*
	
	-----�G�t�F�N�g�ЂȌ`-----


	//����������������������������������������������������������������������������������
	//�G�t�F�N�g������
	//����������������������������������������������������������������������������������
	//��������������������������������������������������������������������
	// �f�[�^
	//��������������������������������������������������������������������
	class EffectSampleData :public BData
	{
	public:
		using SP = shared_ptr<EffectSampleData>;

		json LoadDataJson(bool useBinary_)						override;	//�e�L�X�g�f�[�^�ǂݍ���
		EffectSampleData();			//�R���X�g���N�^�i���ɏ����Ȃ��j
		~EffectSampleData();		//�f�X�g���N�^  �i���ɏ����Ȃ��j
		static EffectSampleData::SP UseData();	//�Q�[���G���W������f�[�^���󂯎��
		static int myNum;			//�Q�[���G���W���ɂ���f�[�^�z��̒��ŁA���g�̗v�f�������ԍ�
		//�i�����g�p����ꍇ�AmyPG.cpp�̓ǂݍ��݊֐��ɂ��̃N���X��ǉ�����̂��Y�ꂸ�ɁI�����j
		//-----------------------------
		//�f�[�^�ϐ��͈ȉ��ɒǉ�����
		//-----------------------------
	};
	//��������������������������������������������������������������������
	// �I�u�W�F�N�g
	//��������������������������������������������������������������������
	class EffectSample :public EffectStd
	{
		//�����E���s�֘A����������������������������������������������������
	public:
		~EffectSample();
		typedef shared_ptr<EffectSample>		SP;
		typedef weak_ptr<EffectStd>			WP;
		static EffectSample::SP Create(string imgFilePath_, ML::Size2 size_, bool flagGameEnginePushBack_);
	protected:
		EffectSample();
		bool InitializeCustom() override;	//����������
		//void UpDate() override;			//�X�V�����@�@�ύX����ꍇ�̓I�[�o�[���C�h����
		//void UpDateAdd() override;		//�ǉ����čs���X�V�����iUpDate���I�[�o�[���C�h�����ꍇ�͎��s����Ȃ��j
	private:
		EffectSampleData::SP data;

		//�ǉ������o�E���\�b�h����������������������������������������������
	protected:

	public:
		//�Z�b�^
	};

	
	*/
}