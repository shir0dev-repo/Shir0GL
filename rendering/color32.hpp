#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

#include "color.hpp"
#include "../transform/mathutil.hpp"

namespace sogl {
	struct SOGL_API color32 {
	
		typedef uint8_t u8;
		typedef uint16_t u16;

		static color32 BLACK;
		static color32 RED;
		static color32 YELLOW;
		static color32 GREEN;
		static color32 CYAN;
		static color32 BLUE;
		static color32 MAGENTA;
		static color32 WHITE;

		u8 r, g, b, a;

		// Constructs a color with rgb 0 and alpha 255.
		inline color32() {
			r = 0;
			g = 0;
			b = 0;
			a = 255;
		}

		// Constructs a color from the given rgba values (defaults to black with full alpha).
		inline color32(const u8& r, const u8& g, const u8& b) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = 255;
		}

		inline color32(const u8& r, const u8& g, const u8& b, const u8& a) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		// Interpolates linearly between color a and color b by a factor of t.
		inline static color32 lerp(color32 const& a, color32 const& b, float const& t) {
			u8 l = (u8)(t * 255);
			return a + (b - a) * l;
		}
		
		// Returns a copy of this color with it's rgba values clamped between 0-255.
		inline color32 saturated() const {
			color32 c;
			c.r = std::max(0, std::min((int)r, 255));
			c.g = std::max(0, std::min((int)g, 255));
			c.b = std::max(0, std::min((int)b, 255));
			c.a = std::max(0, std::min((int)a, 255));

			return c;
		}

		// Clamps this color's rgba values between 0-255.
		inline void saturate() {
			r = clamp(r, (u8)0, (u8)255);
			g = clamp(g, (u8)0, (u8)255);
			b = clamp(b, (u8)0, (u8)255);
			a = clamp(a, (u8)0, (u8)255);
		}

		/* Returns a copy of this color with the given color's rgba added to this one.
		   Will clamp values to 255 to avoid integer overflow.*/
		inline color32 operator+(color32 const& col) const {
			// create u16 versions of the result to avoid integer overflow
			u16 r16 = r + col.r;
			u16 g16 = g + col.g;
			u16 b16 = b + col.b;
			u16 a16 = a + col.a;

			clamp(r16, (u16)0, (u16)255);
			clamp(g16, (u16)0, (u16)255);
			clamp(b16, (u16)0, (u16)255);
			clamp(a16, (u16)0, (u16)255);

			color32 c;
			c.r = (u8)r16;
			c.g = (u8)g16;
			c.b = (u8)b16;
			c.a = (u8)a16;

			return c;
		}
		
		/*Adds the given color's rgba values to this color.
		  Will clamp values to 255 to avoid integer overflow.*/
		inline color32& operator+=(color32 const& col) {
			// create u16 versions of the result to avoid integer overflow
			u16 r16 = r + col.r;
			u16 g16 = g + col.g;
			u16 b16 = b + col.b;
			u16 a16 = a + col.a;

			clamp(r16, (u16)0, (u16)255);
			clamp(g16, (u16)0, (u16)255);
			clamp(b16, (u16)0, (u16)255);
			clamp(a16, (u16)0, (u16)255);

			r = (u8)r16;
			g = (u8)g16;
			b = (u8)b16;
			a = (u8)a16;

			return *this;
		}

		// Creates a copy of this color with the given color's rgba values subtracted from this one.
		inline color32 operator-(color32 const& col) const {
			// create signed versions of the result to avoid unsigned wrapping
			int8_t rSigned = r - col.r;
			int8_t gSigned = g - col.g;
			int8_t bSigned = b - col.b;
			int8_t aSigned = a - col.a;
			
			clamp(rSigned, (int8_t)0, (int8_t)255);
			clamp(gSigned, (int8_t)0, (int8_t)255);
			clamp(bSigned, (int8_t)0, (int8_t)255);
			clamp(aSigned, (int8_t)0, (int8_t)255);

			color32 c;
			c.r = (u8)rSigned;
			c.g = (u8)gSigned;
			c.b = (u8)bSigned;
			c.a = (u8)aSigned;

			return c;
		}
		
		// Subtracts the given color's rgba values from this one.
		inline color32& operator-=(color32 const& col) {
			// create signed versions of the result to avoid unsigned wrapping
			int8_t rSigned = r - col.r;
			int8_t gSigned = g - col.g;
			int8_t bSigned = b - col.b;
			int8_t aSigned = a - col.a;

			clamp(rSigned, (int8_t)0, (int8_t)255);
			clamp(gSigned, (int8_t)0, (int8_t)255);
			clamp(bSigned, (int8_t)0, (int8_t)255);
			clamp(aSigned, (int8_t)0, (int8_t)255);
			r = (u8)rSigned;
			g = (u8)rSigned;
			b = (u8)rSigned;
			a = (u8)rSigned;

			return *this;
		}

		/* Returns a copy of this color with it's rgba values scaled by scalar.
		   Will return full zero color if the scalar is negative.*/
		inline color32 operator*(float const& scalar) const {
			if (scalar < 0)
				return color32((u8)0, (u8)0, (u8)0, (u8)0);

			// create float versions of the result to avoid integer nonsense
			float ir = r * scalar;
			float ig = g * scalar;
			float ib = b * scalar;
			float ia = a * scalar;
			color32 col;
			col.r = (u8)clamp(ir, 0.0f, 255.0f);
			col.g = (u8)clamp(ig, 0.0f, 255.0f);
			col.b = (u8)clamp(ib, 0.0f, 255.0f);
			col.a = (u8)clamp(ia, 0.0f, 255.0f);

			return col;
		}
		
		/* Scales this color's rgba values by scalar.
		   Will zero out rgba values if the scalar is negative.*/
		inline color32& operator*=(float const& scalar) {
			if (scalar < 0) {
				r = 0;
				g = 0;
				b = 0;
				a = 0;

				return *this;
			}

			float ir = r * scalar;
			float ig = g * scalar;
			float ib = b * scalar;
			float ia = a * scalar;

			r = (u8)clamp(ir, 0.0f, 255.0f);
			g = (u8)clamp(ig, 0.0f, 255.0f);
			b = (u8)clamp(ib, 0.0f, 255.0f);
			a = (u8)clamp(ia, 0.0f, 255.0f);

			return *this;
		}

		// Converts to a 0-1 representation of this color.
		inline explicit operator color() {
			float fr = (float)r / 255.0f;
			float fg = (float)g / 255.0f;
			float fb = (float)b / 255.0f;
			float fa = (float)a / 255.0f;

			return color(fr, fg, fb, fa);
		}
	};

#ifdef SOGL_EXPORT
	color32 color32::BLACK = color32(0, 0, 0);
	color32 color32::RED = color32(255, 0, 0);
	color32 color32::YELLOW = color32(255, 255, 0);
	color32 color32::GREEN = color32(0, 255, 0);
	color32 color32::CYAN = color32(0, 255, 255);
	color32 color32::BLUE = color32(0, 0, 255);
	color32 color32::MAGENTA = color32(255, 0, 255);
	color32 color32::WHITE = color32(255, 255, 255);
#endif
}