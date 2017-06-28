BBCHIP              := $(if $(BBCHIP),$(BBCHIP),MT6573)

ifeq (off,$(LOG))
#include buildspec.mk
include build/core/main.mk
else
LOG := build_log.txt
$(MAKECMDGOALS): recursive-make
recursive-make:
	make BBCHIP=$(BBCHIP) LOG=off $(MAKECMDGOALS) 2>&1 | "$(word 1,$(if $(LINUX),tee,$(wildcard ../Tools/tee.exe Tools/tee.exe) tee.exe))" $(LOG)
endif
