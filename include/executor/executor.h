#ifndef EXECUTOR_EXECUTOR_H
#define EXECUTOR_EXECUTOR_H

#include <stddef.h>
#include "planner/planner.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TABLE_MAX_ROWS 1024
#define ROW_SIZE 256

typedef struct Table {
    char name[32];
    char rows[TABLE_MAX_ROWS][ROW_SIZE];
    size_t row_count;
} Table;

void table_init(Table *table, const char *name);
void table_insert(Table *table, const char *value);
void table_select_all(const Table *table);
void executor_execute(const Plan *plan, Table *table);

#ifdef __cplusplus
}
#endif

#endif /* EXECUTOR_EXECUTOR_H */
