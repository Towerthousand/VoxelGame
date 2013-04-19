#ifndef FUNCTION3D_HPP
#define FUNCTION3D_HPP
#include "Function.hpp"

class Function2D : public Function{
	public:
		Function2D();
		virtual ~Function2D();
		virtual float get2Data(int x, int y, int z, float data , long seed);
		virtual float get3Data(int x, int y, int z, , long seed);
};

#endif // FUNCTION3D_HPP
