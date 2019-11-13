// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "SPE.h"

struct pos {
  int x, y, z;
} typedef pos;


struct chunkData {
  int oreID;
  pos playerPos;
  pos globalChunkPos;
  int chunk[65536];
} typedef chunkData;


struct assData {
  float chunkVal;
  pos chunkID;
  std::vector<pos>* oreLocations;
} typedef aggData;


float calcDistance(pos playerPos, pos chunkPos) {
  float x = pow(playerPos.x - chunkPos.x, 2);
  float y = pow(playerPos.y - chunkPos.y, 2);
  float z = pow(playerPos.z - chunkPos.z, 2);
  float dist = sqrt(x+y+z);
  return dist;
}


pos getBlockPos(int i, pos globalChunkPos) {
  //i = y*16*16 + z*16 + x
  pos p;
  p.y = i/256 + globalChunkPos.y;
  p.z = (i-(p.y*256))/16 + globalChunkPos.z;
  p.x = (i-(p.y*256)-(p.z*16))+ globalChunkPos.x;

  return p;
}


class ChunkSelect : public Operator {
 public:
   ChunkSelect(int r, int s) : Operator(r, s) {}
   void processData(Data data) {}

   void processData() {
     aggData bestAgg = NULL;
     for (Data d : window) {
       if(bestAgg == NULL || bestAgg.chunkVal < (*(aggData*)d.value).chunkVal ) {
         if(bestAgg != NULL) {
           delete bestAgg.oreLocations;
         }
         bestAgg = *(aggData*)d;
       }
       else {
         delete (*(aggData*)d.value).oreLocations;
       }
     }

     emit(Data(bestAgg, sizeof(aggData)));
   }
};


class ChunkProcessor : public Operator {
 public:
  void processData(Data data) {
    chunkData chunk = *(chunkData*)data.value;
    float count = 0;
    aggData dataToPass;
    std::vector<pos> oreLocations = new std::vector<pos>;
    dataToPass.chunkID = chunk.globalChunkPosition;
    dataToPass.oreLocations = &oreLocations;
    for(int i = 0; i<65536 ; i++) {
      if(chunk.chunk[i] == chunk.oreID) {
        count++;
        oreLocations.push_back(getBlockPos(i, chunk.globalChunkPos));
      }
    }

    dataToPass.chunkVal = count/calcDistance(chunk.playerPos, chunk.globalChunkPos);

    emit(Data(dataToPass, sizeof(aggData)));
  }
};


//TODO may or may not be our job to do this
class Generator : public InputSource {
  void generateData() {
  }
};


//TODO pritn data
class PrintOp : public Operator{
  public:
    void processData(Data data){
        aggData bestChunk = *(aggData*)data.value;
        std::vector<pos> ores = *(bestChunk.oreLocations);
        for(int i=0; i<ores.size(); i++){
          std::cout <<"Best Chunk: " << bestChunk.chunkID << endl;
          std::cout <<"Good shit at pos:" << std::endl;
          std::cout << "x: " << ores[i].x << std::endl;
          std::cout << "z: " << ores[i].z << std::endl;
          std::cout << "y: " << ores[i].y << std::endl;
        }
        delete bestChunk.oreLocations;
    }
}

//TODO
int main(int argc, char** argv) {
  std::cout << "SPE Starting up." << std::endl;

  Generator inputSource;
  ChunkProcessor op1;
  ChunkSelect op2;
  PrintOp op3;

  StreamProcessingEngine spe;

  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.connectOperators(&op2, {&op3});

  spe.run();

  std::cout << "SPE Finished." << std::endl;
  return 0;
}
