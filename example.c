#include "src/logger.h"

int main(){
	initLogger("log.log");
	LOG_INFO("Hello world!");
	LOG_DEBUG("Hello world this is a debug message! [%s]: %i", "example.c", 6);
	LOG_ERROR("Hello world this is an error %s!","404");
	LOG_WARN("Hello world this is a warning!");
}