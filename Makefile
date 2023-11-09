CFLAG = -Wno-implicit-function-declaration
src_file = *.c
obj_file = $(patsubst %.c, %.o, $(src_file))

all: app.exe

app.exe: $(src_file)
	gcc $(CFLAG) -o $@ $(src_file)


clean:
	rm -rf $(obj_file) *.exe