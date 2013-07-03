#include "SkyMap.hpp"
#include "Chunk.hpp"
#include "../SceneMain.hpp"

skyMap::skyMap() {
	values = std::vector<int>(CHUNKSIZE*CHUNKSIZE,INT_MIN);
}

skyMap::~skyMap() {
}

int &skyMap::getHeight(int x, int z) {
	return values[x*CHUNKSIZE*CHUNKSIZE + z];
}

int &skyMap::getHeight(vec2i coord) {
	return values[coord.x*CHUNKSIZE+coord.y];
}

int const &skyMap::getHeight(int x, int z) const {
	return values[x*CHUNKSIZE*CHUNKSIZE + z];
}

int const &skyMap::getHeight(vec2i coord) const {
	return values[coord.x*CHUNKSIZE+coord.y];
}

bool skyMap::updateValues(Chunk *c) {
	bool result = false;
	for(int x = 0; x < CHUNKSIZE; ++x) {
		for(int z = 0; z < CHUNKSIZE; ++z) {
			int lowPos = c->YPOS*CHUNKSIZE;
			int ind = x*CHUNKSIZE+z;
			if(values[ind] < (lowPos+CHUNKSIZE)) {
				for(int y = CHUNKSIZE-1; y >= 0 && lowPos+y >= values[ind]; --y) {
					if (lowPos+y == values[ind]) {
						if(c->getLocal(x,y,z).ID == 0)
							outLog("#ERROR CHUNK WAS MODIFIED OUT OF APPLICATION");
					}
					else if(c->getLocal(x,y,z).ID != 0) {
						values[ind] = lowPos + y;
						result = true;
					}
				}
			}
		}
	}
	return result;
}
