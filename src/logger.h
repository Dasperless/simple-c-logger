#ifndef LOGGER_H
#define LOGGER_H

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


// logger definitions
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define LOG_DEBUG(fmt, ...) logger_log(DEBUG, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  logger_log(INFO , __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  logger_log(WARNING , __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) logger_log(ERROR, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)

#define MAXFILENAME 255

//loglevel enum
typedef enum{
	ERROR,
	WARNING,
	INFO,
	DEBUG
}logLevel;


FILE *logFile; //file pointer



/**
 * @brief Write the current date
 * 
 */
void currentDate(){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	fprintf(logFile, "%d-%d-%d %d:%d:%d  \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

/**
 * @brief Get loglevel name
 * 
 * @param level 
 * @return char* 
 */
char* getLogLevel(logLevel level){
	switch(level){
		case ERROR:
			return "ERROR";
		case WARNING:
			return "WARNING";
		case INFO:
			return "INFO";
		case DEBUG:
			return "DEBUG";
		default:
			return "UNKNOWN";
	}
}

/**
 * @brief Write a message in the logger file
 * 
 * @param level The logger level
 * @param file  The file where the logger is called
 * @param line 	The line where the logger is called
 * @param fmt 	The format
 * @param ... 	Args
 */
void logger_log(logLevel level, const char* file, int line, const char *fmt, ...){
	// Check if it is initialized
	if(logFile == NULL){
		printf("Logger has not been initialized\n");
		return;
	}

	va_list args;

	// Write the current date, the level, the filename and its line
	currentDate();
	fprintf(logFile, "[%s]: ", getLogLevel(level));		// The loglevel
	fprintf(logFile, "%s:%d: ", file, line);			// The file and the line

	// Write the arguments in the file
	va_start(args, fmt); 
	vfprintf(logFile, fmt, args);
	va_end(args);

	// Newline
	fprintf(logFile, "%s", "\n");
	fflush(logFile);
}

/**
 * @brief Initialize the logger
 * 
 * @param logFileName The logfile name
 * @return int 0 on succcess, 1 in case of error.
 */
int initLogger(char* logFileName){
	if (logFileName == NULL){
		printf("The file name cannot be null!\n");
		return 1;
	}
	if(strlen(logFileName)> MAXFILENAME){
		printf("File name too long!\n");
		return 1;
	}
	logFile = fopen(logFileName, "a+");

	return 0;
}

#endif // LOGGER_H 