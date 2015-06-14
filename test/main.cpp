#include "prefix.h"

int main (int argc, char** argv) {
    cout << "Word size: " << sizeof(word) << "\n"
         << "WORD_BITS: " << WORD_BITS << endl;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
