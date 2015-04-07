#include "bitmap.h"

#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

static inline size_t capacity_for_bits(size_t bits) {
    // Be careful not to overflow SIZE_T_MAX
    return ((bits >> 3) + (bits & 0x7 ? 1 : 0));
}

static inline size_t bit_to_byte(size_t bit) {
    return bit >> 3;
}

static inline size_t bit_in_byte(size_t bit) {
    return bit & 0x7;
}

Bitmap::Bitmap(size_t bits /* = 0 */) : m_bits(NULL), m_totalBits(0) {
    this->Allocate(bits);
}

Bitmap::~Bitmap() {
    this->Release();
}

void Bitmap::Allocate(size_t totalBits) {

    if (totalBits == 0) {
        return;
    }

    size_t totalBytes = capacity_for_bits(totalBits);
    uint8_t* bits = new uint8_t[totalBytes];
    memset(bits, 0, totalBytes);

    this->Release();
    m_bits = bits;
    m_totalBits = totalBits;
}

void Bitmap::Release() {

    delete [] m_bits;
    m_bits = NULL;
    m_totalBits = 0;
}


bool Bitmap::IsSet(size_t n) const {
    
    assert(m_bits != NULL);
    assert(n < m_totalBits);

    return ((m_bits[bit_to_byte(n)] & (1 << bit_in_byte(n))) != 0);
}

void Bitmap::Set(size_t n) {
    assert(m_bits != NULL);
    assert(n < m_totalBits);

    m_bits[bit_to_byte(n)] |= (1 << bit_in_byte(n));
}

void Bitmap::Clear(size_t n) {
    assert(m_bits != NULL);
    assert(n < m_totalBits);

    m_bits[bit_to_byte(n)] &= ~(1 << bit_in_byte(n));
}

void Bitmap::Xor(const Bitmap& other) {
    assert(m_bits != NULL);
    assert(m_totalBits == other.m_totalBits);

    uint32_t* a32 = (uint32_t*) m_bits;
    const uint32_t* b32 = (const uint32_t*) other.m_bits;
    size_t nwords = m_totalBits >> 5; // It's correct behaviour to underflow here
    
    for (size_t i = 0; i < nwords; ++i) {
        *a32++ ^= *b32++;
    }

    // Xor any remainders less than word size
    uint8_t* a8 = (uint8_t*) a32;
    const uint8_t* b8 = (const uint8_t*) b32;
    size_t nbytesrem = (capacity_for_bits(m_totalBits) - (nwords * sizeof(uint32_t)));

    for (size_t i = 0; i < nbytesrem; ++i) {
        *a8++ ^= *b8++;    
    }
}

bool Bitmap::FindFirstSetBit(size_t& bitpos) const 
{
    assert(m_bits != NULL);
    
    bitpos = 0;

    // Find a non-zero byte
    uint8_t* ptr = m_bits;
    for (size_t i = 0; (i < capacity_for_bits(m_totalBits)) && (*ptr == 0); ++i, ++ptr) {
        bitpos += 8;
    }

    if (*ptr == 0) {
        return false;
    }

    // Find a non-zero bit in byte
    uint8_t found = *ptr;
    while ((found & 0x1) == 0) {
        found >>= 1;
        ++bitpos;
    }

    return true;
}

