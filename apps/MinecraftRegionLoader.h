#ifndef MINECRAFTREGIONLOADER_H
#define MINECRAFTREGIONLOADER_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <zlib.h>
#include <vector>
#include <string.h>
#include "MinecraftBlocks.h"

#define CHUNK_COUNT 1024
#define SECTOR_SIZE 4096
#define SEG_SIZE 16384

struct pos {
  int x, y, z;
} typedef pos;

struct ChunkData {
  uint8_t oreID;
  pos playerPos;
  pos globalChunkPos;
  bool empty;
  uint8_t chunk[65536];
  ChunkData(): empty(false) {}
} typedef ChunkData;

void die(std::string str) {
  std::cout << "Error: " << str << std::endl;
  exit(0);
}

class MinecraftRegionLoader {
 private:
  struct ChunkInfo {
    unsigned int modified;
    unsigned int offset;
    unsigned int length;
    char type;
    std::vector<uint8_t> data;
  }typedef ChunkInfo;

  ChunkInfo chunkInfo[CHUNK_COUNT];
  std::ifstream file;
  pos playerPos;

  enum TYPE { GZIP = 1, ZLIB };
  const char *Palette = "Palette";
  const char *Minecraft = "minecraft:";
  const char *BlockStates = "BlockStates";
  
  int regionX;
  int regionZ;


 public:
  MinecraftRegionLoader(pos playerPos) : playerPos(playerPos) {
    regionX = (playerPos.x / 16) / 32;
    regionZ = (playerPos.z / 16) / 32;

    char regionFileName[64];
    sprintf(regionFileName, "r.%d.%d.mca", regionX, regionZ);
    std::cout << "Reading region file [" << regionFileName << "]" << std::endl;

    // attempt to open file
    file.open(regionFileName, std::ios::in | std::ios::binary);
    if(!file.is_open())
      die("Failed to open file");


    read_header();
    read_chunks();
  }

  std::vector<ChunkData*> extractChunkData() {
    std::vector<ChunkData*> chunks;

    int chunkCount = 0;
    for(int i=0; i < 1024; i++) {
      ChunkInfo &info = chunkInfo[i];

      if (info.offset == 0) {
        ChunkData *chunkData = new ChunkData;
        chunkData->empty = true;
        chunks.push_back(chunkData);
        continue;
      }

      chunkCount++;

      ChunkData *chunkData = new ChunkData;
      chunkData->playerPos = playerPos;

      int chunkX = (regionX*16*32) + (i%32)*16;
      int chunkZ = (regionZ*16*32) + (i/32)*16;
      chunkData->globalChunkPos = {chunkX, 0, chunkZ};


      // Get handy reference to block data an initialize it
      uint8_t *blockData = chunkData->chunk;
      memset(blockData, blockID["minecraft:unlisted"], 65536);
      int blockIndex = 0;

      uint8_t *dataBegin = &*info.data.begin();
      uint8_t *dataEnd = &*info.data.end();


      uint8_t *palettePtr = &*info.data.begin();
      uint8_t *blockStatesPtr = palettePtr;


      // Parse each section
      while (true) {

        // Advance pointers to the relevant structures for the next section
        blockStatesPtr = std::search(blockStatesPtr+1, dataEnd, BlockStates, BlockStates+strlen(BlockStates));
        palettePtr = std::search(palettePtr+1, dataEnd, Palette, Palette+strlen(Palette));

        // Bail out if we reach the end
        if (blockStatesPtr == dataEnd || palettePtr == dataEnd)
          break;

        ////////////////////////////////
        //
        // Parse and reset the palette
        //

        // Provision new translation table
        std::vector<int> translate(16,66);
        uint8_t *it = palettePtr;
        uint8_t *nextPalette = std::search(palettePtr+1, dataEnd, Palette, Palette+strlen(Palette));
        int pIndex = 0;
        //printf("palette ----------------------\n");
        bool skipThisPalette = false;
        while ((it = std::search(it+1, nextPalette, Minecraft, Minecraft+10)) != nextPalette) {
          // Get BlockID name as string
          uint16_t len = *(uint16_t*)(it-2);
          len = __builtin_bswap16(len);
          std::string blockType(it, it+len);

          // Bail if our palette has more than 16 items in it
          if (pIndex >= 16) {
            skipThisPalette = true;
            break;
          }

          // Associate block type with our ID
          auto idx = blockID.find(blockType);
          if (idx != blockID.end())
            translate[pIndex] = idx->second;
          //std::cout << " - " << blockType << " - " << translate[pIndex] << std::endl;

          pIndex++;
        }
        //std::cout << "Total items: " << pIndex << std::endl << std::endl;

        // Bail if our palette has more than 16 items in it
        if (skipThisPalette) {
          blockIndex += 4096;
          continue;
        }


        ////////////////////////////////
        //
        // Parse and reset the palette
        //

        // Get a pointer to the BlockStates data
        uint32_t blocksSize = *(uint32_t*)(blockStatesPtr + 11);
        blocksSize = __builtin_bswap32(blocksSize);
        uint8_t blocksTagID = *(blockStatesPtr - 3);


        // Fetch the bytes in multiples of 8
        uint64_t *llData = (uint64_t*)(blockStatesPtr + 15);
        for(int j=0; j < 256; j++) {
          *llData = __builtin_bswap64(*llData);
          for(int k=0; k < 8; k++) {
            uint8_t byte = *(((uint8_t*)llData)+k);
            blockData[blockIndex++] = translate[byte & 0x0F];
            blockData[blockIndex++] = translate[(byte >> 4)];
          }
          llData++;
        }

      }


      //// Print out the first section of the chunk
      //for(int y=0; y < 16; y++) {
      //  for(int z=0; z < 16; z++) {
      //    for(int x=0; x < 16; x++)
      //      printf("%2d ", (int)blockData[y*256 + z*16 + x]);
      //    printf("\n");
      //  }
      //  printf("\n");
      //}

      //printf("Chunk [x:%d,z:%d] allocated\n", i%32, i/32);

      chunks.push_back(chunkData);
    }

    std::cout << "REAL chunkCount: " << chunks.size() << std::endl;
    return chunks;
  }

 private:
  bool inflate_(std::vector<uint8_t> &data) {
    int ret;
    z_stream zs;
    char buff[SEG_SIZE];
    std::vector<uint8_t> out_data;
    unsigned long prev_out = 0;

    // initialize zlib structure
    memset(&zs, 0, sizeof(zs));
    if(inflateInit(&zs) != Z_OK)
      return false;

    zs.next_in = (Bytef *) data.data();
    zs.avail_in = data.size();

    // inflate blocks
    do {
      zs.next_out = reinterpret_cast<Bytef *>(buff);
      zs.avail_out = SEG_SIZE;

      // inflate data and place in out_data
      ret = inflate(&zs, 0);
      out_data.insert(out_data.end(), buff, buff + (zs.total_out - prev_out));
      prev_out = zs.total_out;
    } while(ret == Z_OK);

    // check for errors
    inflateEnd(&zs);
    if (ret != Z_STREAM_END)
      return false;

    // assign to data
    data.swap( out_data );
    return true;
  }



  /*
   * Reads header data from a file
   */
  void read_header() {
    char type;
    int value;
    unsigned int offset;

    // check if file is open
    if(!file.is_open())
      die("Failed to open file");

    // read position data into header
    for(unsigned int i = 0; i < CHUNK_COUNT; ++i) {
      file.read(reinterpret_cast<char *>(&value), sizeof(value));
      value = __builtin_bswap32(value);
      chunkInfo[i].offset = value;
      //reg.get_header().get_info_at(i).set_offset(value);
    }

    // read timestamp data into header
    for(unsigned int i = 0; i < CHUNK_COUNT; ++i) {
      file.read(reinterpret_cast<char *>(&value), sizeof(value));
      value = __builtin_bswap32(value);
      chunkInfo[i].modified = value;
      //reg.get_header().get_info_at(i).set_modified(value);
    }

    // read length and compression type data into header
    for(unsigned int i = 0; i < CHUNK_COUNT; ++i) {
      offset = chunkInfo[i].offset;
      //offset = reg.get_header().get_info_at(i).get_offset();

      // skip all empty chunks
      if(!offset)
        continue;

      // collect length and compression data
      file.seekg((offset >> 8) * SECTOR_SIZE, std::ios::beg);
      file.read(reinterpret_cast<char *>(&value), sizeof(value));
      value = __builtin_bswap32(value);

      chunkInfo[i].length = value;
      //reg.get_header().get_info_at(i).set_length(value);

      file.read(&type, sizeof(type));

      chunkInfo[i].type = type;
      //reg.get_header().get_info_at(i).set_type(type);

      chunkInfo[i].offset = (unsigned int) file.tellg();
      //reg.get_header().get_info_at(i).set_offset((unsigned int) file.tellg());
    }
  }

  /*
   * Reads chunk data from a file
   */
  void read_chunks() {

    // check if file is open
    if(!file.is_open())
      die("Failed to open file");

    // iterate though header entries, reading in chunks if they exist
    for(unsigned int i = 0; i < CHUNK_COUNT; ++i) {
      ChunkInfo &info = chunkInfo[i];

      // skip empty chunks
      if(!(info.offset)) {
        continue;
      }

      //// Retrieve raw data
      info.data.resize(info.length);
      file.seekg(info.offset, std::ios::beg);
      file.read((char *) &(info.data[0]), info.length);

      // check for compression type
      switch(info.type) {
      case GZIP:
        die("Unsupported compression type");
        break;
      case ZLIB:
        inflate_(info.data);
        break;
      default:
        die("Unknown compression type");
        break;
      }
    }
  }




};

#endif /* MINECRAFTREGIONLOADER_H */
