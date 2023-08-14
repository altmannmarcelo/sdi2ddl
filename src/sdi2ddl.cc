#include <fstream>
#include <iostream>
#include <unordered_map>
#include "include/collations.h"
#include "include/columns.h"
#include "include/foreign_keys.h"
#include "include/indexes.h"
#include "include/types.h"
#include "include/utils.h"
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

/* Table charset */
static collation_info table_collation;

/** Parse Collation
@param[in]	    collation	  Table collation object
@param[in,out]	ddl         DDL string
@return False in case of errors
*/
static bool parse_collation(const collation_info &collation, string &ddl) {
  ddl += " DEFAULT CHARSET=";
  ddl += collation.charset;

  if (!collation.is_default) {
    ddl += " COLLATE=";
    ddl += collation.collation;
  }
  return true;
};

/** Parse the engine section of SDI JSON
@param[in]	    dd_object	Data Dictionary JSON object
@param[in,out]	ddl     	DDL string
@return False in case of errors */
static bool parse_engine(const rapidjson::Value::ConstObject &dd_object,
                         string &ddl) {
  if (!dd_object.HasMember("engine")) {
    cout << "Error Reading engine from dd_object" << endl;
    return false;
  }
  ddl += " ENGINE=";
  ddl += dd_object["engine"].GetString();
  return true;
}

/** Parse the table section of SDI JSON
@param[in]	    doc	  Root JSON document
@param[in,out]	ddl   DDL string
@return False in case of errors */
static bool parse_table(const Document &doc, string &ddl) {
  for (rapidjson::Value::ConstValueIterator itr = doc.Begin(); itr != doc.End();
       ++itr) {
    if (itr->IsObject() && itr->HasMember("object") &&
        (*itr)["object"].HasMember("dd_object_type") &&
        strcmp((*itr)["object"]["dd_object_type"].GetString(), "Table") == 0 &&
        (*itr)["object"].HasMember("dd_object")) {
      const rapidjson::Value::ConstObject dd_object =
          (*itr)["object"]["dd_object"].GetObject();

      /* table name */
      if (!dd_object.HasMember("name")) {
        cout << "Error Reading Table Name from dd_object" << endl;
        return false;
      }
      ddl += scape_string(dd_object["name"].GetString());
      ddl += " (\n";

      /* table collation */
      if (!dd_object.HasMember("collation_id")) {
        cout << "Error Reading Table collation_id from dd_object" << endl;
        return false;
      }
      find_collation(dd_object["collation_id"].GetInt(), table_collation);

      /* table schema */
      if (!dd_object.HasMember("schema_ref")) {
        cout << "Error Reading Table schema_ref from dd_object" << endl;
        return false;
      }
      table_schema = dd_object["schema_ref"].GetString();

      if (!parse_columns(dd_object, ddl)) return false;
      if (!parse_indexes(dd_object, ddl)) return false;

      if (!parse_foreign_keys(dd_object, ddl)) return false;

      ddl.erase(ddl.size() - 2);
      ddl += "\n";

      ddl += ")";
      if (!parse_engine(dd_object, ddl)) return false;
      if (!parse_collation(table_collation, ddl)) return false;
    }
  }
  return true;
}

int main() {
  populate_charsets();
  string line;
  string json;
  while (getline(std::cin, line) && !line.empty()) {
    json.append(line);
    json.append("\n");
  }

  Document doc;
  doc.Parse(json.c_str());
  if (doc.HasParseError()) {
    cout << "Error parsing JSON: " << doc.GetParseError() << endl;
    return 1;
  }

  string ddl = "CREATE TABLE ";

  if (!parse_table(doc, ddl)) return 1;

  ddl += ";";
  cout << ddl << endl;
  return 0;
}
