#ifndef INDEXES_H
#define INDEXES_H
#include "rapidjson/document.h"
#include "types.h"
#include "utils.h"

using namespace std;
/** Parse the indexes section of SDI JSON
@param[in]	    dd_object	    Data Dictionary JSON object
@param[in,out]	ddl     	    DDL string
@return False in case of errors */
static bool parse_indexes(const rapidjson::Value::ConstObject &dd_object,
                          string &ddl) {
  if (!dd_object.HasMember("indexes")) {
    cout << "Error Reading indexes from dd_object" << endl;
    return false;
  }
  for (auto index = dd_object["indexes"].Begin();
       index != dd_object["indexes"].End(); ++index) {
    enum_index_type index_type;
    if (!index->IsObject()) {
      cout << "Error Reading index object from dd_object" << endl;
      return false;
    }
    if (!index->HasMember("hidden")) {
      cout << "Error Reading elements from index object" << endl;
      return false;
    }
    if ((*index)["hidden"].GetBool()) continue;

    if (!index->HasMember("elements")) {
      cout << "Error Reading elements from index object" << endl;
      return false;
    }
    if (!index->HasMember("type")) {
      cout << "Error Reading type from index object" << endl;
      return false;
    }
    if (!index->HasMember("name")) {
      cout << "Error Reading name from index object" << endl;
      return false;
    }
    index_type = static_cast<enum_index_type>((*index)["type"].GetInt());
    switch (index_type) {
      case IT_PRIMARY:
        ddl += " PRIMARY KEY (";
        break;
      case IT_UNIQUE:
        ddl += " UNIQUE KEY ";
        ddl += scape_string((*index)["name"].GetString());
        ddl += " (";
        break;
      case IT_MULTIPLE:
        ddl += " KEY ";
        ddl += scape_string((*index)["name"].GetString());
        ddl += " (";
        break;
      default:
        cout << "Unsuported Index Type: " << index_type << endl;
        return false;
        break;
    }
    for (auto index_col = (*index)["elements"].Begin();
         index_col != (*index)["elements"].End(); ++index_col) {
      if (!index_col->IsObject()) {
        cout << "Error Reading index column object from dd_object" << endl;
        return false;
      }
      if (!index_col->HasMember("hidden")) {
        cout << "Error Reading hidden from index column object" << endl;
        return false;
      }
      if ((*index_col)["hidden"].GetBool()) continue;

      if (!index_col->HasMember("column_opx")) {
        cout << "Error Reading column_opx from index column object" << endl;
        return false;
      }
      if (column_map.find((*index_col)["column_opx"].GetInt()) ==
          column_map.end()) {
        cout << "Error Index column not found on hash map" << endl;
        return false;
      }
      column_t col = column_map[(*index_col)["column_opx"].GetInt()];
      if (index_type == IT_MULTIPLE && col.hidden == HT_HIDDEN_SQL) {
        ddl += "(";
        ddl += col.generation_expression;
        ddl += "),";
      } else {
        ddl += scape_string(col.name);
        /* check prefix index */
        if (support_prefix_index(col)) {
          if ((*index_col)["length"].GetInt() != col.size) {
            ddl += "(";
            int size = (*index_col)["length"].GetInt() / col.collation.max_len;
            ddl += to_string(size);
            ddl += ")";
          }
        }
        ddl += ",";
      }
    }
    ddl.pop_back();
    ddl += "),\n";
  }
  return true;
}
#endif /* INDEXES_H */
