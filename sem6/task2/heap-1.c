/* heap-1.c */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define HEAP_BLOCKS 16
#define BLOCK_CAPACITY 1024

enum block_status { BLK_FREE = 0, BLK_ONE, BLK_FIRST, BLK_CONT, BLK_LAST };

struct heap {
  struct block {
    char contents[BLOCK_CAPACITY];
  } blocks[HEAP_BLOCKS];
  enum block_status status[HEAP_BLOCKS];
} global_heap = {0};

struct block_id {
  size_t       value;
  bool         valid;
  struct heap* heap;
};

struct block_id block_id_new(size_t value, struct heap* from) {
  return (struct block_id){.valid = true, .value = value, .heap = from};
}
struct block_id block_id_invalid() {
  return (struct block_id){.valid = false};
}

bool block_id_is_valid(struct block_id bid) {
  return bid.valid && bid.value < HEAP_BLOCKS;
}

/* Find block */

bool block_is_free(struct block_id bid) {
  if (!block_id_is_valid(bid))
    return false;
  return bid.heap->status[bid.value] == BLK_FREE;
}

// >>>
enum block_status get_status(struct block_id bid) {
    return bid.heap->status[bid.value];
}

struct block_id set_status(struct block_id bid, enum block_status status) {
    bid.heap->status[bid.value] = status;
    return bid;
}
// <<<

/* Allocate */
//? ? ?
struct block_id try_alloc_many(struct block_id startBid, size_t size) {
    for (size_t j = startBid.value+1; j < HEAP_BLOCKS; j++) {
        struct block_id currBid = block_id_new(j, startBid.heap);
        if (get_status(currBid)!=BLK_FREE) return (struct block_id) {.valid=false, .value=j};
        if (size <= BLOCK_CAPACITY*(j-startBid.value+1)) {
            set_status(startBid, BLK_FIRST);
            set_status(currBid, BLK_LAST);
            for (size_t k = startBid.value+1; k < currBid.value; k++) {
                set_status(block_id_new(k, startBid.heap), BLK_CONT);
            }
            return startBid;
        }
    }
    return (struct block_id) {.valid=false, .value=HEAP_BLOCKS};
}

struct block_id block_allocate(struct heap* heap, size_t size) {
    // >>>
    for (size_t i = 0; i < HEAP_BLOCKS; i++) {
        struct block_id bid = block_id_new(i, heap);
        if (block_is_free(bid)) {
            if (size <= BLOCK_CAPACITY) return set_status(bid, BLK_ONE);
            struct block_id res = try_alloc_many(bid, size);
            if (res.valid) return res;
            i = res.value;
        }
    }
    // <<<
    return block_id_invalid();
}
/* Free */
// ? ? ?
void block_free(struct block_id bid) {
    if (! bid.valid) return;
    enum block_status status = get_status(bid);
    switch (status) {
    case BLK_ONE:
        set_status(bid, BLK_FREE);
        return;
    case BLK_FIRST:
        set_status(bid, BLK_FREE);
        for (size_t i = bid.value+1; i < HEAP_BLOCKS; i++) {
            struct block_id currBid = block_id_new(i, bid.heap);
            enum block_status currStatus = get_status(currBid);
            if (currStatus!=BLK_CONT && currStatus!=BLK_LAST) return;
            set_status(currBid, BLK_FREE);
        }
    default:
        return;
    }
}

/* Printer */
const char* block_repr(struct block_id b) {
  static const char* const repr[] = {[BLK_FREE] = " .",
                                     [BLK_ONE] = " *",
                                     [BLK_FIRST] = "[=",
                                     [BLK_LAST] = "=]",
                                     [BLK_CONT] = " ="};
  if (b.valid)
    return repr[b.heap->status[b.value]];
  else
    return "INVALID";
}

void block_debug_info(struct block_id b, FILE* f) {
  fprintf(f, "%s", block_repr(b));
}

void block_foreach_printer(struct heap* h, size_t count,
                           void printer(struct block_id, FILE* f), FILE* f) {
  for (size_t c = 0; c < count; c++)
    printer(block_id_new(c, h), f);
}

void heap_debug_info(struct heap* h, FILE* f) {
  block_foreach_printer(h, HEAP_BLOCKS, block_debug_info, f);
  fprintf(f, "\n");
}
/*  -------- */


void test1() {
  fprintf(stdout, "Test 1 started!\n");
  struct block_id bid1 = block_allocate(&global_heap, BLOCK_CAPACITY);
  struct block_id bid2 = block_allocate(&global_heap, 4*BLOCK_CAPACITY);
  struct block_id bid3 = block_allocate(&global_heap, BLOCK_CAPACITY);

  heap_debug_info(&global_heap, stdout);

  block_free(bid2);
  heap_debug_info(&global_heap, stdout);

  block_free(bid1);
  block_free(bid3);
  heap_debug_info(&global_heap, stdout);
}
void test2() {
  fprintf(stdout, "Test 2 started!\n");
  struct block_id bid1 = block_allocate(&global_heap, BLOCK_CAPACITY);
  struct block_id bid2 = block_allocate(&global_heap, 4*BLOCK_CAPACITY);
  struct block_id bid3 = block_allocate(&global_heap, BLOCK_CAPACITY);

  block_free(bid2);

  struct block_id bid4 = block_allocate(&global_heap, 6*BLOCK_CAPACITY);
  heap_debug_info(&global_heap, stdout);

  block_free(bid1);
  block_free(bid3);
  block_free(bid4);
  heap_debug_info(&global_heap, stdout);
}
void test3() {
  fprintf(stdout, "Test 3 started!\n");
  struct block_id bid1 = block_allocate(&global_heap, BLOCK_CAPACITY);
  struct block_id bid2 = block_allocate(&global_heap, 4*BLOCK_CAPACITY);
  struct block_id bid3 = block_allocate(&global_heap, BLOCK_CAPACITY);

  block_free(bid2);

  struct block_id bid4 = block_allocate(&global_heap, 11*BLOCK_CAPACITY);
  heap_debug_info(&global_heap, stdout);

  block_free(bid1);
  block_free(bid3);
  block_free(bid4);
  heap_debug_info(&global_heap, stdout);
}

int main() {
  heap_debug_info(&global_heap, stdout);
  test1();
  test2();
  test3();
  return 0;
}
