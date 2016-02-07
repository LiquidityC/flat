#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string>

#include "Database.h"

namespace flat2d
{
	namespace db
	{
		Database::Database(const std::string& filename)
		{
			database = nullptr;
			open(filename);
		}

		Database::~Database()
		{
			close();
		}

		bool Database::open(const std::string& filename)
		{
			if (sqlite3_open(filename.c_str(), &database) == SQLITE_OK) {
				return true;
			}

			return false;
		}

		Database::Result Database::query(const std::string& query)
		{
			sqlite3_stmt* stmt;
			Result result_set;

			if (sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK) {
				int cols = sqlite3_column_count(stmt);
				int result = 0;
				while (true) {
					result = sqlite3_step(stmt);
					if (result == SQLITE_ROW) {
						Row values;
						for (int col = 0; col < cols; col++) {
							values.push_back( (const char*) sqlite3_column_text(stmt, col));
						}
						result_set.push_back(values);
					} else {
						break;
					}
				}
			}

			std::string error = sqlite3_errmsg(database);
			if (error != "unknown error") {
				std::cout << query << " " << error << std::endl;
			}

			sqlite3_finalize(stmt);

			return result_set;
		}

		unsigned int Database::get_table_count(const std::string& table_name)
		{
			Result result = query("SELECT COUNT(*) FROM " + table_name);
			unsigned int count = result.empty() ? 0 : atoi(result[0][0].c_str());

			return count;
		}

		void Database::close()
		{
			if (database != nullptr) {
				sqlite3_close(database);
			}
		}
	} // namespace db NOLINT(readability/namespace)
} // namespace flat2d
