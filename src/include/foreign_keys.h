#ifndef FOREIGN_KEYS_H
#define FOREIGN_KEYS_H
#include <vector>
#include "rapidjson/document.h"
#include "table.h"
#include "types.h"
#include "utils.h"

using namespace std;

/** Parse the foreign_keys section of SDI JSON
@param[in]	    dd_object	    Data Dictionary JSON object
@param[in,out]	ddl     	    DDL string
@return False in case of errors */
static bool parse_foreign_keys(const rapidjson::Value::ConstObject &dd_object,
                               string &ddl) {
  vector<string> reference_names;
  if (!dd_object.HasMember("foreign_keys")) {
    cout << "Error Reading foreign_keys from dd_object" << endl;
    return false;
  }
  for (auto fk = dd_object["foreign_keys"].Begin();
       fk != dd_object["foreign_keys"].End(); ++fk) {
    reference_names.clear();
    if (!fk->IsObject()) {
      cout << "Error Reading foreign_key object from dd_object" << endl;
      return false;
    }
    if (!fk->HasMember("name")) {
      cout << "Error Reading element name from fk object" << endl;
      return false;
    }
    if (!fk->HasMember("elements")) {
      cout << "Error Reading element elements from fk object" << endl;
      return false;
    }
    if (!fk->HasMember("update_rule")) {
      cout << "Error Reading element update_rule from fk object" << endl;
      return false;
    }
    if (!fk->HasMember("delete_rule")) {
      cout << "Error Reading element delete_rule from fk object" << endl;
      return false;
    }
    ddl += " CONSTRAINT ";
    ddl += scape_string((*fk)["name"].GetString());
    ddl += " FOREIGN KEY (";
    for (auto fk_col = (*fk)["elements"].Begin();
         fk_col != (*fk)["elements"].End(); ++fk_col) {
      if (!fk_col->IsObject()) {
        cout << "Error Reading index column object from dd_object" << endl;
        return false;
      }
      if (!fk_col->HasMember("column_opx")) {
        cout << "Error Reading column_opx from fk object" << endl;
        return false;
      }
      if (!fk_col->HasMember("referenced_column_name")) {
        cout << "Error Reading referenced_column_name from fk object" << endl;
        return false;
      }
      if (column_map.find((*fk_col)["column_opx"].GetInt()) ==
          column_map.end()) {
        cout << "Error fk column not found on hash map" << endl;
        return false;
      }
      reference_names.push_back(
          (*fk_col)["referenced_column_name"].GetString());

      ddl += scape_string(column_map[(*fk_col)["column_opx"].GetInt()].name);
      ddl += ",";
    }
    ddl.pop_back();

    ddl += ",";
    ddl.pop_back();
    ddl += ") REFERENCES ";
    if (!fk->HasMember("referenced_table_schema_name")) {
      cout << "Error Reading referenced_table_schema_name from fk object"
           << endl;
      return false;
    }
    if (table_schema.compare(
            (*fk)["referenced_table_schema_name"].GetString()) != 0) {
      ddl += scape_string((*fk)["referenced_table_schema_name"].GetString());
      ddl += ".";
    }
    if (!fk->HasMember("referenced_table_name")) {
      cout << "Error Reading referenced_table_name from fk object" << endl;
      return false;
    }
    ddl += scape_string((*fk)["referenced_table_name"].GetString());
    ddl += " (";
    for (auto ref_col = reference_names.begin();
         ref_col != reference_names.end(); ++ref_col) {
      ddl += scape_string(*ref_col);
      ddl += ",";
    }
    ddl.pop_back();
    ddl += ")";
    /* ON DELETE */
    switch (static_cast<fk_enum_rule>((*fk)["delete_rule"].GetInt())) {
      case fk_enum_rule::RULE_RESTRICT:
        ddl += " ON DELETE RESTRICT";
        break;
      case fk_enum_rule::RULE_CASCADE:
        ddl += " ON DELETE CASCADE";
        break;
      case fk_enum_rule::RULE_SET_NULL:
        ddl += " ON DELETE SET NULL";
        break;
      case fk_enum_rule::RULE_SET_DEFAULT:
        ddl += " ON DELETE SET DEFAULT";
        break;
      case fk_enum_rule::RULE_NO_ACTION:
      default:
        break;
    }
    /* ON UPDATE */
    switch (static_cast<fk_enum_rule>((*fk)["update_rule"].GetInt())) {
      case fk_enum_rule::RULE_RESTRICT:
        ddl += " ON UPDATE RESTRICT";
        break;
      case fk_enum_rule::RULE_CASCADE:
        ddl += " ON UPDATE CASCADE";
        break;
      case fk_enum_rule::RULE_SET_NULL:
        ddl += " ON UPDATE SET NULL";
        break;
      case fk_enum_rule::RULE_SET_DEFAULT:
        ddl += " ON UPDATE SET DEFAULT";
        break;
      case fk_enum_rule::RULE_NO_ACTION:
      default:
        break;
    }
    ddl += ",\n";
  }
  return true;
}
#endif  // FOREIGN_KEYS_H
