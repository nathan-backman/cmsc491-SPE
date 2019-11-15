// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "SPE.h"

struct pos {
  int x, y, z;
} typedef pos;


struct chunkData {
  uint8_t oreID;
  pos playerPos;
  pos globalChunkPos;
  uint8_t chunk[65536];
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
  int relx, rely, relz, diff;

  rely = i/256;
  diff = i-(rely*256);
  relx = diff % 16;
  relz = (diff - relx)/16;

  p.y = rely+ globalChunkPos.y;
  p.z = relz+ globalChunkPos.z;
  p.x = relx+ globalChunkPos.x;

  return p;
}


class ChunkSelect : public Operator {
  public:
    ChunkSelect(int r, int s) : Operator(r, s) {}
    void processData(Data data) {}

    void processData() {
      aggData bestAgg;
      bestAgg.chunkVal = -1;
      for (Data d : window) {
        if(bestAgg.chunkVal == -1 || bestAgg.chunkVal < (*(aggData*)d.value).chunkVal ) {
          if(bestAgg.chunkVal != -1) {
            delete bestAgg.oreLocations;
          }
          bestAgg = *(aggData*)(d.value);
        }
        else {
          delete (*(aggData*)d.value).oreLocations;
        }
      }

      emit(Data(&bestAgg, sizeof(aggData)));
    }
};


class ChunkProcessor : public Operator {
  public:
    void processData(Data data) {
      chunkData chunk = *(chunkData*)data.value;
      float count = 0;
      aggData dataToPass;
      std::vector<pos>* oreLocations = new std::vector<pos>;
      dataToPass.chunkID = chunk.globalChunkPos;
      dataToPass.oreLocations = oreLocations;
      for(int i = 0; i<65536 ; i++) {
        if(chunk.chunk[i] == chunk.oreID){
          count++;
          oreLocations->push_back(getBlockPos(i, chunk.globalChunkPos));
        }
      }

      dataToPass.chunkVal = count/calcDistance(chunk.playerPos, chunk.globalChunkPos);

      emit(Data(&dataToPass, sizeof(aggData)));
    }
};


class Generator : public InputSource {
  void generateData() {
    //open a handle to file 
    std::ofstream out;
    out.open("dataInput.data");

    pos playerPos;
    playerPos.x = 50;
    playerPos.y = 123;
    playerPos.z = 163;

    out << playerPos.x << " ";
    out << playerPos.z << " ";
    out << playerPos.y << std::endl;
    
    for(int i=0; i<3; i++){
      chunkData data;
      data.playerPos = playerPos;
      data.oreID = 5;
      data.globalChunkPos.x = rand()%512;
      data.globalChunkPos.z = rand()%512;
      data.globalChunkPos.y = 0;

      for(int j=0; j<65536; j++){
        data.chunk[j] = (uint8_t)rand()%65;
      }
      
      emit(Data(&data, sizeof(chunkData)));
    }

    out.close();
  }
};


class PrintOp : public Operator{
  public:
    void processData(Data data){
      aggData bestChunk = *(aggData*)data.value;
      std::vector<pos> ores = *(bestChunk.oreLocations);
      for(int i=0; i<ores.size(); i++){
       std::cout <<"Chunk: " << bestChunk.chunkID.x << " " << bestChunk.chunkID.z << " " << bestChunk.chunkID.y << std::endl;
       std::cout <<"Good shit at pos:" << std::endl;
       std::cout << "x: " << ores[i].x << std::endl;
       std::cout << "z: " << ores[i].z << std::endl;
       std::cout << "y: " << ores[i].y << std::endl;
      }
      delete bestChunk.oreLocations;
    }
};

int main(int argc, char** argv) {
  std::cout << "SPE Starting up." << std::endl;

  Generator inputSource;
  ChunkProcessor op1;
  ChunkSelect op2(3, 3);
  PrintOp op3;

  StreamProcessingEngine spe;

  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.connectOperators(&op2, {&op3});

  spe.run();

  std::cout << "SPE Finished." << std::endl;
  return 0;
}
