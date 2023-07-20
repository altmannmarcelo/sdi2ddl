#include <fstream>
#include <iostream>
#include <unordered_map>
#include "include/collations.h"
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

/* Column hash map */
unordered_map<int, string> column_map;

/* Table charset */
static collation_info table_collation;

/** escape a string
@param[in]	    text	  string to escape
@return escaped string */
string scape_string(const string &text) {
  string ret = "`";
  ret.append(text);
  ret.append("`");
  return ret;
}

/** Add column to hash map
@param[in]	    column	  Column JSON object
@return False in case of errors */
static bool add_column_to_map(const rapidjson::Value *column) {
  if (!column->HasMember("ordinal_position") || !column->HasMember("name")) {
    cout << "Error Reading column ordinal_position or name" << endl;
    return false;
  }
  column_map[column->FindMember("ordinal_position")->value.GetInt() - 1] =
      column->FindMember("name")->value.GetString();
  return true;
}

/** Check if a column is reserved
@param[in]	name	Column name
@return True if the column is reserved */
static bool is_column_reserved(const char *name) {
  return (strcmp(name, "DB_TRX_ID") == 0) ||
         (strcmp(name, "DB_ROLL_PTR") == 0) || (strcmp(name, "DB_ROW_ID") == 0);
}

/** Parse single column attribute
@param[in]	    column	  Column JSON object
@param[in,out]	ddl     	DDL string
@param[in]	    attribute	Attribute name
@param[in]	    required	required attribute
@return False in case of errors */
static bool parse_column_attribute(const rapidjson::Value *column, string &ddl,
                                   const char *attribute, bool required) {
  if (!column->HasMember(attribute)) {
    if (required) {
      cout << "Error Reading column attribute " << attribute << endl;
      return false;
    }
    return true;
  }

  if (strcmp(attribute, "is_explicit_collation") == 0) {
    if (column->FindMember(attribute)->value.GetBool()) {
      if (!column->HasMember("collation_id")) {
        cout << "Error Reading column attribute collation_id" << endl;
        return false;
      }
      collation_info collation;
      if (!find_collation(column->FindMember("collation_id")->value.GetInt(),
                          collation)) {
        cout << "Error finding collation info" << endl;
        return false;
      }
      ddl += " CHARACTER SET ";
      ddl += collation.charset;
      ddl += " COLLATE ";
      ddl += collation.collation;
    }

    return true;
  }

  if (strcmp(attribute, "is_nullable") == 0) {
    if (!column->FindMember(attribute)->value.GetBool()) ddl += " NOT NULL";

    return true;
  }
  if (strcmp(attribute, "is_auto_increment") == 0) {
    if (column->FindMember(attribute)->value.GetBool())
      ddl += " AUTO_INCREMENT";

    return true;
  }

  if (strcmp(attribute, "name") == 0) {
    ddl += " ";
    ddl += scape_string(column->FindMember(attribute)->value.GetString());
    return true;
  }

  /* Default */
  ddl += " ";
  ddl += column->FindMember(attribute)->value.GetString();
  return true;
}

/** Parse the column section of SDI JSON
@param[in]	    dd_object	Data Dictionary JSON object
@param[in,out]	ddl     	DDL string
@return False in case of errors */
static bool parse_columns(const rapidjson::Value::ConstObject &dd_object,
                          string &ddl) {
  if (!dd_object.HasMember("columns")) {
    cout << "Error Reading columns from dd_object" << endl;
    return false;
  }
  for (auto col = dd_object["columns"].Begin();
       col != dd_object["columns"].End(); ++col) {
    if (!col->IsObject()) {
      cout << "Error Reading column object from dd_object" << endl;
      return false;
    }

    /* Skip reserved columns */
    if (col->HasMember("name") &&
        is_column_reserved((*col)["name"].GetString()))
      continue;

    if (!add_column_to_map(col)) return false;

    /* Column name - Required */
    if (!parse_column_attribute(col, ddl, "name", true)) return false;
    if (!parse_column_attribute(col, ddl, "column_type_utf8", true))
      return false;
    if (!parse_column_attribute(col, ddl, "is_explicit_collation", false))
      return false;
    if (!parse_column_attribute(col, ddl, "is_nullable", false)) return false;
    if (!parse_column_attribute(col, ddl, "is_auto_increment", false))
      return false;

    ddl += ",\n";
  }
  return true;
}

/** Parse the indexes section of SDI JSON
@param[in]	    dd_object	Data Dictionary JSON object
@param[in,out]	ddl     	DDL string
@param[in,out]	n_index   Number of indexes
@return False in case of errors */
static bool parse_indexes(const rapidjson::Value::ConstObject &dd_object,
                          string &ddl, int &n_index) {
  if (!dd_object.HasMember("indexes")) {
    cout << "Error Reading indexes from dd_object" << endl;
    return false;
  }
  for (auto index = dd_object["indexes"].Begin();
       index != dd_object["indexes"].End(); ++index) {
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
    int type = (*index)["type"].GetInt();
    switch (type) {
      case 1:
        ddl += "PRIMARY KEY (";
        break;
      case 2:
        ddl += "UNIQUE KEY (";
        break;
      case 3:
        ddl += "KEY (";
        break;
      default:
        cout << "Unsuported Index Type: " << type << endl;
        return false;
        break;
    }
    n_index++;
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
      ddl += scape_string(column_map[(*index_col)["column_opx"].GetInt()]);
      ddl += ",";
    }
    ddl.pop_back();
    ddl += "),\n";
  }
  return true;
}

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

      if (!parse_columns(dd_object, ddl)) return false;
      int valid_indexes = 0;
      if (!parse_indexes(dd_object, ddl, valid_indexes)) return false;

      ddl.erase(ddl.size() - 2);
      if (valid_indexes == 0) ddl += "\n";
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
