PLATFORM := xilinx_vck5000_gen4x8_xdma_2_202210_1
TARGET := hw

XSA = $(strip $(patsubst %.xpfm, % , $(shell basename $(PLATFORM))))
AIE_DIR = $(shell readlink -f ./aie)
PL_DIR = $(shell readlink -f ./pl)
HOST_DIR = $(shell readlink -f ./host)
HW_LINK = $(shell readlink -f ./hw/config.cfg)

XCLBIN_NAME = mc
JOBS = 48
ifeq (gen4x8,$(findstring gen4x8, $(XSA)))
	FREQ = 300
else
	FREQ = 100
endif

PACKAGE_FLAGS = -t $(TARGET) --platform $(PLATFORM) --save-temps

VPP_FLAGS = $(PACKAGE_FLAGS)
VPP_FLAGS += -g --optimize 2
VPP_FLAGS += --hls.jobs $(JOBS)
VPP_FLAGS += --config $(HW_LINK)
VPP_FLAGS += --profile.data all:all:all
ifneq (gen3x16,$(findstring gen3x16, $(XSA)))
	VPP_FLAGS += --clock.defaultFreqHz $(shell echo "scale=0;$(FREQ)*1000000/1" | bc)
endif

VPP_LDFLAGS := --vivado.synth.jobs $(JOBS) --vivado.impl.jobs $(JOBS)

BUILD_DIR = build.$(TARGET)
OUTPUT_DIR = $(shell readlink -f ./$(BUILD_DIR))

AIE_SRCS = $(AIE_DIR)/$(BUILD_DIR)/libadf.a
XO_SRCS = $(PL_DIR)/$(BUILD_DIR)/pl_seed2s.xo
XO_SRCS += $(PL_DIR)/$(BUILD_DIR)/pl_mm2s.xo
XO_SRCS += $(PL_DIR)/$(BUILD_DIR)/pl_s2mm.xo
XO_SRCS += $(PL_DIR)/$(BUILD_DIR)/pl_seed2s_16x.xo
XO_SRCS += $(PL_DIR)/$(BUILD_DIR)/pl_mm2s_16x.xo
XO_SRCS += $(PL_DIR)/$(BUILD_DIR)/pl_s2mm_16x.xo
HOST_APP = $(HOST_DIR)/$(XCLBIN_NAME).exe

all: $(OUTPUT_DIR)/${XCLBIN_NAME}.xclbin $(HOST_APP)

$(AIE_SRCS):
	make -C $(AIE_DIR)/ PLATFORM=$(PLATFORM) FREQ=$(FREQ) TARGET=$(TARGET)

$(XO_SRCS):
	make -C $(PL_DIR)/ PLATFORM=$(PLATFORM) FREQ=$(FREQ) TARGET=$(TARGET)

$(HOST_APP):
	make -C $(HOST_DIR)

# Building xsa
$(OUTPUT_DIR)/$(XCLBIN_NAME).xsa: $(AIE_SRCS) $(XO_SRCS)
	@echo "### ***** linking pl kernels into $(XCLBIN_NAME).xsa ... *****"
	sed -i 's/\#nk/nk/' $(HW_LINK);\
	mkdir -p $(OUTPUT_DIR); \
	cd $(OUTPUT_DIR); \
	v++ -l $(VPP_FLAGS) \
	  --temp_dir _x_temp/ \
	  --report_dir reports/ \
	  $(VPP_LDFLAGS) \
	  $^ \
	  -o $@ 2>&1 | tee $(XCLBIN_NAME)_xsa.log
	@echo "### ***** $(XCLBIN_NAME).xsa linking done! *****"

# Building xclbin
$(OUTPUT_DIR)/${XCLBIN_NAME}.xclbin: $(OUTPUT_DIR)/${XCLBIN_NAME}.xsa
	@echo "### ***** packaging xsa into $(XCLBIN_NAME).xclbin ... *****"
	cd $(OUTPUT_DIR); \
	v++ --package $(PACKAGE_FLAGS) \
	  $^ \
	  $(AIE_SRCS) \
	  --temp_dir _x_temp \
	  --report_dir reports/ \
	  --package.boot_mode=ospi \
	  -o $@ 2>&1 | tee $(XCLBIN_NAME)_xclbin.log
	@echo "### ***** $(XCLBIN_NAME).xclbin packaging done! *****"

clean:
	rm -rf *.log *.jou .Xil/

distclean: clean
	rm -rf $(BUILD_DIR)