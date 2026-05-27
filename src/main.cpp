#include <iostream>

#include "execution/executor.h"
#include "sql/parser/parser.h"
#include "sql/tokenizer/tokenizer.h"
#include "storage/pager/pager.h"
#include "storage/table/table_heap.h"

using namespace std;

void printRows(
    const vector<Row>& rows
) {

    if(rows.empty()) {

        cout << "Empty Result"
             << endl;

        return;
    }

    cout << "+----------------------+"
         << endl;

    for(const Row& row : rows) {

        for(const auto& [k, v]
            : row) {

            cout << k
                 << " = "
                 << v
                 << " ";
        }

        cout << endl;
    }

    cout << "+----------------------+"
         << endl;
}

int main() {

    Pager pager("database.db");

    TableHeap table(pager);

    Executor executor(table);

    cout << "MiniDB Started"
         << endl;

    cout << "Type 'exit' to quit"
         << endl;

    while(true) {

        cout << endl
             << "MiniDB > ";

        string sql;

        getline(cin, sql);

        if(sql == "exit") {

            break;
        }

        if(sql.empty()) {

            continue;
        }

        Tokenizer tokenizer(sql);

        vector<Token> tokens =
            tokenizer.tokenize();

        Parser parser(tokens);

        shared_ptr<ASTNode> ast =
            parser.parse();

        if(ast == nullptr) {

            cout << "Parse Error"
                 << endl;

            continue;
        }

        auto insertAst =
            dynamic_pointer_cast
            <
                InsertStatementNode
            >(ast);

        if(insertAst != nullptr) {

            executor.executeInsert(
                insertAst
            );

            cout << "Row Inserted"
                 << endl;

            continue;
        }

        auto selectAst =
            dynamic_pointer_cast
            <
                SelectStatementNode
            >(ast);

        if(selectAst != nullptr) {

            vector<Row> rows =
                executor.executeSelect(
                    selectAst
                );

            printRows(rows);

            continue;
        }

        cout << "Unsupported Query"
             << endl;
    }

    pager.flushAllPages();

    cout << "MiniDB Shutdown"
         << endl;

    return 0;
}