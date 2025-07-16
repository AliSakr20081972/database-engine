# Project Overview

This repository contains a minimal educational database engine written in C.
The goal is to provide a small code base that illustrates fundamental
components of a storage engine such as fixed-size pages and a very simple SQL
parser.

## Directory Structure

- `src/` - Source files for the demo program.
  - `storage/` - Page abstraction for reading and writing fixed-size buffers.
  - `parser/` - Tiny tokenizer for a subset of SQL.
- `include/` - Public headers.
- `Makefile` - Build script for the `db` demo executable.

## Building and Running

Compile the project using `make` and then run the `db` executable:

```sh
make
./db
```

The program will print the contents of a demo page and then list the tokens
produced from a small SQL query.
