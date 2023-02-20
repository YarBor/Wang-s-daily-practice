Sales_data : Sales_data_main.o Sales_data.o
	g++ -o Sales_data Sales_data_main.o Sales_data.o
	rm *.o

Sales_data_main.o : Sales_data_main.cpp Sales_data.h
	g++ -c Sales_data_main.cpp

Sales_data.o : Sales_data.cpp Sales_data.h
	g++ -c Sales_data.cpp

clean :
	rm *.o Sales_data