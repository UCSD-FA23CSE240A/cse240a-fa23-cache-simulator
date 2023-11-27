//========================================================//
//  cache.c                                               //
//  Source file for the Cache Simulator                   //
//                                                        //
//  Implement the I-cache, D-Cache and L2-cache as        //
//  described in the README                               //
//========================================================//

#include "cache.hpp"

//
// TODO:Student Information
//
const char *studentName = "Shiva Surya Seelam";
const char *studentID   = "A59024373";
const char *email       = "sseelam@ucsd.edu";

//------------------------------------//
//        Cache Configuration         //
//------------------------------------//

uint32_t icacheSets;      // Number of sets in the I$
uint32_t icacheAssoc;     // Associativity of the I$
uint32_t icacheBlocksize; // Blocksize of the I$
uint32_t icacheHitTime;   // Hit Time of the I$

uint32_t dcacheSets;      // Number of sets in the D$
uint32_t dcacheAssoc;     // Associativity of the D$
uint32_t dcacheBlocksize; // Blocksize of the D$
uint32_t dcacheHitTime;   // Hit Time of the D$

uint32_t l2cacheSets;     // Number of sets in the L2$
uint32_t l2cacheAssoc;    // Associativity of the L2$
uint32_t l2cacheBlocksize;// Blocksize of the L2$
uint32_t l2cacheHitTime;  // Hit Time of the L2$
uint32_t inclusive;       // Indicates if the L2 is inclusive

uint32_t prefetch;        // Indicate if prefetching is enabled

uint32_t memspeed;        // Latency of Main Memory

//------------------------------------//
//          Cache Statistics          //
//------------------------------------//

uint64_t icacheRefs;       // I$ references
uint64_t icacheMisses;     // I$ misses
uint64_t icachePenalties;  // I$ penalties

uint64_t dcacheRefs;       // D$ references
uint64_t dcacheMisses;     // D$ misses
uint64_t dcachePenalties;  // D$ penalties

uint64_t l2cacheRefs;      // L2$ references
uint64_t l2cacheMisses;    // L2$ misses
uint64_t l2cachePenalties; // L2$ penalties

uint64_t compulsory_miss;  // Compulsory misses on all caches
uint64_t other_miss;       // Other misses (Conflict / Capacity miss) on all caches

//------------------------------------//
//        Cache Data Structures       //
//------------------------------------//

//
//TODO: Add your Cache data structures here
//

// Block Data Structure Definition
typedef struct cache_block
{
  struct cache_block *block_prev_block, *block_next_block;
  uint32_t block_data;
}cache_block;

// Set Data Structure Definition
typedef struct cache_set
{
  cache_block *set_first_block, *set_last_block;
  uint32_t set_size;
};

// Defining Caches and other required parameters
cache_set *icache;
uint32_t ic_offset;
uint32_t ic_index;

cache_set *dcache;
uint32_t dc_offset;
uint32_t dc_index;

cache_set *l2cache;
uint32_t l2c_offset;
uint32_t l2c_index;


//------------------------------------//
//          Cache Functions           //
//------------------------------------//

// Block add function -> adds "block" at the end of "set"

void add_block(cache_set *set, cache_block *block){
  if(set->set_size>0){ 
    /// if this set is not empty add the block at the end of the set
    set->set_last_block->block_next_block = block;
    block->block_prev_block = set->set_last_block;
    set->set_last_block = block;
  }
  else{
    /// set is empty, add the block and update the pointers from NULL
    set->set_first_block = block;
    set->set_last_block = block;
  }
  (set->set_size)++;
}

// Block access function -> provides access to a block in the "set" at a given "index"

cache_block* access_block(cache_set *set, int index){
  if((index > set->set_size) | (index<0)){
    /// If index is invalid return a NULL pointer
    return NULL;
  }

  cache_block *temp_block = set->set_first_block;  /// points to the first block in the "set"
  if(set->set_size==1){
    /// Only one block in the set -> need to make the start and end pointers NULL
    set->set_first_block = NULL;
    set->set_last_block = NULL;
  }
  else if(index == (set->set_size)-1){
    /// Last block in the set -> return last block and update the last second block of the set
    temp_block = set->set_last_block;
    set->set_last_block = set->set_last_block->block_prev_block;
    set->set_last_block->block_next_block = NULL;
  }
  else if(index == 0){
    /// First block in the set -> update the second block of the set
    set->set_first_block = temp_block->block_next_block;
    set->set_first_block->block_prev_block = NULL;
  }
  else {
    for(int i=0; i<index;i++){  /// traversing through the linked list to reach the indexed block;
      temp_block = temp_block->block_next_block;
    }
    /// Rearrange the pointers to remove the indexed block from the "set"
    temp_block->block_prev_block->block_next_block = temp_block->block_next_block;
    temp_block->block_next_block->block_prev_block = temp_block->block_prev_block;
  }
  temp_block->block_next_block = NULL;
  temp_block->block_next_block = NULL;

  return temp_block;
}


// Block remove function -> removes the first block from the "set"

void remove_block(cache_set *set){
  if (set->set_size <=0){
    /// if set is empty nothing needs to be done
    return;
  }

  cache_block *temp_block = set->set_first_block;
  set->set_first_block = temp_block->block_next_block;
  if(set->set_first_block){
    set->set_first_block->block_prev_block = NULL;
  } 
  (set->set_size)--;
  free(temp_block);
}
// Initialize the Cache Hierarchy
//
void
init_cache()
{
  // Initialize cache stats
  icacheRefs        = 0;
  icacheMisses      = 0;
  icachePenalties   = 0;
  dcacheRefs        = 0;
  dcacheMisses      = 0;
  dcachePenalties   = 0;
  l2cacheRefs       = 0;
  l2cacheMisses     = 0;
  l2cachePenalties  = 0;

  compulsory_miss = 0;
  other_miss = 0;
  
  //
  //TODO: Initialize Cache Simulator Data Structures
  //

  icache = (cache_set*)malloc(sizeof(cache_set) * icacheSets);
  dcache = (cache_set*)malloc(sizeof(cache_set) * dcacheSets);
  l2cache = (cache_set*)malloc(sizeof(cache_set) * l2cacheSets);

  int i=0;

  for(i=0 ; i<icacheSets ; i++){
    icache[i].set_first_block = NULL;
    icache[i].set_last_block = NULL;
    icache[i].set_size = 0;
  }

  for(i=0 ; i<icacheSets ; i++){
    dcache[i].set_first_block = NULL;
    dcache[i].set_last_block = NULL;
    dcache[i].set_size = 0;
  }

  for(i=0 ; i<icacheSets ; i++){
    l2cache[i].set_first_block = NULL;
    l2cache[i].set_last_block = NULL;
    l2cache[i].set_size = 0;
  }


}

// Clean Up the Cache Hierarchy
//
void
clean_cache()
{
  //
  //TODO: Clean Up Cache Simulator Data Structures
  //
}

// Perform a memory access through the icache interface for the address 'addr'
// Return the access time for the memory operation
//
uint32_t
icache_access(uint32_t addr)
{
  //
  //TODO: Implement I$
  //
  return memspeed;
}

// Perform a memory access through the dcache interface for the address 'addr'
// Return the access time for the memory operation
//
uint32_t
dcache_access(uint32_t addr)
{
  //
  //TODO: Implement D$
  //
  return memspeed;
}

// Perform a memory access to the l2cache for the address 'addr'
// Return the access time for the memory operation
//
uint32_t
l2cache_access(uint32_t addr)
{
  //
  //TODO: Implement L2$
  //
  return memspeed;
}

// Predict an address to prefetch on icache with the information of last icache access:
// 'pc':     Program Counter of the instruction of last icache access
// 'addr':   Accessed Address of last icache access
// 'r_or_w': Read/Write of last icache access
uint32_t
icache_prefetch_addr(uint32_t pc, uint32_t addr, char r_or_w)
{
  return addr + icacheBlocksize; // Next line prefetching
  //
  //TODO: Implement a better prefetching strategy
  //
}

// Predict an address to prefetch on dcache with the information of last dcache access:
// 'pc':     Program Counter of the instruction of last dcache access
// 'addr':   Accessed Address of last dcache access
// 'r_or_w': Read/Write of last dcache access
uint32_t
dcache_prefetch_addr(uint32_t pc, uint32_t addr, char r_or_w)
{
  return addr + dcacheBlocksize; // Next line prefetching
  //
  //TODO: Implement a better prefetching strategy
  //
}

// Perform a prefetch operation to I$ for the address 'addr'
void
icache_prefetch(uint32_t addr)
{
  //
  //TODO: Implement I$ prefetch operation
  //
}

// Perform a prefetch operation to D$ for the address 'addr'
void
dcache_prefetch(uint32_t addr)
{
  //
  //TODO: Implement D$ prefetch operation
  //
}
