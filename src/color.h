#pragma once

#include <cmath>

namespace kajiya {
class color {
public:
	float r, g, b, a;

	color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
	}

	color operator*(float scalar) const {
		return color(r * scalar, g * scalar, b * scalar, a * scalar);
	}

	color operator/(float scalar) const {
		return color(r / scalar, g / scalar, b / scalar, a / scalar);
	}

	color operator*(const color &other) const {
		return color(r * other.r, g * other.g, b * other.b, a * other.a);
	}

	color operator/(color &other) const {
		return color(r / other.r, g / other.g, b / other.b, a / other.a);
	}

	color operator+(const color &other) {
		return color(r + other.r, g + other.g, b + other.b, a + other.a);
	}

	color operator+(float scalar) {
		return color(r + scalar, g + scalar, b + scalar, a + scalar);
	}

	bool operator==(const color &other) const {
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}

	float norm() const {
		return std::sqrt(r * r + g * g + b * b + a * a);
	}

	color unit() const {
		return *this / norm();
	}

	color clamp() const {
		float R = (r < 0) ? 0 : ((r > 1) ? 1 : r);
		float G = (g < 0) ? 0 : ((g > 1) ? 1 : g);
		float B = (b < 0) ? 0 : ((b > 1) ? 1 : b);
		float A = (a < 0) ? 0 : ((a > 1) ? 1 : a);
		return color(R, G, B, A);
	}

	static color from_hex(int c) {
		return color(((c >> 24) & 0xFF) / 255.f, ((c >> 16) & 0xFF) / 255.f,
					 ((c >> 8) & 0xFF) / 255.f, (c & 0xFF) / 255.f);
	}

	unsigned to_hex() const {
		return (static_cast<unsigned>(r * 255) << 24) +
			   (static_cast<unsigned>(g * 255) << 16) +
			   (static_cast<unsigned>(b * 255) << 8) +
			   static_cast<unsigned>(a * 255);
	}
};

color operator*(float scalar, color &color) {
	return color * scalar;
}
} // namespace kajiya
