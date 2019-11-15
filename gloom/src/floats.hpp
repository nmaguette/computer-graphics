#pragma once

#include <cmath>
#include <ostream>
#include <algorithm>

class float2 {
public:
	float x;
	float y;

	float2() : x(0.0f), y(0.0f) {}
	float2(float const vx, float const vy) : x(vx), y(vy) {}

	template <class T>
	float2(T val) : x(val), y(val) {}

	float2& operator= (float2 const other) {
		x = other.x;
		y = other.y;
		return *this;
	}

	template <class T>
	float2& operator=(T const other) {
		*this = float2(other);
		return *this;
	}

	float2& operator+= (float2 const other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	float2& operator-= (float2 const other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	float2& operator*= (float2 const other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	float2& operator/= (float2 const other) {
		x /= other.x;
		y /= other.y;
		return *this;
	}

	friend float2 operator+ (float2 lhs, float2 const &rhs) { lhs += rhs; return lhs; }
	friend float2 operator- (float2 lhs, float2 const &rhs) { lhs -= rhs; return lhs; }
	friend float2 operator* (float2 lhs, float2 const &rhs) { lhs *= rhs; return lhs; }
	friend float2 operator/ (float2 lhs, float2 const &rhs) { lhs /= rhs; return lhs; }

	bool operator== (float2 const other) const {
		return (x == other.x) && (y == other.y);
	}

	bool operator!= (float2 const other) const {
		return (x != other.x) || (y != other.y);
	}

	float2 clamp(float2 const &lo, float2 const &hi) {
		return float2(
			std::max(std::min(x, hi.x),lo.x),
			std::max(std::min(y, hi.y),lo.y)
		);
	}

	friend std::ostream & operator << (std::ostream &os, float2 const &v) {
        os << "[" << v.x << "," << v.y << "]";
        return os;
    }
};


class float3 {
public:
	float x;
	float y;
	float z;

	float3() : x(0.0f), y(0.0f), z(0.0f) {}
	float3(float2 const v, float vz) : x(v.x), y(v.y), z(vz) {}
	float3(float const vx, float const vy, float const vz) : x(vx), y(vy), z(vz) {}

	template <class T>
	float3(T val) : x(val), y(val), z(val) {}

	float3& operator= (float3 const other) {
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}

	template <class T>
	float3& operator=(T other) {
		*this = float3(other);
		return *this;
	}

	float3& operator+= (float3 const other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	float3& operator-= (float3 const other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	float3& operator*= (float3 const other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	float3& operator/= (float3 const other) {
	    x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
	}

	friend float3 operator+ (float3 lhs, float3 const &rhs) { lhs += rhs; return lhs; }
	friend float3 operator- (float3 lhs, float3 const &rhs) { lhs -= rhs; return lhs; }
	friend float3 operator* (float3 lhs, float3 const &rhs) { lhs *= rhs; return lhs; }
	friend float3 operator/ (float3 lhs, float3 const &rhs) { lhs /= rhs; return lhs; }

	float3 cross(float3 const &v) const {
		return float3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	float dot(float3 const &v) const {
		return (x * v.x + y * v.y + z * v.z);
	}

	float distance(float3 const &v) const {
		return std::sqrt(std::pow(x - v.x, 2) + std::pow(y - v.y, 2) + std::pow(z - v.z,2));
	}

	float3& normalize() {
        float n = x * x + y * y + z * z;
        if (n > 0) {
			(*this) *= 1 / std::sqrt(n);
        }
        return *this;
    }

	bool operator== (float3 const other) const {
		return
			(x == other.x) &&
			(y == other.y) &&
			(z == other.z);
	}

	bool operator!= (float3 const other) const {
		return
			(x != other.x) ||
			(y != other.y) ||
			(z != other.z);
	}

	float3 clamp(float3 const &lo, float3 const &hi) {
		return float3(
			std::max(std::min(x, hi.x),lo.x),
			std::max(std::min(y, hi.y),lo.y),
			std::max(std::min(z, hi.z),lo.z)
		);
	}

	float2 toFloat2() const {
		return float2(x,y);
	}

	friend std::ostream & operator << (std::ostream &os, float3 const &v) {
        os << "[" << v.x << "," << v.y << "," << v.z << "]";
        return os;
    }
};

class float4 {
public:
	float x;
	float y;
	float z;
	float w;

	float4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	float4(float3 const v, float vw) : x(v.x), y(v.y), z(v.z), w(vw) {}
	float4(float const vx, float const vy, float const vz, float const vw) : x(vx), y(vy), z(vz), w(vw) {}

	template <class T>
	float4(T val) : x(val), y(val), z(val), w(val) {}

	float4& operator= (float4 other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

	template <class T>
	float4& operator=(T other) {
		*this = float4(other);
		return *this;
	}

	float4& operator+= (float4 other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	float4& operator-= (float4 other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	float4& operator*= (float4 other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	float4& operator/= (float4 other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	float4 clamp(float4 const &lo, float4 const &hi) {
		return float4(
			std::max(std::min(x, hi.x),lo.x),
			std::max(std::min(y, hi.y),lo.y),
			std::max(std::min(z, hi.z),lo.z),
			std::max(std::min(w, hi.w),lo.w)
		);
	}

	friend float4 operator+ (float4 lhs, float4 const &rhs) { lhs += rhs; return lhs; }
	friend float4 operator- (float4 lhs, float4 const &rhs) { lhs -= rhs; return lhs; }
	friend float4 operator* (float4 lhs, float4 const &rhs) { lhs *= rhs; return lhs; }
	friend float4 operator/ (float4 lhs, float4 const &rhs) { lhs /= rhs; return lhs; }

	bool operator!= (float4 other) {
		return (x != other.x) || (y != other.y) || (z != other.z) || (w != other.w);
	}

	bool operator== (float4 other) {
		return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
	}

	float3 toFloat3() {
		return float3(x,y,z);
	}

	friend std::ostream & operator << (std::ostream &os, const float4 &v)
	{
		os << "[" << v.x << "," << v.y << "," << v.z << "," << v.w << "]";
		return os;
	}
};

// In a way cheating.. These are not really floats.
// Just move along.
struct int2 {
    int x, y;
};