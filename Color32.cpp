#include "pch.h"
#include "Color32.h"
#include "Color.h"

namespace shir0GL {
	color32::color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	color32 color32::lerp(color32 const& a, color32 const& b, float const& t) {
		return a + (b - a) * t;
	}
	color32 color32::saturate(color32 const& a) {
		color32 c;
		c.r = std::max(0, std::min((int)a.r, 255));
		c.g = std::max(0, std::min((int)a.g, 255));
		c.b = std::max(0, std::min((int)a.b, 255));
		c.a = std::max(0, std::min((int)a.a, 255));

		return c;
	}

	void color32::saturate() {
		r = std::max(0, std::min((int)r, 255));
		g = std::max(0, std::min((int)g, 255));
		b = std::max(0, std::min((int)b, 255));
		a = std::max(0, std::min((int)a, 255));

	}

	color32 operator+(color32 const& a, color32 const& b) {
		return color32(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
	}
	color32 color32::operator+=(color32 const& b) {
		*this = *this + b;
		return *this;
	}

	color32 operator-(color32 const& a, color32 const& b) {
		return color32(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
	}
	color32 color32::operator-=(color32 const& b) {
		*this = *this - b;
		return *this;
	}

	color32 operator*(color32 const& a, float const& b) {
		color32 c = a;
		c.r *= b;
		c.g *= b;
		c.b *= b;
		c.a *= b;

		return c;
	}
	color32 color32::operator*=(float const& b) {
		*this = *this * b;
		return *this;
	}

	color32::operator color() {
		return color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}
}