#include "executor/executor.h"
#include <stdio.h>
#include <string.h>

void table_init(Table *table, const char *name)
{
    if (!table || !name)
        return;
    strncpy(table->name, name, sizeof(table->name) - 1);
    table->name[sizeof(table->name) - 1] = '\0';
    table->row_count = 0;
}

void table_insert(Table *table, const char *value)
{
    if (!table || !value || table->row_count >= TABLE_MAX_ROWS)
        return;
    strncpy(table->rows[table->row_count], value, ROW_SIZE - 1);
    table->rows[table->row_count][ROW_SIZE - 1] = '\0';
    table->row_count++;
}

void table_select_all(const Table *table)
{
    if (!table)
        return;
    printf("Table '%s' contents:\n", table->name);
    for (size_t i = 0; i < table->row_count; i++)
        printf("  %zu: %s\n", i + 1, table->rows[i]);
}

void executor_execute(const Plan *plan, Table *table)
{
    if (!plan || !table)
        return;
    switch (plan->type) {
    case PLAN_INSERT_VALUE:
        table_insert(table, plan->value);
        break;
    case PLAN_SELECT_ALL:
        table_select_all(table);
        break;
    default:
        break;
    }
}
