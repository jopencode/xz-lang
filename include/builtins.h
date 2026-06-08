#pragma once

#include "environment.h"
#include "value.h"

Value native_print(int argcount, Value *args);
void register_natives(Environment *env);
