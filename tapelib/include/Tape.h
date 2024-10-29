#pragma once
#include <cstdint>


class Tape
{
    public:
        Tape() = default;
        Tape& operator = (const Tape&) = delete;
        Tape(const Tape&) = delete;
        virtual ~Tape() = default;

        virtual void rewind() const = 0;

        virtual void head_move_left() const = 0;
        virtual void head_move_right() const = 0;

        virtual int read_from_head() const = 0;
        virtual void write_on_head(int value) = 0;

        std::size_t get_head_position() const
        {
            return head_position;
        }

        std::size_t get_capacity() const
        {
            return capacity;
        }

    protected:
        mutable std::size_t head_position;
        std::size_t capacity;
};