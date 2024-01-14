# 自定义变量
OBJ=main.o choiseFuncManu.o keyMonitoring.o newAddressBookUserInterface.o enterFunc.o dataStructure.o addressBookInit.o
TARGET=a.out

.PHONY:a.out

$(TARGET):$(OBJ)
	sudo gcc $^ -o $@
	sudo rm -rf *.o
	sudo ./a.out
	sudo rm -rf *.o a.out $(TARGET)
	clear

choiseFuncManu.o:choiseFuncManu.c
	sudo gcc -c $^ -o $@

keyMonitoring.o:keyMonitoring.c
	sudo gcc -c $^ -o $@

newAddressBookUserInterface.o:newAddressBookUserInterface.c
	sudo gcc -c $^ -o $@

enterFunc.o:enterFunc.c
	sudo gcc -c $^ -o $@

dataStructure.o:dataStructure.c
	sudo gcc -c $^ -o $@

addressBookInit.o:addressBookInit.c
	sudo gcc -c $^ -o $@

main.o:main.c
	sudo gcc -c $^ -o $@

clean:
	sudo rm -rf *.o a.out $(TARGET)
