/* lz4 decompression, inspired by libdragon */

#include "ultra64.h"
#include "functions.h"

// main data
typedef struct lz4_header {
    char magic[4];
    u8 headerOriginalSize[4];
    u8 headerCompressedSize[4];
} lz4_header;

#define OFFSET_MAGIC offsetof(lz4_header, magic)
#define OFFSET_DEC_SIZE offsetof(lz4_header, headerOriginalSize)
#define OFFSET_COMP_SIZE offsetof(lz4_header, headerCompressedSize)
#define HEADER_SIZE ALIGN16(sizeof(lz4_header))
#define DMA_ALIGN 8
#define BUFFER_SIZE sizeof(lz4Data.dataBuffer)

#define MIN_MATCH_SIZE 4
#define LITERALS_RUN_LEN 15
#define MATCH_RUN_LEN 15
#define likely(v) (v)
#define unlikely(v) (v)

#define LZ4ULTRA_DECOMPRESSOR_BUILD_LEN(__len)        \
    {                                                 \
        unsigned int byte;                            \
        do {                                          \
            if (unlikely(pInBlock >= pInBlockEnd))    \
                return -1;                            \
            /* if (dma_race) wait_dma(pInBlock+1); */ \
            byte = (unsigned int)*pInBlock++;         \
            __len += byte;                            \
        } while (unlikely(byte == 255));              \
    }

typedef struct lz4_data {
    lz4_header header;
    u32 originalSize;
    u32 compressedSize;
    u8 headerBuffer[HEADER_SIZE];
    uintptr_t romStart;
    u8* dst;
    size_t size;
    u8 dataBuffer[1024]; // 1MB
    u8* dataBufferEnd;
} lz4_data;

typedef struct lz4_state {
    u8* source;
    u8* destination;
} lz4_state;

static lz4_data lz4Data;
static lz4_state state;
static bool isDebug = true;

u32 getSizeFromHeader(u8* sizePtr) {
    return (sizePtr[0] << 24) | (sizePtr[1] << 16) | (sizePtr[2] << 8) | (sizePtr[3] << 0);
}

bool isMagicValid(char* magic) {
    return magic[0] == 'L' && magic[1] == 'Z' && magic[2] == '4' && magic[3] == '0';
}

void setHeaderData() {
    if (isDebug) {
        osSyncPrintf("ROM START: 0x%08X\n", lz4Data.romStart);
        osSyncPrintf("DATA ADDR: 0x%08X\n", &lz4Data);
    }

    // loading header data
    DmaMgr_DmaRomToRam(lz4Data.romStart, lz4Data.headerBuffer, sizeof(lz4Data.headerBuffer));

    // get header data, prefix, original file size and compressed size
    memcpy(lz4Data.header.magic, &lz4Data.headerBuffer[OFFSET_MAGIC], 4);
    memcpy(lz4Data.header.headerOriginalSize, &lz4Data.headerBuffer[OFFSET_DEC_SIZE], 4);
    memcpy(lz4Data.header.headerCompressedSize, &lz4Data.headerBuffer[OFFSET_COMP_SIZE], 4);

    // get the real values
    lz4Data.originalSize = getSizeFromHeader(lz4Data.header.headerOriginalSize);
    lz4Data.compressedSize = getSizeFromHeader(lz4Data.header.headerCompressedSize);

    if (isDebug) {
        osSyncPrintf("magic: %s, originalSize: 0x%08X, compressedSize: 0x%08X\n", lz4Data.header.magic,
                     lz4Data.originalSize, lz4Data.compressedSize);
    }

    // safety checks
    ASSERT(isMagicValid(lz4Data.header.magic), "isMagicValid(magic)", __FILE__, __LINE__);
    ASSERT(lz4Data.originalSize > 0, "originalSize > 0", __FILE__, __LINE__);
    ASSERT(lz4Data.compressedSize > 0, "compressedSize > 0", __FILE__, __LINE__);
    ASSERT(lz4Data.originalSize > lz4Data.compressedSize, "originalSize > compressedSize", __FILE__, __LINE__);

    lz4Data.romStart += HEADER_SIZE;
}

/* block copy, with desired overlapping behavior */
u8* copyBlock(u8* _src, u8* _dst, u32 n) {
    u8* src = _src;
    u8* dst = _dst;

    do {
        *dst++ = *src++;
    } while (--n);

    return dst;
}

u8* refill(u8* src) {
    u32 offset;
    u32 size;

    /* intermediate buffer is not yet due for a refill */
    if (src < lz4Data.dataBufferEnd - DMA_ALIGN) {
        return src;
    }

    /* the number 8 is used throughout to ensure *
     * dma transfers are always 8 byte aligned   */
    offset = lz4Data.dataBufferEnd - src;
    size = BUFFER_SIZE - DMA_ALIGN;

    /* the last eight bytes wrap around */
    copyBlock(lz4Data.dataBufferEnd - DMA_ALIGN, lz4Data.dataBuffer, DMA_ALIGN);

    /* transfer data from rom */
    DmaMgr_DmaRomToRam(lz4Data.romStart, lz4Data.dataBuffer, size);
    lz4Data.romStart += size;

    return lz4Data.dataBuffer + (DMA_ALIGN - offset);
}

/**
 * Decompress one data block
 *
 * @param pInBlock pointer to compressed data
 * @param nBlockSize size of compressed data, in bytes
 * @param pOutData pointer to output decompression buffer (previously decompressed bytes + room for decompressing this
 * block)
 * @param nBlockMaxSize total size of output decompression buffer, in bytes
 *
 * @return size of decompressed data in bytes, or -1 for error
 */
s32 decompress_lz4_full_mem(u8* pInBlock, u32 nBlockSize, u8* pOutData, u32 nBlockMaxSize) {
    const unsigned char* pInBlockEnd = pInBlock + nBlockSize;
    unsigned char* pCurOutData = pOutData;
    const unsigned char* pOutDataEnd = pCurOutData + nBlockMaxSize;
    const unsigned char* pOutDataFastEnd = pOutDataEnd - 18;

    //    if (dma_race) wait_dma(NULL);
    while (likely(pInBlock < pInBlockEnd)) {
        //   if (dma_race) wait_dma(pInBlock+1);
        const unsigned int token = (unsigned int)*pInBlock++;
        unsigned int nLiterals = ((token & 0xf0) >> 4);

        if (nLiterals != LITERALS_RUN_LEN && pCurOutData <= pOutDataFastEnd && (pInBlock + 16) <= pInBlockEnd) {
            //  if (dma_race) wait_dma(pInBlock+16);
            memcpy(pCurOutData, pInBlock, 16);
        } else {
            if (likely(nLiterals == LITERALS_RUN_LEN))
                LZ4ULTRA_DECOMPRESSOR_BUILD_LEN(nLiterals);

            osSyncPrintf("saucisse 1\n");
            if (unlikely((pInBlock + nLiterals) > pInBlockEnd))
                return -1;
            osSyncPrintf("saucisse 1 ok\n");
            osSyncPrintf("saucisse 2\n");
            if (unlikely((pCurOutData + nLiterals) > pOutDataEnd))
                return -1;
            osSyncPrintf("saucisse 2 ok\n");

            //  if (dma_race) wait_dma(pInBlock+nLiterals);
            memcpy(pCurOutData, pInBlock, nLiterals);
        }

        pInBlock += nLiterals;
        pCurOutData += nLiterals;

        if (likely((pInBlock + 2) <= pInBlockEnd)) {
            unsigned int nMatchOffset;

            //  if (dma_race) wait_dma(pInBlock+2);
            nMatchOffset = (unsigned int)*pInBlock++;
            nMatchOffset |= ((unsigned int)*pInBlock++) << 8;

            unsigned int nMatchLen = (token & 0x0f);

            nMatchLen += MIN_MATCH_SIZE;
            if (nMatchLen != (MATCH_RUN_LEN + MIN_MATCH_SIZE) && nMatchOffset >= 8 && pCurOutData <= pOutDataFastEnd) {
                const unsigned char* pSrc = pCurOutData - nMatchOffset;

                osSyncPrintf("saucisse 3\n");
                if (unlikely(pSrc < pOutData))
                    return -1;
                osSyncPrintf("saucisse 3 ok\n");

                memcpy(pCurOutData, pSrc, 8);
                memcpy(pCurOutData + 8, pSrc + 8, 8);
                memcpy(pCurOutData + 16, pSrc + 16, 2);

                pCurOutData += nMatchLen;
            } else {
                if (likely(nMatchLen == (MATCH_RUN_LEN + MIN_MATCH_SIZE)))
                    LZ4ULTRA_DECOMPRESSOR_BUILD_LEN(nMatchLen);

                osSyncPrintf("saucisse 4\n");
                if (unlikely((pCurOutData + nMatchLen) > pOutDataEnd))
                    return -1;
                osSyncPrintf("saucisse 4 ok\n");
                const unsigned char* pSrc = pCurOutData - nMatchOffset;
                osSyncPrintf("saucisse 5\n");
                if (unlikely(pSrc < pOutData))
                    return -1;
                osSyncPrintf("saucisse 5 ok\n");

                if (nMatchOffset >= 16 && (pCurOutData + nMatchLen) <= pOutDataFastEnd) {
                    const unsigned char* pCopySrc = pSrc;
                    unsigned char* pCopyDst = pCurOutData;
                    const unsigned char* pCopyEndDst = pCurOutData + nMatchLen;

                    do {
                        memcpy(pCopyDst, pCopySrc, 16);
                        pCopySrc += 16;
                        pCopyDst += 16;
                    } while (pCopyDst < pCopyEndDst);

                    pCurOutData += nMatchLen;
                } else {
                    while (nMatchLen--) {
                        *pCurOutData++ = *pSrc++;
                    }
                }
            }
        }

        state.source = refill(state.source);
    }

    return (s32)(pCurOutData - pOutData);
}

void LZ4_Decompress(uintptr_t romStart, u8* dst, size_t size) {
    s32 decompressedSize = -1;

    lz4Data.romStart = romStart;
    lz4Data.dst = dst;
    lz4Data.size = size;
    lz4Data.dataBufferEnd = lz4Data.dataBuffer + BUFFER_SIZE;
    setHeaderData();
    state.source = lz4Data.dataBufferEnd;

    if (isDebug) {
        osSyncPrintf("STATE ADDR: 0x%08X\n", &state);
        osSyncPrintf("BUFFER ADDR: 0x%08X\n", lz4Data.dataBuffer);
    }

    state.source = refill(state.source); // initial refill
    decompressedSize = decompress_lz4_full_mem(state.source, lz4Data.compressedSize, lz4Data.dst, lz4Data.originalSize);
    osSyncPrintf("decompressedSize: %i\n", decompressedSize);

    ASSERT(decompressedSize > 0, "decompressedSize > 0", __FILE__, __LINE__);
    dst = lz4Data.dst;
}
