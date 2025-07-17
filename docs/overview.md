# Project Overview

This repository contains a minimal educational database engine written in C.
The code illustrates fundamental components of a storage engine such as
fixed-size pages and a tiny SQL tokenizer. Additional demonstration modules
include a small query planner and executor, a basic thread pool and a simple
user authentication system.

## Directory Structure

- `src/` - Source files for the demo program.
  - `storage/` - Page abstraction for reading and writing fixed-size buffers.
  - `parser/` - Tiny tokenizer for a subset of SQL.
  - `planner/` - Very small query planner producing simple plan structures.
  - `executor/` - Executes query plans over in-memory tables.
  - `thread/` - Minimal thread pool implementation.
  - `security/` - Simple user account handling.
- `include/` - Public headers.
- `Makefile` - Build script for the `db` demo executable.

## Building and Running

Compile the project using `make` and then run the `db` executable:

```sh
make
./db
```

The program prints the contents of a demo page, tokenizes a query and then
shows simple planning, execution, thread pool usage and authentication
examples. After these demonstrations, an interactive command line interface
starts so you can experiment with your own SQL statements. Type `quit` to exit.
