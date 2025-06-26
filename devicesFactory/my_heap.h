#ifndef MY_HEAP_H
#define MY_HEAP_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

// 堆内存配置
#define configTOTAL_HEAP_SIZE (1024 * 1024)     // 1MB堆空间
#define portBYTE_ALIGNMENT 8                    // 字节对齐
#define portBYTE_ALIGNMENT_MASK (portBYTE_ALIGNMENT - 1)

// 定义堆管理器单例结构体
typedef struct {
    void* (*malloc)(uint32_t xWantedSize);           // 内存分配
    void (*free)(void* pv);                          // 内存释放
    uint32_t (*get_free_heapsize)(void);            // 获取可用堆大小
    uint32_t (*get_min_ever_free_heapsize)(void);   // 获取历史最小可用堆大小
} c_my_heap_t;

// 声明全局单例对象
extern const c_my_heap_t my_heap;

#endif /* MY_HEAP_H */
