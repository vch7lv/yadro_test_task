#include "TapeSorter.h"
#include <stdexcept>

#include <iostream>


TapeSorter::TapeSorter(std::size_t chunk_size) : chunk_size(chunk_size) 
{}

void TapeSorter::sort(std::shared_ptr<Tape> in, std::shared_ptr<Tape> out)
{
    if (in == nullptr || out == nullptr)
    {
        return;
    }

    if (in->get_capacity() > out->get_capacity())
    {
        throw(std::runtime_error("out tape smaller than in tape"));
    }

    size_t n = in->get_capacity();
    size_t l = 0;
    
    assert(chunk_size >= 8);
    size_t buf_size = std::min(chunk_size/4, n+1);
    if (buf_size % 2 == 1) 
    {
        --buf_size;
    }

    buf.resize(buf_size);

    copy_tape(in, out);
    
    in->rewind();
    out->rewind();

    while ( n > buf.size())
    {
        sort_blocks(out, l, n);
    

        read_from_tape_to_buf(out, 0, l, buf.size() / 2);

        for (size_t block_l = buf.size(); block_l < n; block_l += buf.size())
        {
            size_t half_block_size = std::min(buf.size() / 2, n - block_l );

            if (half_block_size != 0) {
                read_from_tape_to_buf(out, buf.size() / 2, l + block_l, half_block_size); 
                std::sort(buf.begin(), buf.begin() + buf.size() / 2 + half_block_size);
                write_to_tape_from_buf(out, buf.size() / 2, l + block_l, half_block_size); 
            }
        }

        write_to_tape_from_buf(out, 0, l, buf.size() / 2);

        n -= buf.size() / 2;
        l += buf.size() / 2;
    }

    read_from_tape_to_buf(out, 0, l, n);
    std::sort(buf.begin(), buf.begin() + n);
    write_to_tape_from_buf(out, 0, l, n);


    buf.clear();
}

void TapeSorter::copy_tape(std::shared_ptr<Tape> in, std::shared_ptr<Tape> out)
{
    assert(in->get_capacity() <= out->get_capacity());

    size_t n = in->get_capacity();

    for (size_t i = 0; i < n-1; ++i)
    {
        out->write_on_head(in->read_from_head());
        out->head_move_right();
        in->head_move_right();
    }

    out->write_on_head(in->read_from_head());
    in->rewind();
    out->rewind();
}

void TapeSorter::sort_blocks
    (std::shared_ptr<Tape> tape, size_t blocks_l, size_t blocks_size)
{
    while (blocks_size != 0) 
    {
        size_t block_size = std::min(buf.size(), blocks_size);

        sort_block(tape, blocks_l, block_size);

        blocks_l += block_size;
        blocks_size -= block_size;
    }
}

void TapeSorter::sort_block(std::shared_ptr<Tape> tape, size_t block_l, size_t block_size)
{
    assert(block_size <= buf.size());
    assert(block_l + block_size <= tape->get_capacity());

    if (block_size == 0) return;

    read_from_tape_to_buf(tape, 0, block_l, block_size);

    std::sort(buf.begin(), buf.begin() + block_size);

    write_to_tape_from_buf(tape, 0, block_l, block_size);
}

void TapeSorter::read_from_tape_to_buf
    (std::shared_ptr<Tape> tape, size_t buf_l, size_t tape_l, size_t size)
{
    assert(buf_l + size <= buf.size());
    assert(tape_l + size <= tape->get_capacity());

    if (size == 0) return;

    size_t tape_r = tape_l + size - 1;
    size_t buf_r = buf_l + size - 1;

    long long head_pos = tape->get_head_position();

    bool in_order = abs(head_pos - tape_l) <= abs(head_pos - tape_r);

    if (in_order)
    {
        move_head_to_position(tape, tape_l);

        for (int i = 0; i < size-1; ++i)
        {
            buf[buf_l+i] = tape->read_from_head();
            tape->head_move_right();
        }
        buf[buf_r] = tape->read_from_head();
    } 
    else 
    {
        move_head_to_position(tape, tape_r);

        for (int i = 0; i < size-1; ++i)
        {
            buf[buf_r-i] = tape->read_from_head();
            tape->head_move_left();
        }
        buf[buf_l] = tape->read_from_head();
    }
}

void TapeSorter::write_to_tape_from_buf
    (std::shared_ptr<Tape> tape, size_t buf_l, size_t tape_l, size_t size)
{
    assert(buf_l + size <= buf.size());
    assert(tape_l + size <= tape->get_capacity());

    if (size == 0) return;

    size_t tape_r = tape_l + size - 1;
    size_t buf_r = buf_l + size - 1;

    long long head_pos = tape->get_head_position();

    bool in_order = abs(head_pos - tape_l) <= abs(head_pos - tape_r);

    if (in_order)
    {
        move_head_to_position(tape, tape_l);

        for (int i = 0; i < size-1; ++i)
        {
            tape->write_on_head(buf[buf_l+i]);
            tape->head_move_right();
        }
        tape->write_on_head(buf[buf_r]);
    } 
    else 
    {
        move_head_to_position(tape, tape_r);

        for (int i = 0; i < size-1; ++i)
        {
            tape->write_on_head(buf[buf_r-i]);
            tape->head_move_left();
        }
        tape->write_on_head(buf[buf_l]);
    }
}

void TapeSorter::move_head_to_position (std::shared_ptr<Tape> tape, std::size_t pos)
{
    assert(pos < tape->get_capacity());

    size_t cur_pos = tape->get_head_position();

    if (cur_pos > pos) {
        for (int i = 0; i < cur_pos - pos; ++i)
            tape->head_move_left();
    }

    if (cur_pos < pos) {
        for (int i = 0; i < pos - cur_pos; ++i)
            tape->head_move_right();
    }
}


void TapeSorter::set_chunk_size(size_t new_size) {
    chunk_size = new_size;
}
std::size_t TapeSorter::get_chunk_size() {
    return chunk_size;
}