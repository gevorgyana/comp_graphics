.PHONY: clean all app check

SRC_DIR:=./src
TEST_DIR:=./tests
BUILD_DIR:=./build

TEST_SOURCES=${TEST_DIR}/*.cpp
APP_SOURCES=${SRC_DIR}/*.cpp

CC:=clang++
CFLAGS:=
DEPEND_PATH:=
INCLUDE_PATH:=

all: app check

app: ${APP_SOURCES}
	mkdir -p ${BUILD_DIR}
	${CC} $< -o ${BUILD_DIR}/application

check: ${TEST_SOURCES}
	mkdir -p ${BUILD_DIR}
	${CC} $< -o ${BUILD_DIR}/tests
