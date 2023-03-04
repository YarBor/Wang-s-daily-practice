HotelManageSys : hotelManageSys.o hotelRoom_listFun.o
	cc  hotelManageSys.o hotelRoom_listFun.o -o HotelManageSys 

cc = gcc

hotelManageSys.o : hotelManageSys.c hotelManageSys.h 
	cc -c hotelManageSys.c hotelManageSys.h

hotelRoom_listFun.o : hotelRoom_listFun.c hotelManageSys.h 
	cc -c hotelRoom_listFun.c hotelManageSys.h

begin :
	rm usrnameAndPassword
	touch usrnameAndPassword
	rm HotelManageSys
	make 
	make clean
	make rmdata

clean :
	rm *.o *.gch

rmdata : 
	rm *.data 