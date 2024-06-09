SrcFiles=libevent_Web.c pub.c  
ObjFiles=$(SrcFiles:.c=.o)  
  
all: libevent_Web
  
libevent_Web: $(ObjFiles)  
	gcc -o $@ $^ -levent  
  
# 编译每个.c文件到.o文件  
%.o: %.c  
	gcc -c $< -o $@  
  
clean:  
	rm -f $(ObjFiles) libevent_Web
  
.PHONY: clean all
