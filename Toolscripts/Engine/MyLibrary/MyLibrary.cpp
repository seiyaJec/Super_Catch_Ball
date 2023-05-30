#include "MyLibrary.h"

#include <memory>
#include <iostream>

using namespace std;

namespace ML {

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//GameDataクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//メンバ表示を行ごとに行う
	string GameData::DebugPrintLine() const
	{
		string debug = DebugPrint();
		for (auto& it : debug) {
			if (it == '/') {
				it = '\n';
			}
		}
		return debug;
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//色クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//デフォルトコンストラクタ
	Color::Color() :
		a(1.0f),
		r(1.0f),
		g(1.0f),
		b(1.0f)
	{ }
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Color::Color(float a_, float r_, float g_, float b_) :
		a(a_),
		r(r_),
		g(g_),
		b(b_)
	{ }
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//演算子定義
	//「>>」
	istream& operator>>(istream& is_, Color& col_) {
		is_ >> col_.a;
		is_ >> col_.r;
		is_ >> col_.g;
		is_ >> col_.b;
		return is_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const Color& color_) {
		j_ = json{ 
			{"alpha", color_.a}, 
			{"red", color_.r},
			{"green", color_.g}, 
			{"blue",color_.b} 
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, Color& color_) {
		j_.at("alpha").get_to(color_.a);
		j_.at("red").get_to(color_.r);
		j_.at("green").get_to(color_.g);
		j_.at("blue").get_to(color_.b);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//DxLibにあるCOLOR_Fへのキャスト定義
	Color::operator DxLib::COLOR_F() const {
		DxLib::COLOR_F color;
		color.a = this->a;
		color.r = this->r;
		color.g = this->g;
		color.b = this->b;
		return color;
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//色情報返却
	std::string Color::DebugPrint() const {
		stringstream ss;
		ss << "a：" << a << '/';
		ss << "r：" << r << '/';
		ss << "g：" << g << '/';
		ss << "b：" << b;

		return ss.str();

	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//2次元サイズクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//デフォルトコンストラクタ
	Size2::Size2() :
		w(0),
		h(0)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Size2::Size2(float w_, float h_) :
		w(w_),
		h(h_)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//演算子定義
	//「>>」
	istream& operator>>(istream& is_, Size2& size_) {
		is_ >> size_.w;
		is_ >> size_.h;
		return is_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const Size2& size_) {
		j_ = json{
			{"w", size_.w},
			{"h", size_.h},
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, Size2& size_) {
		j_.at("w").get_to(size_.w);
		j_.at("h").get_to(size_.h);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string Size2::DebugPrint() const {
		stringstream ss;
		ss << "w：" << w << '/';
		ss << "h：" << h;

		return ss.str();
	}



	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//3次元サイズクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//デフォルトコンストラクタ
	Size3::Size3() :
		Size2(),
		d(0)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Size3::Size3(float w_, float h_, float d_) :
		Size2(w_,h_),
		d(d_)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//演算子定義
	//「>>」
	std::istream& operator>>(std::istream& is_, Size3& size_) {
		is_ >> size_.w;
		is_ >> size_.h;
		is_ >> size_.d;
		return is_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const Size3& size_) {
		j_ = json{
			{"w", size_.w},
			{"h", size_.h},
			{"d", size_.d},
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, Size3& size_) {
		j_.at("w").get_to(size_.w);
		j_.at("h").get_to(size_.h);
		j_.at("d").get_to(size_.d);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string Size3::DebugPrint() const {
		stringstream ss;
		ss << Size2::DebugPrint() << '/';
		ss << d;
		return ss.str();
	}



	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//2次元ベクトルクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//デフォルトコンストラクタ
	Vec2::Vec2() :
		x(0.0f),
		y(0.0f)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Vec2::Vec2(float x_, float y_) :
		x(x_),
		y(y_)
	{ }
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//演算子定義
	//「+」
	Vec2 operator+(const Vec2& vecL_, const Vec2& vecR_) {
		Vec2 result;
		result.x = vecL_.x + vecR_.x;
		result.y = vecL_.y + vecR_.y;
		return result;
	}
	//「-」
	Vec2 operator-(const Vec2& vecL_, const Vec2& vecR_) {
		Vec2 result;
		result.x = vecL_.x - vecR_.x;
		result.y = vecL_.y - vecR_.y;
		return result;
	}
	//「*」
	Vec2 operator*(const Vec2& vecL_, const Vec2& vecR_) {
		Vec2 result;
		result.x = vecL_.x * vecR_.x;
		result.y = vecL_.y * vecR_.y;
		return result;
	}
	//「*」
	Vec2 operator*(const Vec2& vecL_, float value_) {
		Vec2 result;
		result.x = vecL_.x * value_;
		result.y = vecL_.y * value_;
		return result;
	}
	//「+=」
	Vec2& Vec2::operator+= (const Vec2& vec_) {
		*this = *this + vec_;
		return *this;
	}
	//「-=」
	Vec2& Vec2::operator-=(const Vec2& vec_) {
		*this = *this - vec_;
		return *this;
	}
	//「>>」
	istream& operator>>(istream& is_, Vec2& vec_) {
		is_ >> vec_.x;
		is_ >> vec_.y;
		return is_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const Vec2& vec_) {
		j_ = json{
			{"x", vec_.x},
			{"y", vec_.y},
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, Vec2& vec_) {
		j_.at("x").get_to(vec_.x);
		j_.at("y").get_to(vec_.y);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string Vec2::DebugPrint() const {
		stringstream ss;
		ss << "x：" << x << '/';
		ss << "y：" << y;
		return ss.str();
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//3次元ベクトルクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//デフォルトコンストラクタ
	Vec3::Vec3() :
		Vec2(),
		z(0.0f)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Vec3::Vec3(float x_, float y_, float z_) :
		Vec2(x_, y_),
		z(z_)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//大きさ
	float Vec3::Length() const {
		return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//正規化
	Vec3 Vec3::Norm() const{
		Vec3 result;
		float length = this->Length();
		result.x = this->x / length;
		result.y = this->y / length;
		result.z = this->z / length;

		return result;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//内積
	float ML::VDot(const Vec3& vecL_, const Vec3& vecR_) {
		float result = 0;
		result += vecL_.x * vecR_.x;
		result += vecL_.y * vecR_.y;
		result += vecL_.z * vecR_.z;
		return result;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//外積
	Vec3 ML::VCross(const Vec3& vecL_, const Vec3& vecR_) {
		Vec3 result;
		result.x = vecL_.y * vecR_.z - vecL_.z * vecR_.y;
		result.y = vecL_.z * vecR_.x - vecL_.x * vecR_.z;
		result.z = vecL_.x * vecR_.y - vecL_.y * vecR_.x;
		return result;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//演算子定義
	//単項「-」
	Vec3 Vec3::operator-() {
		Vec3 result;
		result.x = -(this->x);
		result.y = -(this->y);
		result.z = -(this->z);
		return result;
	}
	//「+」
	Vec3 operator+(const Vec3& vecL_, const Vec3& vecR_) {
		Vec3 result;
		result.x = vecL_.x + vecR_.x;
		result.y = vecL_.y + vecR_.y;
		result.z = vecL_.z + vecR_.z;
		return result;
	}
	//「-」
	Vec3 operator-(const Vec3& vecL_, const Vec3& vecR_) {
		Vec3 result;
		result.x = vecL_.x - vecR_.x;
		result.y = vecL_.y - vecR_.y;
		result.z = vecL_.z - vecR_.z;
		return result;
	}
	//「*」
	Vec3 operator*(const Vec3& vec_, float scale_) {
		Vec3 result;
		result.x = vec_.x * scale_;
		result.y = vec_.y * scale_;
		result.z = vec_.z * scale_;
		return result;
	}
	//「+=」
	Vec3& Vec3::operator+=(const Vec3& vec_) {
		*this = *this + vec_;
		return *this;
	}
	//「-=」
	Vec3& Vec3::operator-=(const Vec3& vec_) {
		*this = *this - vec_;
		return *this;
	}
	//「*=」
	Vec3& Vec3::operator*=(const Mat4x4& mat_) {
		*this = *this * mat_;
		return *this;
	}
	Vec3& Vec3::operator*=(float value_) {
		*this = *this * value_;
		return *this;
	}
	//「==」
	bool Vec3::operator==(float value_) {
		return
			this->x == value_
			&& this->y == value_
			&& this->z == value_;
	}
	//「!=」
	bool Vec3::operator!=(float value_) {
		return !(*this == value_);
	}
	//「>>」
	istream& operator>>(istream& is_, Vec3& vec_) {
		is_ >> vec_.x;
		is_ >> vec_.y;
		is_ >> vec_.z;
		return is_;
	}
	//「<<」
	ostream& operator<<(ostream& os_, const Vec3& vec_) {
		os_ << vec_.x;
		os_ << vec_.y;
		os_ << vec_.z;
		return os_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const Vec3& vec_) {
		j_ = json{
			{"x", vec_.x},
			{"y", vec_.y},
			{"z", vec_.z},
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, Vec3& vec_) {
		j_.at("x").get_to(vec_.x);
		j_.at("y").get_to(vec_.y);
		j_.at("z").get_to(vec_.z);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//DXライブラリVECTORへのキャスト
	Vec3::operator DxLib::VECTOR() const {
		DxLib::VECTOR vec;
		vec.x = this->x;
		vec.y = this->y;
		vec.z = this->z;
		return vec;
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string Vec3::DebugPrint() const {
		stringstream ss;
		ss << Vec2::DebugPrint() << '/';
		ss << "z：" << z;
		return ss.str();
	}


	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//Mat4x4クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Mat4x4::Mat4x4(bool identity_) :
		DxLib::MATRIX()
	{
		if (identity_ == true) {
			this->Identity();
		}
	}
	Mat4x4::Mat4x4(const MATRIX& v_) :
		MATRIX(v_) 
	{}
	Mat4x4  Mat4x4::operator= (const MATRIX& v_)
	{
		return  (*this) = Mat4x4(v_);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//単位行列
	void  Mat4x4::Identity()
	{
		//単位行列を生成
		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				if (y == x) {
					m[y][x] = 1;
				}
				else {
					m[y][x] = 0;
				}
			}
		}
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//平行移動行列
	void Mat4x4::Translation(const Vec3& vec_) {
		Mat4x4 tr;
		tr.Identity();
		tr.m[3][0] = vec_.x;
		tr.m[3][1] = vec_.y;
		tr.m[3][2] = vec_.z;
		
		*this *= tr;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//X軸回転行列
	void Mat4x4::RotationX(float degree_) {
		Mat4x4 rot;
		rot.Identity();
		rot.m[1][1] = std::cos(ToRadian(degree_));
		rot.m[1][2] = std::sin(ToRadian(degree_));
		rot.m[2][1] = -std::sin(ToRadian(degree_));
		rot.m[2][2] = std::cos(ToRadian(degree_));

		*this *= rot;
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//Y軸回転行列
	void Mat4x4::RotationY(float degree_) {
		Mat4x4 rot;
		rot.Identity();
		rot.m[0][0] = std::cos(ToRadian(degree_));
		rot.m[0][2] = -std::sin(ToRadian(degree_));
		rot.m[2][0] = std::sin(ToRadian(degree_));
		rot.m[2][2] = std::cos(ToRadian(degree_));

		*this *= rot;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//Z軸回転行列
	void Mat4x4::RotationZ(float degree_) {
		Mat4x4 rot;
		rot.Identity();
		rot.m[0][0] = std::cos(ToRadian(degree_));
		rot.m[0][1] = std::sin(ToRadian(degree_));
		rot.m[1][0] = -std::sin(ToRadian(degree_));
		rot.m[1][1] = std::cos(ToRadian(degree_));

		*this *= rot;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//回転行列・ロールピッチヨーの順
	void Mat4x4::RotationXYZ(const Vec3& ang_) {

		Mat4x4 rot(true);
		rot.RotationX(ang_.x);
		rot.RotationY(ang_.y);
		rot.RotationZ(ang_.z);

		*this *= rot;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//任意軸回転行列
	void Mat4x4::RotationFree(const Vec3& axis_, float degree_) {
		Vec3 n = axis_.Norm();
		//角度をラジアン値に変更
		float rad = ToRadian(degree_);
		//cosθ
		float c = cosf(rad);
		//NAN判定
		if (isnan(c)) {
			c = 0;
		}
		//sinθ
		float s = sinf(rad);
		//NAN判定
		if (isnan(s)) {
			s = 1;
		}
		//回転行列生成
		Mat4x4 rot;
		rot.Identity();
		rot.m[0][0] = ((n.x * n.x) * (1 - c)) + c;
		rot.m[0][1] = ((n.x * n.y) * (1 - c)) - (n.z * s);
		rot.m[0][2] = ((n.x * n.z) * (1 - c)) + (n.y * s);

		rot.m[1][0] = ((n.x * n.y) * (1 - c)) + (n.z * s);
		rot.m[1][1] = ((n.y * n.y) * (1 - c)) + c;
		rot.m[1][2] = ((n.y * n.z) * (1 - c)) - (n.x * s);

		rot.m[2][0] = ((n.x * n.z) * (1 - c)) - (n.y * s);
		rot.m[2][1] = ((n.y * n.z) * (1 - c)) + (n.x * s);
		rot.m[2][2] = ((n.z * n.z) * (1 - c)) + c;

		*this *= rot;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//スケール変更
	void Mat4x4::Scale(float sizeX_, float sizeY_, float sizeZ_) {
		Mat4x4 rot;
		rot.Identity();
		rot.m[0][0] = sizeX_;
		rot.m[1][1] = sizeY_;
		rot.m[2][2] = sizeZ_;

		*this *= rot;
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//演算子定義

	//「*」
	Mat4x4 operator*(const Mat4x4& matL_,const Mat4x4& matR_) {
		Mat4x4 result;			//計算結果

		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				for (int i = 0; i < 4; ++i) {
					result.m[y][x] += matL_.m[y][i] * matR_.m[i][x];
				}
			}
		}
		return result;
	}

	Vec3 operator*(const Vec3& vecL_, const Mat4x4& matR_) {
		Vec3 result;			//計算結果

		result.x += vecL_.x * matR_.m[0][0];
		result.x += vecL_.y * matR_.m[1][0];
		result.x += vecL_.z * matR_.m[2][0];
		result.x += matR_.m[3][0];

		result.y += vecL_.x * matR_.m[0][1];
		result.y += vecL_.y * matR_.m[1][1];
		result.y += vecL_.z * matR_.m[2][1];
		result.y += matR_.m[3][1];

		result.z += vecL_.x * matR_.m[0][2];
		result.z += vecL_.y * matR_.m[1][2];
		result.z += vecL_.z * matR_.m[2][2];
		result.z += matR_.m[3][2];

		return result;
	}

	//「*=」
	Mat4x4& Mat4x4::operator*=(const Mat4x4& mat_) {
		*this = *this * mat_;
		return *this;
	}

	//「>>」
	istream& operator>>(istream& is_, Mat4x4& mat_) {
		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				is_ >> mat_.m[y][x];
			}
		}
		return is_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const Mat4x4& mat_) {
		vector<float> mat;
		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				mat.push_back(mat_.m[y][x]);
			}
		}
		
		j_.at("matrix") = mat;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, Mat4x4& mat_) {
		vector<float> matrix;
		j_.at("matrix").get_to(matrix);

		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				mat_.m[y][x] = matrix[(y * 4 + x)];
			}
		}
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string Mat4x4::DebugPrint() const {
		stringstream ss;

		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				ss << 'M' << y << x << "：" << m[y][x] << ' ';
			}
			ss << '\n';
		}

		return ss.str();
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//2次元ボックスクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//デフォルトコンストラクタ
	Box2D::Box2D() :
		Vec2(),
		Size2()
	{ }
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Box2D::Box2D(float x_, float y_, float w_, float h_, bool center_) :
		Vec2(x_, y_),
		Size2(w_, h_)
	{
		if (center_) {
			x -= w / 2;
			y -= h / 2;
		}
	}
	Box2D::Box2D(float x_, float y_, const Size2& size_, bool center_) :
		Vec2(x_, y_),
		Size2(size_)
	{
		if (center_) {
			x -= w / 2;
			y -= h / 2;
		}
	}
	Box2D::Box2D(const Vec2& vec_, float w_, float h_, bool center_) :
		Vec2(vec_),
		Size2(w_, h_)
	{
		if (center_) {
			x -= w / 2;
			y -= h / 2;
		}
	}
	Box2D::Box2D(const Vec2& vec_, const Size2& size_, bool center_) :
		Vec2(vec_),
		Size2(size_)
	{
		if (center_) {
			x -= w / 2;
			y -= h / 2;
		}
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//引数分移動させる
	void Box2D::Offset(float x_, float y_) {
		x += x_;
		y += y_;
	}
	void Box2D::Offset(const Vec2& vec_) {
		*this += vec_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//中心を原点として引数分移動させる
	void Box2D::OffsetCenterBase(float x_, float y_) {
		x += x_ - w / 2;
		y += y_ - h / 2;
	}
	void Box2D::OffsetCenterBase(const Vec2& vec_) {
		this->OffsetCenterBase(vec_.x, vec_.y);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//移動させたクローンを生成
	Box2D Box2D::OffsetCopy(float x_, float y_) {
		return Box2D(
			x + x_,
			y + y_,
			w,
			h
		);
	}
	Box2D Box2D::OffsetCopy(const Vec2& vec_) {
		return this->OffsetCopy(vec_.x, vec_.y);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	// 中心を原点として引数分移動したクローンを生成
	Box2D Box2D::OffsetCenterBaseCopy(float x_, float y_) {
		return Box2D(
			x + x_ - w / 2,
			y + y_ - h / 2,
			w,
			h
		);
	}
	Box2D Box2D::OffsetCenterBaseCopy(const Vec2& vec_) {
		return this->OffsetCenterBaseCopy(vec_.x, vec_.y);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//引数の座標に移動させる
	void Box2D::Set(float x_, float y_) {
		x = x_;
		y = y_;
	}
	void Box2D::Set(const Vec2& vec_) {
		return this->Set(vec_.x, vec_.y);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//引数の座標へ移動させたクローンを生成
	Box2D Box2D::SetCopy(float x_, float y_) {
		return Box2D(
			x_,
			y_,
			w,
			h
		);
	}
	Box2D Box2D::SetCopy(const Vec2& vec_) {
		return this->SetCopy(vec_.x, vec_.y);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//中心を原点として移動させたクローンを生成
	Box2D Box2D::SetCenterBaseCopy(float x_, float y_) {
		return Box2D(
			x_ - w / 2,
			y_ - h / 2,
			w,
			h
		);
	}
	Box2D Box2D::SetCenterBaseCopy(const Vec2& vec_) {
		return this->SetCenterBaseCopy(vec_.x, vec_.y);
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//中心を基点に座標指定
	void Box2D::SetCenterBase(float x_, float y_) {
		x = x_ - (w / 2);
		y = y_ - (h / 2);
	}
	void Box2D::SetCenterBase(const Vec2& vec_) {
		return this->SetCenterBase(vec_.x, vec_.y);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	// サイズ再設定
	void Box2D::Resize(float w_, float h_) {
		w = w_;
		h = h_;
	}
	void Box2D::Resize(const Size2& size_) {
		w = size_.w;
		h = size_.h;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//当たり判定
	bool Box2D::Hit(const Box2D& you_) {
		Rect2 me(*this);
		Rect2 you(you_);

		//x軸の判定
		if (me.l > you.r) { return false; }
		if (me.r < you.l) { return false; }
		//y軸の判定
		if (me.t > you.b) { return false; }
		if (me.b < you.t) { return false; }

		return true;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//演算子定義
	//「>>」
	istream& operator>>(istream& is_, Box2D& box_) {
		is_ >> box_.x;
		is_ >> box_.y;
		is_ >> box_.w;
		is_ >> box_.h;
		return is_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const Box2D& box_) {
		Vec2 pos = box_;
		Size2 size = box_;
		j_ = json{
			{"startpos", pos},
			{"size", size},
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, Box2D& box_) {
		Vec2 pos;
		Size2 size;
		j_.at("startpos").get_to(pos);
		j_.at("size").get_to(size);
		box_.Set(pos);
		box_.Resize(size);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string Box2D::DebugPrint() const {
		stringstream ss;
		ss << "x：" << x << '/';
		ss << "y：" << y << '/';
		ss << "w：" << w << '/';
		ss << "h：" << h;
		return ss.str();
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//3次元ボックスクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//デフォルトコンストラクタ
	Box3D::Box3D() :
		Vec3(),
		Size3(),
		Box2D()
	{ }
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Box3D::Box3D(float x_, float y_, float z_, float w_, float h_, float d_, bool center_) :
		Vec2(x_,y_),
		Size2(w_,h_),
		Vec3(x_, y_, z_),
		Size3(w_, h_, d_),
		Box2D()
	{
		if (center_) {
			x -= w / 2;
			y -= h / 2;
			z -= d / 2;
		}
	}
	Box3D::Box3D(float x_, float y_, float z_, const Size3& size_, bool center_) :
		Vec3(x_, y_, z_),
		Size3(size_),
		Box2D()
	{
		if (center_) {
			x -= w / 2;
			y -= h / 2;
			z -= d / 2;
		}
	}
	Box3D::Box3D(const Vec3& vec_, float w_, float h_, float d_, bool center_) :
		Vec3(vec_),
		Size3(w_, h_, d_)
	{
		if (center_) {
			x -= w / 2;
			y -= h / 2;
			z -= d / 2;
		}
	}
	Box3D::Box3D(const Vec3& vec_, const Size3& size_, bool center_) :
		Vec3(vec_),
		Size3(size_)
	{
		if (center_) {
			x -= w / 2;
			y -= h / 2;
			z -= d / 2;
		}
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//引数分移動させる
	void Box3D::Offset(float x_, float y_, float z_) {
		Box2D::Offset(x_, y_);
		z += z_;
	}
	void Box3D::Offset(const Vec3& vec_) {
		*this += vec_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//中心を原点として引数分移動させる
	void Box3D::OffsetCenterBase(float x_, float y_, float z_) {
		Box2D::OffsetCenterBase(x_, y_);
		z += z_ - d / 2;
	}
	void Box3D::OffsetCenterBase(const Vec3& vec_) {
		this->OffsetCenterBase(vec_.x, vec_.y, vec_.z);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//移動させたクローンを生成
	Box3D Box3D::OffsetCopy(float x_, float y_, float z_) {
		return Box3D(
			x + x_,
			y + y_,
			z + z_,
			w,
			h,
			d
		);
	}
	Box3D Box3D::OffsetCopy(const Vec3& vec_) {
		return this->OffsetCopy(vec_.x, vec_.y,vec_.z);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	// 中心を原点として引数分移動したクローンを生成
	Box3D Box3D::OffsetCenterBaseCopy(float x_, float y_, float z_) {
		return Box3D(
			x + x_ - w / 2,
			y + y_ - h / 2,
			z + z_ - d / 2,
			w,
			h,
			d
		);
	}
	Box3D Box3D::OffsetCenterBaseCopy(const Vec3& vec_) {
		return this->OffsetCenterBaseCopy(vec_.x, vec_.y, vec_.z);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//引数の座標に移動させる
	void Box3D::Set(float x_, float y_, float z_) {
		Box2D::Set(x_, y_);
		z = z_;
	}
	void Box3D::Set(const Vec3& vec_) {
		return this->Set(vec_.x, vec_.y, vec_.z);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//引数の座標へ移動させたクローンを生成
	Box3D Box3D::SetCopy(float x_, float y_, float z_) {
		return Box3D(
			x_,
			y_,
			z_,
			w,
			h,
			d
		);
	}
	Box3D Box3D::SetCopy(const Vec3& vec_) {
		return this->SetCopy(vec_.x, vec_.y, vec_.z);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//中心を原点として移動させたクローンを生成
	Box3D Box3D::SetCenterBaseCopy(float x_, float y_, float z_) {
		Box3D clone(
			x_ - w / 2,
			y_ - h / 2,
			z_ - d / 2,
			w,
			h,
			d
		);

		return clone;
	}
	Box3D Box3D::SetCenterBaseCopy(const Vec3& vec_) {
		return this->SetCenterBaseCopy(vec_.x, vec_.y, vec_.z);
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//中心を基点に座標指定
	void Box3D::SetCenterBase(float x_, float y_, float z_) {
		Box2D::SetCenterBase(x_, y_);
		z = z_ - (d / 2);
	}
	void Box3D::SetCenterBase(const Vec3& vec_) {
		return this->SetCenterBase(vec_.x, vec_.y, vec_.z);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	// サイズ再設定
	void Box3D::Resize(float w_, float h_, float d_) {
		Box2D::Resize(w_, h_);
		d = d_;
	}
	void Box3D::Resize(const Size3& size_) {
		w = size_.w;
		h = size_.h;
		d = size_.d;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//当たり判定
	bool Box3D::Hit(const Box3D& you_) {
		Rect3 me(*this);
		Rect3 you(you_);

		//x軸の判定
		if (me.l > you.r) { return false; }
		if (me.r < you.l) { return false; }
		//y軸の判定
		if (me.t > you.b) { return false; }
		if (me.b < you.t) { return false; }
		//z軸の判定
		if (me.n > you.f) { return false; }
		if (me.f < you.n) { return false; }

		return true;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//演算子定義
	//「>>」
	istream& operator>>(istream& is_, Box3D& box_) {
		is_ >> box_.x;
		is_ >> box_.y;
		is_ >> box_.z;
		is_ >> box_.w;
		is_ >> box_.h;
		is_ >> box_.d;
		return is_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const Box3D& box_) {
		Vec3 pos = box_;
		Size3 size = box_;
		j_ = json{
			{"startpos", pos},
			{"size", size},
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, Box3D& box_) {
		Vec3 pos;
		Size3 size;
		j_.at("startpos").get_to(pos);
		j_.at("size").get_to(size);
		box_.Set(pos);
		box_.Resize(size);
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string Box3D::DebugPrint() const {
		stringstream ss;
		ss << "x：" << x << '/';
		ss << "y：" << y << '/';
		ss << "z：" << z << '/';
		ss << "w：" << w << '/';
		ss << "h：" << h << '/';
		ss << "d：" << d;
		return ss.str();
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//角度付き直方体クラス（中心が原点）
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//デフォルトコンストラクタ
	Obb3::Obb3() :
		dirX(1, 0, 0),
		dirY(0, 1, 0),
		dirZ(0, 0, 1)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Obb3::Obb3(const Vec3& pos_, const Size3& size_) :
		Vec2(pos_.x, pos_.y),
		Vec3(0, 0, pos_.z),
		Size2(size_.w, size_.h),
		Size3(0, 0, size_.d),
		dirX(1, 0, 0),
		dirY(0, 1, 0),
		dirZ(0, 0, 1)
	{
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//移動
	void Obb3::Offset(const Vec3& pos_) {
		this->x += pos_.x;
		this->y += pos_.y;
		this->z += pos_.z;
	}
	//コピー
	Obb3 Obb3::OffsetCopy(const Vec3& pos_) const {
		Obb3 newObj = *this;
		newObj.Offset(pos_);
		return newObj;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//座標指定
	void Obb3::SetPos(const Vec3& pos_) {
		this->x = pos_.x;
		this->y = pos_.y;
		this->z = pos_.z;
	}
	//コピー
	Obb3 Obb3::SetPosCopy(const Vec3& pos_) const {
		Obb3 newObj = *this;
		newObj.SetPos(pos_);
		return newObj;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//方向ベクトルYを更新し、それに合わせてX,Zも更新する
	void Obb3::SetDirectionY(const Vec3& vec_) {

		//Y軸更新
		this->dirY = vec_.Norm();

		//X,Z軸を回転させるための行列を作る
		//通常時のY軸
		Vec3 dirYstd(0, 1, 0);
		//内積の性質を利用してY軸の回転量を求める
		float rot = acos(VDot(dirYstd, dirY) / (dirYstd.Length() * dirY.Length()));
		//回転軸を求める
		Vec3 axis = VCross(dirY, dirYstd);
		if (axis.Length() == 0) {
			axis = Vec3(1, 0, 0);
		}
		//回転行列生成
		Mat4x4 matR;
		matR.Identity();
		matR.RotationFree(axis, ToDegree(rot));

		//X軸更新
		Vec3 dirXstd(1, 0, 0);
		this->dirX = dirXstd * matR;

		//Z軸更新
		Vec3 dirZstd(0, 0, 1);
		this->dirZ = dirZstd * matR;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//サイズ再設定
	void Obb3::Resize(const Size3& size_) {
		this->w = size_.w;
		this->h = size_.h;
		this->d = size_.d;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//当たり判定（マルペケつくろードットコム様より改変）
	bool CheckHit(const Obb3& obb1_, const Obb3& obb2_){
		// 各方向ベクトルの確保
   // （N***:標準化方向ベクトル）
		Vec3 NAe1 = obb1_.get_dirX(), Ae1 = NAe1 * (obb1_.w / 2);
		Vec3 NAe2 = obb1_.get_dirY(), Ae2 = NAe2 * (obb1_.h / 2);
		Vec3 NAe3 = obb1_.get_dirZ(), Ae3 = NAe3 * (obb1_.d / 2);
		Vec3 NBe1 = obb2_.get_dirX(), Be1 = NBe1 * (obb2_.w / 2);
		Vec3 NBe2 = obb2_.get_dirY(), Be2 = NBe2 * (obb2_.h / 2);
		Vec3 NBe3 = obb2_.get_dirZ(), Be3 = NBe3 * (obb2_.d / 2);
		Vec3 Interval = obb1_.get_pos() - obb2_.get_pos();

		// 分離軸 : Ae1
		float rA = Ae1.Length();
		float rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
		float L = fabs(VDot(Interval, NAe1));
		if (L > rA + rB)
			return false; // 衝突していない

		// 分離軸 : Ae2
		rA = Ae2.Length();
		rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
		L = fabs(VDot(Interval, NAe2));
		if (L > rA + rB)
			return false;

		// 分離軸 : Ae3
		rA = Ae3.Length();
		rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
		L = fabs(VDot(Interval, NAe3));
		if (L > rA + rB)
			return false;

		// 分離軸 : Be1
		rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
		rB = Be1.Length();
		L = fabs(VDot(Interval, NBe1));
		if (L > rA + rB)
			return false;

		// 分離軸 : Be2
		rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
		rB = Be2.Length();
		L = fabs(VDot(Interval, NBe2));
		if (L > rA + rB)
			return false;

		// 分離軸 : Be3
		rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
		rB = Be3.Length();
		L = fabs(VDot(Interval, NBe3));
		if (L > rA + rB)
			return false;

		// 分離軸 : C11
		Vec3 Cross;
		Cross = VCross(NAe1, NBe1);
		rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
		rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
		L = fabs(VDot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C12
		Cross = VCross(NAe1, NBe2);
		rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
		rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
		L = fabs(VDot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C13
		Cross = VCross(NAe1, NBe3);
		rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
		rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
		L = fabs(VDot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C21
		Cross = VCross(NAe2, NBe1);
		rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
		rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
		L = fabs(VDot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C22
		Cross = VCross(NAe2, NBe2);
		rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
		rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
		L = fabs(VDot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C23
		Cross = VCross(NAe2, NBe3);
		rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
		rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
		L = fabs(VDot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C31
		Cross = VCross(NAe3, NBe1);
		rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
		rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
		L = fabs(VDot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C32
		Cross = VCross(NAe3, NBe2);
		rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
		rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
		L = fabs(VDot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離軸 : C33
		Cross = VCross(NAe3, NBe3);
		rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
		rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
		L = fabs(VDot(Interval, Cross));
		if (L > rA + rB)
			return false;

		// 分離平面が存在しないので「衝突している」
		return true;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
////当たり判定（マルペケつくろードットコム様より改変）
//	bool CheckHitToMoveObb(const Obb3& obbM_, const Vec3& moveVec_, const Obb3 & obb2_) {
//		// 各方向ベクトルの確保
//   // （N***:標準化方向ベクトル）
//		Vec3 NAe1 = obbM_.get_dirX(), Ae1 = NAe1 * (obbM_.w / 2);
//		Vec3 NAe2 = obbM_.get_dirY(), Ae2 = NAe2 * (obbM_.h / 2);
//		Vec3 NAe3 = obbM_.get_dirZ(), Ae3 = NAe3 * (obbM_.d / 2);
//		Vec3 NAe4 = moveVec_.Norm(),  Ae4 =  moveVec_;
//		Vec3 NAe5 = moveVec_.
//		Vec3 NBe1 = obb2_.get_dirX(), Be1 = NBe1 * (obb2_.w / 2);
//		Vec3 NBe2 = obb2_.get_dirY(), Be2 = NBe2 * (obb2_.h / 2);
//		Vec3 NBe3 = obb2_.get_dirZ(), Be3 = NBe3 * (obb2_.d / 2);
//		Vec3 Interval = obbM_.get_pos() - obb2_.get_pos();
//
//		// 分離軸 : Ae1
//		float rA = Ae1.Length();
//		float rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
//		float L = fabs(VDot(Interval, NAe1));
//		if (L > rA + rB)
//			return false; // 衝突していない
//
//		// 分離軸 : Ae2
//		rA = Ae2.Length();
//		rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
//		L = fabs(VDot(Interval, NAe2));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : Ae3
//		rA = Ae3.Length();
//		rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
//		L = fabs(VDot(Interval, NAe3));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : Be1
//		rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
//		rB = Be1.Length();
//		L = fabs(VDot(Interval, NBe1));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : Be2
//		rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
//		rB = Be2.Length();
//		L = fabs(VDot(Interval, NBe2));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : Be3
//		rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
//		rB = Be3.Length();
//		L = fabs(VDot(Interval, NBe3));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : C11
//		Vec3 Cross;
//		Cross = VCross(NAe1, NBe1);
//		rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
//		rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
//		L = fabs(VDot(Interval, Cross));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : C12
//		Cross = VCross(NAe1, NBe2);
//		rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
//		rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
//		L = fabs(VDot(Interval, Cross));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : C13
//		Cross = VCross(NAe1, NBe3);
//		rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
//		rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
//		L = fabs(VDot(Interval, Cross));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : C21
//		Cross = VCross(NAe2, NBe1);
//		rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
//		rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
//		L = fabs(VDot(Interval, Cross));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : C22
//		Cross = VCross(NAe2, NBe2);
//		rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
//		rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
//		L = fabs(VDot(Interval, Cross));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : C23
//		Cross = VCross(NAe2, NBe3);
//		rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
//		rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
//		L = fabs(VDot(Interval, Cross));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : C31
//		Cross = VCross(NAe3, NBe1);
//		rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
//		rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
//		L = fabs(VDot(Interval, Cross));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : C32
//		Cross = VCross(NAe3, NBe2);
//		rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
//		rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
//		L = fabs(VDot(Interval, Cross));
//		if (L > rA + rB)
//			return false;
//
//		// 分離軸 : C33
//		Cross = VCross(NAe3, NBe3);
//		rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
//		rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
//		L = fabs(VDot(Interval, Cross));
//		if (L > rA + rB)
//			return false;
//
//		// 分離平面が存在しないので「衝突している」
//		return true;
//	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//演算子定義
	//「>>」
	std::istream& operator>>(std::istream& is_, Obb3& obb_) {
		is_ >> obb_.x;
		is_ >> obb_.y;
		is_ >> obb_.z;
		is_ >> obb_.w;
		is_ >> obb_.h;
		is_ >> obb_.d;
		return is_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const Obb3& obb_) {
		Vec3 pos = obb_;
		Size3 size = obb_;
		Vec3 dirY = obb_.dirY;
		j_ = json{
			{"centerpos", pos},
			{"size", size},
			{"directionY", dirY}
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, Obb3& obb_) {
		Vec3 pos;
		Size3 size;
		Vec3 dirY;
		j_.at("centerpos").get_to(pos);
		j_.at("size").get_to(size);
		j_.at("directionY").get_to(dirY);
		obb_.SetPos(pos);
		obb_.Resize(size);
		obb_.SetDirectionY(dirY);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	string Obb3::DebugPrint() const {
		stringstream ss;
		ss << "x：" << x << '/';
		ss << "y：" << y << '/';
		ss << "z：" << z << '/';
		ss << "w：" << w << '/';
		ss << "h：" << h << '/';
		ss << "d：" << d << '\n';
		ss << "dirX「" << dirX.DebugPrint() << "」\n";
		ss << "dirY「" << dirY.DebugPrint() << "」\n";
		ss << "dirZ「" << dirZ.DebugPrint() << "」";
		return ss.str();
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegOnSeparateAxis(const Vec3& sep_, const Vec3& e1_, const Vec3& e2_, const Vec3& e3_) {
		//3つの内積の絶対値の和で投影線分長を計算
		//分離軸Sepは標準化されていること
		float r1 = fabs(VDot(sep_, e1_));
		float r2 = fabs(VDot(sep_, e2_));
		float r3 = 0;

		//3次元の場合のみ処理を行う
		if (e3_.x != 0 || e3_.y != 0 || e3_.z != 0) {
			r3 = fabs(VDot(sep_, e3_));
		}
		return r1 + r2 + r3;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//2次元矩形クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//デフォルトコンストラクタ
	Rect2::Rect2() :
		l(0),
		r(0),
		t(0),
		b(0)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Rect2::Rect2(float l_, float r_, float t_, float b_) :
		l(l_),
		r(r_),
		t(t_),
		b(b_)
	{}
	Rect2::Rect2(const ML::Box2D& box_) :
		l(box_.x),
		r(box_.x + box_.w - 1),
		t(box_.y),
		b(box_.y + box_.h - 1)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//ボックスから変換
	void Rect2::BoxToRect(const ML::Box2D& box_) {
		l = box_.x;
		r = box_.x + box_.w - 1;
		t = box_.y;
		b = box_.y + box_.h - 1;
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//演算子定義
	//「>>」
	istream& operator>>(istream& is_, Rect2& rect_) {
		is_ >> rect_.l;
		is_ >> rect_.r;
		is_ >> rect_.t;
		is_ >> rect_.b;
		return is_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const Rect2& rect_) {
		j_ = json{
			{"left", rect_.l},
			{"right", rect_.r},
			{"top", rect_.t},
			{"bottom", rect_.b},
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, Rect2& rect_) {
		j_.at("left").get_to(rect_.l);
		j_.at("right").get_to(rect_.r);
		j_.at("top").get_to(rect_.t);
		j_.at("bottom").get_to(rect_.b);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string Rect2::DebugPrint() const {
		stringstream ss;
		ss << "left：" << l << '/';
		ss << "right：" << r << '/';
		ss << "top：" << t << '/';
		ss << "bottom：" << b;
		return ss.str();
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//3次元矩形クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//デフォルトコンストラクタ
	Rect3::Rect3() :
		Rect2(),
		n(0),
		f(0)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Rect3::Rect3(float l_, float r_, float t_, float b_, float n_, float f_) :
		Rect2(l_, r_, t_, b_),
		n(n_),
		f(f_)
	{}
	Rect3::Rect3(const ML::Box3D& box_) :
		Rect2(box_),
		n(box_.z),
		f(box_.z + box_.d - 1)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//ボックスから変換
	void Rect3::BoxToRect(const ML::Box3D& box_) {
		Rect2::BoxToRect(box_);
		n = box_.z;
		f = box_.z + box_.d - 1;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//演算子定義
	//「>>」
	istream& operator>>(istream& is_, Rect3& rect_) {
		is_ >> rect_.l;
		is_ >> rect_.r;
		is_ >> rect_.t;
		is_ >> rect_.b;
		is_ >> rect_.n;
		is_ >> rect_.f;
		return is_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const Rect3& rect_) {
		j_ = json{
			{"left", rect_.l},
			{"right", rect_.r},
			{"top", rect_.t},
			{"bottom", rect_.b},
			{"near", rect_.n},
			{"far", rect_.f}
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, Rect3& rect_) {
		j_.at("left").get_to(rect_.l);
		j_.at("right").get_to(rect_.r);
		j_.at("top").get_to(rect_.t);
		j_.at("bottom").get_to(rect_.b);
		j_.at("near").get_to(rect_.n);
		j_.at("far").get_to(rect_.f);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string Rect3::DebugPrint() const {
		stringstream ss;
		ss << Rect2::DebugPrint() << '/';
		ss << "near：" << n << '/';
		ss << "far：" << f;
		return ss.str();
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//変形クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//デフォルトコンストラクタ
	Transform::Transform(float rotDeg_, ML::Vec2 rotCt_, float scX_, float scY_, bool flipX_, bool flipY_)
		:rotateDeg(rotDeg_)
		, rotateCenterPos(rotCt_)
		, scaleX(scX_)
		, scaleY(scY_)
		, flipX(flipX_)
		, flipY(flipY_)
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//演算子定義
	//「>>」
	istream& operator>>(istream& is_, Transform& trs_) {
		is_ >> trs_.rotateDeg;
		is_ >> trs_.rotateCenterPos;
		is_ >> trs_.scaleX;
		is_ >> trs_.scaleY;
		is_ >> trs_.flipX;
		is_ >> trs_.flipY;
		return is_;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルへの書き出し
	void to_json(json& j_, const Transform& trs_) {
		j_ = json{
			{"rotateDeg", trs_.rotateDeg},
			{"rotateCenterPos", trs_.rotateCenterPos},
			{"scaleX", trs_.scaleX},
			{"scaleY", trs_.scaleY},
			{"flipX", trs_.flipX},
			{"flipY", trs_.flipY},
		};
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//jsonファイルの読み込み
	void from_json(const json& j_, Transform& trs_) {
		j_.at("rotateDeg").get_to(trs_.rotateDeg);
		j_.at("rotateCenterPos").get_to(trs_.rotateCenterPos);
		j_.at("scaleX").get_to(trs_.scaleX);
		j_.at("scaleY").get_to(trs_.scaleY);
		j_.at("flipX").get_to(trs_.flipX);
		j_.at("flipY").get_to(trs_.flipY);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string Transform::DebugPrint() const {
		stringstream ss;
		ss << "rotateDeg" << rotateDeg << '/';
		ss << "rotateCenterPos" << rotateCenterPos.DebugPrint() << '/';
		ss << "scaleX" << scaleX << '/';
		ss << "scaleY" << scaleY << '/';
		ss << "flipX" << flipX << '/';
		ss << "flipY" << flipY;
		return ss.str();
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//画像クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//生成
	Image::SP Image::Create(char* filePath_, const Size2& size_) {
		auto img = Image::SP(new Image(filePath_, size_));
		if (img->handle != -1) {
			return img;
		}
		else {
			img.reset();
			return nullptr;
		}
	}
	Image::SP Image::Create(const char* filePath_, const Size2& size_) {
		auto img = Image::SP(new Image(filePath_, size_));
		if (img->handle != -1) {
			return img;
		}
		else {
			img.reset();
			return nullptr;
		}
	}
	Image::SP Image::Create(std::string filePath_, const Size2& size_) {
		auto img = Image::SP(new Image(filePath_, size_));
		if (img->handle != -1) {
			return img;
		}
		else {
			img.reset();
			return nullptr;
		}
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Image::Image(char* filePath_, const Size2& size_) :
		filePath(filePath_),
		size(size_) 
	{
		handle = LoadGraph(filePath.c_str());
	}
	Image::Image(const char* filePath_, const Size2& size_) :
		filePath(filePath_),
		size(size_)
	{
		handle = LoadGraph(filePath.c_str());
	}
	Image::Image(string filePath_, const Size2& size_) :
		filePath(filePath_),
		size(size_)
	{
		handle = LoadGraph(filePath.c_str());
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//中心座標を受け取る
	Vec2 Image::CenterPos() {
		return Vec2(this->size.w / 2, this->size.h / 2);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//描画
	void Image::Draw(int x_, int y_, bool TransFlag, float alpha_) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_ * 255);
		DrawGraph(x_, y_, handle, TransFlag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	void Image::Draw(Vec2 pos_, bool TransFlag, float alpha_) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_ * 255);
		DrawGraph((int)pos_.x, (int)pos_.y, handle, TransFlag);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//中心を原点にして描画
	void Image::DrawCt(int x_, int y_, bool TransFlag, float alpha_) {
		int x = x_ - (int)this->size.w / 2;
		int y = y_ - (int)this->size.h / 2;
		this->Draw(x, y, TransFlag, alpha_);
	}
	void Image::DrawCt(Vec2 pos_, bool TransFlag, float alpha_) {
		int x = (int)pos_.x - (int)this->size.w / 2;
		int y = (int)pos_.y - (int)this->size.h / 2;
		this->Draw(x, y, TransFlag, alpha_);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//中心を原点にして変形描画
	void Image::DrawTransform(Vec2 pos_, Transform trs_, bool TransFlag, float alpha_) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_ * 255);
		DrawRotaGraph3(
			(int)pos_.x,								//中心原点の座標x
			(int)pos_.y,								//中心原点の座標y
			(int)trs_.rotateCenterPos.x,	//画像上の回転の中心座標
			(int)trs_.rotateCenterPos.y,	//画像上の回転の中心座標
			trs_.scaleX,					//x方向のサイズ
			trs_.scaleY,					//y方向のサイズ
			ML::ToRadian(trs_.rotateDeg),	//回転
			this->handle,					//画像ハンドル
			TransFlag,						//透明度変更の可否
			trs_.flipX,						//x方向の反転
			trs_.flipY						//y方向の反転
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string Image::DebugPrint() const {
		stringstream ss;
		ss << "filePath：" << filePath << '/';
		ss << "handle" << handle;
		return ss.str();
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//分割画像クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//生成
	PartImage::SP PartImage::Create(char* filePath_, int AllNum_, int xCnt_, int yCnt_, int xSize_, int ySize_) {
		return PartImage::SP(new PartImage(filePath_, AllNum_, xCnt_, yCnt_, xSize_, ySize_));
	}
	PartImage::SP PartImage::Create(const char* filePath_, int AllNum_, int xCnt_, int yCnt_, int xSize_, int ySize_) {
		return PartImage::SP(new PartImage(filePath_, AllNum_, xCnt_, yCnt_, xSize_, ySize_));
	}
	PartImage::SP PartImage::Create(string filePath_, int AllNum_, int xCnt_, int yCnt_, int xSize_, int ySize_) {
		return PartImage::SP(new PartImage(filePath_, AllNum_, xCnt_, yCnt_, xSize_, ySize_));
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	PartImage::PartImage(const string& filePath_, int AllNum_, int xCnt_, int yCnt_, int xSize_, int ySize_) :
		filePath(filePath_),
		AllNum(AllNum_),
		xCount(xCnt_),
		yCount(yCnt_),
		xSize(xSize_),
		ySize(ySize_)
	{
		handleArray = new int[AllNum_];
		LoadDivGraph(filePath_.c_str(), AllNum_, xCnt_, yCnt_, xSize_, ySize_, handleArray);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デストラクタ
	PartImage::~PartImage() {
		delete[] handleArray;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//描画
	void PartImage::Draw(int x_, int y_, int imageNum_, bool TransFlag_, float alpha_) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_ * 255);
		DrawGraph(x_, y_, handleArray[imageNum_], TransFlag_);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	void PartImage::Draw(Vec2 pos_, int imageNum_, bool TransFlag_, float alpha_) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_ * 255);
		DrawGraph((int)pos_.x, (int)pos_.y, handleArray[imageNum_], TransFlag_);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string PartImage::DebugPrint() const {
		stringstream ss;
		ss << "filePath：" << filePath << '/';
		ss << "handleArray：" << handleArray << '/';
		ss << "AllNum：" << AllNum << '/';
		ss << "xCount：" << xCount << '/';
		ss << "yCount：" << yCount << '/';
		ss << "xSize：" << xSize << '/';
		ss << "ySize：" << ySize;
		return ss.str();
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//Fontクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//生成
	Font::SP Font::Create() {
		Font::SP font(new Font());
		if (font) {
			return font;
		}
		else {
			return nullptr;
		}
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//生成（フォント設定あり）
	Font::SP Font::Create(const char* fontName_, int size_, int thick_, int fontType_) {
		Font::SP font(new Font(fontName_, size_, thick_, fontType_));
		if (font) {
			return font;
		}
		else {
			return nullptr;
		}
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デフォルトコンストラクタ
	Font::Font()
	{
		//フォント生成
		handle = CreateFontToHandle(NULL, -1, -1, -1);
		//フォント情報をメンバに代入
		GetFontStateToHandle(fontName, &size, &thick, handle);
		fontType = DX_FONTTYPE_NORMAL;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Font::Font(const char* fontName_, int size_, int thick_, int fontType_) {
		//フォント生成
		handle = CreateFontToHandle(fontName_, size_, thick_, fontType_);
		//フォント情報をメンバに代入
		GetFontStateToHandle(fontName, &size, &thick, handle);
		if (fontType_ < 0) {
			fontType = fontType_;
		}
		else {
			fontType = DX_FONTTYPE_NORMAL;
		}
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//描画
	void Font::Draw(int x_, int y_, const char* str_, const Color& color_) {
		DrawStringToHandle(x_, y_, str_, GetColor(color_), handle);
	}
	void Font::Draw(Vec2 pos_, const char* str_, const Color& color_) {
		DrawStringToHandle((int)pos_.x, (int)pos_.y, str_, GetColor(color_), handle);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string Font::DebugPrint() const {
			stringstream ss;
			ss << "handle：" << handle << '/';
			ss << "fontName：" << fontName << '/';
			ss << "size：" << size << '/';
			ss << "thick：" << thick << '/';
			ss << "fontType：" << fontType;
			return ss.str();
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//メッシュクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//生成（読み込み可能な形式：x   mqo   mv1   pmd(+vmd))
	//（DXライブラリのツールにてfbxをmv1に変換可能）
	Model3D::SP Model3D::Create(char* filePath_) {		//char型
		string fpath = filePath_;
		return SP(new Model3D(fpath));
	}
	Model3D::SP Model3D::Create(const char* filePath_) {//const char型
		string fpath = filePath_;
		return SP(new Model3D(fpath));
	}
	Model3D::SP Model3D::Create(const std::string& filePath_) {//string型
		return SP(new Model3D(filePath_));
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Model3D::Model3D(const string& filePath_) :
		filePath(filePath_),
		handle(MV1LoadModel(filePath_.c_str())),
		diffuse(1, 1, 1, 1),
		specler(1, 1, 1, 1),
		emissive(1, 1, 1, 1),
		ambient(1, 1, 1, 1)
	{ }
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デストラクタ
	Model3D::~Model3D() {
		DxLib::MV1DeleteModel(this->handle);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//描画
	void Model3D::Draw(const Mat4x4& matWorld_) {
		DxLib::MV1SetMatrix(this->handle, matWorld_);
		DxLib::MV1DrawModel(this->handle);
	}
	void Model3D::DrawStd(const Vec3& pos_, const Vec3& angle_, const Vec3& scale_) {
		Mat4x4 matWorld;
		matWorld.Identity();
		matWorld.Scale(scale_.x, scale_.y, scale_.z);
		matWorld.RotationX(angle_.x);
		matWorld.RotationY(angle_.y);
		matWorld.RotationZ(angle_.z);
		matWorld.Translation(pos_);

		DxLib::MV1SetMatrix(this->handle, matWorld);
		DxLib::MV1DrawModel(this->handle);
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//マテリアル設定
	//ディフューズ
	void Model3D::SetDiffuse(ML::Color& dif_) {
		this->diffuse = dif_;
		DxLib::MV1SetDifColorScale(this->handle, this->diffuse);
	}
	//スペキュラ
	void Model3D::SetSpecler(ML::Color& spec_) {
		this->specler = spec_;
		DxLib::MV1SetSpcColorScale(this->handle, this->specler);
	}
	//エミッシブ
	void Model3D::SetEmissive(ML::Color& emi_) {
		this->emissive = emi_;
		DxLib::MV1SetEmiColorScale(this->handle, this->emissive);
	}
	//アンビエント
	void Model3D::SetAmbient(ML::Color& amb_) {
		this->ambient = amb_;
		DxLib::MV1SetAmbColorScale(this->handle, this->ambient);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	string Model3D::DebugPrint() const {
		stringstream ss;
		ss << "filePath：" << filePath << '/';
		ss << "handle：" << handle;
		return ss.str();
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//メッシュ複製用クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Model3DClone::Model3DClone(Model3D::SP baseModel_)
		:wp_base(baseModel_)
		, Model3D()
	{
		filePath = baseModel_->get_filePath();
		handle = DxLib::MV1DuplicateModel(baseModel_->get_handle());
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デストラクタ
	Model3DClone::~Model3DClone() {
		DxLib::MV1DeleteModel(this->handle);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//生成
	Model3DClone::SP Model3DClone::Create(Model3D::SP baseModel_) {
		if (baseModel_ != nullptr) {
			return make_shared<Model3DClone>(baseModel_);
		}
		else {
			return nullptr;
		}
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//描画
	void Model3DClone::Draw(const Mat4x4& matWorld_) {
		if (!wp_base.lock()) {
			throw "コピー元が解放されているため、読み込めません。";
		}
		else {
			Model3D::Draw(matWorld_);
		}
	}
	void Model3DClone::DrawStd(const Vec3& pos_, const Vec3& angle_, const Vec3& scale_) {
		if(!wp_base.lock()) {
			throw "コピー元が解放されているため、読み込めません。";
		}
		else {
			Model3D::DrawStd(pos_, angle_, scale_);
		}
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	string Model3DClone::DebugPrint() const {
		stringstream ss;
		ss << Model3D::DebugPrint() << '/';
		ss << "wp_base：" << wp_base.lock();
		return ss.str();
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//キーボードクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	// デフォルトコンストラクタ
	InKeyBoard::InKeyBoard() :
		preInp{},
		inp{}
	{}

	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//入力情報更新
	void InKeyBoard::UpDate() {
		//1フレーム前の入力情報のコピー
		for (int i = 0; i < ML::InKeyBoard::keysSize; ++i) {
			this->preInp[i] = this->inp[i];
		}

		//現在の入力情報を取得
		DxLib::GetHitKeyStateAll(this->inp);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー

	//入力開始時のフレームならtrue
	bool InKeyBoard::Down(int key_) {
		if (preInp[key_] == 0
			&& inp[key_] == 1) {
			return true;
		}
		return false;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//入力中ならtrue
	bool InKeyBoard::On(int key_) {
		if (inp[key_] == 1) {
			return true;
		}
		return false;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//入力終了時のフレームならtrue
	bool InKeyBoard::Up(int key_) {
		if (preInp[key_] == 1
			&& inp[key_] == 0) {
			return true;
		}
		return false;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//入力無しならtrue
	bool InKeyBoard::Off(int key_) {
		if (inp[key_] == 0) {
			return true;
		}
		return false;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string InKeyBoard::DebugPrint() const {
		stringstream ss;

		//1フレーム前の入力
		ss << "preInput：";
		for (int i = 0; i < keysSize; ++i) {
			if (preInp[i] == 1) {
				ss << i << '/';
			}
		}
		ss << '\n';

		//現フレームの入力
		ss << "input：";
		for (int i = 0; i < keysSize; ++i) {
			if (inp[i] == 1) {
				ss << i << '/';
			}
		}
		return ss.str();
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//マウスクラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	// デフォルトコンストラクタ
	InMouse::InMouse() :
		preInp{},
		inp{}
	{}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//マウスボタン番号をマクロからクラス内の数値に変換
	int InMouse::Convert(int macro_) {
		switch (macro_) {
		//左ボタン
		case MOUSE_INPUT_LEFT: 
			return MI::LEFT;
		//右ボタン
		case MOUSE_INPUT_RIGHT: 
			return MI::RIGHT;
		//中ボタン
		case MOUSE_INPUT_MIDDLE: 
			return MI::MIDDLE;
		//それ以外
		default:
			return -1;
		}
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//位置情報取得
	ML::Vec2 InMouse::getPos() const{
		int posX = 0;
		int posY = 0;
		DxLib::GetMousePoint(&posX, &posY);

		return ML::Vec2(posX, posY);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//ホイールの回転量
	int InMouse::GetWheelRotVol() {
		return this->wheelRotation;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//レイの標準ベクトルを取得する
	ML::Vec3 InMouse::GetRayNormOfScreen(shared_ptr<Camera> cam_, int screenCenterY_) const {
		//カメラが取得できない場合は0で返す
		if (!cam_) { return ML::Vec3(0, 0, 0); }

		ML::Vec2 msPos = this->getPos() + cam_->getViewScreenCenter();	//マウス座標
		msPos.y -= screenCenterY_;
		ML::Vec3 scPos(msPos.x, msPos.y, 0);	//スクリーン座標

		VECTOR RaySt = DxLib::ConvScreenPosToWorldPos_ZLinear(scPos);
		scPos.z = 1.0f;
		VECTOR RayEn = DxLib::ConvScreenPosToWorldPos_ZLinear(scPos);


		ML::Vec3 RayVec(
			RayEn.x - RaySt.x,
			RayEn.y - RaySt.y,
			RayEn.z - RaySt.z
		);
		return RayVec.Norm();

	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//入力情報更新
	void InMouse::UpDate() {
		//1フレーム前の入力情報のコピー
		for (int i = 0; i < ML::InMouse::buttonsSize; ++i) {
			this->preInp[i] = this->inp[i];
		}
		
		//現在の入力情報を取得
		this->inp[MI::LEFT] = ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0);
		this->inp[MI::RIGHT] = ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0);
		this->inp[MI::MIDDLE] = ((GetMouseInput() & MOUSE_INPUT_MIDDLE) != 0);
		this->wheelRotation = GetMouseWheelRotVol();

	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー

	//入力開始時のフレームならtrue
	bool InMouse::Down(int button_) {
		int button = InMouse::Convert(button_);
		if (preInp[button] == 0
			&& inp[button] == 1) {
			return true;
		}
		return false;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//入力中ならtrue
	bool InMouse::On(int button_) {
		int button = InMouse::Convert(button_);
		if (inp[button] == 1) {
			return true;
		}
		return false;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//入力終了時のフレームならtrue
	bool InMouse::Up(int button_) {
		int button = InMouse::Convert(button_);
		if (preInp[button] == 1
			&& inp[button] == 0) {
			return true;
		}
		return false;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//入力無しならtrue
	bool InMouse::Off(int button_) {
		int button = InMouse::Convert(button_);
		if (inp[button] == 0) {
			return true;
		}
		return false;
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デバッグ情報
	std::string InMouse::DebugPrint() const {
		stringstream ss;

		//1フレーム前の入力
		ss << "preInput：";
		for (int i = 0; i < buttonsSize; ++i) {
			if (preInp[i] == 1) {
				ss << i << '/';
			}
		}
		ss << '\n';

		//現フレームの入力
		ss << "input：";
		for (int i = 0; i < buttonsSize; ++i) {
			if (inp[i] == 1) {
				ss << i << '/';
			}
		}
		return ss.str();
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//カメラ基本形クラス
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//コンストラクタ
	Camera::Camera(const ML::Vec3& tg_,	//	被写体の位置
		const ML::Vec3& pos_,	//	カメラの位置
		const ML::Vec3& up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
		float            fov_,	//	視野角
		float            np_,	//	前クリップ平面（これより前は映らない）
		float            fp_,	//	後クリップ平面（これより後ろは映らない）
		float            asp_,	//	アスペクト比（画面の比率に合わせる　横÷縦）
		ML::Box2D		 screen_)	//画面描画範囲
		: pos(pos_), target(tg_), up(up_), fov(fov_), nearPlane(np_), forePlane(fp_), aspect(asp_), screenArea(screen_)
	{
		update();
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//デストラクタ
	Camera::~Camera()
	{
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//	カメラを生成する
	Camera::SP Camera::Create(const ML::Vec3& tg_,	//	被写体の位置
		const ML::Vec3& pos_,	//	カメラの位置
		const ML::Vec3& up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
		float            fov_,	//	視野角
		float            np_,	//	前クリップ平面（これより前は映らない）
		float            fp_,	//	後クリップ平面（これより後ろは映らない）
		float            asp_,	//	アスペクト比（画面の比率に合わせる　横÷縦）
		ML::Box2D		 screen_)	//画面描画範囲
	{
		return Camera::SP(new Camera(tg_, pos_, up_, fov_, np_, fp_, asp_, screen_));
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//カメラの設定
	void Camera::update()
	{
		//描画範囲設定
		DxLib::SetDrawArea(
			(int)this->screenArea.x,
			(int)this->screenArea.y,
			(int)(this->screenArea.x + this->screenArea.w),
			(int)(this->screenArea.y + this->screenArea.h)
		);
		//カメラの中心設定
		DxLib::SetCameraScreenCenter(
			this->screenArea.x + (this->screenArea.w / 2),
			this->screenArea.y + (this->screenArea.h / 2)
		);

		//上方向ベクトル
		DxLib::SetCameraPositionAndTargetAndUpVec(pos, target, up);
		//描画可能距離
		DxLib::SetCameraNearFar(nearPlane, forePlane);
		//視野角
		DxLib::SetupCamera_Perspective(fov);
		//追加のアスペクト比
		DxLib::SetCameraDotAspect(aspect);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//描画する範囲の中心座標を返す
	ML::Vec2 Camera::getViewScreenCenter() {
		float x = this->screenArea.x + this->screenArea.w / 2.0f;
		float y = this->screenArea.y + this->screenArea.h / 2.0f;
		return ML::Vec2(x, y);
	}

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//その他関数群
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//色情報取得
	//透過度無し
	unsigned int GetColor(const Color& color_)
	{
		return DxLib::GetColor((int)(color_.r * 255), (int)(color_.g * 255), (int)(color_.b * 255));
	}
	//8bitカラー
	DxLib::COLOR_U8 GetColorU8(const Color& color_) {
		return DxLib::GetColorU8((int)(color_.r * 255), (int)(color_.g * 255), (int)(color_.b * 255), (int)(color_.a * 255));
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//度数法からラジアン値に変換
	float ToRadian(float degree_) {
		return degree_ * (PI / 180.0f);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//ラジアン値から度数法に変換
	float ToDegree(float radian_) {
		return radian_ * (180.0f / PI);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーー
	//DXライブラリで用意されている矩形描画を利用する関数
	bool DrawBox(const Box2D& box_, const Color& color_, bool fillFlag_) {
		return (bool)DxLib::DrawBox((int)box_.x, (int)box_.y, (int)(box_.x + box_.w), (int)(box_.y + box_.h), ML::GetColor(color_), fillFlag_);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーー
	//DXライブラリ関数を使用してボックスを表示させる
	void DrawBox3D(const Box3D& box_, const Color& col_) {


		//VERTEXを初期化する関数
		auto initVer = [](
			DxLib::VERTEX3D* ver_,	//頂点3つを2組格納できるVERTEX3D[2][3]配列（1面分のデータ）
			const Vec3& norm_,		//設定する面の法線ベクトル
			const Color& col_		//色
			)
		{
			for (int i = 0; i < 2; ++i) {
				for (int j = 0; j < 3; ++j) {
					ver_[i * 3 + j].norm = norm_;
					ver_[i * 3 + j].dif = ML::GetColorU8(col_);
					ver_[i * 3 + j].spc = ML::GetColorU8(ML::Color(0, 0, 0, 0));
					ver_[i * 3 + j].u = 0.0f;
					ver_[i * 3 + j].v = 0.0f;
					ver_[i * 3 + j].su = 0.0f;
					ver_[i * 3 + j].sv = 0.0f;
				}
			}
		};
		//頂点座標を設定する関数
		auto setVerPos = [](
			DxLib::VERTEX3D* ver_,	//頂点3つを2組格納できるVERTEX3D[2][3]配列（1面分のデータ）
			const Vec3& lb_,		//設定する面の左下座標
			const Vec3& lt_,		//設定する面の左上座標
			const Vec3& rt_,		//設定する面の右上座標
			const Vec3& rb_			//設定する面の右下座標
			)
		{
			//面のポリゴン1
			ver_[0].pos = lb_;
			ver_[1].pos = lt_;
			ver_[2].pos = rt_;
			//面のポリゴン2
			ver_[3].pos = rt_;
			ver_[4].pos = rb_;
			ver_[5].pos = lb_;
		};

		Rect3 box(box_);		//ボックスを計算しやすい直方体型に変換
		VERTEX3D ver[6][2][3];	//左から「面の数」「面のポリゴン数」「ポリゴンの頂点数」
		enum Direction { Left, Right, Top, Bottom, Near, Far };	//面を指定するときに使用


		//左面設定
		initVer(&ver[Left][0][0], Vec3(-1, 0, 0), col_);
		setVerPos(
			&ver[Left][0][0],
			Vec3(box.l, box.b, box.f),
			Vec3(box.l, box.t, box.f),
			Vec3(box.l, box.t, box.n),
			Vec3(box.l, box.b, box.n)
		);

		//右面設定
		initVer(&ver[Right][0][0], Vec3(+1, 0, 0), col_);
		setVerPos(
			&ver[Right][0][0],
			Vec3(box.r, box.b, box.n),
			Vec3(box.r, box.t, box.n),
			Vec3(box.r, box.t, box.f),
			Vec3(box.r, box.b, box.f)
		);

		//上面設定
		initVer(&ver[Top][0][0], Vec3(0, +1, 0), col_);
		setVerPos(
			&ver[Top][0][0],
			Vec3(box.l, box.t, box.n),
			Vec3(box.l, box.t, box.f),
			Vec3(box.r, box.t, box.f),
			Vec3(box.r, box.t, box.n)
		);

		//下面設定
		initVer(&ver[Bottom][0][0], Vec3(0, -1, 0), col_);
		setVerPos(
			&ver[Bottom][0][0],
			Vec3(box.r, box.b, box.n),
			Vec3(box.r, box.b, box.f),
			Vec3(box.l, box.b, box.f),
			Vec3(box.l, box.b, box.n)
		);

		//手前面設定
		initVer(&ver[Near][0][0], Vec3(0, 0, -1), col_);
		setVerPos(
			&ver[Near][0][0],
			Vec3(box.l, box.b, box.n),
			Vec3(box.l, box.t, box.n),
			Vec3(box.r, box.t, box.n),
			Vec3(box.r, box.b, box.n)
		);

		//奥面設定
		initVer(&ver[Far][0][0], Vec3(0, 0, +1), col_);
		setVerPos(
			&ver[Far][0][0],
			Vec3(box.r, box.b, box.f),
			Vec3(box.r, box.t, box.f),
			Vec3(box.l, box.t, box.f),
			Vec3(box.l, box.b, box.f)
		);

		//描画
		DxLib::DrawPolygon3D(&ver[0][0][0], 12, DX_NONE_GRAPH, TRUE);
	}

	//DXライブラリ関数を使用してボックスを表示させる
	void DrawBox3D(const Obb3& box_, const Color& col_) {


		//VERTEXを初期化する関数
		auto initVer = [](
			DxLib::VERTEX3D* ver_,	//頂点3つを2組格納できるVERTEX3D[2][3]配列（1面分のデータ）
			const Vec3& norm_,		//設定する面の法線ベクトル
			const Color& col_		//色
			)
		{
			for (int i = 0; i < 2; ++i) {
				for (int j = 0; j < 3; ++j) {
					ver_[i * 3 + j].norm = norm_;
					ver_[i * 3 + j].dif = ML::GetColorU8(col_);
					ver_[i * 3 + j].spc = ML::GetColorU8(ML::Color(0, 0, 0, 0));
					ver_[i * 3 + j].u = 0.0f;
					ver_[i * 3 + j].v = 0.0f;
					ver_[i * 3 + j].su = 0.0f;
					ver_[i * 3 + j].sv = 0.0f;
				}
			}
		};
		//頂点座標を設定する関数
		auto setVerPos = [](
			DxLib::VERTEX3D* ver_,	//頂点3つを2組格納できるVERTEX3D[2][3]配列（1面分のデータ）
			const Vec3& lb_,		//設定する面の左下座標
			const Vec3& lt_,		//設定する面の左上座標
			const Vec3& rt_,		//設定する面の右上座標
			const Vec3& rb_			//設定する面の右下座標
			)
		{
			//面のポリゴン1
			ver_[0].pos = lb_;
			ver_[1].pos = lt_;
			ver_[2].pos = rt_;
			//面のポリゴン2
			ver_[3].pos = rt_;
			ver_[4].pos = rb_;
			ver_[5].pos = lb_;
		};

		//直方体の各点の座標を設定（ABCD-EFGH、下面の点Aを始点としてz+方向に向かって回転）
		Vec3 pos = box_.get_pos();

		Vec3 dirX = box_.get_dirX();	//方向ベクトル（X軸）
		Vec3 dirY = box_.get_dirY();	//方向ベクトル（Y軸）
		Vec3 dirZ = box_.get_dirZ();	//方向ベクトル（Z軸）
		
		Vec3 lenX = dirX * (box_.w / 2);		//中心から面までの長さ（X軸）
		Vec3 lenY = dirY * (box_.h / 2);		//中心から面までの長さ（Y軸）
		Vec3 lenZ = dirZ * (box_.d / 2);		//中心から面までの長さ（Z軸）

		//下面の4点
		Vec3 pA = pos - lenX - lenY - lenZ;
		Vec3 pB = pos - lenX - lenY + lenZ;
		Vec3 pC = pos + lenX - lenY + lenZ;
		Vec3 pD = pos + lenX - lenY - lenZ;
		//上面の4点
		Vec3 pE = pos - lenX + lenY - lenZ;
		Vec3 pF = pos - lenX + lenY + lenZ;
		Vec3 pG = pos + lenX + lenY + lenZ;
		Vec3 pH = pos + lenX + lenY - lenZ;

		VERTEX3D ver[6][2][3];	//左から「面の数」「面のポリゴン数」「ポリゴンの頂点数」
		enum Direction { Left, Right, Top, Bottom, Near, Far };	//面を指定するときに使用


		//左面設定
		initVer(&ver[Left][0][0], -dirX, col_);
		setVerPos(
			&ver[Left][0][0],
			pB,
			pF,
			pE,
			pA
		);

		//右面設定
		initVer(&ver[Right][0][0], dirX, col_);
		setVerPos(
			&ver[Right][0][0],
			pD,
			pH,
			pG,
			pC
		);

		//上面設定
		initVer(&ver[Top][0][0], dirY, col_);
		setVerPos(
			&ver[Top][0][0],
			pF,
			pE,
			pH,
			pG
		);

		//下面設定
		initVer(&ver[Bottom][0][0], -dirY, col_);
		setVerPos(
			&ver[Bottom][0][0],
			pA,
			pB,
			pC,
			pD
		);

		//手前面設定
		initVer(&ver[Near][0][0], -dirZ, col_);
		setVerPos(
			&ver[Near][0][0],
			pE,
			pA,
			pD,
			pH
		);

		//奥面設定
		initVer(&ver[Far][0][0], dirZ, col_);
		setVerPos(
			&ver[Far][0][0],
			pC,
			pG,
			pF,
			pB
		);

		//描画
		DxLib::DrawPolygon3D(&ver[0][0][0], 12, DX_NONE_GRAPH, TRUE);
	}
}