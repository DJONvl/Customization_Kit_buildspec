unexport O_TARGET
unexport obj-y
unexport subdir-y

source := $(patsubst %.o,%.c, $(obj-y))

first_rule: $(subdir-y) $(O_TARGET)

.PHONY: $(subdir-y)

$(subdir-y):
	$(MAKE) -C $@	

$(O_TARGET): $(obj-y)
	$(LD) -r -o $@ $^

sinclude $(source:.c=.d)
%d: %c
	$(CC) $(CFLAGS)  -MM  $< > $@
	
%.o:%.S
	$(CC)  $(ASFLAGS) -Wall -c -O2 -o $@ $< 
	
