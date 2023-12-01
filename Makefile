APPFILES= app/*.c
EJ_1_FILES= ejercicio_1/*.c
EJ_2_FILES= ejercicio_2/*.c
OBJFILES= *.o *.exe
CFLAG = -luser32 -Wno-implicit-function-declaration

app: app.exe
	echo "make app.exe"

app.exe: $(APPFILES)
	gcc $(CFLAG) -o $@ $(APPFILES)

1: 1.exe
	echo "make 1.exe"

1.exe: $(EJ_1_FILES)
	gcc $(CFLAG) -o $@ $(EJ_1_FILES)

2: 2.exe
	echo "make 2.exe"

2.exe: $(EJ_2_FILES)
	gcc $(CFLAG) -o $@ $(EJ_2_FILES)

clean:
	rm -rf $(OBJFILES)
	rm -rf *.log