TEMPLATE = app
CONFIG -= qt
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGLU -lnoise
SOURCES += \
    main.cpp \
    Game.cpp \
    MediaManager.cpp \
    SceneMain.cpp \
    tools.cpp \
    Camera.cpp \
    Chunk.cpp \
    noiseutils.cpp \
    World.cpp \
    Player.cpp

HEADERS += \
    SceneMain.hpp \
    Scene.hpp \
    MediaManager.hpp \
    Game.hpp \
    Camera.hpp \
    tools.hpp \
    Chunk.hpp \
    noiseutils.h \
    World.hpp \
    Player.hpp
