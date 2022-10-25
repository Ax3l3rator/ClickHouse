#include <Interpreters/Context.h>
#include <Storages/MergeTree/MergeList.h>
#include <Storages/System/StorageSystemMoves.h>
#include <Access/ContextAccess.h>


namespace DB
{

NamesAndTypesList StorageSystemMoves::getNamesAndTypes()
{
    return {
        {"database", std::make_shared<DataTypeString>()},
        {"table", std::make_shared<DataTypeString>()},
        {"elapsed", std::make_shared<DataTypeFloat64>()},
        {"result_part_name", std::make_shared<DataTypeString>()},
        {"result_part_path", std::make_shared<DataTypeString>()},
        {"partition_id", std::make_shared<DataTypeString>()},
        {"part_size", std::make_shared<DataTypeUInt64>()},
        {"thread_id", std::make_shared<DataTypeUInt64>()},
    };
}


void StorageSystemMoves::fillData(MutableColumns & res_columns, ContextPtr context, const SelectQueryInfo &) const
{
    const auto access = context->getAccess();
    const bool check_access_for_tables = !access->isGranted(AccessType::SHOW_TABLES);

    for (const auto & move : context->getMovesList().get())
    {
        if (check_access_for_tables && !access->isGranted(AccessType::SHOW_TABLES, move.database, move.table))
            continue;

        size_t i = 0;
        res_columns[i++]->insert(move.database);
        res_columns[i++]->insert(move.table);
        res_columns[i++]->insert(move.elapsed);
        res_columns[i++]->insert(move.result_part_name);
        res_columns[i++]->insert(move.result_part_path);
        res_columns[i++]->insert(move.partition_id);
        res_columns[i++]->insert(move.part_size);
        res_columns[i++]->insert(move.thread_id);
    }
}

}
