APPFILES= app/*.c
DBFILES= database/*.c
OBJFILES= *.o *.exe
CFLAG = -luser32 -Wno-implicit-function-declaration

app: app.exe
	echo "make app.exe"

app.exe: $(APPFILES)
	gcc $(CFLAG) -o $@ $(APPFILES)

db: database.exe
	echo "make database.exe"

database.exe: $(DBFILES)
	gcc $(CFLAG) -o $@ $(DBFILES)

clean:
	rm -rf $(OBJFILES)