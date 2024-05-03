#pragma once

#include <map>

namespace kajiya {
class Spectrum {
public:
	using Frequency = int;
	using Intensity = float;
	std::map<Frequency, Intensity> spectrum;

	Spectrum() {
		// for (Frequency f = 360; f <= 830; f++) {
		// 	spectrum[f] = 0;
		// }
	}

	Spectrum operator*(const Spectrum &other) const {
		Spectrum result;

		for (Frequency f = 360; f <= 830; f++) {
			auto it1 = spectrum.find(f);
			auto it2 = other.spectrum.find(f);

			if (it1 != spectrum.end() && it2 != other.spectrum.end()) {
				result.spectrum[f] = it1->second * it2->second;
			}
		}

		return result;
	}

	Spectrum operator+(const Spectrum &other) const {
		Spectrum result;

		for (Frequency f = 360; f <= 830; f++) {
			auto it1 = spectrum.find(f);
			auto it2 = other.spectrum.find(f);

			if (it1 != spectrum.end() && it2 != other.spectrum.end()) {
				result.spectrum[f] = it1->second + it2->second;
			}
		}

		return result;
	}

	Spectrum operator*(float value) const {
		Spectrum result;

		for (Frequency f = 360; f <= 830; f++) {
			auto it1 = spectrum.find(f);

			if (it1 != spectrum.end()) {
				result.spectrum[f] = it1->second * value;
			}
		}

		return result;
	}

	Spectrum operator/(float value) const {
		Spectrum result;

		for (Frequency f = 360; f <= 830; f++) {
			auto it1 = spectrum.find(f);

			if (it1 != spectrum.end()) {
				result.spectrum[f] = it1->second / value;
			}
		}

		return result;
	}

	float sum() const {
		float sum = 0;
		for (Frequency f = 360; f <= 830; f++) {
			auto it1 = spectrum.find(f);

			if (it1 != spectrum.end()) {
				sum += it1->second;
			}
		}

		return sum;
	}
};

} // namespace kajiya
