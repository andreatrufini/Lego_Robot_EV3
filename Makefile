# The main executable will be located in the same directory as you ran
# "Make" from. To add new .c files, simply add them to the OBJS variable.

export LD_LIBRARY_PATH=~/Desktop/ev3dev-c/lib

CC 			= arm-linux-gnueabi-gcc
CFLAGS 		= -O2 -g -std=gnu99
INCLUDES 	= -I./ev3dev-c/source/ev3 -I./include/ -I./include/mapping -I./include/navigation
LDFLAGS 	= -L./libraries -lrt -lm -lev3dev-c -lpthread -lbluetooth
BUILD_DIR 	= ./build
SOURCE_DIR 	= ./source

OBJS = \
	$(BUILD_DIR)/motors.o \
	$(BUILD_DIR)/sensors.o \
	$(BUILD_DIR)/navigation.o \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/loc_funct.o \
	$(BUILD_DIR)/debugFunctions.o \
	$(BUILD_DIR)/draw_funct.o \
	$(BUILD_DIR)/angles_funct.o \
	$(BUILD_DIR)/obj_detect.o \
	$(BUILD_DIR)/draw.o \
	$(BUILD_DIR)/matrix_movements.o \
	$(BUILD_DIR)/matrix_navigation.o \
	$(BUILD_DIR)/general.o 
		
all: main

main: ${OBJS}
	$(CC) $(INCLUDES) $(CFLAGS) $(OBJS) $(LDFLAGS) -o main

$(OBJS): $(BUILD_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -c $(SOURCE_DIR)/$*.c $(INCLUDES) -o $(BUILD_DIR)/$*.o

clean:
	rm -f $(BUILD_DIR)/*.o
	rm ./main
	
compile:
	cd /src/ev3dev-c/source/ev3/&&make&&sudo make install&&make shared&&sudo make shared-install
