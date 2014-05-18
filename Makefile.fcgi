CC=g++
CFLAGS=-c -O -O1 -O2 -O3 -std=c++0x
MODULE_DIR=module/

all: highlight

highlight: main_fcgi.o data.o php.o js.o html.o css.o sql.o
	$(CC) main_fcgi.o data.o php.o js.o html.o css.o sql.o -fPIC -lfastcgi-daemon2 -shared -o highlight.so

main_fcgi.o: main_fcgi.cpp
	$(CC) $(CFLAGS) main_fcgi.cpp

data.o: data.cpp
	$(CC) $(CFLAGS) data.cpp

php.o: $(MODULE_DIR)php.cpp
	$(CC) $(CFLAGS) $(MODULE_DIR)php.cpp

html.o: $(MODULE_DIR)html.cpp
	$(CC) $(CFLAGS) $(MODULE_DIR)html.cpp

js.o: $(MODULE_DIR)js.cpp
	$(CC) $(CFLAGS) $(MODULE_DIR)js.cpp

css.o: $(MODULE_DIR)css.cpp
	$(CC) $(CFLAGS) $(MODULE_DIR)css.cpp

sql.o: $(MODULE_DIR)sql.cpp
	$(CC) $(CFLAGS) $(MODULE_DIR)sql.cpp

clean:
	rm -rf *.o highlight.so
