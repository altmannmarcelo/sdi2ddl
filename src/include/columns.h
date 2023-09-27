#ifndef COLUMNS_H
#define COLUMNS_H
#include <unordered_map>
#include "rapidjson/document.h"
#include "types.h"
#include "utils.h"
using namespace std;
struct column_t {
  int ordinal_position;
  string name;
  string generation_expression;
  enum_hidden_type hidden;
  enum_column_types type;
  u_int64_t size;
  collation_info collation;
  column_t() = default;  // make unordered_map happy
  column_t(int ordinal_position, string name, string generation_expression,
           enum_hidden_type hidden, enum_column_types type, u_int64_t size,
           int collation_id)
      : ordinal_position(ordinal_position),
        name(name),
        generation_expression(generation_expression),
        hidden(hidden),
        type(type),
        size(size) {
    collation_info cl;
    if (!find_collation(collation_id, cl)) {
      cout << "Error finding collation info" << endl;
    }
    this->collation = cl;
  };
};

typedef column_t column_t;

/* Column hash map */
unordered_map<int, column_t> column_map;

/** Add column to hash map
@param[in]	    column	  Column JSON object
@return column_t object */
static column_t add_column_to_map(const rapidjson::Value *column) {
  column_t col =
      column_t(column->FindMember("ordinal_position")->value.GetInt() - 1,
               column->FindMember("name")->value.GetString(),
               column->FindMember("generation_expression")->value.GetString(),
               static_cast<enum_hidden_type>(
                   column->FindMember("hidden")->value.GetInt()),
               static_cast<enum_column_types>(
                   column->FindMember("type")->value.GetInt()),
               column->FindMember("char_length")->value.GetUint64(),
               column->FindMember("collation_id")->value.GetInt());
  column_map.insert({col.ordinal_position, col});
  return col;
}

/* Check if column has one required member
@param[in]	    column	  Column JSON object
@param[in]	    member	  Member name
@return False in case of errors */
static bool check_member(const rapidjson::Value *column, string member) {
  if (!column->HasMember(member.c_str())) {
    cout << "Error Reading column " << member << endl;
    return false;
  }
  return true;
}

/* Check if column has all required members
@param[in]	    column	  Column JSON object
@return False in case of errors */
static bool check_columns(const rapidjson::Value *column) {
  return check_member(column, "name") && check_member(column, "type") &&
         check_member(column, "is_nullable") &&
         check_member(column, "is_zerofill") &&
         check_member(column, "is_unsigned") &&
         check_member(column, "is_auto_increment") &&
         check_member(column, "is_virtual") && check_member(column, "hidden") &&
         check_member(column, "ordinal_position") &&
         check_member(column, "char_length") &&
         check_member(column, "numeric_precision") &&
         check_member(column, "numeric_scale") &&
         check_member(column, "numeric_scale_null") &&
         check_member(column, "datetime_precision") &&
         check_member(column, "datetime_precision_null") &&
         check_member(column, "has_no_default") &&
         check_member(column, "default_value_null") &&
         check_member(column, "srs_id_null") &&
         check_member(column, "srs_id") &&
         check_member(column, "default_value") &&
         check_member(column, "default_value_utf8_null") &&
         check_member(column, "default_value_utf8") &&
         check_member(column, "default_option") &&
         check_member(column, "update_option") &&
         check_member(column, "comment") &&
         check_member(column, "generation_expression") &&
         check_member(column, "generation_expression_utf8") &&
         check_member(column, "options") &&
         check_member(column, "se_private_data") &&
         check_member(column, "engine_attribute") &&
         check_member(column, "secondary_engine_attribute") &&
         check_member(column, "column_key") &&
         check_member(column, "column_type_utf8") &&
         check_member(column, "elements") &&
         check_member(column, "collation_id") &&
         check_member(column, "is_explicit_collation");
}

/** Check if a column is reserved
@param[in]	name	Column name
@return True if the column is reserved */
static bool is_column_reserved(const char *name) {
  return (strcmp(name, "DB_TRX_ID") == 0) ||
         (strcmp(name, "DB_ROLL_PTR") == 0) || (strcmp(name, "DB_ROW_ID") == 0);
}

/** Check if a column is hidden by user
@param[in]      name    Column name
@return True if the column is hidden on DDL */
static bool is_column_hidden_user(const column_t &col) {
  return col.hidden == enum_hidden_type::HT_HIDDEN_USER;
}


/** Check if a column is hidden
@param[in]	name	Column name
@return True if the column is hidden on DDL */
static bool is_column_hidden(const column_t &col) {
  return col.hidden != enum_hidden_type::HT_VISIBLE;
}

/* Check if charset definition should be skipped
@param[in]	    column_type	  Column JSON object
@return True if the charset definition should be skipped */
static bool skip_charset(const int column_type) {
  /* JSON */
  return column_type == static_cast<int>(enum_column_types::JSON) ||
         column_type == static_cast<int>(enum_column_types::BLOB) ||
         column_type == static_cast<int>(enum_column_types::TINY_BLOB) ||
         column_type == static_cast<int>(enum_column_types::MEDIUM_BLOB) ||
         column_type == static_cast<int>(enum_column_types::LONG_BLOB);
}

/* Check if column type support index prefix
@param[in]	    column_type	  Column column_t object
@return True if the type supports index prefix */
static bool support_prefix_index(column_t column) {
  return column.type == enum_column_types::VARCHAR ||
         column.type == enum_column_types::TINY_BLOB ||
         column.type == enum_column_types::MEDIUM_BLOB ||
         column.type == enum_column_types::LONG_BLOB ||
         column.type == enum_column_types::BLOB ||
         column.type == enum_column_types::VAR_STRING ||
         column.type == enum_column_types::STRING;
}

/** Parse single column attribute
@param[in]	    column	  Column JSON object
@param[in,out]	ddl     	DDL string
@param[in]	    attribute	Attribute name
@return False in case of errors */
static bool parse_column_attribute(const rapidjson::Value *column, string &ddl,
                                   const char *attribute) {
  if (strcmp(attribute, "is_explicit_collation") == 0) {
    if (column->FindMember(attribute)->value.GetBool()) {
      if (skip_charset(column->FindMember("type")->value.GetInt())) return true;

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

  if (strcmp(attribute, "generation_expression") == 0) {
    if (strcmp(column->FindMember(attribute)->value.GetString(), "") != 0) {
      ddl += " GENERATED ALWAYS AS (";
      ddl += column->FindMember(attribute)->value.GetString();
      ddl += ")";
      if (column->FindMember("is_virtual")->value.GetBool()) {
        ddl += " VIRTUAL";
      } else {
        ddl += " STORED";
      }
    }
    return true;
  }

  if (strcmp(attribute, "is_nullable") == 0) {
    if (!column->FindMember(attribute)->value.GetBool()) ddl += " NOT NULL";

    return true;
  }
  if (strcmp(attribute, "default_value_null") == 0) {
    /* skip default if is generated */
    if (strcmp(column->FindMember("generation_expression")->value.GetString(),
               "") != 0)
      return true;

    if (column->FindMember(attribute)->value.GetBool()) {
      ddl += " DEFAULT NULL";
    } else {
      if (!column->FindMember("default_value_utf8_null")->value.GetBool() &&
          column->FindMember("default_value_utf8")->value.GetString() != "") {
        ddl += " DEFAULT '";
        ddl += column->FindMember("default_value_utf8")->value.GetString();
        ddl += "'";
      }
    }

    return true;
  }
  if (strcmp(attribute, "is_auto_increment") == 0) {
    if (column->FindMember(attribute)->value.GetBool())
      ddl += " AUTO_INCREMENT";

    return true;
  }

  if (strcmp(attribute, "is_gipk") == 0) {
    auto options = column->FindMember("options")->value.GetString();
    if (is_kipk(options)) ddl += " /*!80023 INVISIBLE */";

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

    if (!check_columns(col)) return false;
    column_t column = add_column_to_map(col);

    if (!is_column_hidden_user(column) && is_column_hidden(column)) continue;

    /* Column name - Required */
    if (!parse_column_attribute(col, ddl, "name")) return false;
    if (!parse_column_attribute(col, ddl, "column_type_utf8")) return false;
    if (!parse_column_attribute(col, ddl, "is_explicit_collation"))
      return false;
    if (!parse_column_attribute(col, ddl, "generation_expression"))
      return false;
    if (!parse_column_attribute(col, ddl, "is_nullable")) return false;
    if (!parse_column_attribute(col, ddl, "default_value_null")) return false;
    if (!parse_column_attribute(col, ddl, "is_auto_increment")) return false;
    if (!parse_column_attribute(col, ddl, "is_gipk")) return false;

    ddl += ",\n";
  }
  return true;
}
#endif /* COLUMNS_H */
