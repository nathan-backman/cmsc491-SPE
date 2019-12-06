// Copyright 2019 [BVU CMSC491 class]
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include "SPE.h"
#include "TCPListener.h"
#include "MinecraftRegionLoader.h"


TCPListener listener;

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
      //std::cout << "chunk procesessor recv data" << std::endl;
      ChunkData &chunk = *(ChunkData*)data.value;

      // Handle an empty chunk
      if (chunk.empty == true) {
        aggData dataToPass;
        dataToPass.oreLocations = new std::vector<pos>;
        dataToPass.chunkVal = 0;
         
        //std::cout << "[empty] chunk procesessor emit data" << std::endl;
        emit(Data(&dataToPass, sizeof(aggData)));
        return;
      }

      // Handle an non-empty chunk
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

      std::cout << "[non-empty] chunk procesessor emit data with #ores:" << oreLocations->size() << std::endl;
      emit(Data(&dataToPass, sizeof(aggData)));
    }
};


class Generator : public InputSource {
  void generateData() {

    std::cout << "Waiting for data from mod..." << std::endl;

    std::string strPos;
    while ( (strPos = listener.GetLine()) != "") {
      std::istringstream is(strPos);
      pos playerPos;
      is >> playerPos.x >> playerPos.y >> playerPos.z;

      //pos playerPos = {0,0,0};
      MinecraftRegionLoader loader(playerPos);
      std::vector<ChunkData*> chunks = loader.extractChunkData();

      for(int i=0; i < chunks.size(); i++) {
        chunks[i]->oreID = 26;
        emit(Data(chunks[i], sizeof(ChunkData)));
      }

      std::cout << "Produced " << chunks.size() << "chunks. Waiting for more data from mod..." << std::endl;
    }

    std::cout << "Minecraft Mod Disconnected" << std::endl;
  }
};


class PrintOp : public Operator{
  public:
    void processData(Data data){

      //std::cout << "chunk print recv data" << std::endl;
      aggData bestChunk = *(aggData*)data.value;
      std::vector<pos> ores = *(bestChunk.oreLocations);
      //std::cout << "ores size: " << ores.size() << std::endl;
      for(int i=0; i<ores.size(); i++){
       std::cout <<"Chunk: " << bestChunk.chunkID.x << " " << bestChunk.chunkID.z << " " << bestChunk.chunkID.y << std::endl;
       std::cout <<"Good shit at pos:" << std::endl;
       std::cout << "x: " << ores[i].x << std::endl;
       std::cout << "z: " << ores[i].z << std::endl;
       std::cout << "y: " << ores[i].y << std::endl;
      }

      if (ores.empty() == false) {
        std::ostringstream os;
        os << "Get ore at " << ores[0].x << " " << ores[0].y << " " << ores[0].z;
        string msg = os.str();
        uint8_t msgSz = msg.size();

        listener.SendData((char*)&msgSz, sizeof(msgSz));
        listener.SendData(msg.c_str(), msgSz);
      }

      delete bestChunk.oreLocations;
    }
};

int main(int argc, char** argv) {
  std::cout << "SPE Starting up." << std::endl;

  int port = 12345;
  listener.Bind(port);

  std::cout << "Waiting for connection..." << std::endl;
  if (listener.WaitForConnection() <= 0) {
    std::cout << "Error - failed to connect" << std::endl;
    return 0;
  }

  Generator inputSource;
  ChunkProcessor op1;
  ChunkSelect op2(1024, 1024);
  PrintOp op3;

  StreamProcessingEngine spe;

  spe.addInputSource(&inputSource, {&op1});
  spe.connectOperators(&op1, {&op2});
  spe.connectOperators(&op2, {&op3});

  spe.run();

  std::cout << "SPE Finished." << std::endl;
  return 0;
}
