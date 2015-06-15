#ifndef INPUT_PROVIDER_H
#define INPUT_PROVIDER_H

#include "../src/buffer.h"

// Input Provider
// =============================================================================

class InputProvider {
public:
    virtual int max_get_cnt () const = 0;

    virtual Buffer const& get () = 0;
};

// AlphabetInputProvider
// =============================================================================

class AlphabetInputProvider : public InputProvider {
public:
    AlphabetInputProvider ();

    virtual int max_get_cnt () const;

    virtual Buffer const& get ();

private:
    Buffer m_input;
    BufferCharWriter m_writer;
};

#endif // INPUT_PROVIDER
