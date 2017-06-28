O_TARGET	:= lib.o

obj-y 		:= debug/debug.o 
obj-y 		+= heap/heap.o 

obj-y 		+= libc/malloc.o 
obj-y 		+= libc/printf.o 
#obj-y 		+= libc/string/memset.o
obj-y 		+= libc/string/memcmp.o
obj-y 		+= libc/string/strlcpy.o
obj-y 		+= libc/string/strlen.o
obj-y 		+= libc/string/atoi.o
obj-y 		+= libc/string/rand.o

obj-y 		+= libc/string/strnicmp.o
obj-y 		+= libc/string/strncmp.o
obj-y 		+= libc/string/strncat.o
obj-y 		+= libc/string/strncpy.o
obj-y     += libc/string/strstr.o
obj-y 		+= libc/string/ctype.o


obj-y 		+= libc/string/arch/arm/memcpy.o
obj-y 		+= libc/string/arch/arm/memset.o

include $(TOPDIR)/Rules.make
