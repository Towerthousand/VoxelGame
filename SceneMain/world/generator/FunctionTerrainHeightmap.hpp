#ifndef FUNCTIONTERRAINHEIGHTMAP_HPP
#define FUNCTIONTERRAINHEIGHTMAP_HPP
#include "FunctionTerrain.hpp"

class Function2D;
class FunctionTerrainHeightmap : public FunctionTerrain {
	public:
		FunctionTerrainHeightmap(Function2D* source, char blockID);
		virtual ~FunctionTerrainHeightmap();
		ID3Data getID3Data(int x, int y, int z, int sx, int sy, int sz); //world coords
	private:
		Function2D* source;
		char blockID;
};

#endif // FUNCTIONTERRAINHEIGHTMAP_HPP
