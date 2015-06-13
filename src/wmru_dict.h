#ifndef WMRU_DICT
#define WMRU_DICT

#include "prefix.h"
#include <list>
#include <vector>

#include "pool_dict.h"

// WmruPool
// =============================================================================
//
// TODO doc
class WmruPool : public CodewordPool {
public:
    WmruPool (int limit, bool single_char_codewords);

    int match (int i);

private:
    std::list<int> m_queue;

    std::vector<std::list<int>::iterator> m_queue_positions;

    int m_size;
};

// Wmru
// =============================================================================

struct Wmru {
    typedef PoolEncodeDict<WmruPool> EncodeDict;
    typedef PoolDecodeDict<WmruPool> DecodeDict;
};

#endif // WMRU_DICT
