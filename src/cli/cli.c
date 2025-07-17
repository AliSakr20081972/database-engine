#include "cli/cli.h"
#include "planner/planner.h"
#include <stdio.h>
#include <string.h>

void cli_run(Table *table)
{
    if (!table)
        return;

    char line[512];
    printf("Enter SQL statements or 'quit' to exit.\n");
    while (1) {
        printf("db> ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin))
            break;
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, "quit") == 0 || strcmp(line, "exit") == 0)
            break;
        if (line[0] == '\0')
            continue;
        Plan plan;
        if (planner_plan(line, &plan))
            executor_execute(&plan, table);
        else
            printf("Error: could not parse input.\n");
    }
}
