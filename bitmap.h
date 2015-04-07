#ifndef KATA_BITMAP_H
#define KATA_BITMAP_H

#include <stdint.h>
#include <stddef.h>

class Bitmap
{
public:

    explicit Bitmap(size_t bits = 0);
    ~Bitmap();

    void Allocate(size_t numberOfBits); // Throws std::bad_alloc if no memory
    void Release();

    bool IsSet(size_t bitnum) const;
    void Set(size_t bitnum);
    void Clear(size_t bitnum);

    void Xor(const Bitmap& other); // destructive XOR between two maps. should be same size.

    bool FindFirstSetBit(size_t& bitpos) const;

private:

    uint8_t* m_bits;
    size_t m_totalBits;  // In bits
};


#endif

