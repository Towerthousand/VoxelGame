#ifndef FUNCTION3D_HPP
#define FUNCTION3D_HPP
#include "tools.hpp"

typedef std::vector<std::vector<std::vector<float> > > float3Data;
typedef std::vector<std::vector<float> > float2Data;

class Function3D { //abstract
	public:
		Function3D() {}
		virtual ~Function3D() {}
		//x,y,z multiples of CHUNKSIZE
		virtual float3Data getFloat3Data(int x, int y, int z, long seed) = 0;
		//data returned must be CHUNKSIZExCHUNKSIZExCHUNKSIZE
};

#endif // FUNCTION3D_HPP
