TEMPLATE = app
CONFIG -= qt
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGLU
INCLUDEPATH += $$PWD/
QMAKE_CXXFLAGS += -std=c++11
CXX_FLAGS += -O2
SOURCES += \
    $$PWD/main.cpp \
    $$PWD/Game.cpp \
    $$PWD/tools.cpp \
	$$PWD/Math.cpp \
	$$PWD/graphics/FontManager.cpp \
	$$PWD/graphics/Texture.cpp \
	$$PWD/graphics/TextureManager.cpp \
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
	$$PWD/SceneMain/entities/Entity.cpp \
	$$PWD/SceneMain/entities/Camera.cpp \
	$$PWD/SceneMain/entities/Player.cpp \
	$$PWD/SceneMain/entities/items/Arrow.cpp \
	$$PWD/SceneMain/entities/items/Polla.cpp \
	$$PWD/SceneMain/entities/enemies/Enemy.cpp \
	$$PWD/SceneMain/entities/enemies/Skeleton.cpp

HEADERS += \
    $$PWD/Scene.hpp \
    $$PWD/Game.hpp \
    $$PWD/tools.hpp \
	$$PWD/Math.hpp \
	$$PWD/graphics/FontManager.hpp \
	$$PWD/graphics/Texture.hpp \
	$$PWD/graphics/TextureManager.hpp \
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
	$$PWD/SceneMain/entities/Entity.hpp \
	$$PWD/SceneMain/entities/Camera.hpp \
	$$PWD/SceneMain/entities/Player.hpp \
	$$PWD/SceneMain/entities/items/Arrow.hpp \
	$$PWD/SceneMain/entities/items/Polla.hpp \
	$$PWD/SceneMain/entities/enemies/Enemy.hpp \
	$$PWD/SceneMain/entities/enemies/Skeleton.hpp
