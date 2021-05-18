#include "duckdb/web/webdb.h"

#include <sstream>

#include "duckdb/common/types/date.hpp"
#include "duckdb/common/types/timestamp.hpp"
#include "duckdb/execution/operator/persistent/buffered_csv_reader.hpp"
#include "duckdb/web/io/ifstream.h"
#include "duckdb/web/io/memory_filesystem.h"
#include "duckdb/web/test/config.h"
#include "gtest/gtest.h"
#include "parquet-extension.hpp"

using namespace duckdb::web;
using namespace std;

namespace {

TEST(WebDB, InvalidSQL) {
    auto db = make_shared<WebDB>();
    WebDB::Connection conn{*db};
    auto expected = conn.SendQuery(R"RAW(
        INVALID SQL
    )RAW");
    ASSERT_FALSE(expected.ok());
}

TEST(WebDB, RunQuery) {
    auto db = make_shared<WebDB>();
    WebDB::Connection conn{*db};
    auto buffer = conn.RunQuery("SELECT (v & 127)::TINYINT FROM generate_series(0, 2000) as t(v);");
    ASSERT_TRUE(buffer.ok()) << buffer.status().message();
}

TEST(WebDB, SendQuery) {
    auto db = make_shared<WebDB>();
    WebDB::Connection conn{*db};
    auto buffer = conn.SendQuery("SELECT (v & 127)::TINYINT FROM generate_series(0, 2000) as t(v);");
    ASSERT_TRUE(buffer.ok()) << buffer.status().message();
}

}  // namespace
