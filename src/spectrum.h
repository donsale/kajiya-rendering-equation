#pragma once

#include <map>

namespace kajiya {
class Spectrum {
public:
	static const int number_of_frequenices			  = 471;
	std::array<float, number_of_frequenices> spectrum = {0};

	Spectrum() {
	}

	Spectrum operator*(const Spectrum &other) const {
		Spectrum result;

		for (int i = 0; i < number_of_frequenices; i++) {
			result.spectrum[i] = spectrum[i] * other.spectrum[i];
		}

		return result;
	}

	Spectrum operator+(const Spectrum &other) const {
		Spectrum result;

		for (int i = 0; i < number_of_frequenices; i++) {
			result.spectrum[i] = spectrum[i] + other.spectrum[i];
		}

		return result;
	}

	Spectrum operator*(float value) const {
		Spectrum result;

		for (int i = 0; i < number_of_frequenices; i++) {
			result.spectrum[i] = spectrum[i] * value;
		}

		return result;
	}

	Spectrum operator/(float value) const {
		Spectrum result;

		for (int i = 0; i < number_of_frequenices; i++) {
			result.spectrum[i] = spectrum[i] / value;
		}

		return result;
	}

	float sum() const {
		float sum = 0;
		for (int i = 0; i < number_of_frequenices; i++) {
			sum += spectrum[i];
		}

		return sum;
	}

	Spectrum normalize() {
		Spectrum result;
		float s = sum();

		for (int i = 0; i < number_of_frequenices; i++) {
			result.spectrum[i] = spectrum[i] / s;
		}

		return result;
	}
};

} // namespace kajiya
