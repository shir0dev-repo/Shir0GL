#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

#include "color32.hpp"
#include <algorithm>

namespace sogl {

	struct SOGL_API color {
	
		// color(0, 0, 0)	
		static color BLACK;
		// color(1, 0, 0)	
		static color RED;
		// color(1, 1, 0)	
		static color YELLOW;
		// color(0, 1, 0)	
		static color GREEN;
		// color(0, 1, 1)	
		static color CYAN;
		// color(0, 0, 1)	
		static color BLUE;
		// color(1, 0, 1)	
		static color MAGENTA;
		// color(1, 1, 1)	
		static color WHITE;

		float r;
		float g;
		float b;
		float a;

		// Constructs a color with rgb 0 and alpha 1.
		inline color() {
			r = 0;
			g = 0;
			b = 0;
			a = 1;
		}

		// Constructs a color with the given rgb values, and sets alpha to 1.
		inline color(float r, float g, float b) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = 1;
		}

		// Constructs a color from the given rgba values.
		inline color(const float& r, const float& g, const float& b, const float& a) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		// Constructs a copy of the given color.
		inline color(const color& c) {
			*this = c;
		}

		// Copies the given color's rgba values to this color.
		color& operator=(const color& c) {
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
		}

		// Interpolates linearly between color a and color a by a factor of t.
		inline static color lerp(const color& a, const color& b, const float& t) {
			color c = b - a;
			c = c + (b - a);
			return a + (b - a) * t;
		}

		// Clamps a color's values between 0-1.
		inline color saturated() const {
			color c(*this);
			
			c.r = std::max(0.0f, std::min(r, 1.0f));
			c.g = std::max(0.0f, std::min(g, 1.0f));
			c.b = std::max(0.0f, std::min(b, 1.0f));
			c.a = std::max(0.0f, std::min(a, 1.0f));

			return c;
		}

		// Clamps a color's values between 0-1.
		inline void saturate() {
			r = std::max(0.0f, std::min(r, 1.0f));
			g = std::max(0.0f, std::min(g, 1.0f));
			b = std::max(0.0f, std::min(b, 1.0f));
			a = std::max(0.0f, std::min(a, 1.0f));
		}

		// Returns a copy of this color with the given color's rgba added to this one.
		inline color operator+(const color& c) const {
			color col(*this);

			col.r += c.r;
			col.g += c.g;
			col.b += c.b;
			col.a += c.a;

			return col;
		}

		// Adds the given color's rgba values to this color.
		inline color& operator+=(const color& c) {
			r += c.r;
			g += c.g;
			b += c.b;
			a += c.a;

			return *this;
		}

		// Returns a copy of this color with the given color's rgba subtracted from this one.
		inline color operator-(const color& c) const {
			color col(*this);

			col.r -= c.r;
			col.g -= c.g;
			col.b -= c.b;
			col.a -= c.a;

			return col;
		}

		// Subtracts the given color's rgba values from this color.
		inline color& operator-=(const color& c) {
			r -= c.r;
			g -= c.g;
			b -= c.b;
			a -= c.a;

			return *this;
		}

		// Returns a copy of this color with it's rgba values scaled by scalar.
		inline color operator*(float const& scalar) const {
			color col = *this;

			col.r *= scalar;
			col.g *= scalar;
			col.b *= scalar;
			col.a *= scalar;

			return col;
		}
		
		// Scales this color's rgba values by scalar.
		inline color& operator*=(float const& scalar) {
			r *= scalar;
			g *= scalar;
			b *= scalar;
			a *= scalar;

			return *this;
		}

		// Converts to a 32-bit representation of this color.
		inline explicit operator color32() {
			return color32(
				(uint8_t)(r * 255),
				(uint8_t)(g * 255),
				(uint8_t)(b * 255),
				(uint8_t)(a * 255));
		}
	};

#ifdef SOGL_EXPORT
	color color::BLACK = color(0, 0, 0);
	color color::RED = color(1, 0, 0);
	color color::YELLOW = color(1, 1, 0);
	color color::GREEN = color(0, 1, 0);
	color color::CYAN = color(0, 1, 1);
	color color::BLUE = color(0, 0, 1);
	color color::MAGENTA = color(1, 0, 1);
	color color::WHITE = color(1, 1, 1);
#endif
}