#pragma once
#include "Tape.h"
#include <memory>
#include <fstream>
#include <chrono>
#include <iostream>

class FileTape final : public Tape
{
    public:
        static std::unique_ptr<Tape> get_instance(const std::string& file_path, size_t capacity);
        static std::unique_ptr<Tape> get_instance(const std::string& file_path);

        static void read_configure_file(const std::string& file_path);

        FileTape(const std::string& file_path, size_t capacity);
        FileTape(const std::string& file_path);
        ~FileTape();

    private:
        mutable std::fstream stream;

        virtual void rewind() const override;

        virtual void head_move_left() const override;
        virtual void head_move_right() const override;

        virtual int read_from_head() const override;
        virtual void write_on_head(int value) override;

        static std::chrono::milliseconds rewind_delay;
        static std::chrono::milliseconds head_move_delay;
        static std::chrono::milliseconds read_delay;
        static std::chrono::milliseconds write_delay;
};
