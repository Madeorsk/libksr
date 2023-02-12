#include <ksr/logging.h>

int main(void)
{
	ksrlog_error("test message.");
	ksrlog_warning("test message.");
	ksrlog_success("test message.");
}
