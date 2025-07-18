#include <stdio.h>
#include "storage/page.h"
#include "parser/parser.h"
#include "planner/planner.h"
#include "executor/executor.h"
#include "cli/cli.h"
#include "thread/thread_pool.h"
#include "security/user.h"

typedef struct { Plan plan; Table *table; } ExecArg;
static void exec_task(void *arg)
{
    ExecArg *e = (ExecArg*)arg;
    executor_execute(&e->plan, e->table);
}

/*
 * Entry point demonstrating usage of the storage and parser modules.
 *
 * The program creates a single page, writes a string into it and prints the
 * serialized contents. It then tokenizes a small SQL query and prints each
 * token to STDOUT.
 */

int main(int argc, char *argv[])
{
    bool run_demo = true;
    if (argc > 1 && strcmp(argv[1], "--cli-only") == 0)
        run_demo = false;

    Table table;
    table_init(&table, "table1");

    if (run_demo) {
        Page page;
        page_init(&page, 1);

    /* Demo: modify page data */
    const char *msg = "hello";
    memcpy(page_data_mut(&page), msg, 6); /* include null terminator */
    page_mark_dirty(&page, true);

    uint8_t buffer[PAGE_SIZE];
    page_to_bytes(&page, buffer);

    printf("Page ID: %llu\n", (unsigned long long)page_get_id(&page));
    printf("Dirty: %d\n", page_is_dirty(&page));
    printf("Data: %s\n", (char*)buffer);

    /* Demo: simple SQL tokenization */
    const char *query = "SELECT * FROM table1;";
    Parser parser;
    parser_init(&parser, query);

    printf("Tokens:\n");
    for (;;) {
        Token tok = parser_next_token(&parser);
        if (tok.type == TOKEN_EOF)
            break;
        printf("  %d: '%.*s'\n", tok.type, (int)tok.length, tok.start);
    }

    /* Demo: planner and executor */

        Plan plan;
        const char *insert_q = "INSERT INTO table1 VALUES ('abc');";
        if (planner_plan(insert_q, &plan))
            executor_execute(&plan, &table);

        const char *select_q = "SELECT * FROM table1;";
        if (planner_plan(select_q, &plan))
            executor_execute(&plan, &table);

    /* Demo: user security */
    User user;
    user_create(&user, "alice", "secret");
    printf("Auth success: %d\n", user_authenticate(&user, "alice", "secret"));

    /* Demo: thread pool executing two inserts */
    ThreadPool pool;
        if (!thread_pool_init(&pool, 2)) {
            fprintf(stderr, "Failed to initialize thread pool\n");
            return 1;
        }

    Plan p1, p2;
        if (!planner_plan("INSERT INTO table1 VALUES ('thread1');", &p1) ||
            !planner_plan("INSERT INTO table1 VALUES ('thread2');", &p2)) {
            fprintf(stderr, "Failed to plan insert statements\n");
            thread_pool_shutdown(&pool);
            return 1;
        }

    ExecArg a1 = { p1, &table };
    ExecArg a2 = { p2, &table };

        if (!thread_pool_submit(&pool, exec_task, &a1))
            fprintf(stderr, "Failed to submit task 1\n");
        if (!thread_pool_submit(&pool, exec_task, &a2))
            fprintf(stderr, "Failed to submit task 2\n");
        thread_pool_shutdown(&pool);

        if (planner_plan(select_q, &plan))
            executor_execute(&plan, &table);

    }
    /* Start interactive CLI */
    cli_run(&table);

    return 0;
}
