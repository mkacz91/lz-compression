#include "input_provider.h"

// AlphabetInputProvider
// =============================================================================

AlphabetInputProvider::AlphabetInputProvider () :
    m_writer(m_input)
{
    /* Do nothing. */
}

int AlphabetInputProvider::max_get_cnt () const {
    return 10;
}

Buffer const& AlphabetInputProvider::get () {
    for (int i = 0; i < 5; ++i) {
        for (int a = 0; a < CHAR_CNT; ++a)
            m_writer.put(a);
    }
    return m_input;
}
