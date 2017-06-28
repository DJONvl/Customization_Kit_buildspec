O_TARGET	:= kernel_lib_$(DA_TYPE).o

obj-y 		:= dpc.o

ifeq ($(DA_TYPE),loop)
obj-y 		+= kmain.o

endif
obj-y 		+= thread.o
obj-y 		+= event.o
obj-y 		+= mutex.o

obj-y 		+= timer.o


include $(TOPDIR)/Rules.make


