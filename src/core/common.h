#ifndef HGE_TYPES_H
#define HGE_TYPES_H

#include <filesystem>
#include "api/build_dll.h"

typedef std::filesystem::path HGE_Path;


struct ENGINE_API HGE_Vec2{
	float x, y;

	HGE_Vec2() : x(0.f), y(0.f)
	{}
	HGE_Vec2(float _value) : x(_value), y(_value)
	{}
	HGE_Vec2(float _x, float _y) : x(_x), y(_y)
	{}
};
inline HGE_Vec2 operator+(const HGE_Vec2& a, const HGE_Vec2& b)
{
	return {a.x+b.x, a.y+b.y};
}
inline HGE_Vec2 operator-(const HGE_Vec2& a, const HGE_Vec2& b)
{
	return {a.x-b.x, a.y-b.y};
}
inline HGE_Vec2 operator*(const HGE_Vec2& a, const HGE_Vec2& b)
{
	return {a.x*b.x, a.y*b.y};
}
inline HGE_Vec2 operator/(const HGE_Vec2& a, const HGE_Vec2& b)
{
	return {a.x/b.x, a.y/b.y};
}
inline HGE_Vec2& operator+=(HGE_Vec2& a, const HGE_Vec2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}
inline HGE_Vec2& operator-=(HGE_Vec2& a, const HGE_Vec2& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}
inline HGE_Vec2& operator*=(HGE_Vec2& a, const HGE_Vec2& b)
{
	a.x *= b.x;
	a.y *= b.y;
	return a;
}
inline HGE_Vec2& operator/=(HGE_Vec2& a, const HGE_Vec2& b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}
inline bool operator!=(const HGE_Vec2& a, const HGE_Vec2& b)
{
	return a.x != b.x || a.y != b.y;
}


struct ENGINE_API HGE_Vec3{
	float x, y, z;

	HGE_Vec3() : x(0.f), y(0.f), z(0.f)
	{}
	HGE_Vec3(float _value) : x(_value), y(_value), z(_value)
	{}
	HGE_Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{}
};
inline HGE_Vec3 operator+(const HGE_Vec3& a, const HGE_Vec3& b)
{
	return {a.x + b.x, a.y + b.y, a.z + b.z};
}
inline HGE_Vec3 operator-(const HGE_Vec3& a, const HGE_Vec3& b)
{
	return {a.x - b.x, a.y - b.y, a.z - b.z};
}
inline HGE_Vec3 operator*(const HGE_Vec3& a, const HGE_Vec3& b)
{
	return {a.x * b.x, a.y * b.y, a.z * b.z};
}
inline HGE_Vec3 operator/(const HGE_Vec3& a, const HGE_Vec3& b)
{
	return {a.x / b.x, a.y / b.y, a.z / b.z};
}
inline HGE_Vec3& operator+=(HGE_Vec3& a, const HGE_Vec3& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}
inline HGE_Vec3& operator-=(HGE_Vec3& a, const HGE_Vec3& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}
inline HGE_Vec3& operator*=(HGE_Vec3& a, const HGE_Vec3& b)
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return a;
}
inline HGE_Vec3& operator/=(HGE_Vec3& a, const HGE_Vec3& b)
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	return a;
}
inline bool operator!=(const HGE_Vec3& a, const HGE_Vec3& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z;
}

HGE_Vec3 ENGINE_API HGE_GetForwardVector(HGE_Vec3 _rotator);
HGE_Vec3 ENGINE_API HGE_GetRightVector(HGE_Vec3 _rotator);
HGE_Vec3 ENGINE_API HGE_GetUpVector(HGE_Vec3 _rotator);


struct ENGINE_API HGE_Vec4{
	float x, y, z, w;

	HGE_Vec4() : x(0.f), y(0.f), z(0.f), w(0.f)
	{}
	HGE_Vec4(float _value) : x(_value), y(_value), z(_value), w(_value)
	{}
	HGE_Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{}
};
inline HGE_Vec4 operator+(const HGE_Vec4& a, const HGE_Vec4& b)
{
	return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}
inline HGE_Vec4 operator-(const HGE_Vec4& a, const HGE_Vec4& b)
{
	return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}
inline HGE_Vec4 operator*(const HGE_Vec4& a, const HGE_Vec4& b)
{
	return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}
inline HGE_Vec4 operator/(const HGE_Vec4& a, const HGE_Vec4& b)
{
	return {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}
inline HGE_Vec4& operator+=(HGE_Vec4& a, const HGE_Vec4& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	a.w += b.w;
	return a;
}
inline HGE_Vec4& operator-=(HGE_Vec4& a, const HGE_Vec4& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	a.w -= b.w;
	return a;
}
inline HGE_Vec4& operator*=(HGE_Vec4& a, const HGE_Vec4& b)
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	a.w *= b.w;
	return a;
}
inline HGE_Vec4& operator/=(HGE_Vec4& a, const HGE_Vec4& b)
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	a.w /= b.w;
	return a;
}
inline bool operator!=(const HGE_Vec4& a, const HGE_Vec4& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}


struct ENGINE_API HGE_Transform {
	HGE_Vec3 location_;
	HGE_Vec3 rotation_;
	HGE_Vec3 scale_;

	HGE_Transform() : location_(HGE_Vec3()), rotation_(HGE_Vec3()), scale_(HGE_Vec3(1.f)) {}
	HGE_Transform(HGE_Vec3 location, HGE_Vec3 rotation, HGE_Vec3 scale) : location_(location), rotation_(rotation), scale_(scale) {}
};
inline HGE_Transform operator*(const HGE_Transform& a, const HGE_Transform& b)
{
	HGE_Transform t{};
	t.location_ = a.location_ + b.location_;
	t.rotation_ = a.rotation_ + b.rotation_;
	t.scale_ = a.scale_ * b.scale_;
	return t;
}
inline bool operator!=(const HGE_Transform& a, const HGE_Transform& b)
{
	return a.location_ != b.location_ ||
				 a.rotation_ != b.rotation_ ||
				 a.scale_    != b.scale_;
}


struct ENGINE_API HGE_2D_Transform {
	HGE_Vec3 location_;
	float rotation_;
	//given in degrees
	HGE_Vec2 scale_;

	HGE_2D_Transform() : location_(HGE_Vec3()), rotation_(0.f), scale_(HGE_Vec2()) {}
};



namespace common
{
	inline float time_dilatation_ = 1.f;
}
#endif