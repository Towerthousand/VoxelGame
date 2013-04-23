#ifndef FUNCTION3DSIMPLEX_HPP
#define FUNCTION3DSIMPLEX_HPP
#include "Function3D.hpp"

class Function3DSimplex : public Function3D {
	public:
		Function3DSimplex();
		~Function3DSimplex();
		virtual float3Data getFloat3Data(int x, int y, int z, long seed);
	private:
		float scale;
		float min;
		float max;
};

#endif // FUNCTION3DSIMPLEX_HPP
