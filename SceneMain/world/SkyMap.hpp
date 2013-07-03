#ifndef SKYMAP_HPP
#define SKYMAP_HPP
#include "tools.hpp"

class Chunk;
class SceneMain;
class skyMap {
	public:
		skyMap();
		~skyMap();

		int &getHeight(int x, int z);
		int &getHeight(vec2i coord);
		int const &getHeight(int x, int z) const;
		int const &getHeight(vec2i coord) const;

		bool updateValues(Chunk* c); //returns wether there was a change

	private:
		std::vector<int> values;
};

#endif // SKYMAP_HPP
