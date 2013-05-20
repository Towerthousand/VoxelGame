#ifndef SKYMAP_HPP
#define SKYMAP_HPP
#include "tools.hpp"

class Chunk;
class SceneMain;
class skyMap {
	public:
		skyMap();
		~skyMap();

		int &operator()(int x, int z);
		int &operator()(vec2i coord);
		int const &operator()(int x, int z) const;
		int const &operator()(vec2i coord) const;

		bool updateValues(Chunk* c);

	private:
		std::vector<int> values;
};

#endif // SKYMAP_HPP
