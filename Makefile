CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage

STRING_SRC = string.c
SPRINTF_SRC = sprintf.c
SRC = $(STRING_SRC) $(SPRINTF_SRC)

STRING_OBJ = $(STRING_SRC:.c=.o)
SPRINTF_OBJ = $(SPRINTF_SRC:.c=.o)
OBJ = $(STRING_OBJ) $(SPRINTF_OBJ)

TEST_STRING_SRC = test_string.c
TEST_SPRINTF_SRC = test_sprintf.c
TEST_STRERROR_SRC = test_strerror.c
TEST_STRTOK_SRC = test_strtok.c

TEST_STRING_OBJ = $(TEST_STRING_SRC:.c=.o)
TEST_SPRINTF_OBJ = $(TEST_SPRINTF_SRC:.c=.o)
TEST_STRERROR_OBJ = $(TEST_STRERROR_SRC:.c=.o)
TEST_STRTOK_OBJ = $(TEST_STRTOK_SRC:.c=.o)

PKG_CONFIG := $(shell command -v pkg-config 2> /dev/null)
ifeq ($(PKG_CONFIG),)
	CHECK_CFLAGS =
	CHECK_LIBS =
else
	CHECK_CFLAGS = $(shell pkg-config --cflags check)
	CHECK_LIBS = $(shell pkg-config --libs check)
endif

string.a: $(OBJ)
	ar rcs string.a $(OBJ)

all: string.a

%.o: %.c string.h sprintf.h
	$(CC) $(CFLAGS)  -c $< -o $@

test_string: $(OBJ) $(TEST_STRING_OBJ)
	$(CC) $(CFLAGS) $(CHECK_CFLAGS) -o $@ $^ $(CHECK_LIBS) -lm -pthread

test_sprintf: $(OBJ) $(TEST_SPRINTF_OBJ)
	$(CC) $(CFLAGS) $(CHECK_CFLAGS) -o $@ $^ $(CHECK_LIBS) -lm -pthread

test_strerror: $(OBJ) $(TEST_STRERROR_OBJ)
	$(CC) $(CFLAGS) $(CHECK_CFLAGS) -o $@ $^ $(CHECK_LIBS) -lm -pthread

test_strtok: $(OBJ) $(TEST_STRTOK_OBJ)
	$(CC) $(CFLAGS) $(CHECK_CFLAGS) -o $@ $^ $(CHECK_LIBS) -lm -pthread

test: test_string test_sprintf test_strerror test_strtok
	./test_string
	./test_sprintf
	./test_strerror
	./test_strtok

rebuild: clean all

run_tests: string.a test
	@echo "Все тесты выполнены успешно!"


gcov_report: clean
	@echo "Генерация отчета покрытия с lcov..."
	$(CC) $(CFLAGS) $(GCOVFLAGS) $(TEST_STRING_SRC) $(STRING_SRC) $(SPRINTF_SRC) -o test_string_coverage -lcheck -lsubunit -lm -pthread
	$(CC) $(CFLAGS) $(GCOVFLAGS) $(TEST_SPRINTF_SRC) $(STRING_SRC) $(SPRINTF_SRC) -o test_sprintf_coverage -lcheck -lsubunit -lm -pthread
	$(CC) $(CFLAGS) $(GCOVFLAGS) $(TEST_STRERROR_SRC) $(STRING_SRC) $(SPRINTF_SRC) -o test_strerror_coverage -lcheck -lsubunit -lm -pthread
	$(CC) $(CFLAGS) $(GCOVFLAGS) $(TEST_STRTOK_SRC) $(STRING_SRC) $(SPRINTF_SRC) -o test_strtok_coverage -lcheck -lsubunit -lm -pthread
	@echo "Запуск тестов с coverage..."
	./test_string_coverage
	./test_sprintf_coverage
	./test_strerror_coverage
	./test_strtok_coverage
	@echo "Создание HTML отчета с lcov..."
	lcov -t "string" -o coverage.info -c -d .
	genhtml -o ./report coverage.info
	@echo "Отчет создан в папке ./report"
	@echo "Для просмотра отчета:"
	@echo "Откройте файл: ./report/index.html"


clean:
	rm -f *.o *.a test_string test_sprintf test_strerror test_strtok
	rm -f test_*_coverage *.gcda *.gcno *.gcov coverage.info
	rm -rf ./report