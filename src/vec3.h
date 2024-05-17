#pragma once

#include <cmath>
#include <iostream>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace kajiya {
class Vec3 {
public:
	float x, y, z;

	Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {
	}

	Vec3 operator+(const Vec3 &other) const {
		return Vec3(x + other.x, y + other.y, z + other.z);
	}

	Vec3 operator-(const Vec3 &other) const {
		return Vec3(x - other.x, y - other.y, z - other.z);
	}

	Vec3 operator*(const float scalar) const {
		return Vec3(x * scalar, y * scalar, z * scalar);
	}

	Vec3 operator*(const Vec3 &other) const {
		return Vec3(x * other.x, y * other.y, z * other.z);
	}

	Vec3 operator/(const float scalar) const {
		return Vec3(x / scalar, y / scalar, z / scalar);
	}

	bool operator==(const Vec3 &other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	static float dot(const Vec3 &v1, const Vec3 &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	static Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
		return Vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
					v1.x * v2.y - v1.y * v2.x);
	}

	float norm() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	float norm_squared() const {
		return x * x + y * y + z * z;
	}

	Vec3 operator-() const {
		return Vec3(-x, -y, -z);
	}

	Vec3 unit() const {
		return *this / norm();
	}

	Vec3 reflect_around(Vec3 &other) const {
		Vec3 tangential_component =
			other.unit() * Vec3::dot(other.unit(), *this);

		return tangential_component * 2 - *this;
	}

	void print() const {
		std::printf("(%2.6f, %2.6f, %2.6f)\n", x, y, z);
	}

	glm::vec3 to_glm_vec3() {
		return glm::vec3(x, y, z);
	}

	glm::vec4 to_glm_vec4() {
		return glm::vec4(x, y, z, 1);
	}

	static Vec3 from_glm_vec3(glm::vec3 v) {
		return Vec3(v.x, v.y, v.z);
	}

	static Vec3 from_glm_vec4(glm::vec4 v) {
		return Vec3(v.x, v.y, v.z);
	}
};
} // namespace kajiya
