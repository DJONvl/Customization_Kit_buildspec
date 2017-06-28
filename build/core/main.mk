include build/core/gmsl.mk
override BBCHIP     := $(call uc,$(if $(BBCHIP),$(BBCHIP),MT6573))
BUILD_TYPE          := $(if $(BUILD_TYPE),$(BUILD_TYPE),Debug)

CUSTOM := MTK
-include build/core/configs/custom.mk
CUSTOM := $(strip $(CUSTOM))

ifeq (MT6516,$(BBCHIP))
  $(error BBCHIP=MT6516 is deprecated. Please use BBCHIP=MT6516_AP or BBCHIP=MT6516_MD instead.)
endif

include build/core/definition.mk
include build/core/configs/base.mk

ifneq (,$(SPLIT_DA_METHOD))
include build/core/single.mk
else
include buildspec.mk
ifeq (,$(SPEC))
SPEC := buildspec.mk
BBCHIPS := $(BBCHIP)
endif
include build/core/allinone.mk
endif
