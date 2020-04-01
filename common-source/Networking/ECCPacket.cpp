#include "ECCPacket.h"

#include <cstring>

// from https://rosettacode.org/wiki/CRC-32#C
uint32_t
rc_crc32(uint32_t crc, const char *buf, size_t len)
{
    static uint32_t table[256];
    static int have_table = 0;
    uint32_t rem;
    uint8_t octet;
    int i, j;
    const char *p, *q;

    /* This check is not thread safe; there is no mutex. */
    if (have_table == 0) {
        /* Calculate CRC table. */
        for (i = 0; i < 256; i++) {
            rem = i;  /* remainder from polynomial division */
            for (j = 0; j < 8; j++) {
                if (rem & 1) {
                    rem >>= 1;
                    rem ^= 0xedb88320;
                } else
                    rem >>= 1;
            }
            table[i] = rem;
        }
        have_table = 1;
    }

    crc = ~crc;
    q = buf + len;
    for (p = buf; p < q; p++) {
        octet = *p;  /* Cast to unsigned octet. */
        crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
    }
    return ~crc;
}

const void *ECCPacket::onSend(std::size_t &size)
{
    uint32_t crc = rc_crc32(0, (const char*)getData(), getDataSize());

    m_ecc_data.resize(getDataSize() + sizeof(crc));
    std::memcpy(m_ecc_data.data(), &crc, sizeof(crc));
    std::memcpy(m_ecc_data.data() + sizeof(crc), getData(), getDataSize());

    size = m_ecc_data.size();
    return m_ecc_data.data();
}

void ECCPacket::onReceive(const void *data, std::size_t size)
{
    const sf::Uint8* data_ptr = (const sf::Uint8*)data;

    uint32_t received_crc = *(uint32_t*)data_ptr;
    data_ptr += sizeof(received_crc);

    uint32_t computed_crc = rc_crc32(0, (const char*)data_ptr, size - sizeof(received_crc));

    if (received_crc != computed_crc)
    {
        m_corrupted = true; // package has been corrupted or tampered with !
        return;
    }

    append(data_ptr, size - sizeof(received_crc));
}
