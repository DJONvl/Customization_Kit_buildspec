O_TARGET	:= arch_arm_lib_$(DA_TYPE).o

ifeq ($(DA_TYPE),init)
obj-y 		+= crt-init.o
else
obj-y 		:= arch.o
obj-y 		+= cache.o
obj-y 		+= faults.o
obj-y 		+= mmu.o
obj-y 		+= thread.o
obj-y 		+= asm.o
obj-y 		+= crt-loop.o
obj-y 		+= exceptions.o
endif

obj-y 		+= ops.o
obj-y 		+= cache-ops.o

include $(TOPDIR)/Rules.make


