#include "SkyMap.hpp"
#include "Chunk.hpp"
#include "../SceneMain.hpp"

skyMap::skyMap() {
	values = std::vector<int>(CHUNKSIZE*CHUNKSIZE,INT_MIN);
}

skyMap::~skyMap() {
}

int &skyMap::operator()(int x, int z) {
	return values[x*CHUNKSIZE*CHUNKSIZE + z];
}

int &skyMap::operator()(vec2i coord) {
	return values[coord.x*CHUNKSIZE+coord.y];
}

int const &skyMap::operator()(int x, int z) const {
	return values[x*CHUNKSIZE*CHUNKSIZE + z];
}

int const &skyMap::operator()(vec2i coord) const {
	return values[coord.x*CHUNKSIZE+coord.y];
}

bool skyMap::updateValues(Chunk *c) {
	bool result = true;
	for(int x = 0; x < CHUNKSIZE; ++x)
		for(int z = 0; z < CHUNKSIZE; ++z) {
			int lowPos = c->YPOS*CHUNKSIZE;
			if((*this)(x,z) < (lowPos+CHUNKSIZE)) {
				for(int y = CHUNKSIZE-1; y >= 0 && lowPos+y >= values[x*CHUNKSIZE+z]; --y) {
					if((*c)(x,y,z).ID != 0) {
						values[x*CHUNKSIZE+z] = lowPos + y;
						result = true;
					}
				}
			}
		}
}
