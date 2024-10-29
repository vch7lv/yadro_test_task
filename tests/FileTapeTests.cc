#include <gtest/gtest.h>
#include <FileTape.h>
#include <vector>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;
using std::vector;

static bool check_tape(std::shared_ptr<Tape> t, const std::vector<int>& vec)
{
    t->rewind();


    for (size_t i = 0; i < vec.size() - 1; ++i)
    {
        if (t->read_from_head() != vec[i]) {
            return false;
        }
        t->head_move_right();
    }

    if (t->read_from_head() != vec.back()) {
        return false;
    }

    t->rewind();

    return true;
}

static void fill_tape(std::shared_ptr<Tape> t, const std::vector<int>& vec)
{
    t->rewind();

    for (size_t i = 0; i < vec.size() - 1; ++i)
    {
        t->write_on_head(vec[i]);
        t->head_move_right();
    }
    t->write_on_head(vec.back());

    t->rewind();
}

TEST(FileTapeTest, read_write_check) {
    int n = 1000;
    vector<int> a (n);

    srand(time(0));
    for (int i = 0; i < n; ++i)
    {
        a[i] = rand() % 1000000000;
    }

    fs::path dir = fs::temp_directory_path();
    fs::create_directories(dir / "tests");

    std::shared_ptr<Tape> tape = FileTape::get_instance(dir / "tests/tape", a.size());

    fill_tape(tape, a);
    EXPECT_TRUE(check_tape(tape, a));

    fs::remove_all(dir / "tests");
}

TEST(FileTapeTest, check_ctor) {

    fs::path dir = fs::temp_directory_path();
    fs::create_directories(dir / "tests");

    std::shared_ptr<Tape> tape = FileTape::get_instance(dir / "tests/tape", 10);

    EXPECT_TRUE(tape->get_capacity() == 10);
    EXPECT_TRUE(tape->get_head_position() == 0);

    fs::remove_all(dir / "tests");
}

TEST(FileTapeTest, check_head_moves) {

    fs::path dir = fs::temp_directory_path();
    fs::create_directories(dir / "tests");

    std::shared_ptr<Tape> tape = FileTape::get_instance(dir / "tests/tape", 10);

    EXPECT_EQ(tape->get_capacity(), 10);
    EXPECT_EQ(tape->get_head_position(), 0);

    for (size_t i = 0; i < 9; ++i) {
        tape->head_move_right();
        EXPECT_EQ(tape->get_head_position(), i+1);
    }

    tape->rewind();

    EXPECT_EQ(tape->get_head_position(), 0);

    fs::remove_all(dir / "tests");
}