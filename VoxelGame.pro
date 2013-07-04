TEMPLATE = app
CONFIG -= qt
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL
INCLUDEPATH += $$PWD/
QMAKE_CXXFLAGS += -std=c++11 -O3
SOURCES += \
    $$PWD/main.cpp \
    $$PWD/Game.cpp \
    $$PWD/tools.cpp \
    $$PWD/Scene.cpp \
    $$PWD/RenderState.cpp \
    $$PWD/graphics/FontManager.cpp \
    $$PWD/graphics/Texture.cpp \
    $$PWD/graphics/TextureManager.cpp \
    $$PWD/graphics/Shader.cpp \
    $$PWD/graphics/ShaderProgram.cpp \
    $$PWD/audio/AudioManager.cpp \
    $$PWD/audio/Effect.cpp \
    $$PWD/audio/Music.cpp \
    $$PWD/input/InputManager.cpp \
    $$PWD/SceneMain/SceneMain.cpp \
    $$PWD/SceneMain/GameObject.cpp \
    $$PWD/SceneMain/Model.cpp \
    $$PWD/SceneMain/Hitbox.cpp \
    $$PWD/SceneMain/world/Chunk.cpp \
    $$PWD/SceneMain/world/World.cpp \
    $$PWD/SceneMain/world/SkyMap.cpp \
    $$PWD/SceneMain/entities/Entity.cpp \
    $$PWD/SceneMain/entities/Camera.cpp \
    $$PWD/SceneMain/entities/Player.cpp \
    $$PWD/SceneMain/entities/items/Arrow.cpp \
    $$PWD/SceneMain/entities/items/Polla.cpp \
    $$PWD/SceneMain/entities/enemies/Enemy.cpp \
    $$PWD/SceneMain/entities/enemies/Skeleton.cpp \
    $$PWD/SceneMain/world/generator/ChunkGenerator.cpp \
    $$PWD/SceneMain/world/generator/Function3DSimplex.cpp \
    $$PWD/SceneMain/world/generator/Function3DYcoord.cpp \
    $$PWD/SceneMain/world/generator/Function3DAdd.cpp \
    $$PWD/SceneMain/world/generator/Function3DSub.cpp \
    $$PWD/SceneMain/world/generator/Function2DConst.cpp \
    $$PWD/SceneMain/world/generator/Function3DDiv.cpp \
    $$PWD/SceneMain/world/generator/FunctionTerrrainVolume.cpp \
    $$PWD/SceneMain/world/generator/FunctionTerrainOverlay.cpp \
    $$PWD/SceneMain/world/generator/Function2DSimplex.cpp \
    $$PWD/SceneMain/world/generator/FunctionTerrainHeightmap.cpp \
    $$PWD/SceneMain/world/generator/FunctionTerrainJoin.cpp

HEADERS += \
    $$PWD/Scene.hpp \
    $$PWD/Game.hpp \
    $$PWD/tools.hpp \
    $$PWD/RenderState.hpp \
    $$PWD/graphics/FontManager.hpp \
    $$PWD/graphics/Texture.hpp \
    $$PWD/graphics/TextureManager.hpp \
    $$PWD/graphics/Shader.hpp \
    $$PWD/graphics/ShaderProgram.hpp \
    $$PWD/audio/AudioManager.hpp \
    $$PWD/audio/Effect.hpp \
    $$PWD/audio/Music.hpp \
    $$PWD/input/InputManager.hpp \
    $$PWD/SceneMain/SceneMain.hpp \
    $$PWD/SceneMain/GameObject.hpp \
    $$PWD/SceneMain/Model.hpp \
    $$PWD/SceneMain/Hitbox.hpp \
    $$PWD/SceneMain/world/Chunk.hpp \
    $$PWD/SceneMain/world/World.hpp \
    $$PWD/SceneMain/world/SkyMap.hpp \
    $$PWD/SceneMain/entities/Entity.hpp \
    $$PWD/SceneMain/entities/Camera.hpp \
    $$PWD/SceneMain/entities/Player.hpp \
    $$PWD/SceneMain/entities/items/Arrow.hpp \
    $$PWD/SceneMain/entities/items/Polla.hpp \
    $$PWD/SceneMain/entities/enemies/Enemy.hpp \
    $$PWD/SceneMain/entities/enemies/Skeleton.hpp \
    $$PWD/SceneMain/world/generator/ChunkGenerator.hpp \
    $$PWD/SceneMain/world/generator/Function3D.hpp \
    $$PWD/SceneMain/world/generator/Function2D.hpp \
    $$PWD/SceneMain/world/generator/Function3DSimplex.hpp \
    $$PWD/SceneMain/world/generator/Function3DYcoord.hpp \
    $$PWD/SceneMain/world/generator/Function3DAdd.hpp \
    $$PWD/SceneMain/world/generator/Function3DSub.hpp \
    $$PWD/SceneMain/world/generator/Function2DConst.hpp \
    $$PWD/SceneMain/world/generator/Function3DDiv.hpp \
    $$PWD/SceneMain/world/generator/FunctionTerrain.hpp \
    $$PWD/SceneMain/world/generator/FunctionTerrrainVolume.hpp \
    $$PWD/SceneMain/world/generator/FunctionTerrainOverlay.hpp \
    $$PWD/SceneMain/world/generator/Function2DSimplex.hpp \
    $$PWD/SceneMain/world/generator/FunctionTerrainHeightmap.hpp \
    $$PWD/SceneMain/world/generator/FunctionTerrainJoin.hpp \
    $$PWD/glm/*.hpp

OTHER_FILES += \
    $$PWD/shaders/terrain_fragment.glsl \
    $$PWD/shaders/terrain_vertex.glsl \
    $$PWD/shaders/model_vertex.glsl \
    $$PWD/shaders/model_fragment.glsl \
    shaders/black_vertex.glsl \
    shaders/black_fragment.glsl
