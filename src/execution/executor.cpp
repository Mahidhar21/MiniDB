#include "executor.h"

using namespace std;

Executor::Executor(
    TableHeap& table
)
    : table_(table) {
}

bool Executor::evaluateWhere(
    const Row& row,
    shared_ptr<ExpressionNode> expr
) {

    auto binary =
        dynamic_pointer_cast
        <
            BinaryExpressionNode
        >(expr);

    if(binary == nullptr) {
        return true;
    }

    auto left =
        dynamic_pointer_cast
        <
            IdentifierNode
        >(binary->left);

    auto right =
        dynamic_pointer_cast
        <
            NumberNode
        >(binary->right);

    auto it =
        row.find(left->name);

    if(it == row.end()) {
        return false;
    }

    int value =
        stoi(it->second);

    return value == right->value;
}

void Executor::executeInsert(
    shared_ptr<InsertStatementNode>
    insert
) {

    Row row;

    row["id"] =
        insert->values[0];

    row["name"] =
        insert->values[1];

    table_.insertRow(row);
}

vector<Row>
Executor::executeSelect(
    shared_ptr<SelectStatementNode>
    select
) {

    vector<Row> results;

    vector<Row> rows =
        table_.scanTable();

    for(const Row& row : rows) {

        if(select->whereClause
           != nullptr) {

            if(!evaluateWhere(
                    row,
                    select->whereClause
                )) {

                continue;
            }
        }

        results.push_back(row);
    }

    return results;
}