# MiniDB

A lightweight relational database engine built from scratch in **C++** to explore how modern database systems work internally.

Rather than relying on an existing database library, MiniDB implements the core components of a relational database itself—from parsing SQL queries to managing pages on disk. The goal of the project was to understand what happens beneath the abstractions provided by production databases such as PostgreSQL and SQLite.

MiniDB is the culmination of several concepts I explored through previous projects and coursework. During a Data Structures laboratory course, I implemented a **B+ Tree**, which introduced me to indexing and on-disk data structures. I also spent time **attempting to build a simple C++ compiler**, an experience that gave me practical exposure to lexical analysis, tokenization, recursive-descent parsing, and Abstract Syntax Trees (ASTs). Although that compiler project was never completed, it provided the foundation for implementing MiniDB's SQL tokenizer, parser, and AST. Bringing these ideas together in a database engine allowed me to connect concepts from data structures, compiler design, and systems programming into a single end-to-end project.

The project follows the complete lifecycle of a SQL query—from tokenization and parsing to execution on a custom storage engine backed by persistent disk storage.

---

# Features

* Persistent disk-backed storage
* Fixed-size **4 KB page architecture**
* Slotted page implementation for variable-sized records
* Binary record serialization and deserialization
* Disk Manager for page-level file I/O
* Pager for in-memory page caching
* Buffer Pool Manager architecture
* Table Heap for record management
* SQL tokenizer
* Recursive-descent SQL parser
* Abstract Syntax Tree (AST) generation
* Query execution engine
* Interactive SQL REPL
* Basic support for **INSERT** and **SELECT** statements

---

# Project Architecture

```text
                 SQL Query
                     │
                     ▼
             +----------------+
             |   Tokenizer    |
             +----------------+
                     │
                     ▼
             +----------------+
             |    Parser      |
             +----------------+
                     │
                     ▼
             +----------------+
             |      AST       |
             +----------------+
                     │
                     ▼
             +----------------+
             |   Executor     |
             +----------------+
                     │
                     ▼
             +----------------+
             |   Table Heap   |
             +----------------+
                     │
                     ▼
          +-----------------------+
          | Buffer Pool Manager   |
          +-----------------------+
                     │
                     ▼
             +----------------+
             |     Pager      |
             +----------------+
                     │
                     ▼
             +----------------+
             |  Disk Manager  |
             +----------------+
                     │
                     ▼
                database.db
```

---

# Storage Architecture

MiniDB stores all data in **4096-byte pages**, following the same page-oriented design used by modern relational database systems.

```text
database.db
│
├── Page 0
├── Page 1
├── Page 2
├── ...
└── Page N
```

Each page consists of:

* Page metadata
* Slot directory
* Serialized records

Records are identified using **Record Identifiers (RIDs)**.

```text
RID
├── Page ID
└── Slot Number
```

This layout allows variable-length records to be stored efficiently while supporting future extensions such as record deletion and page compaction.

---

# Folder Structure

```text
MiniDB
│
├── src
│   ├── storage
│   │   ├── disk
│   │   ├── pager
│   │   ├── page
│   │   ├── table
│   │   └── buffer
│   │
│   ├── parser
│   │   ├── tokenizer
│   │   ├── parser
│   │   └── ast
│   │
│   ├── executor
│   ├── repl
│   └── main.cpp
│
├── database.db
└── README.md
```

---

# Building the Project

## Requirements

* C++17 or later
* g++
* Windows or Linux

## Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/MiniDB.git
cd MiniDB
```

## Compile

### Windows

```bash
g++ -std=c++17 ^
src/main.cpp ^
src/storage/page/page.cpp ^
src/storage/disk/disk_manager.cpp ^
src/storage/pager/pager.cpp ^
src/storage/table/table_heap.cpp ^
src/storage/buffer/buffer_pool_manager.cpp ^
src/parser/tokenizer/tokenizer.cpp ^
src/parser/parser/parser.cpp ^
src/executor/executor.cpp ^
-o mini_db.exe
```

### Linux

```bash
g++ -std=c++17 \
src/main.cpp \
src/storage/page/page.cpp \
src/storage/disk/disk_manager.cpp \
src/storage/pager/pager.cpp \
src/storage/table/table_heap.cpp \
src/storage/buffer/buffer_pool_manager.cpp \
src/parser/tokenizer/tokenizer.cpp \
src/parser/parser/parser.cpp \
src/executor/executor.cpp \
-o mini_db
```

---

# Running

Windows

```bash
mini_db.exe
```

Linux

```bash
./mini_db
```

---

# Example Usage

Insert records:

```sql
INSERT INTO users VALUES (1, 'Alice');
INSERT INTO users VALUES (2, 'Bob');
```

Retrieve all records:

```sql
SELECT * FROM users;
```

Retrieve a specific record:

```sql
SELECT * FROM users WHERE id = 2;
```

Exit the REPL:

```text
exit
```

---

# Query Processing Pipeline

Every SQL query passes through the following stages:

```text
SQL Query
     │
     ▼
Tokenizer
     │
     ▼
Parser
     │
     ▼
AST
     │
     ▼
Executor
     │
     ▼
Table Heap
     │
     ▼
Buffer Pool Manager
     │
     ▼
Pager
     │
     ▼
Disk Manager
     │
     ▼
database.db
```

---

# Core Components

### Disk Manager

Handles reading and writing fixed-size pages between memory and the database file.

### Pager

Caches pages in memory to reduce disk I/O and provides page-level access to higher layers.

### Buffer Pool Manager

Maintains a limited in-memory page cache and coordinates page replacement.

### Slotted Pages

Implements an efficient page layout for storing variable-length records while enabling future support for updates and deletions.

### Table Heap

Stores table records across multiple pages and returns Record Identifiers (RIDs) for retrieval.

### Tokenizer

Converts raw SQL into a sequence of lexical tokens.

Example:

```sql
SELECT * FROM users WHERE id = 1;
```

becomes

```text
SELECT
STAR
FROM
IDENTIFIER(users)
WHERE
IDENTIFIER(id)
EQUAL
NUMBER(1)
```

### Parser

Uses recursive-descent parsing to transform the token stream into an Abstract Syntax Tree.

### AST

Represents SQL statements as structured C++ objects that can be executed independently of the original query string.

### Executor

Traverses the AST and performs the requested operations on the storage engine.

---

# Supported SQL

```sql
INSERT INTO users VALUES (1, 'Alice');

INSERT INTO users VALUES (2, 'Bob');

SELECT * FROM users;

SELECT * FROM users WHERE id = 2;
```

---

# Current Limitations

MiniDB is designed as an educational database engine, so several advanced database features are intentionally left for future work.

* B+ Tree indexes
* UPDATE
* DELETE
* JOIN operations
* Transactions
* Write-Ahead Logging (WAL)
* Crash recovery
* Query optimizer
* Concurrency control
* Secondary indexes

---

# Tech Stack

* C++
* STL
* File I/O
* Object-Oriented Programming
* Systems Programming

---

# What I Learned

MiniDB became much more than a database project—it brought together concepts from multiple areas of computer science that I had explored independently over time.

Implementing a B+ Tree during a Data Structures course gave me an appreciation for efficient indexing and storage structures. Separately, while attempting to build a simple C++ compiler, I learned about lexical analysis, tokenization, recursive-descent parsing, and Abstract Syntax Trees. Even though that compiler was never completed, the experience proved invaluable when implementing MiniDB's SQL parsing pipeline.

Building MiniDB allowed me to combine these ideas with storage management, page organization, binary serialization, and query execution into a cohesive system. More importantly, it taught me how large software systems are built from modular components, where each layer has a clearly defined responsibility. Implementing these components from scratch gave me a much deeper understanding of how relational databases work internally and significantly strengthened my skills in systems programming, software architecture, and modern C++.
