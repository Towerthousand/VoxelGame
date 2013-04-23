#ifndef FUNCTION2D_HPP
#define FUNCTION2D_HPP
#include "Function3D.hpp"

class Function2D : public Function3D{ //abstract
	public:
		Function2D() {}
		virtual ~Function2D() {}
		//x,y,z multiples of CHUNKSIZE
		virtual float2Data getFloat2Data(int x, int y, long seed) = 0;
		//data returned must be CHUNKSIZExCHUNKSIZE
		virtual float3Data getFloat3Data(int x, int y, int z, long seed) {
			float2Data layer = getFloat2Data(x,y,z,seed);
			float3Data result(CHUNKSIZE, float2Data);
			for(int i = 0; i < CHUNKSIZE; ++i) {
				result[i] = layer;
			}
			return result;
		}
		//data returned must be CHUNKSIZExCHUNKSIZExCHUNKSIZE
};

#endif // FUNCTION2D_HPP
