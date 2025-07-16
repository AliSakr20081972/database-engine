#ifndef PLANNER_PLANNER_H
#define PLANNER_PLANNER_H

#include <stdbool.h>
#include "parser/parser.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum PlanType {
    PLAN_UNKNOWN,
    PLAN_SELECT_ALL,
    PLAN_INSERT_VALUE,
} PlanType;

typedef struct Plan {
    PlanType type;
    char table[32];
    char value[256];
} Plan;

bool planner_plan(const char *query, Plan *out_plan);

#ifdef __cplusplus
}
#endif

#endif /* PLANNER_PLANNER_H */
