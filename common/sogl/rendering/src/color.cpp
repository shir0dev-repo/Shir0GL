#include <sogl/rendering/color.hpp>
#include <sogl/rendering/color32.hpp>

namespace sogl {
	color color::BLACK			= color(0.0f, 0.0f, 0.0f);
	color color::RED			= color(1.0f, 0.0f, 0.0f);
	color color::YELLOW			= color(1.0f, 1.0f, 0.0f);
	color color::GREEN			= color(0.0f, 1.0f, 0);
	color color::CYAN			= color(0.0f, 1.0f, 1.0f);
	color color::BLUE			= color(0.0f, 0.0f, 1.0f);
	color color::MAGENTA		= color(1.0f, 0.0f, 1.0f);
	color color::WHITE			= color(1.0f, 1.0f, 1.0f);

	color32 color32::BLACK		= color32(0, 0, 0);
	color32 color32::RED		= color32(255, 0, 0);
	color32 color32::YELLOW		= color32(255, 255, 0);
	color32 color32::GREEN		= color32(0, 255, 0);
	color32 color32::CYAN		= color32(0, 255, 255);
	color32 color32::BLUE		= color32(0, 0, 255);
	color32 color32::MAGENTA	= color32(255, 0, 255);
	color32 color32::WHITE		= color32(255, 255, 255);
}