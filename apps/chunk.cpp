// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "SPE.h"

struct pos {
  int x, y, z;
} typedef pos;

struct block {
  int id;
  pos p;
} typedef block;

struct chunkData {
  vector<block> chunk;
  int oreID;
  pos playerPos;
} typedef chunkData;

struct assData {
  int chunkID;
  float chunkVal;
  vector<pos> oreLocations;
} typedef aggData;

float calcDistance(pos playerPos, pos chunkPos) {
  float x = pow(playerPos.x - chunkPos.x, 2);
  float y = pow(playerPos.y - chunkPos.y, 2);
  float z = pow(playerPos.z - chunkPos.z, 2);
  float dist = sqrt(x+y+z);
  return dist;
}


class ChunkProcessor : public Operator {
 public:
  void processData(Data data) {
  }
};

class ChunkSelect : public Operator {
 public:
  void processData(Data data) {
    chunkData chunk = *(chunkData*)data.value;
    float count = 0;
    aggData dataToPass;
    dataToPass.chunkID = chunk.chunk[0];
    for(auto b : chunk.chunk) {
      if(b.id == chunk.oreID) {
        count++;
        dataToPass.oreLocations.push_back(b.p);
      }
    }
    //TODO calculate distance from player to chunk
    //TODO set chunkVal and emit
    dataToPass.chunkVal = count/calcDistance(chunk.playerPos, chunk.chunk[0].p);
    //FIXME wtf is size??
    emit(dataToPass, sizeof(aggData));
  }
};

//TODO
class Generator : public InputSource {
  void generateData() {
  }
};

int main(int argc, char** argv) {
  std::cout << "SPE Starting up." << std::endl;

  NumberGenerator inputSource;
  IncrementOp op1;
  OutputOp op2;

  StreamProcessingEngine spe;

  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});

  spe.run();

  std::cout << "SPE Finished." << std::endl;
  return 0;
}
