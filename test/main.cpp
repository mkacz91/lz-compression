#include "prefix.hpp"

int main (int argc, char** argv) {
    cout << "Word size: " << sizeof(word) << "\n"
         << "WORD_LENGTH: " << WORD_LENGTH << endl;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
