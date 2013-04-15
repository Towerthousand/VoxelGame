#ifndef _MATH_ALGEBRA3_DEFINED_
#define _MATH_ALGEBRA3_DEFINED_

#include <cmath>

// User must define MATH_ASSERT before including for enable asserts
// Like: #define MATH_ASSERT(expression) assert(expression)

#include <math.h>
#include <string.h>

namespace math {

template <typename T> struct vec2;
template <typename T> struct vec3;
template <typename T> struct vec4;

//-VEC2----------------------------------------------------------------------//
template <typename T>
struct vec2
{
	T x, y;

	vec2() : x(0), y(0) {}
	vec2(T _x, T _y) : x(_x), y(_y) {}
	vec2(T a) : x(a), y(a) {}
	template <typename U> vec2(const vec2<U> &v) : x(v.x), y(v.y) {}

	double module() const {
		return sqrt(moduleSqr());
	}

	T moduleSqr() const {
		return x*x + y*y;
	}

	vec2<T> normalized() {
		double dist = module();
		#ifndef MATH_ASSERT
		if (dist == 0.0) return *this;
		#else
		MATH_ASSERT(dist != 0.0);
		#endif
		return *this * (1.0/dist);
	}

	vec2<T> &normalize() {
		double dist = module();
		#ifndef MATH_ASSERT
		if (dist == 0.0) return *this;
		#else
		MATH_ASSERT(dist != 0.0);
		#endif
		*this *= (1.0/dist);
		return *this;
	}

	T &operator [] (int i) {
		return ((T*)this)[i];
	}

	const T &operator [] (int i) const {
		return ((T*)this)[i];
	}


	vec2<T> operator - () {
		return vec2<T>(-x, -y);
	}

	template <typename U> vec2<T> &operator += (const vec2<U> &other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	template <typename U> vec2<T> &operator -= (const vec2<U> &other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template <typename U>  vec2<T> &operator*=(U num) {
		x *= num;
		y *= num;
		return *this;
	}

	vec2<T> &operator/=(double num) {
		return *this *= (1.0/num);
	}

	vec2<T> &operator/=(float num) {
		return *this *= (1.0f/num);
	}

	template <typename U> vec2<T> &operator/=(U num) {
		x /= num;
		y /= num;
		return *this;
	}

	template <typename U> vec2<T> &operator=(const vec2<U> &other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}
};

typedef vec2<char>           vec2c;
typedef vec2<unsigned char>  vec2uc;
typedef vec2<short>          vec2s;
typedef vec2<unsigned short> vec2us;
typedef vec2<int>            vec2i;
typedef vec2<unsigned int>   vec2ui;
typedef vec2<long>           vec2l;
typedef vec2<unsigned long>  vec2ul;
typedef vec2<float>          vec2f;
typedef vec2<double>         vec2d;

template <typename T, typename U>
bool operator == (const vec2<T> &a, const vec2<U> &b) {
	return a.x == b.x && a.y == b.y;
}

template <typename T, typename U>
bool operator != (const vec2<T> &a, const vec2<U> &b) {
	return a.x != b.x || a.y != b.y;
}

template <typename T, typename U>
bool operator < (const vec2<T> &a, const vec2<U> &b) {
	if ( a.x == b.x) return a.y < b.y;
	return a.x < b.x;
}

template <typename T, typename U>
bool operator > (const vec2<T> &a, const vec2<U> &b) {
	if ( a.x == b.x) return a.y > b.y;
	return a.x > b.x;
}


//-VEC3----------------------------------------------------------------------//
template <typename T>
struct vec3
{
	T x, y, z;

	vec3() : x(0), y(0), z(0) {}
	vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	vec3(T a) : x(a), y(a), z(a) {}
	template <typename U> vec3(const vec2<U> &v) : x(v.x), y(v.y), z( 0 ) {}
	template <typename U> vec3(const vec3<U> &v) : x(v.x), y(v.y), z(v.z) {}


	double module() const {
		return sqrt(moduleSqr());
	}

	T moduleSqr() const {
		return x*x + y*y + z*z;
	}

	vec3<T> normalized() {
		double dist = module();
		#ifndef MATH_ASSERT
		if (dist == 0.0) return *this;
		#else
		MATH_ASSERT(dist != 0.0);
		#endif
		return *this * (1.0/dist);
	}

	vec3<T> &normalize() {
		double dist = module();
		#ifndef MATH_ASSERT
		if (dist == 0.0) return *this;
		#else
		MATH_ASSERT(dist != 0.0);
		#endif
		*this *= (1.0/dist);
		return *this;
	}

	T &operator [] (int i) {
		return ((T*)this)[i];
	}

	const T &operator [] (int i) const {
		return ((T*)this)[i];
	}


	vec3<T> operator - () {
		return vec3<T>(-x, -y, -z);
	}

	template <typename U> vec3<T> &operator += (const vec3<U> &other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	template <typename U> vec3<T> &operator -= (const vec3<U> &other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	template <typename U> vec3<T> &operator*=(U num) {
		x *= num;
		y *= num;
		z *= num;
		return *this;
	}

	vec3<T> &operator/=(double num) {
		return *this *= (1.0/num);
	}

	vec3<T> &operator/=(float num) {
		return *this *= (1.0f/num);
	}

	template <typename U> vec3<T> &operator/=(U num) {
		x /= num;
		y /= num;
		z /= num;
		return *this;
	}

	template <typename U> vec3<T> &operator=(const vec3<U> &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
};

typedef vec3<char>           vec3c;
typedef vec3<unsigned char>  vec3uc;
typedef vec3<short>          vec3s;
typedef vec3<unsigned short> vec3us;
typedef vec3<int>            vec3i;
typedef vec3<unsigned int>   vec3ui;
typedef vec3<long>           vec3l;
typedef vec3<unsigned long>  vec3ul;
typedef vec3<float>          vec3f;
typedef vec3<double>         vec3d;

template <typename T, typename U>
bool operator == (const vec3<T> &a, const vec3<U> &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

template <typename T, typename U>
bool operator != (const vec3<T> &a, const vec3<U> &b) {
	return a.x != b.x || a.y != b.y || a.z != b.z;
}

template <typename T, typename U>
bool operator < (const vec3<T> &a, const vec3<U> &b) {
	if ( a.x == b.x) {
		if( a.y == b.y) return a.z < b.z;
		return a.y < b.y;
	}
	return a.x < b.x;
}

template <typename T, typename U>
bool operator > (const vec3<T> &a, const vec3<T> &b) {
	if ( a.x == b.x)
	{
		if( a.y == b.y) return a.z > b.z;
		return a.y > b.y;
	}
	return a.x > b.x;
}

//-VEC4----------------------------------------------------------------------//
template <typename T>
struct vec4
{
	T x, y, z, w;

	vec4() : x(0), y(0), z(0), w(0) {}
	vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
	vec4(T a) : x(a), y(a), z(a) {}
	template <typename U> vec4(const vec2<U> &v) : x(v.x), y(v.y), z( 0 ), w( 0 ) {}
	template <typename U> vec4(const vec3<U> &v) : x(v.x), y(v.y), z(v.z), w( 0 ) {}
	template <typename U> vec4(const vec4<U> &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	double module() const {
		return sqrt(moduleSqr());
	}

	T moduleSqr() const {
		return x*x + y*y + z*z + w*w;
	}

	vec4<T> normalized() {
		double dist = module();
		#ifndef MATH_ASSERT
		if (dist == 0.0) return *this;
		#else
		MATH_ASSERT(dist != 0.0);
		#endif
		return *this / dist;
	}

	vec4<T> &normalize() {
		double dist = module();
		#ifndef MATH_ASSERT
		if (dist == 0.0) return *this;
		#else
		MATH_ASSERT(dist != 0.0);
		#endif
		return *this /= dist;
	}

	T &operator [] (int i) {
		return ((T*)this)[i];
	}

	const T &operator [] (int i) const {
		return ((T*)this)[i];
	}


	vec4<T> operator - () {
		return vec4<T>(-x, -y, -z, -w);
	}

	template <typename U> vec4<T> &operator += (const vec4<U> &other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	template <typename U> vec4<T> &operator -= (const vec4<U> &other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	template <typename U> vec4<T> &operator*=(U num) {
		x *= num;
		y *= num;
		z *= num;
		w *= num;
		return *this;
	}

	vec4<T> &operator/=(double num) {
		return *this *= (1.0/num);
	}

	vec4<T> &operator/=(float num) {
		return *this *= (1.0f/num);
	}

	template <typename U> vec4<T> &operator/=(U num) {
		x /= num;
		y /= num;
		z /= num;
		w /= num;
		return *this;
	}

	template <typename U> vec4<T> &operator=(const vec4<U> &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}
};

typedef vec4<char>           vec4c;
typedef vec4<unsigned char>  vec4uc;
typedef vec4<short>          vec4s;
typedef vec4<unsigned short> vec4us;
typedef vec4<int>            vec4i;
typedef vec4<unsigned int>   vec4ui;
typedef vec4<long>           vec4l;
typedef vec4<unsigned long>  vec4ul;
typedef vec4<float>          vec4f;
typedef vec4<double>         vec4d;


template <typename T, typename U>
bool operator == (const vec4<T> &a, const vec4<U> &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

template <typename T, typename U>
bool operator != (const vec4<T> &a, const vec4<U> &b) {
	return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

template <typename T, typename U>
bool operator < (const vec4<T> &a, const vec4<U> &b) {
	if ( a.x == b.x) {
		if( a.y == b.y) {
			if (a.z == b.z) return a.w < b.w;
			return a.z < b.z;
		}
		return a.y < b.y;
	}
	return a.x < b.x;
}

template <typename T, typename U>
bool operator > (const vec4<T> &a, const vec4<T> &b) {
	if ( a.x == b.x) {
		if( a.y == b.y) {
			if (a.z == b.z) return a.w > b.w;
			return a.z > b.z;
		}
		return a.y > b.y;
	}
	return a.x > b.x;
}

//-MAT3----------------------------------------------------------------------//
template <typename T>
struct mat3
{
	T v[9]; // column-major

	mat3() {}
	mat3(const T val[16]) {*this = val;}
	mat3(const mat3 &mat) {*this = mat;}
	mat3(T a00, T a01, T a02,
	     T a10, T a11, T a12,
	     T a20, T a21, T a22) //row-major constructor
	{
		v[0] = a00; v[1] = a10; v[2] = a20;
		v[3] = a01; v[4] = a11; v[5] = a21;
		v[6] = a02; v[7] = a12; v[8] = a22;
	}

	T &operator()(unsigned int x, unsigned int y) {
		return v[3*x + y];
	}

	T *operator[](unsigned int i) {
		return &v[3*i];
	}

	const T *operator[](unsigned int i) const {
		return &v[3*i];
	}

	mat3<T> &operator=(const mat3<T> &m) {
		return *this = m.v;
	}

	mat3<T> &operator=(const T val[16]) {
		memcpy(this->v, val, 16 * sizeof(T));
		return *this;
	}

	template <typename U> mat3<T> &operator=(const U val[16]) {
		v[0] = val[0]; v[1] = val[1]; v[2] = val[2];
		v[3] = val[3]; v[4] = val[4]; v[5] = val[5];
		v[6] = val[6]; v[7] = val[7]; v[8] = val[8];
		return *this;
	}

	mat3<T> operator - ()
	{
		mat3<T> res;
		res.v[0] = -v[0]; res.v[1] = -v[1]; res.v[2] = -v[2];
		res.v[3] = -v[3]; res.v[4] = -v[4]; res.v[5] = -v[5];
		res.v[6] = -v[6]; res.v[7] = -v[7]; res.v[8] = -v[8];
		return res;
	}

	template <typename U> mat3<T> &operator*=(const mat3<U> &mat) {
		return *this = *this * mat;
	}

	mat3<T> &identity() {
		v[0] = 1; v[1] = 0; v[2] = 0;
		v[3] = 0; v[4] = 1; v[5] = 0;
		v[6] = 0; v[7] = 0; v[8] = 1;
		return *this;
	}

	static mat3<T> fromIdentity() {
		return mat3<T>().identity();
	}

	mat3<T> transposed() {
		mat3<T> mat;

		for(int i = 0; i < 3; i++)
		{
			mat[0][i] = (*this)[i][0];
			mat[1][i] = (*this)[i][1];
			mat[2][i] = (*this)[i][2];
		}

		return mat;
	}

	void transpose(void) {
		*this = this->transposed();
	}
};

typedef mat3<float>   mat3f;
typedef mat3<double>  mat3d;

//-MAT4----------------------------------------------------------------------//
template <typename T>
struct mat4
{
	T v[16]; // column-major

	mat4() {}
	mat4(const T val[16]) {*this = val;}
	mat4(const mat4 &mat) {*this = mat;}
	mat4(T a00, T a01, T a02, T a03,
		 T a10, T a11, T a12, T a13,
		 T a20, T a21, T a22, T a23,
		 T a30, T a31, T a32, T a33 ) //row-major constructor
	{
		v[0] = a00; v[1] = a10; v[2] = a20; v[3] = a30;
		v[4] = a01; v[5] = a11; v[6] = a21; v[7] = a31;
		v[8] = a02; v[9] = a12; v[10]= a22; v[11]= a32;
		v[12]= a03; v[13]= a13; v[14]= a23; v[15]= a33;
	}

	T &operator()(unsigned int x, unsigned int y) {
		return v[4*x + y];
	}

	T *operator[](unsigned int i) {
		return &v[4*i];
	}

	const T *operator[](unsigned int i) const {
		return &v[4*i];
	}

	mat4<T> &operator=(const mat4<T> &m) {
		return *this = m.v;
	}

	mat4<T> &operator=(const T val[16]) {
		memcpy(this->v, val, 16 * sizeof(T));
		return *this;
	}

	template <typename U> mat4<T> &operator=(const U val[16]) {
		v[0] = val[0]; v[1] = val[1]; v[2] = val[2]; v[3] = val[3];
		v[4] = val[4]; v[5] = val[5]; v[6] = val[6]; v[7] = val[7];
		v[8] = val[8]; v[9] = val[9]; v[10]= val[10];v[11]= val[11];
		v[12]= val[12];v[13]= val[13];v[14]= val[14];v[15]= val[15];
		return *this;
	}

	mat4<T> operator - ()
	{
		mat4<T> res;
		res.v[0] = -v[0]; res.v[1] = -v[1]; res.v[2] = -v[2]; res.v[3] = -v[3];
		res.v[4] = -v[4]; res.v[5] = -v[5]; res.v[6] = -v[6]; res.v[7] = -v[7];
		res.v[8] = -v[8]; res.v[9] = -v[9]; res.v[10]= -v[10];res.v[11]= -v[11];
		res.v[12]= -v[12];res.v[13]= -v[13];res.v[14]= -v[14];res.v[15]= -v[15];
		return res;
	}

	mat4<T> &operator*=(const mat4<T> &mat) {
		return *this = *this * mat;
	}

	mat4<T> &identity() {
		v[0] = 1; v[1] = 0; v[2] = 0; v[3] = 0;
		v[4] = 0; v[5] = 1; v[6] = 0; v[7] = 0;
		v[8] = 0; v[9] = 0; v[10]= 1; v[11]= 0;
		v[12]= 0; v[13]= 0; v[14]= 0; v[15]= 1;
		return *this;
	}

	static mat4<T> fromIdentity() {
		return mat4<T>().identity();
	}

	static mat4<T> fromRotateRad(T angle, const vec3<T> &axis) {
		double c = cos(angle);
		double s = sin(angle);
		double t = 1.0f - c;

		vec3<T> a = axis.normalized();
		return mat4(
		    t * a.x * a.x + c, t * a.x * a.y - s * a.z, t * a.x * a.z + s * a.y, 0,
		    t * a.x * a.y + s * a.z, t * a.y * a.y + c, t * a.y * a.z - s * a.x, 0,
		    t * a.x * a.z - s * a.y, t * a.y * a.z + s * a.x, t * a.z * a.z + c, 0,
		    0, 0, 0, 1);
	}

	static mat4<T> fromRotateRad(T angle, T x, T y, T z) {
		return fromRotateRad(angle, vec3<T>(x,y,z));
	}

	static mat4<T> fromRotate(T angle, const vec3<T> &axis) {
		return fromRotateRad(angle * (M_PI/180), axis);
	}

	static mat4<T> fromRotate(T angle, T x, T y, T z) {
		return fromRotateRad(angle * (M_PI/180), vec3<T>(x,y,z));
	}

	static mat4<T> fromTranslate(T x, T y, T z) {
		return mat4<T>(
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1 );
	}

	static mat4<T> fromTranslate(const vec3<T> &v) {
		return fromTranslate(v.x, v.y, v.z);
	}

	static mat4<T> fromScale(T x, T y, T z) {
		return mat4<T>(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1 );
	}

	static mat4<T> fromScale(const vec3<T> &v) {
		return fromScale(v.x, v.y, v.z);
	}

	static mat4<T> fromFrustrum(T l, T r, T b, T t, T n, T f) {
		return mat4<T>(
		(2 * n)/(r - l),               0,      (r + l) / (r - l),                    0,
			      0, (2 * n)/(t - b),      (t + b) / (t - b),                    0,
			      0,               0, -1 * (f + n) / (f - n), (-2 * f * n)/(f - n),
			      0,               0,                     -1,                    0);
	}

	static mat4<T> fromOrtho(T l, T r, T b, T t, T n, T f) {
		return mat4<T>(
		2/(r - l),         0,         0, (r + l)/(l - r),
			0, 2/(t - b),         0, (t + b)/(b - t),
			0,         0, 2/(n - f), (f + n)/(n - f),
			0,         0,         0,               1);
	}

	mat4<T> &rotate(T angle, T x, T y, T z) {
		return *this *= fromRotate(angle, x, y, z);
	}

	mat4<T> &rotate(T angle, const vec3<T> &axis) {
		return *this *= fromRotate(angle, axis);
	}

	mat4<T> &rotateRad(T angle, T x, T y, T z) {
		return *this *= fromRotateRad(angle, x, y, z);
	}

	mat4<T> &rotateRad(T angle, const vec3<T> &axis) {
		return *this *= fromRotateRad(angle, axis);
	}

	mat4<T> &translate(T x, T y, T z) {
		return *this *= fromTranslate(x, y, z);
	}

	mat4<T> &translate(const vec3<T> &vec) {
		return *this *= fromTranslate(vec);
	}

	mat4<T> &scale(T x, T y, T z) {
		return *this *= fromScale(x, y, z);
	}

	mat4<T> &scale(const vec3<T> &vec) {
		return *this *= fromScale(vec);
	}

	T det() {
		return
			((*this)[0][3]*(*this)[1][2]*(*this)[2][1]*(*this)[3][0]) -
			((*this)[0][2]*(*this)[1][3]*(*this)[2][1]*(*this)[3][0]) -
			((*this)[0][3]*(*this)[1][1]*(*this)[2][2]*(*this)[3][0]) +
			((*this)[0][1]*(*this)[1][3]*(*this)[2][2]*(*this)[3][0]) +
			((*this)[0][2]*(*this)[1][1]*(*this)[2][3]*(*this)[3][0]) -
			((*this)[0][1]*(*this)[1][2]*(*this)[2][3]*(*this)[3][0]) -
			((*this)[0][3]*(*this)[1][2]*(*this)[2][0]*(*this)[3][1]) +
			((*this)[0][2]*(*this)[1][3]*(*this)[2][0]*(*this)[3][1]) +
			((*this)[0][3]*(*this)[1][0]*(*this)[2][2]*(*this)[3][1]) -
			((*this)[0][0]*(*this)[1][3]*(*this)[2][2]*(*this)[3][1]) -
			((*this)[0][2]*(*this)[1][0]*(*this)[2][3]*(*this)[3][1]) +
			((*this)[0][0]*(*this)[1][2]*(*this)[2][3]*(*this)[3][1]) +
			((*this)[0][3]*(*this)[1][1]*(*this)[2][0]*(*this)[3][2]) -
			((*this)[0][1]*(*this)[1][3]*(*this)[2][0]*(*this)[3][2]) -
			((*this)[0][3]*(*this)[1][0]*(*this)[2][1]*(*this)[3][2]) +
			((*this)[0][0]*(*this)[1][3]*(*this)[2][1]*(*this)[3][2]) +
			((*this)[0][1]*(*this)[1][0]*(*this)[2][3]*(*this)[3][2]) -
			((*this)[0][0]*(*this)[1][1]*(*this)[2][3]*(*this)[3][2]) -
			((*this)[0][2]*(*this)[1][1]*(*this)[2][0]*(*this)[3][3]) +
			((*this)[0][1]*(*this)[1][2]*(*this)[2][0]*(*this)[3][3]) +
			((*this)[0][2]*(*this)[1][0]*(*this)[2][1]*(*this)[3][3]) -
			((*this)[0][0]*(*this)[1][2]*(*this)[2][1]*(*this)[3][3]) -
			((*this)[0][1]*(*this)[1][0]*(*this)[2][2]*(*this)[3][3]) +
			((*this)[0][0]*(*this)[1][1]*(*this)[2][2]*(*this)[3][3]);
	}

	mat4<T> inverted(bool * invertible = 0) {
		mat4<T> res;
		T determinant = this->det();

		if(determinant == 0) { // Singular matrix
			if (invertible) *invertible = false;
			else
			{
				#ifdef MATH_ASSERT
				MATH_ASSERT(determinant == 0);
				#endif
			}

			return fromIdentity();
		}

		if (invertible) *invertible = true;
		T invDet = 1/determinant;

		res[0][0] = invDet *  ((-(*this)[1][3]*(*this)[2][2]*(*this)[3][1]) +
					((*this)[1][2]*(*this)[2][3]*(*this)[3][1]) +
					((*this)[1][3]*(*this)[2][1]*(*this)[3][2]) -
					((*this)[1][1]*(*this)[2][3]*(*this)[3][2]) -
					((*this)[1][2]*(*this)[2][1]*(*this)[3][3]) +
					((*this)[1][1]*(*this)[2][2]*(*this)[3][3]));
		res[0][1] = invDet *   (((*this)[0][3]*(*this)[2][2]*(*this)[3][1]) -
					((*this)[0][2]*(*this)[2][3]*(*this)[3][1]) -
					((*this)[0][3]*(*this)[2][1]*(*this)[3][2]) +
					((*this)[0][1]*(*this)[2][3]*(*this)[3][2]) +
					((*this)[0][2]*(*this)[2][1]*(*this)[3][3]) -
					((*this)[0][1]*(*this)[2][2]*(*this)[3][3]));
		res[0][2] = invDet *  ((-(*this)[0][3]*(*this)[1][2]*(*this)[3][1]) +
					((*this)[0][2]*(*this)[1][3]*(*this)[3][1]) +
					((*this)[0][3]*(*this)[1][1]*(*this)[3][2]) -
					((*this)[0][1]*(*this)[1][3]*(*this)[3][2]) -
					((*this)[0][2]*(*this)[1][1]*(*this)[3][3]) +
					((*this)[0][1]*(*this)[1][2]*(*this)[3][3]));
		res[0][3] = invDet *   (((*this)[0][3]*(*this)[1][2]*(*this)[2][1]) -
					((*this)[0][2]*(*this)[1][3]*(*this)[2][1]) -
					((*this)[0][3]*(*this)[1][1]*(*this)[2][2]) +
					((*this)[0][1]*(*this)[1][3]*(*this)[2][2]) +
					((*this)[0][2]*(*this)[1][1]*(*this)[2][3]) -
					((*this)[0][1]*(*this)[1][2]*(*this)[2][3]));

		res[1][0] = invDet *   (((*this)[1][3]*(*this)[2][2]*(*this)[3][0]) -
					((*this)[1][2]*(*this)[2][3]*(*this)[3][0]) -
					((*this)[1][3]*(*this)[2][0]*(*this)[3][2]) +
					((*this)[1][0]*(*this)[2][3]*(*this)[3][2]) +
					((*this)[1][2]*(*this)[2][0]*(*this)[3][3]) -
					((*this)[1][0]*(*this)[2][2]*(*this)[3][3]));
		res[1][1] = invDet *  ((-(*this)[0][3]*(*this)[2][2]*(*this)[3][0]) +
					((*this)[0][2]*(*this)[2][3]*(*this)[3][0]) +
					((*this)[0][3]*(*this)[2][0]*(*this)[3][2]) -
					((*this)[0][0]*(*this)[2][3]*(*this)[3][2]) -
					((*this)[0][2]*(*this)[2][0]*(*this)[3][3]) +
					((*this)[0][0]*(*this)[2][2]*(*this)[3][3]));
		res[1][2] = invDet *   (((*this)[0][3]*(*this)[1][2]*(*this)[3][0]) -
					((*this)[0][2]*(*this)[1][3]*(*this)[3][0]) -
					((*this)[0][3]*(*this)[1][0]*(*this)[3][2]) +
					((*this)[0][0]*(*this)[1][3]*(*this)[3][2]) +
					((*this)[0][2]*(*this)[1][0]*(*this)[3][3]) -
					((*this)[0][0]*(*this)[1][2]*(*this)[3][3]));
		res[1][3] = invDet *  ((-(*this)[0][3]*(*this)[1][2]*(*this)[2][0]) +
					((*this)[0][2]*(*this)[1][3]*(*this)[2][0]) +
					((*this)[0][3]*(*this)[1][0]*(*this)[2][2]) -
					((*this)[0][0]*(*this)[1][3]*(*this)[2][2]) -
					((*this)[0][2]*(*this)[1][0]*(*this)[2][3]) +
					((*this)[0][0]*(*this)[1][2]*(*this)[2][3]));

		res[2][0] = invDet *  ((-(*this)[1][3]*(*this)[2][1]*(*this)[3][0]) +
					((*this)[1][1]*(*this)[2][3]*(*this)[3][0]) +
					((*this)[1][3]*(*this)[2][0]*(*this)[3][1]) -
					((*this)[1][0]*(*this)[2][3]*(*this)[3][1]) -
					((*this)[1][1]*(*this)[2][0]*(*this)[3][3]) +
					((*this)[1][0]*(*this)[2][1]*(*this)[3][3]));
		res[2][1] = invDet *   (((*this)[0][3]*(*this)[2][1]*(*this)[3][0]) -
					((*this)[0][1]*(*this)[2][3]*(*this)[3][0]) -
					((*this)[0][3]*(*this)[2][0]*(*this)[3][1]) +
					((*this)[0][0]*(*this)[2][3]*(*this)[3][1]) +
					((*this)[0][1]*(*this)[2][0]*(*this)[3][3]) -
					((*this)[0][0]*(*this)[2][1]*(*this)[3][3]));
		res[2][2] = invDet *  ((-(*this)[0][3]*(*this)[1][1]*(*this)[3][0]) +
					((*this)[0][1]*(*this)[1][3]*(*this)[3][0]) +
					((*this)[0][3]*(*this)[1][0]*(*this)[3][1]) -
					((*this)[0][0]*(*this)[1][3]*(*this)[3][1]) -
					((*this)[0][1]*(*this)[1][0]*(*this)[3][3]) +
					((*this)[0][0]*(*this)[1][1]*(*this)[3][3]));
		res[2][3] = invDet *   (((*this)[0][3]*(*this)[1][1]*(*this)[2][0]) -
					((*this)[0][1]*(*this)[1][3]*(*this)[2][0]) -
					((*this)[0][3]*(*this)[1][0]*(*this)[2][1]) +
					((*this)[0][0]*(*this)[1][3]*(*this)[2][1]) +
					((*this)[0][1]*(*this)[1][0]*(*this)[2][3]) -
					((*this)[0][0]*(*this)[1][1]*(*this)[2][3]));

		res[3][0] = invDet *   (((*this)[1][2]*(*this)[2][1]*(*this)[3][0]) -
					((*this)[1][1]*(*this)[2][2]*(*this)[3][0]) -
					((*this)[1][2]*(*this)[2][0]*(*this)[3][1]) +
					((*this)[1][0]*(*this)[2][2]*(*this)[3][1]) +
					((*this)[1][1]*(*this)[2][0]*(*this)[3][2]) -
					((*this)[1][0]*(*this)[2][1]*(*this)[3][2]));
		res[3][1] = invDet *  ((-(*this)[0][2]*(*this)[2][1]*(*this)[3][0]) +
					((*this)[0][1]*(*this)[2][2]*(*this)[3][0]) +
					((*this)[0][2]*(*this)[2][0]*(*this)[3][1]) -
					((*this)[0][0]*(*this)[2][2]*(*this)[3][1]) -
					((*this)[0][1]*(*this)[2][0]*(*this)[3][2]) +
					((*this)[0][0]*(*this)[2][1]*(*this)[3][2]));
		res[3][2] = invDet *   (((*this)[0][2]*(*this)[1][1]*(*this)[3][0]) -
					((*this)[0][1]*(*this)[1][2]*(*this)[3][0]) -
					((*this)[0][2]*(*this)[1][0]*(*this)[3][1]) +
					((*this)[0][0]*(*this)[1][2]*(*this)[3][1]) +
					((*this)[0][1]*(*this)[1][0]*(*this)[3][2]) -
					((*this)[0][0]*(*this)[1][1]*(*this)[3][2]));
		res[3][3] = invDet *  ((-(*this)[0][2]*(*this)[1][1]*(*this)[2][0]) +
					((*this)[0][1]*(*this)[1][2]*(*this)[2][0]) +
					((*this)[0][2]*(*this)[1][0]*(*this)[2][1]) -
					((*this)[0][0]*(*this)[1][2]*(*this)[2][1]) -
					((*this)[0][1]*(*this)[1][0]*(*this)[2][2]) +
					((*this)[0][0]*(*this)[1][1]*(*this)[2][2]));
		return res;
	}

	bool inverse() {
		bool invertible;
		mat4<T> inv = this->inverted(&invertible);
		if (invertible) *this = inv;
		return invertible;
	}

	mat4<T> transposed() {
		mat4<T> mat;

		for(int i = 0; i < 4; i++)
		{
			mat[0][i] = (*this)[i][0];
			mat[1][i] = (*this)[i][1];
			mat[2][i] = (*this)[i][2];
			mat[3][i] = (*this)[i][3];
		}

		return mat;
	}

	void transpose(void) {
		*this = this->transposed();
	}
};

typedef mat4<float>   mat4f;
typedef mat4<double>  mat4d;

//-VEC2----------------------------------------------------------------------//

template <typename T, typename U>
vec2<T> operator + (const vec2<T> &a, const vec2<U> &b) {
	return vec2<T>(a.x + b.x, a.y + b.y);
}

template <typename T, typename U>
vec2<T> operator - (const vec2<T> &a, const vec2<U> &b) {
	return vec2<T>(a.x - b.x, a.y - b.y);
}

template <typename T, typename U>
vec2<T> operator * (const vec2<T> &a, U num) {
	return vec2<T>(a.x*num, a.y*num);
}

template <typename T, typename U>
vec2<T> operator * (U num, const vec2<T> &a) {
	return a*num;
}

template <typename T, typename U>
vec3<T> operator * (const mat3<T> &a, const vec3<U> &v)
{
    vec3<T> res;

    res.x = a.v[0]*v.x + a.v[3]*v.y + a.v[6]*v.z;
    res.y = a.v[1]*v.x + a.v[4]*v.y + a.v[7]*v.z;
    res.z = a.v[2]*v.x + a.v[5]*v.y + a.v[8]*v.z;

    return res;
}

template <typename T, typename U>
vec3<T> operator * (const vec3<T> &v, const mat3<U> &a)
{
    return a.transpose() * v;
}

//cross product
template <typename T>
vec3<T> operator ^ (const vec2<T> &a, const vec2<T> &b) {
	return vec3<T>( 0, 0, a.x*b.y - a.y*b.x);
}

//float specialization
template <typename T>
vec2<T> operator / (const vec2<T> &a, float num) {
	return a*(1.0f/num);
}

//double specialization
template <typename T>
vec2<T> operator / (const vec2<T> &a, double num) {
	return a*(1.0/num);
}

//general case
template <typename T, typename U>
vec2<T> operator / (const vec2<T> &a, U num) {
	return vec2<T>(a.x/num, a.y/num);
}

//-VEC3----------------------------------------------------------------------//

template <typename T, typename U>
vec3<T> operator + (const vec3<T> &a, const vec3<U> &b) {
	return vec3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename T, typename U>
vec3<T> operator - (const vec3<T> &a, const vec3<U> &b) {
	return vec3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <typename T, typename U>
vec3<T> operator * (const vec3<T> &a, U num) {
	return vec3<T>(a.x*num, a.y*num, a.z*num);
}

template <typename T, typename U>
vec3<T> operator * (U num, const vec3<T> &a) {
	return a*num;
}

//dot product
template <typename T>
T operator * (const vec3<T> &a, const vec3<T> &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//cross product
template <typename T>
vec3<T> operator ^ (const vec3<T> &a, const vec3<T> &b) {
	return vec3<T>(
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x);
}

//float specialization
template <typename T>
vec3<T> operator / (const vec3<T> &a, float num) {
	return a*(1.0f/num);
}

//double specialization
template <typename T>
vec3<T> operator / (const vec3<T> &a, double num) {
	return a*(1.0/num);
}

//general case
template <typename T, typename U>
vec3<T> operator / (const vec3<T> &a, U num) {
	return vec3<T>(a.x/num, a.y/num, a.z/num);
}

/* TODO
vec3<T>  operator *  (const mat4 &a, const vec3<T> &v);  // M . v
vec3<T>  operator *  (const vec3<T> &v, const mat4 &a);  // v . M
*/

//-VEC4----------------------------------------------------------------------//

template <typename T, typename U>
vec4<T> operator + (const vec4<T> &a, const vec4<U> &b) {
	return vec4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <typename T, typename U>
vec4<T> operator - (const vec4<T> &a, const vec4<U> &b) {
	return vec4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template <typename T, typename U>
vec4<T> operator * (const vec4<T> &a, U num) {
	return vec4<T>(a.x*num, a.y*num, a.z*num, a.w*num);
}

template <typename T, typename U>
vec4<T> operator * (U num, const vec4<T> &a) {
	return a*num;
}

template <typename T, typename U>
vec4<T> operator*(const mat4<T> &a, const vec4<U> &v) {

	vec4<T> res;

	res.x = a.v[0] * v.x + a.v[4] * v.y + a.v[8] * v.z + a.v[12] * v.w;
	res.y = a.v[1] * v.x + a.v[5] * v.y + a.v[9] * v.z + a.v[13] * v.w;
	res.z = a.v[2] * v.x + a.v[6] * v.y + a.v[10]* v.z + a.v[14] * v.w;
	res.w = a.v[3] * v.x + a.v[7] * v.y + a.v[11]* v.z + a.v[15] * v.w;

	return res;
}

template <typename T, typename U>
vec4<T> operator*(const vec4<T> &v, const mat4<U> &a) {
    return a.transpose()*v;
}

//dot product
template <typename T>
T operator * (const vec4<T> &a, const vec4<T> &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

//float specialization
template <typename T>
vec4<T> operator / (const vec4<T> &a, float num) {
	return a*(1.0f/num);
}

//double specialization
template <typename T>
vec4<T> operator / (const vec4<T> &a, double num) {
	return a*(1.0/num);
}

//general case
template <typename T, typename U>
vec4<T> operator / (const vec4<T> &a, U num) {
	return vec4<T>(a.x/num, a.y/num, a.z/num);
}

//-MAT4----------------------------------------------------------------------//

template <typename T, typename U>
mat4<T> operator * (const mat4<T> &a, const mat4<U> &b)
{
	mat4<T> res;

	res(0,0) = a(0,0)*b(0,0) + a(1,0)*b(0,1) + a(2,0)*b(0,2) + a(3,0)*b(0,3);
	res(0,1) = a(0,1)*b(0,0) + a(1,1)*b(0,1) + a(2,1)*b(0,2) + a(3,1)*b(0,3);
	res(0,2) = a(0,2)*b(0,0) + a(1,2)*b(0,1) + a(2,2)*b(0,2) + a(3,2)*b(0,3);
	res(0,3) = a(0,3)*b(0,0) + a(1,3)*b(0,1) + a(2,3)*b(0,2) + a(3,3)*b(0,3);
	res(1,0) = a(0,0)*b(1,0) + a(1,0)*b(1,1) + a(2,0)*b(1,2) + a(3,0)*b(1,3);
	res(1,1) = a(0,1)*b(1,0) + a(1,1)*b(1,1) + a(2,1)*b(1,2) + a(3,1)*b(1,3);
	res(1,2) = a(0,2)*b(1,0) + a(1,2)*b(1,1) + a(2,2)*b(1,2) + a(3,2)*b(1,3);
	res(1,3) = a(0,3)*b(1,0) + a(1,3)*b(1,1) + a(2,3)*b(1,2) + a(3,3)*b(1,3);
	res(2,0) = a(0,0)*b(2,0) + a(1,0)*b(2,1) + a(2,0)*b(2,2) + a(3,0)*b(2,3);
	res(2,1) = a(0,1)*b(2,0) + a(1,1)*b(2,1) + a(2,1)*b(2,2) + a(3,1)*b(2,3);
	res(2,2) = a(0,2)*b(2,0) + a(1,2)*b(2,1) + a(2,2)*b(2,2) + a(3,2)*b(2,3);
	res(2,3) = a(0,3)*b(2,0) + a(1,3)*b(2,1) + a(2,3)*b(2,2) + a(3,3)*b(2,3);
	res(3,0) = a(0,0)*b(3,0) + a(1,0)*b(3,1) + a(2,0)*b(3,2) + a(3,0)*b(3,3);
	res(3,1) = a(0,1)*b(3,0) + a(1,1)*b(3,1) + a(2,1)*b(3,2) + a(3,1)*b(3,3);
	res(3,2) = a(0,2)*b(3,0) + a(1,2)*b(3,1) + a(2,2)*b(3,2) + a(3,2)*b(3,3);
	res(3,3) = a(0,3)*b(3,0) + a(1,3)*b(3,1) + a(2,3)*b(3,2) + a(3,3)*b(3,3);

	return res;
}

template <typename T, typename U>
mat4<T> operator + (const mat4<T> &a, const mat4<U> &b)
{
	mat4<T> res;

	res.v[0] = a.v[0] + b.v[0];
	res.v[1] = a.v[1] + b.v[1];
	res.v[2] = a.v[2] + b.v[2];
	res.v[3] = a.v[3] + b.v[3];
	res.v[4] = a.v[4] + b.v[4];
	res.v[5] = a.v[5] + b.v[5];
	res.v[6] = a.v[6] + b.v[6];
	res.v[7] = a.v[7] + b.v[7];
	res.v[8] = a.v[8] + b.v[8];
	res.v[9] = a.v[9] + b.v[9];
	res.v[10]= a.v[10]+ b.v[10];
	res.v[11]= a.v[11]+ b.v[11];
	res.v[12]= a.v[12]+ b.v[12];
	res.v[13]= a.v[13]+ b.v[13];
	res.v[14]= a.v[14]+ b.v[14];
	res.v[15]= a.v[15]+ b.v[15];

	return res;
}

template <typename T, typename U>
mat4<T> operator - (const mat4<T> &a, const mat4<U> &b)
{
	mat4<T> res;

	res.v[0] = a.v[0] - b.v[0];
	res.v[1] = a.v[1] - b.v[1];
	res.v[2] = a.v[2] - b.v[2];
	res.v[3] = a.v[3] - b.v[3];
	res.v[4] = a.v[4] - b.v[4];
	res.v[5] = a.v[5] - b.v[5];
	res.v[6] = a.v[6] - b.v[6];
	res.v[7] = a.v[7] - b.v[7];
	res.v[8] = a.v[8] - b.v[8];
	res.v[9] = a.v[9] - b.v[9];
	res.v[10]= a.v[10]- b.v[10];
	res.v[11]= a.v[11]- b.v[11];
	res.v[12]= a.v[12]- b.v[12];
	res.v[13]= a.v[13]- b.v[13];
	res.v[14]= a.v[14]- b.v[14];
	res.v[15]= a.v[15]- b.v[15];

	return res;
}

template <typename T, typename U>
mat4<T> operator * (const mat4<T> &a, U num)
{
	mat4<T> res;

	res.v[0] = a.v[0] * num;
	res.v[1] = a.v[1] * num;
	res.v[2] = a.v[2] * num;
	res.v[3] = a.v[3] * num;
	res.v[4] = a.v[4] * num;
	res.v[5] = a.v[5] * num;
	res.v[6] = a.v[6] * num;
	res.v[7] = a.v[7] * num;
	res.v[8] = a.v[8] * num;
	res.v[9] = a.v[9] * num;
	res.v[10]= a.v[10]* num;
	res.v[11]= a.v[11]* num;
	res.v[12]= a.v[12]* num;
	res.v[13]= a.v[13]* num;
	res.v[14]= a.v[14]* num;
	res.v[15]= a.v[15]* num;

	return res;
}

template <typename T, typename U>
mat4<T> operator * (U num, const mat4<T> &a) {
	return a*num;
}

template <typename T>
mat4<T> operator / (const mat4<T> &a, float num) {
	return a*(1.0f/num);

}

template <typename T>
mat4<T> operator / (const mat4<T> &a, double num) {
	return a*(1.0/num);
}

template <typename T, typename U>
mat4<T> operator / (const mat4<T> &a, U num)
{
	mat4<T> res;

	res.v[0] = a.v[0] / num;
	res.v[1] = a.v[1] / num;
	res.v[2] = a.v[2] / num;
	res.v[3] = a.v[3] / num;
	res.v[4] = a.v[4] / num;
	res.v[5] = a.v[5] / num;
	res.v[6] = a.v[6] / num;
	res.v[7] = a.v[7] / num;
	res.v[8] = a.v[8] / num;
	res.v[9] = a.v[9] / num;
	res.v[10]= a.v[10]/ num;
	res.v[11]= a.v[11]/ num;
	res.v[12]= a.v[12]/ num;
	res.v[13]= a.v[13]/ num;
	res.v[14]= a.v[14]/ num;
	res.v[15]= a.v[15]/ num;

	return res;
}

template <typename T, typename U>
bool operator == (const mat4<T> &a, const mat4<U> &b) {
	return
	a.v[0] == b.v[0] &&
	a.v[1] == b.v[1] &&
	a.v[2] == b.v[2] &&
	a.v[3] == b.v[3] &&
	a.v[4] == b.v[4] &&
	a.v[5] == b.v[5] &&
	a.v[6] == b.v[6] &&
	a.v[7] == b.v[7] &&
	a.v[8] == b.v[8] &&
	a.v[9] == b.v[9] &&
	a.v[10]== b.v[10]&&
	a.v[11]== b.v[11]&&
	a.v[12]== b.v[12]&&
	a.v[13]== b.v[13]&&
	a.v[14]== b.v[14]&&
	a.v[15]== b.v[15];
}

template <typename T, typename U>
bool operator != (const mat4<T> &a, const mat4<U> &b) {
	return
	a.v[0] != b.v[0] ||
	a.v[1] != b.v[1] ||
	a.v[2] != b.v[2] ||
	a.v[3] != b.v[3] ||
	a.v[4] != b.v[4] ||
	a.v[5] != b.v[5] ||
	a.v[6] != b.v[6] ||
	a.v[7] != b.v[7] ||
	a.v[8] != b.v[8] ||
	a.v[9] != b.v[9] ||
	a.v[10]!= b.v[10]||
	a.v[11]!= b.v[11]||
	a.v[12]!= b.v[12]||
	a.v[13]!= b.v[13]||
	a.v[14]!= b.v[14]||
	a.v[15]!= b.v[15];
}

} //namespace math
#endif // _MATH_ALGEBRA3_DEFINED_
