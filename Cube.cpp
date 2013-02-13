void drawToWorld(bool front, bool back, bool left, bool right, bool bottom, bool top) {
	if(front) {
		vertexPoints.push_back(sf::Vector3f(x    , y+1.0, z+1.0));
		vertexPoints.push_back(sf::Vector3f(x    , y    , z+1.0));
		vertexPoints.push_back(sf::Vector3f(x+1.0, y    , z+1.0));
		vertexPoints.push_back(sf::Vector3f(x+1.0, y+1.0, z+1.0));

		for (int i = 0; i < 4; ++i) {
			normals.push_back(sf::Vector3f( 0, 0, 1));
		}

		textureCoords.push_back(sf::Vector2f(0,0));
		textureCoords.push_back(sf::Vector2f(0,16.0));
		textureCoords.push_back(sf::Vector2f(16.0,16.0));
		textureCoords.push_back(sf::Vector2f(16.0,0));
	}
	if(back){
		vertexPoints.push_back(sf::Vector3f(x+1.0, y+1.0, z));
		vertexPoints.push_back(sf::Vector3f(x+1.0, y    , z));
		vertexPoints.push_back(sf::Vector3f(x    , y    , z));
		vertexPoints.push_back(sf::Vector3f(x    , y+1.0, z));

		for (int i = 0; i < 4; ++i) {
			normals.push_back(sf::Vector3f( 0, 0, -1));
		}

		textureCoords.push_back(sf::Vector2f(0,0));
		textureCoords.push_back(sf::Vector2f(0,16.0));
		textureCoords.push_back(sf::Vector2f(16.0,16.0));
		textureCoords.push_back(sf::Vector2f(16.0,0));
	}
	if(left){
		vertexPoints.push_back(sf::Vector3f(x+1.0, y+1.0, z+1.0));
		vertexPoints.push_back(sf::Vector3f(x+1.0, y    , z+1.0));
		vertexPoints.push_back(sf::Vector3f(x+1.0, y    , z    ));
		vertexPoints.push_back(sf::Vector3f(x+1.0, y+1.0, z    ));

		for (int i = 0; i < 4; ++i) {
			normals.push_back(sf::Vector3f( 1, 0, 0));
		}
		textureCoords.push_back(sf::Vector2f(0,0));
		textureCoords.push_back(sf::Vector2f(0,16.0));
		textureCoords.push_back(sf::Vector2f(16.0,16.0));
		textureCoords.push_back(sf::Vector2f(16.0,0));
	}
	if(right){
		vertexPoints.push_back(sf::Vector3f(x    , y+1.0, z    ));
		vertexPoints.push_back(sf::Vector3f(x    , y    , z    ));
		vertexPoints.push_back(sf::Vector3f(x    , y    , z+1.0));
		vertexPoints.push_back(sf::Vector3f(x    , y+1.0, z+1.0));

		for (int i = 0; i < 4; ++i) {
			normals.push_back(sf::Vector3f( -1, 0, 0));
		}
		textureCoords.push_back(sf::Vector2f(0,0));
		textureCoords.push_back(sf::Vector2f(0,16.0));
		textureCoords.push_back(sf::Vector2f(16.0,16.0));
		textureCoords.push_back(sf::Vector2f(16.0,0));
	}
	if(bottom){
		vertexPoints.push_back(sf::Vector3f(x+1.0, y    , z+1.0));
		vertexPoints.push_back(sf::Vector3f(x    , y    , z+1.0));
		vertexPoints.push_back(sf::Vector3f(x    , y    , z    ));
		vertexPoints.push_back(sf::Vector3f(x+1.0, y    , z    ));

		for (int i = 0; i < 4; ++i) {
			normals.push_back(sf::Vector3f( 0, -1, 0));
		}

		textureCoords.push_back(sf::Vector2f(32.0,0));
		textureCoords.push_back(sf::Vector2f(32.0,16.0));
		textureCoords.push_back(sf::Vector2f(48.0,16.0));
		textureCoords.push_back(sf::Vector2f(48.0,0));
	}
	if(top){
		vertexPoints.push_back(sf::Vector3f(x    , y+1.0, z+1.0));
		vertexPoints.push_back(sf::Vector3f(x+1.0, y+1.0, z+1.0));
		vertexPoints.push_back(sf::Vector3f(x+1.0, y+1.0, z    ));
		vertexPoints.push_back(sf::Vector3f(x    , y+1.0, z    ));

		for (int i = 0; i < 4; ++i) {
			normals.push_back(sf::Vector3f( 0, 1, 0));
		}

		textureCoords.push_back(sf::Vector2f(16.0,0));
		textureCoords.push_back(sf::Vector2f(16.0,16.0));
		textureCoords.push_back(sf::Vector2f(32.0,16.0));
		textureCoords.push_back(sf::Vector2f(32.0,0));
	}
}
