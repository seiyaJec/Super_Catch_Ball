#pragma once
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "../nlohmann/json.hpp"
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <fstream>

using json = nlohmann::json;

namespace ML
{
	constexpr float PI = 3.141592f;
	enum VALUE { ZERO, ONE, TWO, THREE, FOUR, FIVE };

	class Color;
	class Size2;
	class Size3;
	class Vec2;
	class Vec3;
	class Mat4x4;
	class Box2D;
	class Box3D;
	class Obb3;
	class Rect2;
	class Rect3;
	class Transform;
	class Image;
	class PartImage;
	class Model3D;
	class Model3DClone;
	class Font;
	class Input;
	class InKeyBoard;
	class InMouse;
	class EffekseerInitData;
	class EffekseerObj;
	class EffekseerPlayObj;

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//新規クラス統括クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class GameData {
	public:
		virtual std::string DebugPrint() const = 0;
		virtual std::string DebugPrintLine() const;

	};


	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//色クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Color : virtual public GameData {
	public:
		//KEY: 値の範囲オーバー対策したほうが良いかも...
		float a;		//α地
		float r;		//赤
		float g;		//緑
		float b;		//青

		//デフォルトコンストラクタ
		Color();

		//コンストラクタ
		Color(float a_, float r_, float g_, float b_);

		//演算子定義
		friend std::istream& operator>>(std::istream& is_, Color& col_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Color& color_);
		friend void from_json(const json& j_, Color& color_);

		//DxLibにあるCOLOR_Fへのキャスト定義
		operator DxLib::COLOR_F() const;

		//色情報返却
		std::string DebugPrint() const;
	};
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//2次元サイズクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Size2 : virtual public GameData {
	public:
		float w;		//横幅
		float h;		//縦幅

		//デフォルトコンストラクタ
		Size2();

		//コンストラクタ
		Size2(float w_, float h_);

		//演算子定義
		friend std::istream& operator>>(std::istream& is_, Size2& size_);
		
		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Size2& size_);
		friend void from_json(const json& j_, Size2& size_);

		//デバッグ情報
		std::string DebugPrint() const;
	};

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//3次元サイズクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Size3 : virtual public Size2 {
	public:
		float d;		//奥行（depth）

		//デフォルトコンストラクタ
		Size3();

		//コンストラクタ
		Size3(float w_, float h_, float d_);

		//演算子定義
		friend std::istream& operator>>(std::istream& is_, Size3& size_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Size3& size_);
		friend void from_json(const json& j_, Size3& size_);

		//デバッグ情報
		std::string DebugPrint() const;
	};

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//2次元ベクトルクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Vec2 : virtual public GameData {
	public:
		float x;
		float y;

		//デフォルトコンストラクタ
		Vec2();

		//コンストラクタ
		Vec2(float x_, float y_);

		//デバッグ情報
		std::string DebugPrint() const;

		//演算子定義
		friend Vec2 operator+(const Vec2& vecL_, const Vec2& vecR_);
		friend Vec2 operator-(const Vec2& vecL_, const Vec2& vecR_);
		friend Vec2 operator*(const Vec2& vecL_, const Vec2& vecR_);
		friend Vec2 operator*(const Vec2& vecL_, float value_);
		Vec2& operator+= (const Vec2& vec_);
		Vec2& operator-=(const Vec2& vec_);
		Vec2& operator*=(const Vec2& vec_);
		Vec2& operator*=(float value_);
		friend std::istream& operator>>(std::istream& is_, Size3& size_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Vec2& vec_);
		friend void from_json(const json& j_, Vec2& vec_);
	};

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//3次元ベクトルクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Vec3 : virtual public Vec2 {
	public:
		float z;

		//デフォルトコンストラクタ
		Vec3();

		//コンストラクタ
		Vec3(float x_, float y_, float z_);
		Vec3(const DxLib::VECTOR& vec_);

		//デバッグ情報
		std::string DebugPrint() const;

		//大きさ
		float Length() const;

		//正規化
		Vec3 Norm() const;

		//内積
		friend float VDot(const Vec3& vecL_, const Vec3& vecR_);

		//外積
		friend Vec3 VCross(const Vec3& vecL_, const Vec3& vecR_);
		
		//XYZ同値のオブジェクトを生成
		static Vec3 CreateXYZ(float XYZ_);

		//演算子定義
		Vec3 operator-();
		friend Vec3 operator+(const Vec3& vecL_, const Vec3& vecR_);
		friend Vec3 operator-(const Vec3& vecL_, const Vec3& vecR_);
		friend Vec3 operator*(const Vec3& vec_, float scale_);
		Vec3& operator+= (const Vec3& vec_);
		Vec3& operator-=(const Vec3& vec_);
		Vec3& operator*=(const Mat4x4& mat_);
		Vec3& operator*=(float value_);
		bool  operator==(float value_);
		bool  operator!=(float value_);
		friend std::istream& operator>>(std::istream& is_, Vec3& vec_);
		friend std::ostream& operator<<(std::ostream& os_, const Vec3& vec_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Vec3& vec_);
		friend void from_json(const json& j_, Vec3& vec_);

		//DxLibにあるVECTORへのキャスト定義
		operator DxLib::VECTOR() const;
	};
	
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//行列クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Mat4x4 : virtual public GameData, public DxLib::MATRIX {
	public:
		Mat4x4(bool identity_ = false);
		Mat4x4(const MATRIX& v_);
		Mat4x4  operator= (const MATRIX& v_);
		//単位行列
		void  Identity();
		//平行移動行列
		void  Translation(const Vec3& vec_);
		//X軸回転行列
		void RotationX(float degree_);
		//Y軸回転行列
		void RotationY(float degree_);
		//Z軸回転行列
		void RotationZ(float degree_);
		//回転行列・ロールピッチヨー（XYZ）の順
		void RotationXYZ(const Vec3& ang_);
		//任意軸回転行列
		void RotationFree(const Vec3& axis_, float degree_);
		//サイズ変更
		void Scale(float sizeX_, float sizeY_, float sizeZ_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Mat4x4& vec_);
		friend void from_json(const json& j_, Mat4x4& vec_);

		//デバッグ情報
		std::string DebugPrint() const;

		//演算子定義
		friend Mat4x4 operator*(const Mat4x4& matL_, const Mat4x4& matR_);
		friend Vec3 operator*(const Vec3& vecL_, const Mat4x4& matR_);
		Mat4x4& operator*=(const Mat4x4& mat_);
		friend std::istream& operator>>(std::istream& is_, Mat4x4& mat_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Mat4x4& mat_);
		friend void from_json(const json& j_, Mat4x4& mat_);
	};

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//2次元ボックスクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Box2D : virtual public Vec2, virtual public Size2 {
	public:
		//デフォルトコンストラクタ
		Box2D();

		//コンストラクタ
		//KEY: center_はあまり必要ないと思う,Box2D(float w_,float h)orOffsetBox(w_,h_)などに...
		Box2D(float x_, float y_, float w_, float h_, bool center_ = false);
		Box2D(float x_, float y_, const Size2& size_, bool center_ = false);
		Box2D(const Vec2& vec_, float w_, float h_, bool center_ = false);
		Box2D(const Vec2& vec_, const Size2& size_, bool center_ = false);

		//引数分移動させる
		void Offset(float x_, float y_);
		void Offset(const Vec2& vec_);

		//中心を原点として引数分移動させる
		//KEY: 使用方法がわかりにくい/必要ないと思う
		void OffsetCenterBase(float x_, float y_);
		void OffsetCenterBase(const Vec2& v_);

		//移動させたクローンを生成
		Box2D OffsetCopy(float x_, float y_);
		Box2D OffsetCopy(const Vec2& vec_);

		//中心を原点として引数分移動させたクローンを生成
		//KEY: 使用方法がわかりにくい/必要ないと思う
		Box2D OffsetCenterBaseCopy(float x_, float y_);
		Box2D OffsetCenterBaseCopy(const Vec2& vec_);

		//引数の座標に移動させる
		//KEY: 使用方法がわかりにくい/必要ないと思う
		void Set(float x_, float y_);
		void Set(const Vec2& vec_);

		//引数の座標へ移動させたクローンを生成
		Box2D SetCopy(float x_, float y_);
		Box2D SetCopy(const Vec2& vec_);

		//中心を原点として移動させたクローンを生成
		//KEY: 使用方法がわかりにくい/必要ないと思う
		Box2D SetCenterBaseCopy(float x_, float y_);
		Box2D SetCenterBaseCopy(const Vec2& vec_);

		//中心を基点に座標指定
		//KEY: 使用方法がわかりにくい/必要ないと思う
		void SetCenterBase(float x_, float y_);
		void SetCenterBase(const Vec2& vec_);

		//サイズ再設定
		void Resize(float w_, float h_);
		void Resize(const Size2& size_);

		//当たり判定
		bool Hit(const Box2D& you_);

		//演算子定義
		friend std::istream& operator>>(std::istream& is_, Box2D& box_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Box2D& box_);
		friend void from_json(const json& j_, Box2D& box_);
		
		//デバッグ情報
		std::string DebugPrint() const;

	};

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//3次元ボックスクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Box3D : virtual public Vec3, virtual public Size3, virtual public Box2D{
	public:
		//デフォルトコンストラクタ
		Box3D();

		//コンストラクタ
		Box3D(float x_, float y_, float z_, float w_, float h_, float d_, bool center_ = false);
		Box3D(float x_, float y_, float z_, const Size3& size_, bool center_ = false);
		Box3D(const Vec3& vec_, float w_, float h_, float d_, bool center_ = false);
		Box3D(const Vec3& vec_, const Size3& size_, bool center_ = false);

		//引数分移動させる
		void Offset(float x_, float y_, float z_);
		void Offset(const Vec3& vec_);

		//中心を原点として引数分移動させる
		void OffsetCenterBase(float x_, float y_, float z_);
		void OffsetCenterBase(const Vec3& vec_);

		//移動させたクローンを生成
		Box3D OffsetCopy(float x_, float y_, float z_);
		Box3D OffsetCopy(const Vec3& vec_);

		//中心を原点として引数分移動させたクローンを生成
		Box3D OffsetCenterBaseCopy(float x_, float y_, float z_);
		Box3D OffsetCenterBaseCopy(const Vec3& vec_);

		//引数の座標に移動させる
		void Set(float x_, float y_, float z_);
		void Set(const Vec3& vec_);

		//引数の座標へ移動させたクローンを生成
		Box3D SetCopy(float x_, float y_, float z_);
		Box3D SetCopy(const Vec3& vec_);

		//中心を原点として移動させたクローンを生成
		Box3D SetCenterBaseCopy(float x_, float y_, float z_);
		Box3D SetCenterBaseCopy(const Vec3& vec_);

		//中心を基点に座標指定
		void SetCenterBase(float x_, float y_, float z_);
		void SetCenterBase(const Vec3& vec_);

		//サイズ再設定
		void Resize(float w_, float h_, float d_);
		void Resize(const Size3& size_);

		//当たり判定
		bool Hit(const Box3D& you_);

		//演算子定義
		friend std::istream& operator>>(std::istream& is_, Box3D& box_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Box3D& box_);
		friend void from_json(const json& j_, Box3D& box_);

		//デバッグ情報
		std::string DebugPrint() const;

	};
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//角度付き直方体クラス（中心が原点）
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Obb3 :virtual public Vec3, virtual public Size3 {
	private:
		Vec3 dirX;		//方向ベクトルX
		Vec3 dirY;		//方向ベクトルY
		Vec3 dirZ;		//方向ベクトルZ

	public:

		//デフォルトコンストラクタ
		Obb3();

		//コンストラクタ
		Obb3(const Vec3& pos_, const Size3& size_);

		//方向ベクトルYを更新し、それに合わせてX,Zも更新する
		//（自動で正規化が行われる）
		void SetDirectionY(const Vec3& vec_);

		//移動
		void Offset(const Vec3& pos_);
		//コピー
		Obb3 OffsetCopy(const Vec3& pos_) const;

		//座標指定
		void SetPos(const Vec3& pos_);
		//コピー
		Obb3 SetPosCopy(const Vec3& pos_) const;

		//サイズ再設定
		void Resize(const Size3& size_);

		//ゲッタ
		Vec3 get_dirX() const { return dirX; }
		Vec3 get_dirY() const { return dirY; }
		Vec3 get_dirZ() const { return dirZ; }
		Vec3 get_pos() const { return Vec3(x, y, z); }
		Size3 get_size() const { return Size3(w, h, d); }

		//演算子定義
		friend std::istream& operator>>(std::istream& is_, Obb3& obb_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Obb3& box_);
		friend void from_json(const json& j_, Obb3& box_);

		//デバッグ情報
		std::string DebugPrint() const;
	};
	//当たり判定
	bool CheckHit(const Obb3& obb1_, const Obb3& obb2_);

	// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegOnSeparateAxis(const Vec3& sep, const Vec3& e1, const Vec3& e2, const Vec3& e3 = Vec3());

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//2次元矩形クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Rect2 :virtual public GameData {
	public:
		float l;	//左(x)	left
		float r;	//右(x)	right
		float t;	//上(y)	top
		float b;	//下(y)	bottom

		//デフォルトコンストラクタ
		Rect2();

		//コンストラクタ
		Rect2(float l_, float r_, float t_, float b_);
		Rect2(const ML::Box2D& box_);

		//ボックスから変換
		void BoxToRect(const ML::Box2D& box_);

		//演算子定義
		friend std::istream& operator>>(std::istream& is_, Rect2& rect_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Rect2& box_);
		friend void from_json(const json& j_, Rect2& box_);

		//デバッグ情報
		std::string DebugPrint() const;
	};

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//3次元直方体クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Rect3 :virtual public Rect2 {
	public:
		float n;	//手前(z)	near
		float f;	//奥（z)	far

		//デフォルトコンストラクタ
		Rect3();

		//コンストラクタ
		Rect3(float l_, float t_, float r_, float b_, float n, float f_);
		Rect3(const ML::Box3D& box_);

		//ボックスから変換
		void BoxToRect(const ML::Box3D& box_);

		//演算子定義
		friend std::istream& operator>>(std::istream& is_, Rect3& rect_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Rect3& box_);
		friend void from_json(const json& j_, Rect3& box_);

		//デバッグ情報
		std::string DebugPrint() const;
	};
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//変形クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Transform :virtual public GameData {
	public:
		float rotateDeg;			//角度（度数法）
		Vec2  rotateCenterPos;			//回転の中心
		float scaleX;			//横幅
		float scaleY;			//縦幅
		bool  flipX;			//X方向反転
		bool  flipY;			//Y方向反転
		
		//デフォルトコンストラクタ
		Transform(float rotDeg_ = 0.0f, ML::Vec2 rotCt_ = ML::Vec2(), float scX_ = 1.0f, float scY_ = 1.0f, bool flipX_ = false, bool flipY_ = false);

		//演算子定義
		friend std::istream& operator>>(std::istream& is_, Transform& trs_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const Transform& trs_);
		friend void from_json(const json& j_, Transform& trs_);

		//デバッグ情報
		std::string DebugPrint() const;
	};
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//画像クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Image : virtual public GameData {
	private:
		std::string filePath;		//ファイルへのパス
		int handle;			//画像ロード時に渡されるユニークな値
		Size2 size;			//画像サイズ
	protected:
		//コンストラクタ
		Image(char* filePath_, const Size2& size_);
		Image(const char* filePath, const Size2& size_);
		Image(std::string filePath_, const Size2& size_);
	public:
		using SP = std::shared_ptr<Image>;

		//生成
		static Image::SP Create(char* filePath_, const Size2& size_ = Size2());
		static Image::SP Create(const char* filePath_, const Size2& size_ = Size2());
		static Image::SP Create(std::string filePath_, const Size2& size_ = Size2());

		//ゲッタ
		Size2 get_size() const { return size; }
		std::string get_filePath() const{ return filePath; }

		//中心座標を受け取る
		Vec2  CenterPos();

		//描画
		//KEY: 使用方法が複雑/もう少しわかりやすくしてほしい
		void Draw(int x_, int y_, bool TransFlag = true, float alpha_ = 1.0f);
		void Draw(Vec2 pos_, bool TransFlag = true, float alpha_ = 1.0f);
		//中心を原点にして描画
		void DrawCt(int x_, int y_, bool TransFlag = true, float alpha_ = 1.0f);
		void DrawCt(Vec2 pos_, bool TransFlag = true, float alpha_ = 1.0f);
		//中心を原点にして変形描画
		void DrawTransform(Vec2 posCt_, const Transform& trs_ = Transform(), bool TransFlag = true, float alpha = 1.0f);
		//指定部分だけ描画
		void DrawPartRect(ML::Vec2 pos_, const ML::Box2D& src_, bool transFlag_ = true, float alpha_ = 1.0f, bool flipFlag_ = false);


		//デバッグ情報
		std::string DebugPrint() const;
	};

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//分割画像クラス
	//KEY: 使用方法が複雑...
	// 
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class PartImage : virtual public GameData {
	private:
		std::string filePath;		//ファイルへのパス
		int* handleArray;		//画像ロード時に渡されるユニークな値の配列
		int AllNum;			//分割画像総数
		int xCount;			//横の分割数
		int yCount;			//縦の分割数
		int xSize;			//分割した画像の横幅
		int ySize;			//分割した画像の縦幅
	public:
		using SP = std::shared_ptr<PartImage>;


		//生成
		static PartImage::SP Create(char* filePath_, int AllNum_, int xCnt_, int yCnt_, int xSize_, int ySize_);
		static PartImage::SP Create(const char* filePath_, int AllNum_, int xCnt_, int yCnt_, int xSize_, int ySize_);
		static PartImage::SP Create(std::string filePath_, int AllNum_, int xCnt_, int yCnt_, int xSize_, int ySize_);



		//コンストラクタ
		PartImage(const std::string& filePath_, int AllNum_, int xCnt_, int yCnt_, int xSize_, int ySize_);

		//デストラクタ
		~PartImage();

		//ゲッタ
		int get_AllNum() { return AllNum; }
		int get_xCount() { return xCount; }
		int get_yCount() { return yCount; }
		int get_xSize() { return xSize; }
		int get_ySize() { return ySize; }


		//描画
		void Draw(int x_, int y_, int imageNum_, bool TransFlag = true, float alpha_ = 1.0f);
		void Draw(Vec2 pos_, int imageNum_, bool TransFlag = true, float alpha_ = 1.0f);


		//デバッグ情報
		std::string DebugPrint() const;
	};


	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//フォントクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Font : virtual public GameData {
	public:
		using SP = std::shared_ptr<Font>;
	protected:
		int handle;		//フォントハンドル
		char* fontName;	//フォント名
		int size;		//サイズ
		int thick;		//太さ
		int fontType;	//フォントのタイプ
	public:
		//生成
		//KEY: フォントタイプをenum classにするといいかも...
		//KEY: 負数の入力があったときにどうするか...?
		static Font::SP Create();
		static Font::SP Create(const char* fontName_, int size_, int thick_ = -1, int fontType_ = -1);

		//デフォルトコンストラクタ
		Font();

		//コンストラクタ
		Font(const char* fontName_, int size_, int thick_, int fontType_ = -1);

		//ゲッタ
		int get_size() const { return size; }

		//KEY: str_をstringにするといいかも...
		void Draw(int x_, int y_, const char* str_, const Color& color_ = Color(1, 1, 1, 1));
		void Draw(Vec2 pos_, const char* str_, const Color& color_ = Color(1, 1, 1, 1));

		//デバッグ情報
		std::string DebugPrint() const;
	};

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//メッシュクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Model3D :virtual public GameData {
	private:
		//コンストラクタ
		Model3D(const std::string& filePath_);
	protected:
		std::string filePath;		//ファイルパス
		int handle;			//メッシュへのハンドル
		std::map<int,int> handleAnim;		//再生中のアニメーションへのハンドル
		Model3D() {};		//デフォルトコンストラクタ

		//指定したアニメーションがアタッチされているか
		bool CheckAnimAttach(int animIndex_);
	public:
		//デストラクタ
		~Model3D();
		using SP = std::shared_ptr<Model3D>;

		//生成
		static SP Create(char* filePath_);
		static SP Create(const char* filePath_);
		static SP Create(const std::string& filePath_);

		//ゲッタ
		virtual std::string get_filePath() { return filePath; }
		virtual int get_handle() { return handle; }


		//アニメーション
		//使用の流れ：アタッチ～使用～アニメーション変更時～デタッチ～新しくアタッチ～使用...
	
		//指定番号のアニメーションをアタッチ
		virtual bool AttachAnim(int animIndex_);
		//指定番号のアニメーションをデタッチ
		virtual bool DetachAnim(int animIndex_);
		//アニメーションを全てデタッチ
		virtual void DetachAnimAll();
		//アニメーションの時間を指定
		virtual bool SetAnimTime(float time_);
		//アタッチ中のアニメーション総時間を受け取る
		virtual float GetAttachAnimTotalTime();

		//描画
		virtual void Draw(const Mat4x4& matWorld_);
		virtual void DrawStd(const Vec3& pos_, const Vec3& angle_ = Vec3(0, 0, 0), const Vec3& scale_ = Vec3(1, 1, 1));

	public:
		//デバッグ情報
		std::string DebugPrint() const;
	};
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//メッシュ複製用クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Model3DClone : virtual protected Model3D {
	private:
		std::weak_ptr<Model3D> wp_base;

	public:
		//コンストラクタ
		Model3DClone(Model3D::SP baseModel_);
		//デストラクタ
		~Model3DClone();
		using SP = std::shared_ptr<Model3DClone>;

		//生成
		static Model3DClone::SP Create(Model3D::SP baseModel_);

		//ゲッタ
		std::string get_filePath() override; 
		int get_handle() override;

		//アニメーション
		//使用の流れ：アタッチ～使用～アニメーション変更時～デタッチ～新しくアタッチ～使用...

		//指定番号のアニメーションをアタッチ
		bool AttachAnim(int animIndex_);
		//指定番号のアニメーションをデタッチ
		bool DetachAnim(int animIndex_);
		//アニメーションを全てデタッチ
		void DetachAnimAll();
		//アニメーションの時間を指定
		bool SetAnimTime(float time_);
		//アタッチ中のアニメーション総時間を受け取る
		float GetAttachAnimTotalTime();

		//描画
		void Draw(const Mat4x4& matWorld_);
		void DrawStd(const Vec3& pos_, const Vec3& angle_ = Vec3(0, 0, 0), const Vec3& scale_ = Vec3(1, 1, 1));

		//デバッグ情報
		std::string DebugPrint() const;
	};
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//入力クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Input : virtual public GameData {
	public:
		virtual void UpDate() = 0;			//入力情報更新処理
		virtual bool Down(int key_) = 0;	//入力開始時のフレームならtrue
		virtual bool On(int key_) = 0;		//入力中ならtrue
		virtual bool Up(int key_) = 0;		//入力終了時のフレームならtrue
		virtual bool Off(int key_) = 0;		//入力無しならtrue
	};
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//キーボードクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class InKeyBoard : virtual public Input {
	public:
		static const int keysSize = 256;

		char preInp[keysSize];		//1フレーム前の入力情報
		char inp[keysSize];			//入力情報

		//デフォルトコンストラクタ
		InKeyBoard();

		void UpDate()	override;		//入力情報更新処理
		bool Down(int key_) override;	//入力開始時のフレームならtrue
		bool On(int key_) override;		//入力中ならtrue
		bool Up(int key_) override;		//入力終了時のフレームならtrue
		bool Off(int key_) override;		//入力無しならtrue

		//デバッグ情報
		std::string DebugPrint() const;
	};
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//マウスクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class InMouse : virtual public Input {
	private:
		static const int buttonsSize = 3;
		enum MI{LEFT, RIGHT, MIDDLE};
		bool preInp[buttonsSize];		//1フレーム前の入力情報
		bool inp[buttonsSize];			//入力情報

		//KEY: 値の範囲がわからない...
		int wheelRotation;				//ホイール回転量
		static int  Convert(int macro_);		//マウスボタン番号をマクロからクラス内の数値に変換
	public:

		//デフォルトコンストラクタ
		InMouse();

		//位置情報取得
		Vec2 getPos();

		//入力情報関連
		void UpDate()	override;		//入力情報更新処理
		bool Down(int button_) override;	//入力開始時のフレームならtrue
		bool On(int button_) override;		//入力中ならtrue
		bool Up(int button_) override;		//入力終了時のフレームならtrue
		bool Off(int button_) override;		//入力無しならtrue

		//ホイールの回転量（上方向ならプラス、した方向ならマイナス）
		int GetWheelRotVol();

		//デバッグ情報
		std::string DebugPrint() const;
	};
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//エフェクシアデータ保存クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class EffekseerInitData : public GameData
	{
	public:
		std::string filePath;		//エフェクトへのファイルパス
		float			scale;			//元のサイズからの倍率

		EffekseerInitData();

		//設定
		void Set(const std::string& filePath_, float scale_);

		//jsonファイルへの読み書き
		friend void to_json(json& j_, const EffekseerInitData& efc_);
		friend void from_json(const json& j_, EffekseerInitData& efc_);

		//デバッグ情報
		std::string DebugPrint() const;
	};
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//エフェクシアオブジェクトクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class EffekseerObj : public GameData
	{
		//コンストラクタ
		EffekseerObj(const EffekseerInitData& efkData_);
		const int handleResource;		//エフェクトのハンドル
	public:
		using SP = std::shared_ptr<EffekseerObj>;
		using WP = std::weak_ptr<EffekseerObj>;

		//デストラクタ
		~EffekseerObj();

		//生成
		static EffekseerObj::SP Create(const EffekseerInitData& efkData_);
		
		//再生　座標や回転・色などの操作が必要な場合は返り値を受け取る
		std::shared_ptr<EffekseerPlayObj> Play();
		
		//デバッグ情報
		std::string DebugPrint() const override;
	};
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//エフェクシア再生オブジェクトクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class EffekseerPlayObj :public GameData
	{
	public:
		using SP = std::shared_ptr<EffekseerPlayObj>;
		using WP = std::shared_ptr<EffekseerPlayObj>;
	private:
		//コンストラクタ
		EffekseerPlayObj(int handle_);
		int handlePlay;
	public:
		friend std::shared_ptr<EffekseerPlayObj> EffekseerObj::Play();
		using SP = std::shared_ptr<EffekseerPlayObj>;
		using WP = std::shared_ptr<EffekseerPlayObj>;

		//座標設定
		void SetPos(const ML::Vec3& pos_);
		//度数法で回転情報設定（ZXY順に回転）
		void SetRotateDeg(const ML::Vec3& angleDeg_);
		//ラジアン値で回転情報設定（ZXY順に回転）
		void SetRotateRad(const ML::Vec3& angleRad_);
		//スケール設定（1.0が等倍）
		void SetScale(const ML::Vec3& scale_);
		//色設定
		void SetColor(const ML::Color& color_);
		//再生が終了したか
		bool IsEndPlay();


		//再生を中止　※再開不可
		void Stop();

		//デバッグ情報
		std::string DebugPrint() const override;
	};
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//カメラ基本形クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	class Camera
	{
	protected:
		Camera(
			const ML::Vec3& tg_,	//	被写体の位置
			const ML::Vec3& pos_,	//	カメラの位置
			const ML::Vec3& up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
			float				fov_,	//	視野角
			float				np_,	//	前クリップ平面（これより前は映らない）
			float				fp_,	//	後クリップ平面（これより後ろは映らない）
			float				asp_,	//	アスペクト比（画面の比率に合わせる　横÷縦）	
			ML::Box2D			screen_);	//画面に描画する範囲		
	public:
		//	ビュー情報（方向関連）
		ML::Vec3 target;			//	被写体の位置
		ML::Vec3 pos;			//	カメラの位置
		ML::Vec3 up;				//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
		//	射影情報（視野範囲関連）
		float fov;					//	視野角
		float nearPlane;			//	前クリップ平面（これより前は映らない）
		float forePlane;			//	後クリップ平面（これより後ろは映らない）
		float aspect;				//	アスペクト比（画面の比率に合わせる　横÷縦）
		ML::Box2D screenArea;		//　画面に描画する範囲
		//	行列情報
		ML::Mat4x4  matView, matProj;

		using SP = std::shared_ptr<Camera>;
		//　生成
		//	カメラを生成する
		static SP Create(
			const ML::Vec3& tg_,	//	被写体の位置
			const ML::Vec3& pos_,	//	カメラの位置
			const ML::Vec3& up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
			float				fov_,	//	視野角
			float				np_,	//	前クリップ平面（これより前は映らない）
			float				fp_,	//	後クリップ平面（これより後ろは映らない）
			float				asp_,	//	アスペクト比（画面の比率に合わせる　横÷縦）	
			ML::Box2D			screen_);	//画面に描画する範囲

		~Camera();
		//	カメラの設定
		void update();

		//描画するスクリーンの中心を返す
		ML::Vec2 getScreenCenter();
	};



	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//関数定義
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//ColorをDXライブラリに対応させる
	unsigned int GetColor(const Color& color_);
	DxLib::COLOR_U8 GetColorU8(const Color& color_);
	//度数法からラジアン値に変換
	float ToRadian(float degree_);
	ML::Vec3 ToRadian(const ML::Vec3 degree_);
	//ラジアン値から度数法に変換
	float ToDegree(float radian_);
	//Dxライブラリで用意されている矩形描画を利用する
	bool DrawBox(const Box2D& box_, const Color& color_, bool fillFlag_ = false);
	//DXライブラリ関数を使用してボックスを表示させる
	void DrawBox3D(const Box3D& box_, const Color& col_ = Color(1, 1, 1, 1));
	void DrawBox3D(const Obb3& box_, const Color& col_ = Color(1, 1, 1, 1));

	//--------------------------------------
	// テンプレート関数
	//--------------------------------------
	//データファイルからデータを読みこむ
	template <class Type>
	void ReadData(std::istream& is_, Type& type_, bool isbin_ = false) {
		//通常の読み込み
		if (isbin_ == false) {
			std::string data;
			getline(is_, data);
			//「>>>」までの部分は無視する
			while (data[0] != '>' 
				|| data[1] != '>'
				|| data[2] != '>') {
				data.erase(data.begin());
			}
			//「>>>」を削除
			data.erase(data.begin(), (data.begin() + 3));
			//読み込んだデータを第2引数に代入する
			std::stringstream ss(data);
			ss >> type_;
		}
		//バイナリデータの読み込み
		else {
			is_.read((char*)&type_, sizeof(type_));
		}
	}
}