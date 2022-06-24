CC = pandoc
FLAGS = -t beamer
TARGET = presentazione_iot.md
OUT = presentazione_iot.pdf

all: $(OUT)

$(OUT): $(TARGET)
	$(CC) -s $(TARGET) $(FLAGS) -o $(OUT)

.PHONY: clean

clean:
	rm -f $(OUT)
 