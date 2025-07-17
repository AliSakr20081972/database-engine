#ifndef CLI_CLI_H
#define CLI_CLI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "executor/executor.h"

/* Run an interactive command-line interface allowing users to submit
 * simple SQL statements. Supported statements are the same as the
 * planner and executor modules: SELECT * FROM <table>; and
 * INSERT INTO <table> VALUES ('value');
 */
void cli_run(Table *table);

#ifdef __cplusplus
}
#endif

#endif /* CLI_CLI_H */
