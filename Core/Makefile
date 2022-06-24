CC = clang++

EXECUTABLE = bin/TrafficTracker

SOURCE = ./source/main.cpp

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)
	$(CC) $(SOURCE) -o $(EXECUTABLE) -O3 -lyaml-cpp -lreadline -lmysqlpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_dnn -lopencv_videoio -lstdc++ -lopencv_video -lm

clean:
	rm -f $(EXECUTABLE)