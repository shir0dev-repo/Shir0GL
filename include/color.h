#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

namespace sogl {
	struct color32;

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

		float r, g, b, a;

		// Constructs a color from the given rgba values (defaults to black with full alpha).
		color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
		color(const color& c);
		color& operator=(const color& c);

		// Interpolates linearly between color a and color a by a factor of t.
		static color lerp(const color& a, const color& b, const float& t);

		// Clamps a color's values between 0-1.
		static color saturate(color const&);

		// Clamps a color's values between 0-1.
		void saturate();

		// Blends colors additively.
		color operator+(const color& c) const;

		// Blends colors additively.
		color& operator+=(const color& c);

		// Blends colors subtractively.
		color operator-(const color& c) const;

		// Blends colors subtractively.
		color& operator-=(const color& c);

		// Scales components of color.
		color operator*(float const& c) const;
		
		// Scales components of color.
		color& operator*=(float const& c);

		// Converts to a 32-bit representation of this color.
		explicit operator color32();
	};
}