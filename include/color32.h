#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

namespace sogl {
	typedef unsigned char uint8_t;
	struct color;

	struct SOGL_API color32 {
	
		static color32 BLACK;
		static color32 RED;
		static color32 YELLOW;
		static color32 GREEN;
		static color32 CYAN;
		static color32 BLUE;
		static color32 MAGENTA;
		static color32 WHITE;

		uint8_t r, g, b, a;

		// Constructs a color from the given rgba values (defaults to black with full alpha).
		color32(uint8_t = 0, uint8_t = 0, uint8_t = 0, uint8_t = 255);

		// Interpolates linearly between color a and color a by a factor of t.
		static color32 lerp(color32 const&, color32 const&, float const&);
		
		// Clamps a color's values between 0-255.
		static color32 saturate(color32 const&);

		// Clamps a color's values between 0-255.
		void saturate();

		// Blends colors additively.
		color32 operator+(color32 const& b) const;
		
		// Blends colors additively.
		color32& operator+=(color32 const& b);

		// Blends colors subtractively.
		color32 operator-(color32 const& b) const;
		
		// Blends colors subtractively.
		color32& operator-=(color32 const& b);

		// Scales components of color.
		color32 operator*(float const& b) const;
		
		// Scales components of color.
		color32& operator*=(float const& b);

		// Converts to a 0-1 representation of this color.
		explicit operator color();
	};
}