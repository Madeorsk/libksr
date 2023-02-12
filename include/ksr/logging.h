#pragma once

#include <stdio.h>

/*
 * Logging constants, that can be used to style logs.
 */

#define TERMSTYLE_RESET            "\033[0m"
#define TERMSTYLE_DEFAULT_COLOR    "\033[39m"

#define TERMSTYLE_BLACK            "\033[30m"
#define TERMSTYLE_RED              "\033[31m"
#define TERMSTYLE_GREEN            "\033[32m"
#define TERMSTYLE_YELLOW           "\033[33m"
#define TERMSTYLE_BLUE             "\033[34m"
#define TERMSTYLE_MAGENTA          "\033[35m"
#define TERMSTYLE_CYAN             "\033[36m"
#define TERMSTYLE_WHITE            "\033[37m"

#define TERMSTYLE_BOLD             "\033[1m"
#define TERMSTYLE_DIM              "\033[2m"
#define TERMSTYLE_UNDERLINED       "\033[4m"
#define TERMSTYLE_BLINK            "\033[5m"
#define TERMSTYLE_INVERTED         "\033[7m"
#define TERMSTYLE_HIDDEN           "\033[8m"

#define TERMSTYLE_BOLD_RESET       "\033[22m"
#define TERMSTYLE_DIM_RESET        "\033[22m"
#define TERMSTYLE_UNDERLINED_RESET "\033[24m"
#define TERMSTYLE_BLINK_RESET      "\033[25m"
#define TERMSTYLE_INVERTED_RESET   "\033[27m"
#define TERMSTYLE_HIDDEN_RESET     "\033[28m"

/*
 * Default file descriptors for out stream and error stream.
 */

#define KSRLOG_ERR_STREAM stderr
#define KSRLOG_OUT_STREAM stdout


/**
 * Create a log string.
 */
#define _ksrlog_log_str(label, color, str, padding_str) padding_str color TERMSTYLE_BOLD "[" label "]" TERMSTYLE_BOLD_RESET " " str TERMSTYLE_DEFAULT_COLOR "\n"

/**
 * Log an error.
 */
static inline void ksrlog_error(const char *message)
{
	fprintf(KSRLOG_ERR_STREAM, _ksrlog_log_str("ERROR", TERMSTYLE_RED, "%s", "  "), message);
}

/**
 * Log a warning.
 */
static inline void ksrlog_warning(const char *message)
{
	fprintf(KSRLOG_ERR_STREAM, _ksrlog_log_str("WARNING", TERMSTYLE_YELLOW, "%s", ""), message);
}

/**
 * Log a success.
 */
static inline void ksrlog_success(const char *message)
{
	fprintf(KSRLOG_OUT_STREAM, _ksrlog_log_str("SUCCESS", TERMSTYLE_GREEN, "%s", ""), message);
}
