
# Set this flag to "yes" (no quotes) to use JTAG; otherwise ISP (SPI) is used
PROGRAM_WITH_JTAG := yes

# Feel free to ignore anything below this line
PROGRAMMER := atmelice_isp
ifeq ($(PROGRAM_WITH_JTAG), yes)
	PROGRAMMER := atmelice
endif

SOURCE_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
TARGET_CPU := atmega162
TARGET_DEVICE := m162

# List all source files to be compiled; separate with space
SOURCE_FILES := ./$(SOURCE_DIR)/main.c \
								./$(SOURCE_DIR)/uart.c \
								./$(SOURCE_DIR)/sys.c \
								./$(SOURCE_DIR)/gpio.c \
								./$(SOURCE_DIR)/timer.c \
								./$(SOURCE_DIR)/adc.c \
								./$(SOURCE_DIR)/oled.c \
								./$(SOURCE_DIR)/spi.c \
								./$(SOURCE_DIR)/menu.c \
								./$(SOURCE_DIR)/mcp2515.c \


CC := avr-gcc
CFLAGS := -Os -std=c11 -I $(INCLUDE_DIR) -mmcu=$(TARGET_CPU) -ggdb

OBJECT_FILES = $(patsubst ./$(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCE_FILES))

.DEFAULT_GOAL := $(BUILD_DIR)/main.hex

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: ./$(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.hex: $(OBJECT_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)/a.out
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/a.out $(BUILD_DIR)/main.hex

.PHONY: flash
flash: $(BUILD_DIR)/main.hex
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: erase
erase:
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e
	
.PHONY: debug
debug:
	if pgrep avarice; then pkill avarice; fi
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i
	x-terminal-emulator -e avarice --edbg --ignore-intr :4242
	sleep 2
	avr-gdb -tui -iex "target remote localhost:4242" $(BUILD_DIR)/a.out
	killall -s 9 avarice	
