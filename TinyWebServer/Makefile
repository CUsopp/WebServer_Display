OBJS = main.o webserver.o config.o ./log/log.o ./CGImysql/sql_connection_pool.o ./http/http_conn.o ./timer/lst_timer.o
TARGET = server

$(TARGET) : $(OBJS)
	g++ $^ -o $@ -lpthread -lmysqlclient

%.o : %.cpp
	g++ -c $< -o $@

.PHONY : clean
clean:
	rm -rf $(OBJS) $(TARGET)
