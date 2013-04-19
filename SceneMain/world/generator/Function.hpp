#ifndef FUNCTION_HPP
#define FUNCTION_HPP
#include "tools.hpp"

typedef std::vector<std::vector<std::vector<float> > > float3Data;
typedef std::vector<std::vector<float> > float2Data;
typedef std::vector<std::vector<std::vector<float> > > ID3Data;

class Function { //abstract
	public:
		Function();
		virtual ~Function();
		virtual ID3Data getID3Data(int x, int y, int z, long seed); //returns a cube's index
};

#endif // FUNCTION_HPP
