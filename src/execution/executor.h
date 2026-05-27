#pragma once

#include <memory>
#include <vector>

#include "../sql/ast/ast.h"
#include "../storage/table/table_heap.h"

using namespace std;

class Executor {
private:
    TableHeap& table_;

    bool evaluateWhere(
        const Row& row,
        shared_ptr<ExpressionNode> expr
    );

public:
    explicit Executor(
        TableHeap& table
    );

    void executeInsert(
        shared_ptr<InsertStatementNode>
        insert
    );

    vector<Row> executeSelect(
        shared_ptr<SelectStatementNode>
        select
    );
};