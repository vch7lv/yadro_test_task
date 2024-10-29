#include <gtest/gtest.h>
#include "TapeSorter.h"
#include "FileTape.h"
#include <vector>
#include <filesystem>
#include <iostream>
#include <ctime>

namespace fs = std::filesystem;
using std::vector;


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

TEST(TapeSorterTest, sort_with_big_buf) {
    TapeSorter sorter(1000);

    vector<int> a {1,-2,3,-4,5,6,-7,8,9,-10,11,-12,-13,14,-15,16};
    vector<int> b {-23,545,-34,0,0,0,234,34,53,2,234234,34,0,34,1};
    vector<int> c {100};

    fs::path dir = fs::temp_directory_path();
    fs::create_directories(dir / "tests");

    std::shared_ptr<Tape> tape_a_in = FileTape::get_instance(dir / "tests/ain", a.size());
    std::shared_ptr<Tape> tape_a_out = FileTape::get_instance(dir / "tests/aout", a.size());
    fill_tape(tape_a_in, a);
    sorter.sort(tape_a_in, tape_a_out);
    std::sort(a.begin(), a.end());

    EXPECT_TRUE(check_tape(tape_a_out, a));

    std::shared_ptr<Tape> tape_b_in = FileTape::get_instance(dir / "tests/bin", b.size());
    std::shared_ptr<Tape> tape_b_out = FileTape::get_instance(dir / "tests/bout", b.size());
    fill_tape(tape_b_in, b);
    sorter.sort(tape_b_in, tape_b_out);
    std::sort(b.begin(), b.end());

    EXPECT_TRUE(check_tape(tape_b_out, b));

    std::shared_ptr<Tape> tape_c_in = FileTape::get_instance(dir / "tests/cin", c.size());
    std::shared_ptr<Tape> tape_c_out = FileTape::get_instance(dir / "tests/cout", c.size());
    fill_tape(tape_c_in, c);
    sorter.sort(tape_c_in, tape_c_out);
    std::sort(c.begin(), c.end());

    EXPECT_TRUE(check_tape(tape_c_out, c));


    fs::remove_all(dir / "tests");
}

TEST(TapeSorterTest, sort_with_small_buf) {
    TapeSorter sorter(20);



    vector<int> a {6, 14, 2, 9, 8, 3, 13, 10, 4, 5, 1, 0, 11, 12, 7 };
    vector<int> b {12, 10, 9, 14, 0, 8, 5, 2, 1, 11, 3, 6, 13, 7, 4};
    vector<int> c {100};

    fs::path dir = fs::temp_directory_path();
    fs::create_directories(dir / "tests");

    std::shared_ptr<Tape> tape_a_in = FileTape::get_instance(dir / "tests/ain", a.size());
    std::shared_ptr<Tape> tape_a_out = FileTape::get_instance(dir / "tests/aout", a.size());
    fill_tape(tape_a_in, a);
    sorter.sort(tape_a_in, tape_a_out);
    std::sort(a.begin(), a.end());

    EXPECT_TRUE(check_tape(tape_a_out, a));

    std::shared_ptr<Tape> tape_b_in = FileTape::get_instance(dir / "tests/bin", b.size());
    std::shared_ptr<Tape> tape_b_out = FileTape::get_instance(dir / "tests/bout", b.size());
    fill_tape(tape_b_in, b);
    sorter.sort(tape_b_in, tape_b_out);
    std::sort(b.begin(), b.end());

    EXPECT_TRUE(check_tape(tape_b_out, b));

    std::shared_ptr<Tape> tape_c_in = FileTape::get_instance(dir / "tests/cin", c.size());
    std::shared_ptr<Tape> tape_c_out = FileTape::get_instance(dir / "tests/cout", c.size());
    fill_tape(tape_c_in, c);
    sorter.sort(tape_c_in, tape_c_out);
    std::sort(c.begin(), c.end());

    EXPECT_TRUE(check_tape(tape_c_out, c));


    fs::remove_all(dir / "tests");
}

TEST(TapeSorterTest, big_random_tape) {

    TapeSorter sorter(24);

    int n = 1000;

    vector<int> a (n);

    srand(time(0));
    for (int i = 0; i < n; ++i)
    {
        a[i] = rand() % 1000000000;
    }


    fs::path dir = fs::temp_directory_path();
    fs::create_directories(dir / "tests");


    std::shared_ptr<Tape> tape_a_in = FileTape::get_instance(dir / "tests/ain", a.size());
    std::shared_ptr<Tape> tape_a_out = FileTape::get_instance(dir / "tests/aout", a.size());
    fill_tape(tape_a_in, a);

    sorter.sort(tape_a_in, tape_a_out);
    std::sort(a.begin(), a.end());

    EXPECT_TRUE(check_tape(tape_a_out, a));

    fs::remove_all(dir / "tests");

}