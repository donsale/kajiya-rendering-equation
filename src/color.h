#pragma once

#include <cmath>

namespace kajiya {
class Color {
public:
	float r, g, b, a;

	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
	}

	Color operator*(float scalar) const {
		return Color(r * scalar, g * scalar, b * scalar, a * scalar);
	}

	Color operator/(float scalar) const {
		return Color(r / scalar, g / scalar, b / scalar, a / scalar);
	}

	Color operator*(const Color &other) const {
		return Color(r * other.r, g * other.g, b * other.b, a * other.a);
	}

	Color operator/(Color &other) const {
		return Color(r / other.r, g / other.g, b / other.b, a / other.a);
	}

	Color operator+(const Color &other) {
		return Color(r + other.r, g + other.g, b + other.b, a + other.a);
	}

	Color operator+(float scalar) {
		return Color(r + scalar, g + scalar, b + scalar, a + scalar);
	}

	bool operator==(const Color &other) const {
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}

	float norm() const {
		return std::sqrt(r * r + g * g + b * b + a * a);
	}

	Color unit() const {
		return *this / norm();
	}

	Color clamp() const {
		float R = (r < 0) ? 0 : ((r > 1) ? 1 : r);
		float G = (g < 0) ? 0 : ((g > 1) ? 1 : g);
		float B = (b < 0) ? 0 : ((b > 1) ? 1 : b);
		float A = (a < 0) ? 0 : ((a > 1) ? 1 : a);
		return Color(R, G, B, A);
	}

	static Color from_hex(int c) {
		return Color(((c >> 24) & 0xFF) / 255.f, ((c >> 16) & 0xFF) / 255.f,
					 ((c >> 8) & 0xFF) / 255.f, (c & 0xFF) / 255.f);
	}

	unsigned to_hex() const {
		return (static_cast<unsigned>(r * 255) << 24) +
			   (static_cast<unsigned>(g * 255) << 16) +
			   (static_cast<unsigned>(b * 255) << 8) +
			   static_cast<unsigned>(a * 255);
	}
};

Color operator*(float scalar, Color &color) {
	return color * scalar;
}
} // namespace kajiya
