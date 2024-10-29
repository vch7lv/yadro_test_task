#pragma once
#include "Tape.h"
#include <memory>
#include <cassert>
#include <vector>
#include <algorithm>

class TapeSorter 
{
    public:
        TapeSorter(std::size_t chunk_size);

        void sort(std::shared_ptr<Tape> in, std::shared_ptr<Tape> out);

        void set_chunk_size(size_t new_size);
        size_t get_chunk_size();

    private:

        std::vector<int> buf;
        size_t chunk_size = 0;

        void copy_tape(std::shared_ptr<Tape> in, std::shared_ptr<Tape> out);
        void sort_blocks(std::shared_ptr<Tape> tape, size_t blocks_l, size_t blocks_size);
        void sort_block(std::shared_ptr<Tape> tape, size_t block_l, size_t block_size);
        void read_from_tape_to_buf(std::shared_ptr<Tape> tape, size_t buf_l, size_t tape_l, size_t size);
        void write_to_tape_from_buf(std::shared_ptr<Tape> tape, size_t buf_l, size_t tape_l, size_t size);
        void move_head_to_position (std::shared_ptr<Tape> tape, std::size_t pos);
};