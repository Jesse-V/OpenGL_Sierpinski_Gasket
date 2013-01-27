
#ifndef __ANGEL_VEC_H__
#define __ANGEL_VEC_H__

#include "Angel.h"

namespace Angel
{
	struct vec4
	{
		GLfloat  x;
		GLfloat  y;
		GLfloat  z;
		GLfloat  w;

		vec4(GLfloat s = GLfloat(0.0)):
			x(s), y(s), z(s)//, w(s)
		{}

		vec4(GLfloat x, GLfloat y, GLfloat z):
			x(x), y(y), z(z), w(1)
		{}
	};

}

#endif // __ANGEL_VEC_H__