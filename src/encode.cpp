#include "prefix.h"

#include <fstream>
using std::ifstream;
using std::ofstream;

#include "huffman.h"
#include "lz78.h"
#include "lzw.h"
#include "mra_dict.h"
#include "smru_dict.h"
#include "wmru_dict.h"

string exec_name;

int fail () {
    cout << "usage:\n\t" << exec_name << " "
         << "e [lz78|lzw] [smru|wmru|mra] dictsize filename"
         << "\n\t" << exec_name << " "
         << "d filename"
         << "\nexample:\n\t" << exec_name << " "
         << "e lzw wmru 500 hello.txt"
         << "\n\t" << exec_name << " "
         << "d hello.lz"
         << endl;
    return 1;
}

enum Scheme {
    LZ78,
    LZW
};

enum Dictionary {
    SMRU,
    WMRU,
    MRA
};

std::ostream& operator << (std::ostream& ostr, milliseconds d) {
    return ostr << d.count() << "ms";
}

Lz const* get_encoder (
    string const& s_scheme,
    string const& s_dict,
    string const& s_limit
) {
    Scheme scheme;
    if (s_scheme == "lz78") {
        scheme = LZ78;
    } else if (s_scheme == "lzw") {
        scheme = LZW;
    } else {
        cout << "Expected 'lz78' or 'lzw', got '" << s_scheme << "'\n";
        return nullptr;
    }

    Dictionary dict;
    if (s_dict == "smru") {
        dict = SMRU;
    } else if (s_dict == "wmru") {
        dict = WMRU;
    } else if (s_dict == "mra") {
        dict = MRA;
    } else {
        cout << "Expected 'smru', 'wmru' or 'mra', got '" << s_dict << "'\n";
        return nullptr;
    }

    int limit = stoi(s_limit);
    if (limit < 3 || limit > 100000) {
        cout << "Expected number between 3 and 100 000, got '"
             << s_limit << "'\n";
        return nullptr;
    }

    switch (scheme) {
        case LZ78:
            switch (dict) {
                case SMRU: return new Lz78<Smru>(limit); break;
                case WMRU: return new Lz78<Wmru>(limit); break;
                case  MRA: return new Lz78<Mra>(limit); break;
            }
            break;
        case LZW:
            switch (dict) {
                case SMRU: return new Lzw<Smru>(limit); break;
                case WMRU: return new Lzw<Wmru>(limit); break;
                case  MRA: return new Lzw<Mra>(limit); break;
            }
            break;
    }
}

int encode (int argc, char** argv) {
    if (argc < 5)
        return fail();

    Lz const* encoder = get_encoder(argv[2], argv[3], argv[4]);
    if (encoder == nullptr)
        return fail();

    string s_infile = argv[5];
    ifstream infile(s_infile.c_str());
    if (!infile) {
        cout << "Cannot open " << s_infile << "\n";
        delete encoder;
        return fail();
    }
    Buffer input;
    BufferCharWriter writer(input);
    char a;
    while (infile.get(a))
        writer.put(a);
    infile.close();

    cout << "Original file: " << double(input.size()) / 8000.0 << "kB\n";

    cout << "Encoding with LZ ... " << flush;
    auto t0 = system_clock::now();
    Buffer lz_input = encoder->encode(input);
    auto t1 = system_clock::now();
    cout << "done"
         << "\n    time taken: " << duration_cast<milliseconds>(t1 - t0)
         << "\n          size: " << double(lz_input.size()) / 8000.0 << "kB"
         << "\n     codewords: " << lz_input.size() / encoder->codeword_bits()
         << endl;
    delete encoder;

    cout << "Encoding with Huffman ... " << flush;
    auto t2 = system_clock::now();
    Buffer huffman_lz_input = Huffman::encode(lz_input);
    auto t3 = system_clock::now();
    cout << "done"
         << "\n    time taken: " << duration_cast<milliseconds>(t3 - t2)
         << "\n          size: " << double(huffman_lz_input.size()) / 8000.0
                                 << "kB"
         << endl;

    string s_outfile = s_infile + ".lz";
    cout << "Saving to " << s_outfile << " ... " << flush;
    ofstream outfile(s_outfile.c_str());
    if (!outfile) {
        cout << "Cannot create " << s_outfile << "\n";
        return fail();
    }
    BufferCharReader reader(huffman_lz_input);
    word last_word = reader.last_word();
    int const char_cnt = (huffman_lz_input.size() / WORD_BITS) * WORD_CHARS;
    int remaining_bits = huffman_lz_input.size() - CHAR_BITS * char_cnt;
    outfile << argv[2] << " " << argv[3] << " " << argv[4] << " "
            << last_word << " " << remaining_bits << " ";
    for (int i = 0; i < char_cnt; ++i)
        outfile.put(reader.get());
    cout << char_cnt << " ";
    outfile.close();
    cout << "done" << endl;

    return 0;
}

int decode (int argc, char** argv) {
    if (argc < 3)
        return fail();

    string s_outfile = argv[2];
    ifstream outfile(s_outfile.c_str());
    if (!outfile) {
        cout << "Cannot open " << s_outfile << "\n";
        return fail();
    }

    string s_scheme;
    string s_dict;
    string s_limit;
    word last_word;
    int remaining_bits;
    outfile >> s_scheme >> s_dict >> s_limit >> last_word >> remaining_bits;

    Lz const* encoder = get_encoder(s_scheme, s_dict, s_limit);
    if (encoder == nullptr)
        return fail();

    Buffer output;
    BufferCharWriter writer(output);
    char a;
    outfile.get(a);
    assert(a == ' ');
    while (outfile.get(a)) {
        writer.put(a);
    }
    writer.put_last_word(last_word, remaining_bits);

    cout << "Compressed file: " << double(output.size()) / 8000.0 << "kB"
         << "\n encoded using: " << s_scheme << " " << s_dict
         << "\n     dict size: " << s_limit
         << endl;

    cout << "Decoding with Huffman ... " << flush;
    auto t0 = system_clock::now();
    Buffer huffman_output = Huffman::decode(output);
    auto t1 = system_clock::now();
    cout << "done"
         << "\n    time taken: " << duration_cast<milliseconds>(t1 - t0)
         << "\n          size: " << double(huffman_output.size()) / 8000.0
                                 << "kB"
         << endl;

    cout << "Decoding with LZ ... " << flush;
    auto t2 = system_clock::now();
    Buffer lz_huffman_output = encoder->decode(huffman_output);
    auto t3 = system_clock::now();
    cout << "done"
         << "\n    time taken: " << duration_cast<milliseconds>(t3 - t2)
         << "\n          size: " << double(lz_huffman_output.size()) / 8000.0
                                 << "kB"
         << "\n     codewords: " << huffman_output.size()
                                  / encoder->codeword_bits()
         << endl;
    delete encoder;

    string s_infile = s_outfile + ".zl";
    cout << "Saving to " << s_infile << " ... " << flush;
    ofstream infile(s_infile.c_str());
    if (!infile) {
        cout << "Cannot create " << s_infile << "\n";
        return fail();
    }
    BufferCharReader reader(lz_huffman_output);
    while (!reader.eob())
        infile.put(reader.get());
    infile.close();
    cout << "done" << endl;

    return 0;
}

int main (int argc, char** argv) {
    exec_name = argv[0];
    if (argc < 2)
        return fail();

    string s_dir = argv[1];

    if (s_dir == "e") {
        return encode(argc, argv);
    } else if (s_dir == "d") {
        return decode(argc, argv);
    } else {
        cout << "Expected 'e' or 'd', got '" << s_dir << "'\n";
        return fail();
    }
    return 0;
}
