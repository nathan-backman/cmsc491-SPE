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

pos getBlockPos(int i, pos globalChunkPos) {
  //TODO
  //need to find the global position for a block given its index within a chunk
  //i = y*16*16 + z*16 + x
  //fuck
}


class ChunkSelect : public Operator {
 public:
   ChunkSelect(int r, int s) : Operator(r, s) {}
   void processData(Data data) {}

   void processData() {
     aggData bestAgg = NULL;
     for (Data d : window) {
       if(bestAgg == NULL || bestAgg.chunkVal < (*(aggData*)d.value).chunkVal ) {
         bestAgg = *(aggData*)d
       }
     }

     emit(Data(bestAgg, sizeof(aggData) + (sizeof(pos) * bestAgg.oreLocations.capacity()) ));
   }
};

class ChunkProcessor : public Operator {
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

    dataToPass.chunkVal = count/calcDistance(chunk.playerPos, chunk.chunk[0].p);

    emit(Data(dataToPass, sizeof(aggData) + (sizeof(pos) * dataToPass.oreLocations.capacity()) ));
  }
};

//TODO may or may not be our job to do this
class Generator : public InputSource {
  void generateData() {
  }
};

//TODO pritn data

//TODO
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
