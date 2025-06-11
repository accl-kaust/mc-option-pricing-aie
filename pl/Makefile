PLATFORM := xilinx_vck5000_gen4x8_xdma_2_202210_1
TARGET := hw
FREQ := 250

XSA = $(strip $(patsubst %.xpfm, % , $(shell basename $(PLATFORM))))

BUILD_DIR = build.$(TARGET)
VPP_FLAGS = -t $(TARGET) --platform $(PLATFORM)# --save-temps
VPP_FLAGS += --kernel_frequency $(FREQ)

all: $(BUILD_DIR)/pl_seed2s.xo $(BUILD_DIR)/pl_mm2s.xo $(BUILD_DIR)/pl_s2mm.xo $(BUILD_DIR)/pl_seed2s_16x.xo $(BUILD_DIR)/pl_mm2s_16x.xo $(BUILD_DIR)/pl_s2mm_16x.xo

$(BUILD_DIR)/pl_mm2s.xo: pl_mm2s.cpp
	v++ $(VPP_FLAGS) -k $(subst $(BUILD_DIR)/,,$(subst .xo,,$@)) -c -g -o $@ $^

$(BUILD_DIR)/pl_seed2s.xo: pl_seed2s.cpp
	v++ $(VPP_FLAGS) -k $(subst $(BUILD_DIR)/,,$(subst .xo,,$@)) -c -g -o $@ $^

$(BUILD_DIR)/pl_s2mm.xo: pl_s2mm.cpp
	v++ $(VPP_FLAGS) -k $(subst $(BUILD_DIR)/,,$(subst .xo,,$@)) -c -g -o $@ $^

$(BUILD_DIR)/pl_seed2s_16x.xo: pl_seed2s_16x.cpp
	v++ $(VPP_FLAGS) -k $(subst $(BUILD_DIR)/,,$(subst .xo,,$@)) -c -g -o $@ $^

$(BUILD_DIR)/pl_mm2s_16x.xo: pl_mm2s_16x.cpp
	v++ $(VPP_FLAGS) -k $(subst $(BUILD_DIR)/,,$(subst .xo,,$@)) -c -g -o $@ $^

$(BUILD_DIR)/pl_s2mm_16x.xo: pl_s2mm_16x.cpp
	v++ $(VPP_FLAGS) -k $(subst $(BUILD_DIR)/,,$(subst .xo,,$@)) -c -g -o $@ $^

clean:
	rm -rf $(BUILD_DIR) *.log *.jou .Xil/