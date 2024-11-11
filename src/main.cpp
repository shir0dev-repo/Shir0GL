#include <vec3f.h>
#include <quat.h>

using namespace sogl;

int main() {
	vec3f v(0, 25, 0);
	quat q(vec3f(90, 0, 0));
	vec3f v1;
	v1 = (v * q);

	return 0;
}