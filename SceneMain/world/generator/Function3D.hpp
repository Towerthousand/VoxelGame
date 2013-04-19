#ifndef FUNCTION3D_HPP
#define FUNCTION3D_HPP
#include "Function.hpp"

class Function3D : public Function{
	public:
		Function3D();
		virtual ~Function3D();
		virtual float getData(int x, int y, int z, long seed);
};

#endif // FUNCTION3D_HPP
