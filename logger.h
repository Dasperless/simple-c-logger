#ifndef LOGGER_H
#define LOGGER_H

//Includes para el logger
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>

//log level enum
typedef enum{
	ERROR,
	WARNING,
	INFO,
	DEBUG
}logLevel;

// Definiciones para el logger
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define LOG_DEBUG(fmt, ...) logger_log(DEBUG, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  logger_log(INFO , __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  logger_log(WARNING , __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) logger_log(ERROR, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)

enum{
	maxLogSizeName = 255
};

FILE *logFile; //Puntero del archivo



/**
 * @brief Escribe la fecha actual
 * 
 */
void currentDate(){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	fprintf(logFile, "%d-%d-%d %d:%d:%d  \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

/**
 * @brief Escribe en el archivo de log
 * 
 * @param message mensaje a escribir
 */
void logInfo(char* message){
	currentDate();
	fprintf(logFile, "INFO: %s\n", message);
	fflush(logFile);
}

/**
 * @brief Escribe un mensaje de advertencia en el archivo de log
 * 
 * @param message El mensaje a escribir
 */
void logWarning(char* message){
	currentDate();
	fprintf(logFile, "WARNING: %s\n", message);
	fflush(logFile);
}

/**
 * @brief Escribe un mensaje de error en el archivo de log
 * 
 * @param message El mensaje a escribir
 */
void logError(char* message){
	currentDate();
	fprintf(logFile, "ERROR: %s\n", message);
	fflush(logFile);
}

/**
 * @brief Obtiene un stringo con el nombre del nivel de log
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
 * @brief Escribe un mensaje en el archivo de log
 * 
 * @param level El nivel del logger
 * @param file  El archivo donde se llama el logger
 * @param line 	La linea donde se llama el logger
 * @param fmt 	El formato del mensaje
 * @param ... 	Argumentos del mensaje
 */
void logger_log(logLevel level, const char* file, int line, const char *fmt, ...){
	if(logFile == NULL){
		printf("No se ha inicializado el logger\n");
		return;
	}

	va_list args;

	//Ingresa la fecha y hora, nivel y el nombre del archivo con su linea
	currentDate();
	fprintf(logFile, "[%s]: ", getLogLevel(level));// El nivel del log
	fprintf(logFile, "%s:%d: ", file, line);//El archivo y el numero de linea

	//Se imprimen los argumentos del mensaje
	va_start(args, fmt); 
	vfprintf(logFile, fmt, args);
	va_end(args);

	//Salto de línea
	fprintf(logFile, "%s", "\n");
	fflush(logFile);
}

/**
 * @brief Inicializa el logger
 * 
 * @param logFileName El nombre del archivo de log
 * @return int 0 en caso de exito, 1 en caso de error
 */
int initLogger(char* logFileName){
	if (logFileName == NULL){
		printf("El nombre del archivo no puede ser nulo!\n");
		return 1;
	}
	if(strlen(logFileName)> maxLogSizeName){
		printf("Nombre de archivo demasiado largo!\n");
		return 1;
	}
	logFile = fopen(logFileName, "a+");

	return 0;
}

#endif // LOGGER_H 