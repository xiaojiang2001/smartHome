#include "my_heap.h"

/* Private Macros */
// 最小块大小，确保每个块至少能容纳两个BlockLink_t结构体
#define heapMINIMUM_BLOCK_SIZE ((size_t)(xHeapStructSize << 1)) 
// 堆对齐字节数 
#define heapBITS_PER_BYTE ((size_t)8)

/* Private Typedef */
// 定义块链接结构体
typedef struct A_BLOCK_LINK {
    struct A_BLOCK_LINK* pxNextFreeBlock;    // 指向下一个空闲块
    size_t xBlockSize;                       // 空闲块的大小
} BlockLink_t;

/* Private Functions Declare */
static void* my_heap_malloc(uint32_t xWantedSize);
static void my_heap_free(void* pv);
static uint32_t my_heap_get_free_heapsize(void);
static uint32_t my_heap_get_min_ever_free_heapsize(void);
static void my_heap_init(void);
static void my_heap_insert_block_into_freelist(BlockLink_t* pxBlockToInsert);

/* Private Variables */
// 堆管理器单例实现
// 导出堆管理器单例
const c_my_heap_t my_heap = {
    .malloc = my_heap_malloc,
    .free = my_heap_free,
    .get_free_heapsize = my_heap_get_free_heapsize,
    .get_min_ever_free_heapsize = my_heap_get_min_ever_free_heapsize
};

static uint8_t ucHeap[configTOTAL_HEAP_SIZE];           // 定义堆空间
// 堆结构体大小，包含BlockLink_t结构体和对齐字节
static const uint32_t xHeapStructSize = (sizeof(BlockLink_t) + ((uint32_t)(portBYTE_ALIGNMENT - 1))) & ~((uint32_t)portBYTE_ALIGNMENT - 1);
static BlockLink_t xStart, *pxEnd = NULL;   /* 创建链表头和链表尾指针 */
static uint32_t xFreeBytesRemaining = 0U;   // 当前可用堆空间
static uint32_t xMinimumEverFreeBytesRemaining = 0U;   // 历史最小可用堆空间
static uint32_t xBlockAllocatedBit = 0;     // 分配位，用于标记块是否已分配

/* 堆管理器单例方法实现 */
static void* my_heap_malloc(uint32_t xWantedSize) 
{
    // pxBlock 指向当前空闲块，pxPreviousBlock指向前一个空闲块，pxNewBlockLink用于分割块
    BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;    
    void* pvReturn = NULL;  // 返回分配的内存指针

    // 首次调用时初始化
    if (pxEnd == NULL) {
        my_heap_init();
    }

    // 检查请求大小是否有效
    if ((xWantedSize & xBlockAllocatedBit) == 0) {
        if (xWantedSize > 0) {
            xWantedSize += xHeapStructSize;
            // 字节对齐
            if ((xWantedSize & portBYTE_ALIGNMENT_MASK) != 0) {
                xWantedSize += (portBYTE_ALIGNMENT - (xWantedSize & portBYTE_ALIGNMENT_MASK));
            }
        }

        if ((xWantedSize > 0) && (xWantedSize <= xFreeBytesRemaining)) {
            // 寻找合适的块
            pxPreviousBlock = &xStart;
            pxBlock = xStart.pxNextFreeBlock;
            
            // 遍历空闲链表
            while ((pxBlock->xBlockSize < xWantedSize) && (pxBlock->pxNextFreeBlock != NULL)) {
                pxPreviousBlock = pxBlock;
                pxBlock = pxBlock->pxNextFreeBlock;
            }

            // 找到合适的块
            if (pxBlock != pxEnd) {
                pvReturn = (void*)((uint8_t*)pxPreviousBlock->pxNextFreeBlock + xHeapStructSize);
                pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

                // 分割块（如果剩余空间足够大）
                if ((pxBlock->xBlockSize - xWantedSize) > heapMINIMUM_BLOCK_SIZE) {
                    pxNewBlockLink = (void*)((uint8_t*)pxBlock + xWantedSize);
                    pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xWantedSize;
                    pxBlock->xBlockSize = xWantedSize;
                    my_heap_insert_block_into_freelist(pxNewBlockLink);
                }

                xFreeBytesRemaining -= pxBlock->xBlockSize;
                if (xFreeBytesRemaining < xMinimumEverFreeBytesRemaining) {
                    xMinimumEverFreeBytesRemaining = xFreeBytesRemaining;
                }

                pxBlock->xBlockSize |= xBlockAllocatedBit;
                pxBlock->pxNextFreeBlock = NULL;
            }
        }
    }

    return pvReturn;
}

static void my_heap_free(void* pv) {
    uint8_t* puc = (uint8_t*)pv;
    BlockLink_t* pxLink;

    if (pv != NULL) {
        puc -= xHeapStructSize;
        pxLink = (void*)puc;

        if ((pxLink->xBlockSize & xBlockAllocatedBit) != 0) {
            pxLink->xBlockSize &= ~xBlockAllocatedBit;
            xFreeBytesRemaining += pxLink->xBlockSize;
            my_heap_insert_block_into_freelist((BlockLink_t*)pxLink);
        }
    }
}

static uint32_t my_heap_get_free_heapsize(void) {
    return xFreeBytesRemaining;
}

static uint32_t my_heap_get_min_ever_free_heapsize(void) {
    return xMinimumEverFreeBytesRemaining;
}

// 初始化堆管理器
static void my_heap_init(void) 
{
    BlockLink_t* pxFirstFreeBlock;      // 指向第一个空闲块
    uint8_t* pucAlignedHeap;            // 指向对齐后的堆起始地址
    uint32_t uxAddress;                 // 用于对齐地址的变量
    uint32_t xTotalHeapSize = configTOTAL_HEAP_SIZE;    // 总堆大小

    // 对齐堆起始地址
    uxAddress = (uint32_t)ucHeap;
    if ((uxAddress & portBYTE_ALIGNMENT_MASK) != 0) {
        uxAddress += (portBYTE_ALIGNMENT - 1);
        uxAddress &= ~((uint32_t)portBYTE_ALIGNMENT_MASK);
        xTotalHeapSize -= uxAddress - (uint32_t)ucHeap;
    }
    pucAlignedHeap = (uint8_t*)uxAddress;

    // 初始化起始块
    xStart.pxNextFreeBlock = (void*)pucAlignedHeap;
    xStart.xBlockSize = 0;

    // 设置结束块
    uxAddress = ((uint32_t)pucAlignedHeap) + xTotalHeapSize;
    uxAddress -= sizeof(BlockLink_t);
    uxAddress &= ~((uint32_t)portBYTE_ALIGNMENT_MASK);
    pxEnd = (void*)uxAddress;
    pxEnd->xBlockSize = 0;
    pxEnd->pxNextFreeBlock = NULL;

    // 初始化第一个空闲块
    pxFirstFreeBlock = (void*)pucAlignedHeap;
    pxFirstFreeBlock->xBlockSize = uxAddress - (uint32_t)pxFirstFreeBlock;
    pxFirstFreeBlock->pxNextFreeBlock = pxEnd;

    // 初始化堆统计信息
    xMinimumEverFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
    xFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;

    // 设置分配位
    xBlockAllocatedBit = ((uint32_t)1) << ((sizeof(uint32_t) * heapBITS_PER_BYTE) - 1);
}

// 插入空闲块到链表
static void my_heap_insert_block_into_freelist(BlockLink_t* pxBlockToInsert) {
    BlockLink_t* pxIterator;
    uint8_t* puc;

    for (pxIterator = &xStart; pxIterator->pxNextFreeBlock < pxBlockToInsert; pxIterator = pxIterator->pxNextFreeBlock) {
        // 找到合适的插入位置
    }

    // 尝试与前一个块合并
    puc = (uint8_t*)pxIterator;
    if ((puc + pxIterator->xBlockSize) == (uint8_t*)pxBlockToInsert) {
        pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
        pxBlockToInsert = pxIterator;
    }

    // 尝试与后一个块合并
    puc = (uint8_t*)pxBlockToInsert;
    if ((puc + pxBlockToInsert->xBlockSize) == (uint8_t*)pxIterator->pxNextFreeBlock) {
        if (pxIterator->pxNextFreeBlock != pxEnd) {
            pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
            pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
        } else {
            pxBlockToInsert->pxNextFreeBlock = pxEnd;
        }
    } else {
        pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
    }

    if (pxIterator != pxBlockToInsert) {
        pxIterator->pxNextFreeBlock = pxBlockToInsert;
    }
}


