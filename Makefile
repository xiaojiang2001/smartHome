# 编译器
CC = gcc

# 编译选项
# 编译选项
CFLAGS = -Wall -Wextra -std=c99 -pthread -I./lib/libcurl/__install/include -I$(SRC_DIR) -I$(DEVICE_DIR) -I$(COMMAND_DIR) -IcontrolPthread -IcommandFactory -IdevicesFactory

# 目标文件
TARGET = smartHome

# 源文件和头文件路径
SRC_DIR = .
DEVICE_DIR = devicesFactory/devices
COMMAND_DIR = commandFactory/commands

# 源文件列表
SRCS = 	$(wildcard $(SRC_DIR)/*.c) 		\
       	$(wildcard $(DEVICE_DIR)/*.c) 	\
       	$(wildcard $(COMMAND_DIR)/*.c)	\
		$(wildcard commandFactory/*.c) 	\
	    $(wildcard devicesFactory/*.c)  \
		$(wildcard controlPthread/*.c)

# 对象文件列表
OBJS = $(SRCS:.c=.o)

# 头文件列表
INCS = $(wildcard $(SRC_DIR)/*.h) 		\
	   $(wildcard $(DEVICE_DIR)/*.h) 	\
       $(wildcard $(COMMAND_DIR)/*.h) 	\
       $(wildcard commandFactory/*.h) 	\
	   $(wildcard devicesFactory/*.h)  	\
	   $(wildcard controlPthread/*.h)
	   
# 库文件路径
LIBS = -L./lib/libcurl/__install/lib -lcurl -lwiringPi
# LIBS = -I ./lib/libcurl/__install/include -L ./lib/libcurl/__install/lib -lcurl -lwiringPi  

# 默认目标
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# 编译源文件生成目标文件
%.o: %.c $(INCS)
	@$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	@echo "Cleaning up object files and the target executable..."
	@rm -f $(OBJS) $(TARGET)