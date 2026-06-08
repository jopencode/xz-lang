#pragma once

#include "ast.h"
#include "environment.h"
#include "value.h"

Value execute(AstNode *node, Environment *env);
