#include "pch.h"
#include "Color.h"
#include "Color32.h"

namespace shir0GL {
	color::color(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	color color::lerp(color const& a, color const& b, float const& t) {
		return a + (b - a) * t;
	}
	color color::saturate(color const& a) {
		color c(a);
		c.saturate();
		return c;
	}

	void color::saturate() {
		r = std::max(0.0f, std::min(r, 1.0f));
		g = std::max(0.0f, std::min(g, 1.0f));
		b = std::max(0.0f, std::min(b, 1.0f));
		a = std::max(0.0f, std::min(a, 1.0f));
	}

	color operator+(color const& a, color const& b) {
		return color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
	}
	color color::operator+=(color const& b) {
		*this = *this + b;
		return *this;
	}

	color operator-(color const& a, color const& b) {
		return color(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
	}
	color color::operator-=(color const& b) {
		*this = *this - b;
		return *this;
	}

	color operator*(color const& a, float const& b) {
		color c = a;
		c.r *= b;
		c.g *= b;
		c.b *= b;
		c.a *= b;

		return c;
	}
	color color::operator*=(float const& b) {
		*this = *this * b;
		return *this;
	}

	color::operator color32() {
		color32 c;
		c.r = r * 255;
		c.g = g * 255;
		c.b = b * 255;
		c.a = a * 255;
		c.saturate();

		return c;
	}
}