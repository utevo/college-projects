#ifndef GET_HANDLER_H
#define GET_HANDLER_H
#include "../command.h"
#include "../saver.h"
#include "../reader.h"
#include "../connector.h"
#include <sstream>
#include <limits>
#include <sys/types.h>


bool get_handler(Command c, Saver *saver, Connector *);

bool put_handler(Command c, Reader *reader, Connector *);

bool get_dir_handler(Command c, Connector *);

bool record_handler(Command c, Connector *);

#endif