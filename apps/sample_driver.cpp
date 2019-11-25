// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "SPE.h"
#include "TCPListener.h"

struct pos {
  int x, y, z;
} typedef pos;


struct chunkData {
  uint8_t oreID;
  pos playerPos;
  pos globalChunkPos;
  uint8_t chunk[65536];
} __attribute__((packed)) typedef chunkData;


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

  p.y = rely + globalChunkPos.y;
  p.z = relz + globalChunkPos.z;
  p.x = relx + globalChunkPos.x;

  return p;
}


class ChunkSelect : public Operator {
  public:
    ChunkSelect(int r, int s) : Operator(r, s) {}
    void processData(Data data) {
      std::cout << "chunk select recv one data" << std::endl;
      emit(Data(&data, sizeof(aggData)));
      std::cout << "chunk select emit data" << std::endl;
    }

    void processData() {
      std::cout << "chunk select recv data" << std::endl;
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

      std::cout << "chunk select emit data" << std::endl;
      emit(Data(&bestAgg, sizeof(aggData)));
    }
};


class ChunkProcessor : public Operator {
  public:
    void processData(Data data) {
      std::cout << "chunk procesessor recv data" << std::endl;
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

      std::cout << "chunk procesessor emit data" << std::endl;
      emit(Data(&dataToPass, sizeof(aggData)));
    }
};


class Generator : public InputSource {
  void generateData() {

    TCPListener listener;
    listener.Bind(12345);
    std::cout << "Waiting for connections on 12345..." << std::endl;
    std::cout << "RecvData size: " << sizeof(chunkData) << std::endl;

    listener.WaitForConnection();
    chunkData recvData;
    while ( listener.GetData((char*)&recvData, sizeof(recvData)) ) {
      std::cout << "Connection received." << std::endl;

      std::cout << "oreID: " << (int)recvData.oreID << std::endl;
      std::cout << "playerPos.x: " << recvData.playerPos.x << std::endl;
      std::cout << "playerPos.y: " << recvData.playerPos.y << std::endl;
      std::cout << "playerPos.z: " << recvData.playerPos.z << std::endl;
      std::cout << "globalChunkPos.x: " << recvData.globalChunkPos.x << std::endl;
      std::cout << "globalChunkPos.y: " << recvData.globalChunkPos.y << std::endl;
      std::cout << "globalChunkPos.z: " << recvData.globalChunkPos.z << std::endl;
      //std::cout << "chunk[0]: " << (int)recvData.chunk[0] << std::endl;
      //std::cout << "chunk[1]: " << (int)recvData.chunk[1] << std::endl;
      //std::cout << "chunk[2]: " << (int)recvData.chunk[2] << std::endl;

      recvData.oreID = 26;

      //std::cout << "10 64 7: " << (int)recvData.chunk[16762-256] << std::endl;
      //std::cout << "10 65 7: " << (int)recvData.chunk[16762] << std::endl;
      //std::cout << "10 66 7: " << (int)recvData.chunk[16762+256] << std::endl;
      //std::cout << "10 67 7: " << (int)recvData.chunk[16762+512] << std::endl;
      emit(Data(&recvData, sizeof(chunkData)));
      std::cout << "Waiting for connections on 12345..." << std::endl;
    }
      
  }
};


class PrintOp : public Operator{
  public:
    void processData(Data data){
      std::cout << "chunk print recv data" << std::endl;
      aggData bestChunk = *(aggData*)data.value;
      std::vector<pos> ores = *(bestChunk.oreLocations);
      std::cout << "ores size: " << ores.size() << std::endl;
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
  ChunkSelect op2(1, 1);
  PrintOp op3;

  StreamProcessingEngine spe;

  spe.addInputSource(&inputSource, {&op1});
  //spe.connectOperators(&op1, {&op2});
  spe.connectOperators(&op1, {&op3});

  spe.run();

  std::cout << "SPE Finished." << std::endl;
  return 0;
}
