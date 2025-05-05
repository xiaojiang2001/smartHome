# 编译器
CC := aarch64-linux-gnu-gcc

# 源文件与目标文件
SRC := main.c common.c 	\
		$(wildcard commandFactory/*.c)				\
		$(wildcard commandFactory/commands/*.c)		\
		$(wildcard devicesFactory/*.c)				\
		$(wildcard devicesFactory/devices/*.c)		\
		$(wildcard controlPthread/*.c)				\

OBJ := $(SRC:.c=.o)
TARGET := a.out

# 头文件包含路径
INC_DIRS := 					\
	-I ./						\
	-I ./devicesFactory			\
	-I ./devicesFactory/devices	\
	-I ./commandFactory			\
	-I ./commandFactory/commands	\
	-I ./controlPthread			\
    -I ./local/include 			\
	-I ./3rd/usr/local/include 	\
    -I ./3rd/usr/include				\
	-I ./3rd/usr/include/aarch64-linux-gnu

# 库文件链接路径
LIB_DIRS := \
    -L./local/lib \
    -L./3rd/aarch64-linux-gnu/lib \
    -L./3rd/usr/lib					\
	-L./3rd/usr/local/lib 			\
	-L./3rd/lib/aarch64-linux-gnu 	\
	-L./3rd/usr/lib/aarch64-linux-gnu


# 链接库
LIBS := -lwiringPi  -pthread -lexpat -lz -lcrypt

# 编译参数
CFLAGS := -Wall -Wextra -O2 $(INC_DIRS)

# 链接参数
LDFLAGS := $(LIB_DIRS) $(LIBS)

# 默认目标
all: $(TARGET)

# 生成目标程序
$(TARGET): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

# 编译源文件为对象文件
$(OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理中间文件和可执行文件
clean:
	rm -f $(OBJ) $(TARGET)