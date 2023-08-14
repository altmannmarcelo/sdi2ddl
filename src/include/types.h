#ifndef TYPES_H
#define TYPES_H
/* https://github.com/mysql/mysql-server/blob/trunk/sql/dd/types/column.h#L52 */
enum class enum_column_types {
  DECIMAL = 1,  // This is 1 > than MYSQL_TYPE_DECIMAL
  TINY,
  SHORT,
  LONG,
  FLOAT,
  DOUBLE,
  TYPE_NULL,
  TIMESTAMP,
  LONGLONG,
  INT24,
  DATE,
  TIME,
  DATETIME,
  YEAR,
  NEWDATE,
  VARCHAR,
  BIT,
  TIMESTAMP2,
  DATETIME2,
  TIME2,
  NEWDECIMAL,
  ENUM,
  SET,
  TINY_BLOB,
  MEDIUM_BLOB,
  LONG_BLOB,
  BLOB,
  VAR_STRING,
  STRING,
  GEOMETRY,
  JSON
};

enum enum_hidden_type {
    /// The column is visible (a normal column)
    HT_VISIBLE = 1,
    /// The column is completely invisible to the server
    HT_HIDDEN_SE = 2,
    /// The column is visible to the server, but hidden from the user.
    /// This is used for i.e. implementing functional indexes.
    HT_HIDDEN_SQL = 3,
    /// User table column marked as INVISIBLE by using the column visibility
    /// attribute. Column is hidden from the user unless it is explicitly
    /// referenced in the statement. Column is visible to the server.
    HT_HIDDEN_USER = 4
  };

/* https://github.com/mysql/mysql-server/blob/trunk/sql/dd/types/index.h#L57 */

  enum enum_index_type  // similar to Keytype in sql_class.h but w/o FOREIGN_KEY
  { IT_PRIMARY = 1,
    IT_UNIQUE,
    IT_MULTIPLE,
    IT_FULLTEXT,
    IT_SPATIAL };

  enum enum_index_algorithm  // similar to ha_key_alg
  { IA_SE_SPECIFIC = 1,
    IA_BTREE,
    IA_RTREE,
    IA_HASH,
    IA_FULLTEXT };

  /* https://github.com/mysql/mysql-server/blob/trunk/sql/dd/types/foreign_key.h
   */
  enum class fk_enum_rule {
    RULE_NO_ACTION = 1,
    RULE_RESTRICT,
    RULE_CASCADE,
    RULE_SET_NULL,
    RULE_SET_DEFAULT
  };

  enum class fk_enum_match_option {
    OPTION_NONE = 1,
    OPTION_PARTIAL,
    OPTION_FULL,
  };
#endif /* TYPES_H */
