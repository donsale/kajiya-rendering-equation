#pragma once

#include <cmath>

namespace whitted {
class vec3 {
public:
	float x, y, z;

	vec3(float x, float y, float z) : x(x), y(y), z(z) {
	}

	vec3 operator+(const vec3 &other) const {
		return vec3(x + other.x, y + other.y, z + other.z);
	}

	vec3 operator-(const vec3 &other) const {
		return vec3(x - other.x, y - other.y, z - other.z);
	}

	vec3 operator*(const float scalar) const {
		return vec3(x * scalar, y * scalar, z * scalar);
	}

	vec3 operator/(const float scalar) const {
		return vec3(x / scalar, y / scalar, z / scalar);
	}

	bool operator==(const vec3 &other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	static float dot(const vec3 &v1, const vec3 &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	static vec3 cross(const vec3 &v1, const vec3 &v2) {
		return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
					v1.x * v2.y - v1.y * v2.x);
	}

	float norm() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	float norm_squared() const {
		return x * x + y * y + z * z;
	}

	vec3 operator-() const {
		return vec3(-x, -y, -z);
	}

	vec3 unit() const {
		return *this / norm();
	}

	vec3 reflect_around(vec3 &other) const {
		vec3 tangential_component =
			other.unit() * vec3::dot(other.unit(), *this);

		return tangential_component * 2 - *this;
	}
};
} // namespace whitted
