SRC_DIR = ./src
INC_DIR = ./include

lib_LTLIBRARIES = libmytcp.la
libmytcp_la_SOURCES = $(SRC_DIR)/conn.cpp $(SRC_DIR)/bsdsocket.cpp $(SRC_DIR)/state.cpp $(SRC_DIR)/unixsocket.cpp
libmytcp_la_SOURCES += $(INC_DIR)/conn.h $(INC_DIR)/socket.h $(INC_DIR)/state.h $(INC_DIR)/strategy.h $(INC_DIR)/codegen/af_x.h $(INC_DIR)/codegen/sock_x.h
libmytcp_la_CXXFLAGS = -I$(INC_DIR)