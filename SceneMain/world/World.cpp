#include "World.hpp"
#include "Chunk.hpp"
#include "../entities/Player.hpp"
#include "../SceneMain.hpp"
#include "../../Game.hpp"

int asdf(int x)
{
    x = (x%6 + 6)%6;
    if(x > 2) x = 5-x;
    return x;
}

World::World(SceneMain* parentScene, Player* player) :
    playerTargetsBlock(false), targetedBlock(0,0,0),
    last(0,0,0), parentScene(parentScene), player(player),
    chunkGen(parentScene,2) {

    int num = 500;

    for(int x = -num; x <= num; x++)
            for(int z = -num; z < num; z++)
                setCubeID(x, asdf(x)+asdf(z), z, 4);
}

World::~World() {
}

bool World::getOutOfBounds(int x, int y, int z) const{
    //Con los octrees NUNCA hay nada out of bounds!
    //Ademas creo que esta funcion no se usa...
    return false;
}

Cube World::getCube(int x, int y, int z) const {
    return octree.get(x, y, z, OCTREE_SIZE);
}

void World::setCubeID(int x, int y, int z, unsigned char id) {
    octree.set(x, y, z, OCTREE_SIZE, id);
}

void World::setCubeLight(int x, int y, int z, unsigned char light) {
}


void World::update(float deltaTime) {

    //todo fix
    octree.update(0, 0, 0, OCTREE_SIZE, this->parentScene, (int)player->pos.x, (int)player->pos.y, (int)player->pos.z);
    traceView(player,10);

    /*
    chunkGen.chunkMutex.lock();
    while(!chunkGen.chunksLoaded.empty()) {
        Chunk* newChunk = chunkGen.chunksLoaded.front();
        chunkGen.chunksLoaded.pop_front();
        addChunk(getCoords(newChunk->getPos()).first,newChunk);
        calculateLight(newChunk->getPos() + vec3i(CHUNKSIZE*0.5),CHUNKSIZE*0.5 -1);
    }
    chunkGen.chunkMutex.unlock();
    vec3i playerChunkPos = vec3i(std::floor(player->pos.x),std::floor(player->pos.y),std::floor(player->pos.z))/CHUNKSIZE;
    std::priority_queue<std::pair<float,vec3i>, std::vector<std::pair<float,vec3i> >, FunctorCompare > queue;
    for (int x = -WORLDWIDTH/2; x < WORLDWIDTH/2; ++x)
        for (int y = -WORLDHEIGHT/2; y < WORLDHEIGHT/2; ++y)
            for (int z = -WORLDWIDTH/2; z < WORLDWIDTH/2; ++z){
                vec3i chunkPos(playerChunkPos.x+x,playerChunkPos.y+y,playerChunkPos.z+z);
                float dist = glm::length(vec3f(CHUNKSIZE*chunkPos) - player->pos);
                if(getChunk(chunkPos) == NULL) {
                    queue.push(std::pair<float,vec3i>(-dist,chunkPos));
                }
            }
    if(!queue.empty()) {
        while(!queue.empty()) {
            chunkGen.queueChunk(queue.top().second);
            queue.pop();
        }
    }

    for(std::map<vec3i,Chunk*,FunctorCompare>::iterator it = chunks.begin(); it != chunks.end(); ++it)
        if (it->second->markedForRedraw == true)
            it->second->update(deltaTime);
            */
}

void World::draw() const {

    //TODO Put occlusion and frustum culling again.
    octree.draw(0, 0, 0, OCTREE_SIZE);

    /*
    parentScene->chunksDrawn = 0;
    //empty and frustum culling

    for(std::map<vec3i,Chunk*,FunctorCompare>::const_iterator it = chunks.begin(); it != chunks.end(); ++it) {
        vec3f center(it->second->getPos() + vec3i(CHUNKSIZE/2));
        if (it->second->vertexCount == 0) //nothing to draw
            it->second->outOfView = true;
        else //check if it's actually inside of view
            it->second->outOfView = !player->insideFrustum(center,sqrt(3*((CHUNKSIZE/2)*(CHUNKSIZE/2))));
    }

    //do occlusion culling here!
    std::priority_queue<std::pair<float,Chunk*> > queryList; //chunks to be queried, ordered by distance

    //sort by distance
    float dist = 0;

    for(std::map<vec3i,Chunk*,FunctorCompare>::const_iterator it = chunks.begin(); it != chunks.end(); ++it) {
        if(!it->second->outOfView) {
            dist = glm::length(vec3f(it->second->getPos()) + vec3f(CHUNKSIZE/2) - parentScene->player->pos);
            queryList.push(std::pair<float,Chunk*>(-dist,it->second));
        }
    }

    int layers = 10;
    int chunksPerLayer = queryList.size()/layers + int(queryList.size()%layers > 0); //chunks per pass
    //first layer is always drawn
    for(int i = 0; i < chunksPerLayer && queryList.size() > 0; i++) {
        std::pair<float,Chunk*> c = queryList.top();
        queryList.pop();
        c.second->draw();
        ++parentScene->chunksDrawn;
    }
    //Query other layers
    for(int currLayer = 1; currLayer < layers && queryList.size() > 0; ++currLayer) {
        std::vector<GLuint> queries(chunksPerLayer,0);
        std::vector<Chunk*> chunkPointers(chunksPerLayer,NULL);

        //disable rendering state
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
        glDisable(GL_LIGHTING);

        //generate and send the queries
        int queriesSent = 0;
        glGenQueries(chunksPerLayer, &queries[0]);
        for (int i = 0; i < chunksPerLayer && queryList.size() > 0; ++i) {
            Chunk* currChunk = queryList.top().second;
            chunkPointers[i] = currChunk;
            queryList.pop();

            glBeginQuery(GL_ANY_SAMPLES_PASSED,queries[i]);
            currChunk->drawBoundingBox();
            glEndQuery(GL_ANY_SAMPLES_PASSED);
            ++queriesSent;
        }

        //enable rendering state
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
        glDisable(GL_LIGHTING);

        //collect query results
        for (int i = 0; i < queriesSent; ++i) {
            //if we have pending query, get result
            GLint seen;
            glGetQueryObjectiv(queries[i],GL_QUERY_RESULT, &seen);
            //if seen, draw it
            if (seen > 0) {
                chunkPointers[i]->draw();
                ++parentScene->chunksDrawn;
            }
            else
                chunkPointers[i]->drawBoundingBox();
        }
        //delete the queries
        glDeleteQueries(queries.size(),&queries[0]);
    }*/
}

//Based on: Fast Voxel Traversal Algorithm for Ray Tracinge
//By: John Amanatides et al.
//Implemented by Jordi "BuD" Santiago Provencio
void World::traceView(const Player *playerCam, float tMax) {

    //TODO: Esto se puede optimizar a saquisimo !!
    //TODO: Quiza se cuelga si haces un clic al infinito porque getOutOfBounds ahora devuelve siempre FALSE.
    //TODO: Seguramente deberia parar al chocar con un nodo que no este cargado hasta nivel de bloques individuales.

    if (!getOutOfBounds(floor(playerCam->camPos.x),floor(playerCam->camPos.y),floor(playerCam->camPos.z)) &&
            getCube(floor(playerCam->camPos.x),floor(playerCam->camPos.y),floor(playerCam->camPos.z)).id != 0) {
        playerTargetsBlock = true;
        targetedBlock = vec3f(floor(playerCam->camPos.x),floor(playerCam->camPos.y),floor(playerCam->camPos.z));
        return;
    }

    vec3f   pos(playerCam->camPos.x,playerCam->camPos.y,playerCam->camPos.z),
            dir(cos(-playerCam->camRot.x*DEG_TO_RAD)*(-sin(-playerCam->camRot.y*DEG_TO_RAD)),
                sin(-playerCam->camRot.x*DEG_TO_RAD),
                -cos(-playerCam->camRot.x*DEG_TO_RAD)*cos(-playerCam->camRot.y*DEG_TO_RAD)),
            vox(floor(pos.x), floor(pos.y), floor(pos.z)),
            step(0,0,0),
            next(0,0,0),
            tMaxc(tMax,tMax,tMax),
            tDelta(tMax,tMax,tMax);

    if (dir.x < 0) step.x = -1;
    else step.x = 1;
    if (dir.y < 0) step.y = -1;
    else step.y = 1;
    if (dir.z < 0) step.z = -1;
    else step.z = 1;

    next.x = vox.x + (step.x > 0 ? 1 : 0);
    next.y = vox.y + (step.y > 0 ? 1 : 0);
    next.z = vox.z + (step.z > 0 ? 1 : 0);

    if (dir.x != 0) {
        tDelta.x = step.x/dir.x;
        tMaxc.x = (next.x - pos.x)/dir.x;
    }
    if (dir.y != 0) {
        tDelta.y = step.y/dir.y;
        tMaxc.y = (next.y - pos.y)/dir.y;
    }
    if (dir.z != 0) {
        tDelta.z = step.z/dir.z;
        tMaxc.z = (next.z - pos.z)/dir.z;
    }

    float tCurr = 0;
    while (tCurr < tMax) {
        last = vox;
        if(tMaxc.x < tMaxc.y) {
            if(tMaxc.x < tMaxc.z) {
                tCurr = tMaxc.x;
                tMaxc.x = tMaxc.x + tDelta.x;
                vox.x = vox.x + step.x;
            }
            else {
                tCurr = tMaxc.z;
                vox.z = vox.z + step.z;
                tMaxc.z = tMaxc.z + tDelta.z;

            }
        }
        else {
            if(tMaxc.y < tMaxc.z) {
                tCurr = tMaxc.y;
                vox.y = vox.y + step.y;
                tMaxc.y = tMaxc.y + tDelta.y;
            }
            else {
                tCurr = tMaxc.z;
                vox.z = vox.z + step.z;
                tMaxc.z= tMaxc.z + tDelta.z;
            }
        }
        if(!getOutOfBounds(vox.x,vox.y,vox.z) && getCube(vox.x,vox.y,vox.z).id != 0) {
            playerTargetsBlock = true;
            targetedBlock = vox;
            return;
        }
    }
    playerTargetsBlock = false;
}

void World::calculateLight(vec3i source, int radius) { //BFS
    //TODO Fix

    /*
    sf::Clock clock;
    clock.restart();
    int updateRad = radius + 1; //if any component distance >= updateRad, lighting is OK
    std::vector<vec3i> blocksToCheck[MAXLIGHT+1];
    for(int x = source.x-updateRad; x <= source.x+updateRad; ++x) {
        for(int y = source.y-updateRad; y <= source.y+updateRad; ++y) {
            for(int z = source.z-updateRad; z <= source.z+updateRad; ++z) {
                if (!getOutOfBounds(x,y,z)){
                    std::pair<vec3i,vec3i> matrixCoords = getCoords(x,y,z);
                    Chunk* chunk = getChunk(matrixCoords.first);
                    Cube* cube = &chunk->cubes[matrixCoords.second.x*CHUNKSIZE*CHUNKSIZE
                            +matrixCoords.second.y*CHUNKSIZE
                            +matrixCoords.second.z];
                    if(chunk != NULL) {
                        switch(cube->ID) {
                        case 0: //air
                            if (x == source.x-updateRad || x == source.x+updateRad
                                    ||y == source.y-updateRad || y == source.y+updateRad
                                    ||z == source.z-updateRad || z == source.z+updateRad) { //if it is on border, mark it as node
                                if (cube->light > MINLIGHT)
                                    blocksToCheck[cube->light].push_back(vec3i(x,y,z));
                            }
                            else
                                setCubeLightRaw(x,y,z,MINLIGHT);
                            break;
                        case 4: //lightblock
                            setCubeLightRaw(x,y,z,MAXLIGHT);
                            blocksToCheck[MAXLIGHT].push_back(vec3i(x,y,z));
                            break;
                        default:
                            setCubeLightRaw(x,y,z,0);
                            break;
                        }
                    }
                }
            }
        }
    }
    for (int i = MAXLIGHT; i > MINLIGHT; --i)  {
        for(uint j = 0; j < blocksToCheck[i].size(); ++j) {
            vec3i source = blocksToCheck[i][j];
            processCubeLighting(source,vec3i(1,0,0),blocksToCheck[i-1]);
            processCubeLighting(source,vec3i(-1,0,0),blocksToCheck[i-1]);
            processCubeLighting(source,vec3i(0,1,0),blocksToCheck[i-1]);
            processCubeLighting(source,vec3i(0,-1,0),blocksToCheck[i-1]);
            processCubeLighting(source,vec3i(0,0,1),blocksToCheck[i-1]);
            processCubeLighting(source,vec3i(0,0,-1),blocksToCheck[i-1]);
        }
    }*/
}

void World::calculateLightManhattan(vec3i source, int radius) { //BFS

    //TODO Fix

    /*
    sf::Clock clock;
    clock.restart();
    int updateRad = radius + 1; //if manhattan dist >= updateRad, lighting is OK
    std::vector<vec3i>  blocksToCheck[MAXLIGHT+1];
    for(int x = source.x-updateRad; x < source.x+updateRad; ++x) {
        for(int y = source.y-updateRad; y < source.y+updateRad; ++y) {
            for(int z = source.z-updateRad; z < source.z+updateRad; ++z) {
                int manhattanDistance = std::abs(x-source.x)+std::abs(y-source.y)+std::abs(z-source.z);
                if (manhattanDistance <= updateRad && !getOutOfBounds(x,y,z)){
                    std::pair<vec3i,vec3i> matrixCoords = getCoords(x,y,z);
                    Chunk* chunk = getChunk(matrixCoords.first);
                    Cube* cube = &chunk->cubes[matrixCoords.second.x*CHUNKSIZE*CHUNKSIZE
                            +matrixCoords.second.y*CHUNKSIZE
                            +matrixCoords.second.z];
                    if(chunk != NULL) {
                        switch(cube->ID) {
                        case 0: //air
                            if (manhattanDistance == updateRad) { //if it is on border, mark it as node
                                if (cube->light > MINLIGHT)
                                    blocksToCheck[cube->light].push_back(vec3i(x,y,z));
                            }
                            else
                                setCubeLightRaw(x,y,z,MINLIGHT);
                            break;
                        case 4: //lightblock
                            setCubeLightRaw(x,y,z,MAXLIGHT);
                            blocksToCheck[MAXLIGHT].push_back(vec3i(x,y,z));
                            break;
                        default:
                            setCubeLightRaw(x,y,z,0);
                            break;
                        }
                    }
                }
            }
        }
    }
    for (int i = MAXLIGHT; i > MINLIGHT; --i)  {
        for(uint j = 0; j < blocksToCheck[i].size(); ++j) {
            vec3i source = blocksToCheck[i][j];
            processCubeLighting(source,vec3i(1,0,0),blocksToCheck[i-1]);
            processCubeLighting(source,vec3i(-1,0,0),blocksToCheck[i-1]);
            processCubeLighting(source,vec3i(0,1,0),blocksToCheck[i-1]);
            processCubeLighting(source,vec3i(0,-1,0),blocksToCheck[i-1]);
            processCubeLighting(source,vec3i(0,0,1),blocksToCheck[i-1]);
            processCubeLighting(source,vec3i(0,0,-1),blocksToCheck[i-1]);
        }
    }*/
}

void World::processCubeLighting(const vec3i& source, const vec3i& offset, std::vector<vec3i> &queue) { //BFS node processing

    //TODO Fix

    /*
    vec3i subject = source+offset;
    if(!getOutOfBounds(subject.x,subject.y,subject.z)) {
        Cube sub = getCubeRaw(subject.x,subject.y,subject.z);
        if(sub.id == 0) {
            Cube src = getCubeRaw(source.x,source.y,source.z);
            if(sub.light < src.light-1) {
                setCubeLightRaw(subject.x,subject.y,subject.z,src.light-1);
                queue.push_back(subject);
            }
        }
    }*/
}

void World::drawWireCube(const vec3f &pos) const {
    mat4f poppedMat = parentScene->getState().model;
    parentScene->getState().model =
            glm::translate(parentScene->getState().model,
                           vec3f(pos.x-0.0025,pos.y-0.0025,pos.z-0.0025));
    parentScene->getState().model =
            glm::scale(parentScene->getState().model,vec3f(1.005,1.005,1.005));
    parentScene->getState().updateShaderUniforms(parentScene->getShader("MODEL"));
    parentScene->getShader("MODEL").use();
    glLineWidth(1.5);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor4f(0.0,0.0,0.0,0.5);
    glVertexPointer(3, GL_INT, 0, &vertexPoints[0]);
    glDrawElements(GL_LINES,24,GL_UNSIGNED_INT,&indexes[0]);
    glDisableClientState(GL_VERTEX_ARRAY);
    glColor4f(1.0,1.0,1.0,1.0);
    parentScene->getState().model = poppedMat;
}

const int World::vertexPoints[8][3] = {
    {0,0,1},
    {1,0,1},
    {1,1,1},
    {0,1,1},
    {0,0,0},
    {1,0,0},
    {1,1,0},
    {0,1,0}
};

const int World::indexes[24] = {
    0,1,
    1,2,
    2,3,
    3,0,
    4,5,
    5,6,
    6,7,
    7,4,
    0,4,
    1,5,
    2,6,
    3,7,
};
