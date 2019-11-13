#include <iostream>
#include <map>

std::map<std::string,int> blockID = {
  {"minecraft:acacia_leaves", 0},
  {"minecraft:acacia_log", 1},
  {"minecraft:air", 2},
  {"minecraft:andesite", 3},
  {"minecraft:azure_bluet", 4},
  {"minecraft:bedrock", 5},
  {"minecraft:birch_planks", 6},
  {"minecraft:brown_mushroom", 7},
  {"minecraft:cave_air", 8},
  {"minecraft:chest", 9},
  {"minecraft:clay", 10},
  {"minecraft:coal_ore", 11},
  {"minecraft:cobblestone", 12},
  {"minecraft:cobblestone_slab", 13},
  {"minecraft:cobblestone_stairs", 14},
  {"minecraft:cobblestone_wall", 15},
  {"minecraft:cobweb", 16},
  {"minecraft:cornflower", 17},
  {"minecraft:dandelion", 18},
  {"minecraft:dark_oak_fence", 19},
  {"minecraft:dark_oak_leaves", 20},
  {"minecraft:dark_oak_log", 21},
  {"minecraft:dark_oak_planks", 22},
  {"minecraft:dark_oak_slab", 23},
  {"minecraft:dark_oak_stairs", 24},
  {"minecraft:diamond_block", 25},
  {"minecraft:diamond_ore", 26},
  {"minecraft:diorite", 27},
  {"minecraft:dirt", 28},
  {"minecraft:gold_ore", 29},
  {"minecraft:granite", 30},
  {"minecraft:grass", 31},
  {"minecraft:grass_block", 32},
  {"minecraft:gravel", 33},
  {"minecraft:iron_ore", 34},
  {"minecraft:lapis_ore", 35},
  {"minecraft:lava", 36},
  {"minecraft:lilac", 37},
  {"minecraft:mossy_cobblestone", 38},
  {"minecraft:mossy_cobblestone_stairs", 39},
  {"minecraft:mushroom_stem", 40},
  {"minecraft:oak_fence", 41},
  {"minecraft:oak_leaves", 42},
  {"minecraft:oak_log", 43},
  {"minecraft:oak_planks", 44},
  {"minecraft:obsidian", 45},
  {"minecraft:oxeye_daisy", 46},
  {"minecraft:poppy", 47},
  {"minecraft:rail", 48},
  {"minecraft:red_mushroom", 49},
  {"minecraft:red_mushroom_block", 50},
  {"minecraft:redstone_ore", 51},
  {"minecraft:sand", 52},
  {"minecraft:seagrass", 53},
  {"minecraft:spawner", 54},
  {"minecraft:stone", 55},
  {"minecraft:sugar_cane", 56},
  {"minecraft:sunflower", 57},
  {"minecraft:tall_grass", 58},
  {"minecraft:tall_seagrass", 59},
  {"minecraft:torch", 60},
  {"minecraft:vine", 61},
  {"minecraft:wall_torch", 62},
  {"minecraft:water", 63},
  {"minecraft:white_wall_banner", 64},
  {"minecraft:emerald_ore", 65},
  {"minecraft:unlisted", 66},
};

int main() {
  std::cout << "Hello" << std::endl;

  std::cout << "diamond_ore is " << blockID["minecraft:diamond_ore"] << std::endl;

  // to test if a block is diamond
  // int blockOfInterest = blockID["minecraft:diamond_ore"];
  // if (chunkBlock[y*16*16 + z*16 + x] == blockOfInterest) { ... }
  // if (chunkBlock[i] == blockOfInterest) { ... }

  return 0;
}
