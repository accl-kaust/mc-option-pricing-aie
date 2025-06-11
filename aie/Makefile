PLATFORM := xilinx_vck5000_gen4x8_xdma_2_202210_1
TARGET := x86sim

XPFM = $(shell platforminfo -p $(PLATFORM) --json="file")
XSA = $(strip $(patsubst %.xpfm, % , $(shell basename $(PLATFORM))))

# OUTPUT PRODUCTS 
BUILD_DIR = build.$(TARGET)
WORK_DIR = work
SRC_DIR = $(shell readlink -f src/)
DATA_DIR = $(shell readlink -f data/)

# DEPENDENCIES for make aie
GRAPH_CPP := $(SRC_DIR)/graph.cpp
DEPS := $(GRAPH_CPP)
DEPS += $(SRC_DIR)/kernels.hpp
DEPS += $(SRC_DIR)/graph.hpp
DEPS += $(SRC_DIR)/rng.cpp
DEPS += $(SRC_DIR)/pathgenprice.cpp
# DEPS += $(SRC_DIR)/pathgen.cpp
# DEPS += $(SRC_DIR)/pathpricer.cpp
AIE_FLAGS += --platform=$(XPFM)

all: $(BUILD_DIR)/libadf.a

$(BUILD_DIR)/libadf.a: $(DEPS)
	@mkdir -p $(BUILD_DIR);
	cd $(BUILD_DIR); \
	aiecompiler -v --target=$(TARGET) \
		-include="$(XILINX_VITIS)/aietools/include" \
		-include="$(SRC_DIR)"  \
		-include="$(DATA_DIR)" \
		-stacksize=2752 \
		$(AIE_FLAGS) \
		$(GRAPH_CPP) \
		-workdir=$(WORK_DIR) 2>&1 | tee aiecompiler.log; 
# export C_INCLUDE_PATH=/usr/include/; 
#  unset C_INCLUDE_PATH

clean:
	rm -rf $(BUILD_DIR)

sim:
	@if [ $(TARGET) = "x86sim" ]; then \
	cd $(BUILD_DIR); \
		x86simulator --pkg-dir=$(WORK_DIR) --i=$(shell readlink -f ./$(BUILD_DIR)/$(WORK_DIR)) ;\
	fi
	@if [ $(TARGET) = "hw" ]; then \
		cd $(BUILD_DIR); \
		aiesimulator --pkg-dir=$(WORK_DIR) --i=$(shell readlink -f ./$(BUILD_DIR)/$(WORK_DIR)) --profile --dump-vcd=foo ; \
	fi