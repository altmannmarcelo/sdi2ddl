using namespace std;
struct collation_info_t {
  string collation;
  string charset;
  int id;
  bool is_default;
  collation_info_t(){};
  collation_info_t(string collation, string charset, int id, bool is_default)
      : collation(collation),
        charset(charset),
        id(id),
        is_default(is_default){};
};

typedef collation_info_t collation_info;

unordered_map<int, collation_info> collation_map;

/*
SELECT CONCAT('collation_map.insert({', ID, ',
collation_info_t("',COLLATION_NAME,'", "', CHARACTER_SET_NAME, '", ', ID, ', ',
IF(STRCMP(IS_DEFAULT, "Yes") = 0, "true", "false"), ')});')   FROM
INFORMATION_SCHEMA.COLLATIONS;
*/

/** Find specific collation in the hashmap
@param[in]	    id	        collation id
@param[in,out]	collation   collation_info object
@return False in case of not found */
bool find_collation(const int &id, collation_info &collation) {
  auto it = collation_map.find(id);
  if (it == collation_map.end()) {
    return false;
  }
  collation = it->second;
  return true;
}

void populate_charsets() {
  collation_map.insert(
      {32, collation_info_t("armscii8_general_ci", "armscii8", 32, true)});
  collation_map.insert(
      {64, collation_info_t("armscii8_bin", "armscii8", 64, false)});
  collation_map.insert(
      {11, collation_info_t("ascii_general_ci", "ascii", 11, true)});
  collation_map.insert({65, collation_info_t("ascii_bin", "ascii", 65, false)});
  collation_map.insert({1, collation_info_t("big5_chinese_ci", "big5", 1, true)});
  collation_map.insert({84, collation_info_t("big5_bin", "big5", 84, false)});
  collation_map.insert({63, collation_info_t("binary", "binary", 63, true)});
  collation_map.insert(
      {26, collation_info_t("cp1250_general_ci", "cp1250", 26, true)});
  collation_map.insert(
      {34, collation_info_t("cp1250_czech_cs", "cp1250", 34, false)});
  collation_map.insert(
      {44, collation_info_t("cp1250_croatian_ci", "cp1250", 44, false)});
  collation_map.insert({66, collation_info_t("cp1250_bin", "cp1250", 66, false)});
  collation_map.insert(
      {99, collation_info_t("cp1250_polish_ci", "cp1250", 99, false)});
  collation_map.insert(
      {14, collation_info_t("cp1251_bulgarian_ci", "cp1251", 14, false)});
  collation_map.insert(
      {23, collation_info_t("cp1251_ukrainian_ci", "cp1251", 23, false)});
  collation_map.insert({50, collation_info_t("cp1251_bin", "cp1251", 50, false)});
  collation_map.insert(
      {51, collation_info_t("cp1251_general_ci", "cp1251", 51, true)});
  collation_map.insert(
      {52, collation_info_t("cp1251_general_cs", "cp1251", 52, false)});
  collation_map.insert(
      {57, collation_info_t("cp1256_general_ci", "cp1256", 57, true)});
  collation_map.insert({67, collation_info_t("cp1256_bin", "cp1256", 67, false)});
  collation_map.insert(
      {29, collation_info_t("cp1257_lithuanian_ci", "cp1257", 29, false)});
  collation_map.insert({58, collation_info_t("cp1257_bin", "cp1257", 58, false)});
  collation_map.insert(
      {59, collation_info_t("cp1257_general_ci", "cp1257", 59, true)});
  collation_map.insert({4, collation_info_t("cp850_general_ci", "cp850", 4, true)});
  collation_map.insert({80, collation_info_t("cp850_bin", "cp850", 80, false)});
  collation_map.insert(
      {40, collation_info_t("cp852_general_ci", "cp852", 40, true)});
  collation_map.insert({81, collation_info_t("cp852_bin", "cp852", 81, false)});
  collation_map.insert(
      {36, collation_info_t("cp866_general_ci", "cp866", 36, true)});
  collation_map.insert({68, collation_info_t("cp866_bin", "cp866", 68, false)});
  collation_map.insert(
      {95, collation_info_t("cp932_japanese_ci", "cp932", 95, true)});
  collation_map.insert({96, collation_info_t("cp932_bin", "cp932", 96, false)});
  collation_map.insert({3, collation_info_t("dec8_swedish_ci", "dec8", 3, true)});
  collation_map.insert({69, collation_info_t("dec8_bin", "dec8", 69, false)});
  collation_map.insert(
      {97, collation_info_t("eucjpms_japanese_ci", "eucjpms", 97, true)});
  collation_map.insert({98, collation_info_t("eucjpms_bin", "eucjpms", 98, false)});
  collation_map.insert(
      {19, collation_info_t("euckr_korean_ci", "euckr", 19, true)});
  collation_map.insert({85, collation_info_t("euckr_bin", "euckr", 85, false)});
  collation_map.insert(
      {248, collation_info_t("gb18030_chinese_ci", "gb18030", 248, true)});
  collation_map.insert(
      {249, collation_info_t("gb18030_bin", "gb18030", 249, false)});
  collation_map.insert(
      {250, collation_info_t("gb18030_unicode_520_ci", "gb18030", 250, false)});
  collation_map.insert(
      {24, collation_info_t("gb2312_chinese_ci", "gb2312", 24, true)});
  collation_map.insert({86, collation_info_t("gb2312_bin", "gb2312", 86, false)});
  collation_map.insert({28, collation_info_t("gbk_chinese_ci", "gbk", 28, true)});
  collation_map.insert({87, collation_info_t("gbk_bin", "gbk", 87, false)});
  collation_map.insert(
      {92, collation_info_t("geostd8_general_ci", "geostd8", 92, true)});
  collation_map.insert({93, collation_info_t("geostd8_bin", "geostd8", 93, false)});
  collation_map.insert(
      {25, collation_info_t("greek_general_ci", "greek", 25, true)});
  collation_map.insert({70, collation_info_t("greek_bin", "greek", 70, false)});
  collation_map.insert(
      {16, collation_info_t("hebrew_general_ci", "hebrew", 16, true)});
  collation_map.insert({71, collation_info_t("hebrew_bin", "hebrew", 71, false)});
  collation_map.insert({6, collation_info_t("hp8_english_ci", "hp8", 6, true)});
  collation_map.insert({72, collation_info_t("hp8_bin", "hp8", 72, false)});
  collation_map.insert(
      {37, collation_info_t("keybcs2_general_ci", "keybcs2", 37, true)});
  collation_map.insert({73, collation_info_t("keybcs2_bin", "keybcs2", 73, false)});
  collation_map.insert({7, collation_info_t("koi8r_general_ci", "koi8r", 7, true)});
  collation_map.insert({74, collation_info_t("koi8r_bin", "koi8r", 74, false)});
  collation_map.insert(
      {22, collation_info_t("koi8u_general_ci", "koi8u", 22, true)});
  collation_map.insert({75, collation_info_t("koi8u_bin", "koi8u", 75, false)});
  collation_map.insert(
      {5, collation_info_t("latin1_german1_ci", "latin1", 5, false)});
  collation_map.insert(
      {8, collation_info_t("latin1_swedish_ci", "latin1", 8, true)});
  collation_map.insert(
      {15, collation_info_t("latin1_danish_ci", "latin1", 15, false)});
  collation_map.insert(
      {31, collation_info_t("latin1_german2_ci", "latin1", 31, false)});
  collation_map.insert({47, collation_info_t("latin1_bin", "latin1", 47, false)});
  collation_map.insert(
      {48, collation_info_t("latin1_general_ci", "latin1", 48, false)});
  collation_map.insert(
      {49, collation_info_t("latin1_general_cs", "latin1", 49, false)});
  collation_map.insert(
      {94, collation_info_t("latin1_spanish_ci", "latin1", 94, false)});
  collation_map.insert(
      {2, collation_info_t("latin2_czech_cs", "latin2", 2, false)});
  collation_map.insert(
      {9, collation_info_t("latin2_general_ci", "latin2", 9, true)});
  collation_map.insert(
      {21, collation_info_t("latin2_hungarian_ci", "latin2", 21, false)});
  collation_map.insert(
      {27, collation_info_t("latin2_croatian_ci", "latin2", 27, false)});
  collation_map.insert({77, collation_info_t("latin2_bin", "latin2", 77, false)});
  collation_map.insert(
      {30, collation_info_t("latin5_turkish_ci", "latin5", 30, true)});
  collation_map.insert({78, collation_info_t("latin5_bin", "latin5", 78, false)});
  collation_map.insert(
      {20, collation_info_t("latin7_estonian_cs", "latin7", 20, false)});
  collation_map.insert(
      {41, collation_info_t("latin7_general_ci", "latin7", 41, true)});
  collation_map.insert(
      {42, collation_info_t("latin7_general_cs", "latin7", 42, false)});
  collation_map.insert({79, collation_info_t("latin7_bin", "latin7", 79, false)});
  collation_map.insert(
      {38, collation_info_t("macce_general_ci", "macce", 38, true)});
  collation_map.insert({43, collation_info_t("macce_bin", "macce", 43, false)});
  collation_map.insert(
      {39, collation_info_t("macroman_general_ci", "macroman", 39, true)});
  collation_map.insert(
      {53, collation_info_t("macroman_bin", "macroman", 53, false)});
  collation_map.insert(
      {13, collation_info_t("sjis_japanese_ci", "sjis", 13, true)});
  collation_map.insert({88, collation_info_t("sjis_bin", "sjis", 88, false)});
  collation_map.insert({10, collation_info_t("swe7_swedish_ci", "swe7", 10, true)});
  collation_map.insert({82, collation_info_t("swe7_bin", "swe7", 82, false)});
  collation_map.insert(
      {18, collation_info_t("tis620_thai_ci", "tis620", 18, true)});
  collation_map.insert({89, collation_info_t("tis620_bin", "tis620", 89, false)});
  collation_map.insert({35, collation_info_t("ucs2_general_ci", "ucs2", 35, true)});
  collation_map.insert({90, collation_info_t("ucs2_bin", "ucs2", 90, false)});
  collation_map.insert(
      {128, collation_info_t("ucs2_unicode_ci", "ucs2", 128, false)});
  collation_map.insert(
      {129, collation_info_t("ucs2_icelandic_ci", "ucs2", 129, false)});
  collation_map.insert(
      {130, collation_info_t("ucs2_latvian_ci", "ucs2", 130, false)});
  collation_map.insert(
      {131, collation_info_t("ucs2_romanian_ci", "ucs2", 131, false)});
  collation_map.insert(
      {132, collation_info_t("ucs2_slovenian_ci", "ucs2", 132, false)});
  collation_map.insert(
      {133, collation_info_t("ucs2_polish_ci", "ucs2", 133, false)});
  collation_map.insert(
      {134, collation_info_t("ucs2_estonian_ci", "ucs2", 134, false)});
  collation_map.insert(
      {135, collation_info_t("ucs2_spanish_ci", "ucs2", 135, false)});
  collation_map.insert(
      {136, collation_info_t("ucs2_swedish_ci", "ucs2", 136, false)});
  collation_map.insert(
      {137, collation_info_t("ucs2_turkish_ci", "ucs2", 137, false)});
  collation_map.insert(
      {138, collation_info_t("ucs2_czech_ci", "ucs2", 138, false)});
  collation_map.insert(
      {139, collation_info_t("ucs2_danish_ci", "ucs2", 139, false)});
  collation_map.insert(
      {140, collation_info_t("ucs2_lithuanian_ci", "ucs2", 140, false)});
  collation_map.insert(
      {141, collation_info_t("ucs2_slovak_ci", "ucs2", 141, false)});
  collation_map.insert(
      {142, collation_info_t("ucs2_spanish2_ci", "ucs2", 142, false)});
  collation_map.insert(
      {143, collation_info_t("ucs2_roman_ci", "ucs2", 143, false)});
  collation_map.insert(
      {144, collation_info_t("ucs2_persian_ci", "ucs2", 144, false)});
  collation_map.insert(
      {145, collation_info_t("ucs2_esperanto_ci", "ucs2", 145, false)});
  collation_map.insert(
      {146, collation_info_t("ucs2_hungarian_ci", "ucs2", 146, false)});
  collation_map.insert(
      {147, collation_info_t("ucs2_sinhala_ci", "ucs2", 147, false)});
  collation_map.insert(
      {148, collation_info_t("ucs2_german2_ci", "ucs2", 148, false)});
  collation_map.insert(
      {149, collation_info_t("ucs2_croatian_ci", "ucs2", 149, false)});
  collation_map.insert(
      {150, collation_info_t("ucs2_unicode_520_ci", "ucs2", 150, false)});
  collation_map.insert(
      {151, collation_info_t("ucs2_vietnamese_ci", "ucs2", 151, false)});
  collation_map.insert(
      {159, collation_info_t("ucs2_general_mysql500_ci", "ucs2", 159, false)});
  collation_map.insert(
      {12, collation_info_t("ujis_japanese_ci", "ujis", 12, true)});
  collation_map.insert({91, collation_info_t("ujis_bin", "ujis", 91, false)});
  collation_map.insert(
      {54, collation_info_t("utf16_general_ci", "utf16", 54, true)});
  collation_map.insert({55, collation_info_t("utf16_bin", "utf16", 55, false)});
  collation_map.insert(
      {101, collation_info_t("utf16_unicode_ci", "utf16", 101, false)});
  collation_map.insert(
      {102, collation_info_t("utf16_icelandic_ci", "utf16", 102, false)});
  collation_map.insert(
      {103, collation_info_t("utf16_latvian_ci", "utf16", 103, false)});
  collation_map.insert(
      {104, collation_info_t("utf16_romanian_ci", "utf16", 104, false)});
  collation_map.insert(
      {105, collation_info_t("utf16_slovenian_ci", "utf16", 105, false)});
  collation_map.insert(
      {106, collation_info_t("utf16_polish_ci", "utf16", 106, false)});
  collation_map.insert(
      {107, collation_info_t("utf16_estonian_ci", "utf16", 107, false)});
  collation_map.insert(
      {108, collation_info_t("utf16_spanish_ci", "utf16", 108, false)});
  collation_map.insert(
      {109, collation_info_t("utf16_swedish_ci", "utf16", 109, false)});
  collation_map.insert(
      {110, collation_info_t("utf16_turkish_ci", "utf16", 110, false)});
  collation_map.insert(
      {111, collation_info_t("utf16_czech_ci", "utf16", 111, false)});
  collation_map.insert(
      {112, collation_info_t("utf16_danish_ci", "utf16", 112, false)});
  collation_map.insert(
      {113, collation_info_t("utf16_lithuanian_ci", "utf16", 113, false)});
  collation_map.insert(
      {114, collation_info_t("utf16_slovak_ci", "utf16", 114, false)});
  collation_map.insert(
      {115, collation_info_t("utf16_spanish2_ci", "utf16", 115, false)});
  collation_map.insert(
      {116, collation_info_t("utf16_roman_ci", "utf16", 116, false)});
  collation_map.insert(
      {117, collation_info_t("utf16_persian_ci", "utf16", 117, false)});
  collation_map.insert(
      {118, collation_info_t("utf16_esperanto_ci", "utf16", 118, false)});
  collation_map.insert(
      {119, collation_info_t("utf16_hungarian_ci", "utf16", 119, false)});
  collation_map.insert(
      {120, collation_info_t("utf16_sinhala_ci", "utf16", 120, false)});
  collation_map.insert(
      {121, collation_info_t("utf16_german2_ci", "utf16", 121, false)});
  collation_map.insert(
      {122, collation_info_t("utf16_croatian_ci", "utf16", 122, false)});
  collation_map.insert(
      {123, collation_info_t("utf16_unicode_520_ci", "utf16", 123, false)});
  collation_map.insert(
      {124, collation_info_t("utf16_vietnamese_ci", "utf16", 124, false)});
  collation_map.insert(
      {56, collation_info_t("utf16le_general_ci", "utf16le", 56, true)});
  collation_map.insert({62, collation_info_t("utf16le_bin", "utf16le", 62, false)});
  collation_map.insert(
      {60, collation_info_t("utf32_general_ci", "utf32", 60, true)});
  collation_map.insert({61, collation_info_t("utf32_bin", "utf32", 61, false)});
  collation_map.insert(
      {160, collation_info_t("utf32_unicode_ci", "utf32", 160, false)});
  collation_map.insert(
      {161, collation_info_t("utf32_icelandic_ci", "utf32", 161, false)});
  collation_map.insert(
      {162, collation_info_t("utf32_latvian_ci", "utf32", 162, false)});
  collation_map.insert(
      {163, collation_info_t("utf32_romanian_ci", "utf32", 163, false)});
  collation_map.insert(
      {164, collation_info_t("utf32_slovenian_ci", "utf32", 164, false)});
  collation_map.insert(
      {165, collation_info_t("utf32_polish_ci", "utf32", 165, false)});
  collation_map.insert(
      {166, collation_info_t("utf32_estonian_ci", "utf32", 166, false)});
  collation_map.insert(
      {167, collation_info_t("utf32_spanish_ci", "utf32", 167, false)});
  collation_map.insert(
      {168, collation_info_t("utf32_swedish_ci", "utf32", 168, false)});
  collation_map.insert(
      {169, collation_info_t("utf32_turkish_ci", "utf32", 169, false)});
  collation_map.insert(
      {170, collation_info_t("utf32_czech_ci", "utf32", 170, false)});
  collation_map.insert(
      {171, collation_info_t("utf32_danish_ci", "utf32", 171, false)});
  collation_map.insert(
      {172, collation_info_t("utf32_lithuanian_ci", "utf32", 172, false)});
  collation_map.insert(
      {173, collation_info_t("utf32_slovak_ci", "utf32", 173, false)});
  collation_map.insert(
      {174, collation_info_t("utf32_spanish2_ci", "utf32", 174, false)});
  collation_map.insert(
      {175, collation_info_t("utf32_roman_ci", "utf32", 175, false)});
  collation_map.insert(
      {176, collation_info_t("utf32_persian_ci", "utf32", 176, false)});
  collation_map.insert(
      {177, collation_info_t("utf32_esperanto_ci", "utf32", 177, false)});
  collation_map.insert(
      {178, collation_info_t("utf32_hungarian_ci", "utf32", 178, false)});
  collation_map.insert(
      {179, collation_info_t("utf32_sinhala_ci", "utf32", 179, false)});
  collation_map.insert(
      {180, collation_info_t("utf32_german2_ci", "utf32", 180, false)});
  collation_map.insert(
      {181, collation_info_t("utf32_croatian_ci", "utf32", 181, false)});
  collation_map.insert(
      {182, collation_info_t("utf32_unicode_520_ci", "utf32", 182, false)});
  collation_map.insert(
      {183, collation_info_t("utf32_vietnamese_ci", "utf32", 183, false)});
  collation_map.insert(
      {33, collation_info_t("utf8mb3_general_ci", "utf8mb3", 33, true)});
  collation_map.insert(
      {76, collation_info_t("utf8mb3_tolower_ci", "utf8mb3", 76, false)});
  collation_map.insert({83, collation_info_t("utf8mb3_bin", "utf8mb3", 83, false)});
  collation_map.insert(
      {192, collation_info_t("utf8mb3_unicode_ci", "utf8mb3", 192, false)});
  collation_map.insert(
      {193, collation_info_t("utf8mb3_icelandic_ci", "utf8mb3", 193, false)});
  collation_map.insert(
      {194, collation_info_t("utf8mb3_latvian_ci", "utf8mb3", 194, false)});
  collation_map.insert(
      {195, collation_info_t("utf8mb3_romanian_ci", "utf8mb3", 195, false)});
  collation_map.insert(
      {196, collation_info_t("utf8mb3_slovenian_ci", "utf8mb3", 196, false)});
  collation_map.insert(
      {197, collation_info_t("utf8mb3_polish_ci", "utf8mb3", 197, false)});
  collation_map.insert(
      {198, collation_info_t("utf8mb3_estonian_ci", "utf8mb3", 198, false)});
  collation_map.insert(
      {199, collation_info_t("utf8mb3_spanish_ci", "utf8mb3", 199, false)});
  collation_map.insert(
      {200, collation_info_t("utf8mb3_swedish_ci", "utf8mb3", 200, false)});
  collation_map.insert(
      {201, collation_info_t("utf8mb3_turkish_ci", "utf8mb3", 201, false)});
  collation_map.insert(
      {202, collation_info_t("utf8mb3_czech_ci", "utf8mb3", 202, false)});
  collation_map.insert(
      {203, collation_info_t("utf8mb3_danish_ci", "utf8mb3", 203, false)});
  collation_map.insert(
      {204, collation_info_t("utf8mb3_lithuanian_ci", "utf8mb3", 204, false)});
  collation_map.insert(
      {205, collation_info_t("utf8mb3_slovak_ci", "utf8mb3", 205, false)});
  collation_map.insert(
      {206, collation_info_t("utf8mb3_spanish2_ci", "utf8mb3", 206, false)});
  collation_map.insert(
      {207, collation_info_t("utf8mb3_roman_ci", "utf8mb3", 207, false)});
  collation_map.insert(
      {208, collation_info_t("utf8mb3_persian_ci", "utf8mb3", 208, false)});
  collation_map.insert(
      {209, collation_info_t("utf8mb3_esperanto_ci", "utf8mb3", 209, false)});
  collation_map.insert(
      {210, collation_info_t("utf8mb3_hungarian_ci", "utf8mb3", 210, false)});
  collation_map.insert(
      {211, collation_info_t("utf8mb3_sinhala_ci", "utf8mb3", 211, false)});
  collation_map.insert(
      {212, collation_info_t("utf8mb3_german2_ci", "utf8mb3", 212, false)});
  collation_map.insert(
      {213, collation_info_t("utf8mb3_croatian_ci", "utf8mb3", 213, false)});
  collation_map.insert(
      {214, collation_info_t("utf8mb3_unicode_520_ci", "utf8mb3", 214, false)});
  collation_map.insert(
      {215, collation_info_t("utf8mb3_vietnamese_ci", "utf8mb3", 215, false)});
  collation_map.insert({223, collation_info_t("utf8mb3_general_mysql500_ci",
                                          "utf8mb3", 223, false)});
  collation_map.insert(
      {45, collation_info_t("utf8mb4_general_ci", "utf8mb4", 45, false)});
  collation_map.insert({46, collation_info_t("utf8mb4_bin", "utf8mb4", 46, false)});
  collation_map.insert(
      {224, collation_info_t("utf8mb4_unicode_ci", "utf8mb4", 224, false)});
  collation_map.insert(
      {225, collation_info_t("utf8mb4_icelandic_ci", "utf8mb4", 225, false)});
  collation_map.insert(
      {226, collation_info_t("utf8mb4_latvian_ci", "utf8mb4", 226, false)});
  collation_map.insert(
      {227, collation_info_t("utf8mb4_romanian_ci", "utf8mb4", 227, false)});
  collation_map.insert(
      {228, collation_info_t("utf8mb4_slovenian_ci", "utf8mb4", 228, false)});
  collation_map.insert(
      {229, collation_info_t("utf8mb4_polish_ci", "utf8mb4", 229, false)});
  collation_map.insert(
      {230, collation_info_t("utf8mb4_estonian_ci", "utf8mb4", 230, false)});
  collation_map.insert(
      {231, collation_info_t("utf8mb4_spanish_ci", "utf8mb4", 231, false)});
  collation_map.insert(
      {232, collation_info_t("utf8mb4_swedish_ci", "utf8mb4", 232, false)});
  collation_map.insert(
      {233, collation_info_t("utf8mb4_turkish_ci", "utf8mb4", 233, false)});
  collation_map.insert(
      {234, collation_info_t("utf8mb4_czech_ci", "utf8mb4", 234, false)});
  collation_map.insert(
      {235, collation_info_t("utf8mb4_danish_ci", "utf8mb4", 235, false)});
  collation_map.insert(
      {236, collation_info_t("utf8mb4_lithuanian_ci", "utf8mb4", 236, false)});
  collation_map.insert(
      {237, collation_info_t("utf8mb4_slovak_ci", "utf8mb4", 237, false)});
  collation_map.insert(
      {238, collation_info_t("utf8mb4_spanish2_ci", "utf8mb4", 238, false)});
  collation_map.insert(
      {239, collation_info_t("utf8mb4_roman_ci", "utf8mb4", 239, false)});
  collation_map.insert(
      {240, collation_info_t("utf8mb4_persian_ci", "utf8mb4", 240, false)});
  collation_map.insert(
      {241, collation_info_t("utf8mb4_esperanto_ci", "utf8mb4", 241, false)});
  collation_map.insert(
      {242, collation_info_t("utf8mb4_hungarian_ci", "utf8mb4", 242, false)});
  collation_map.insert(
      {243, collation_info_t("utf8mb4_sinhala_ci", "utf8mb4", 243, false)});
  collation_map.insert(
      {244, collation_info_t("utf8mb4_german2_ci", "utf8mb4", 244, false)});
  collation_map.insert(
      {245, collation_info_t("utf8mb4_croatian_ci", "utf8mb4", 245, false)});
  collation_map.insert(
      {246, collation_info_t("utf8mb4_unicode_520_ci", "utf8mb4", 246, false)});
  collation_map.insert(
      {247, collation_info_t("utf8mb4_vietnamese_ci", "utf8mb4", 247, false)});
  collation_map.insert(
      {255, collation_info_t("utf8mb4_0900_ai_ci", "utf8mb4", 255, true)});
  collation_map.insert(
      {256, collation_info_t("utf8mb4_de_pb_0900_ai_ci", "utf8mb4", 256, false)});
  collation_map.insert(
      {257, collation_info_t("utf8mb4_is_0900_ai_ci", "utf8mb4", 257, false)});
  collation_map.insert(
      {258, collation_info_t("utf8mb4_lv_0900_ai_ci", "utf8mb4", 258, false)});
  collation_map.insert(
      {259, collation_info_t("utf8mb4_ro_0900_ai_ci", "utf8mb4", 259, false)});
  collation_map.insert(
      {260, collation_info_t("utf8mb4_sl_0900_ai_ci", "utf8mb4", 260, false)});
  collation_map.insert(
      {261, collation_info_t("utf8mb4_pl_0900_ai_ci", "utf8mb4", 261, false)});
  collation_map.insert(
      {262, collation_info_t("utf8mb4_et_0900_ai_ci", "utf8mb4", 262, false)});
  collation_map.insert(
      {263, collation_info_t("utf8mb4_es_0900_ai_ci", "utf8mb4", 263, false)});
  collation_map.insert(
      {264, collation_info_t("utf8mb4_sv_0900_ai_ci", "utf8mb4", 264, false)});
  collation_map.insert(
      {265, collation_info_t("utf8mb4_tr_0900_ai_ci", "utf8mb4", 265, false)});
  collation_map.insert(
      {266, collation_info_t("utf8mb4_cs_0900_ai_ci", "utf8mb4", 266, false)});
  collation_map.insert(
      {267, collation_info_t("utf8mb4_da_0900_ai_ci", "utf8mb4", 267, false)});
  collation_map.insert(
      {268, collation_info_t("utf8mb4_lt_0900_ai_ci", "utf8mb4", 268, false)});
  collation_map.insert(
      {269, collation_info_t("utf8mb4_sk_0900_ai_ci", "utf8mb4", 269, false)});
  collation_map.insert({270, collation_info_t("utf8mb4_es_trad_0900_ai_ci",
                                          "utf8mb4", 270, false)});
  collation_map.insert(
      {271, collation_info_t("utf8mb4_la_0900_ai_ci", "utf8mb4", 271, false)});
  collation_map.insert(
      {273, collation_info_t("utf8mb4_eo_0900_ai_ci", "utf8mb4", 273, false)});
  collation_map.insert(
      {274, collation_info_t("utf8mb4_hu_0900_ai_ci", "utf8mb4", 274, false)});
  collation_map.insert(
      {275, collation_info_t("utf8mb4_hr_0900_ai_ci", "utf8mb4", 275, false)});
  collation_map.insert(
      {277, collation_info_t("utf8mb4_vi_0900_ai_ci", "utf8mb4", 277, false)});
  collation_map.insert(
      {278, collation_info_t("utf8mb4_0900_as_cs", "utf8mb4", 278, false)});
  collation_map.insert(
      {279, collation_info_t("utf8mb4_de_pb_0900_as_cs", "utf8mb4", 279, false)});
  collation_map.insert(
      {280, collation_info_t("utf8mb4_is_0900_as_cs", "utf8mb4", 280, false)});
  collation_map.insert(
      {281, collation_info_t("utf8mb4_lv_0900_as_cs", "utf8mb4", 281, false)});
  collation_map.insert(
      {282, collation_info_t("utf8mb4_ro_0900_as_cs", "utf8mb4", 282, false)});
  collation_map.insert(
      {283, collation_info_t("utf8mb4_sl_0900_as_cs", "utf8mb4", 283, false)});
  collation_map.insert(
      {284, collation_info_t("utf8mb4_pl_0900_as_cs", "utf8mb4", 284, false)});
  collation_map.insert(
      {285, collation_info_t("utf8mb4_et_0900_as_cs", "utf8mb4", 285, false)});
  collation_map.insert(
      {286, collation_info_t("utf8mb4_es_0900_as_cs", "utf8mb4", 286, false)});
  collation_map.insert(
      {287, collation_info_t("utf8mb4_sv_0900_as_cs", "utf8mb4", 287, false)});
  collation_map.insert(
      {288, collation_info_t("utf8mb4_tr_0900_as_cs", "utf8mb4", 288, false)});
  collation_map.insert(
      {289, collation_info_t("utf8mb4_cs_0900_as_cs", "utf8mb4", 289, false)});
  collation_map.insert(
      {290, collation_info_t("utf8mb4_da_0900_as_cs", "utf8mb4", 290, false)});
  collation_map.insert(
      {291, collation_info_t("utf8mb4_lt_0900_as_cs", "utf8mb4", 291, false)});
  collation_map.insert(
      {292, collation_info_t("utf8mb4_sk_0900_as_cs", "utf8mb4", 292, false)});
  collation_map.insert({293, collation_info_t("utf8mb4_es_trad_0900_as_cs",
                                          "utf8mb4", 293, false)});
  collation_map.insert(
      {294, collation_info_t("utf8mb4_la_0900_as_cs", "utf8mb4", 294, false)});
  collation_map.insert(
      {296, collation_info_t("utf8mb4_eo_0900_as_cs", "utf8mb4", 296, false)});
  collation_map.insert(
      {297, collation_info_t("utf8mb4_hu_0900_as_cs", "utf8mb4", 297, false)});
  collation_map.insert(
      {298, collation_info_t("utf8mb4_hr_0900_as_cs", "utf8mb4", 298, false)});
  collation_map.insert(
      {300, collation_info_t("utf8mb4_vi_0900_as_cs", "utf8mb4", 300, false)});
  collation_map.insert(
      {303, collation_info_t("utf8mb4_ja_0900_as_cs", "utf8mb4", 303, false)});
  collation_map.insert(
      {304, collation_info_t("utf8mb4_ja_0900_as_cs_ks", "utf8mb4", 304, false)});
  collation_map.insert(
      {305, collation_info_t("utf8mb4_0900_as_ci", "utf8mb4", 305, false)});
  collation_map.insert(
      {306, collation_info_t("utf8mb4_ru_0900_ai_ci", "utf8mb4", 306, false)});
  collation_map.insert(
      {307, collation_info_t("utf8mb4_ru_0900_as_cs", "utf8mb4", 307, false)});
  collation_map.insert(
      {308, collation_info_t("utf8mb4_zh_0900_as_cs", "utf8mb4", 308, false)});
  collation_map.insert(
      {309, collation_info_t("utf8mb4_0900_bin", "utf8mb4", 309, false)});
  collation_map.insert(
      {310, collation_info_t("utf8mb4_nb_0900_ai_ci", "utf8mb4", 310, false)});
  collation_map.insert(
      {311, collation_info_t("utf8mb4_nb_0900_as_cs", "utf8mb4", 311, false)});
  collation_map.insert(
      {312, collation_info_t("utf8mb4_nn_0900_ai_ci", "utf8mb4", 312, false)});
  collation_map.insert(
      {313, collation_info_t("utf8mb4_nn_0900_as_cs", "utf8mb4", 313, false)});
  collation_map.insert({314, collation_info_t("utf8mb4_sr_latn_0900_ai_ci",
                                          "utf8mb4", 314, false)});
  collation_map.insert({315, collation_info_t("utf8mb4_sr_latn_0900_as_cs",
                                          "utf8mb4", 315, false)});
  collation_map.insert(
      {316, collation_info_t("utf8mb4_bs_0900_ai_ci", "utf8mb4", 316, false)});
  collation_map.insert(
      {317, collation_info_t("utf8mb4_bs_0900_as_cs", "utf8mb4", 317, false)});
  collation_map.insert(
      {318, collation_info_t("utf8mb4_bg_0900_ai_ci", "utf8mb4", 318, false)});
  collation_map.insert(
      {319, collation_info_t("utf8mb4_bg_0900_as_cs", "utf8mb4", 319, false)});
  collation_map.insert(
      {320, collation_info_t("utf8mb4_gl_0900_ai_ci", "utf8mb4", 320, false)});
  collation_map.insert(
      {321, collation_info_t("utf8mb4_gl_0900_as_cs", "utf8mb4", 321, false)});
  collation_map.insert({322, collation_info_t("utf8mb4_mn_cyrl_0900_ai_ci",
                                          "utf8mb4", 322, false)});
  collation_map.insert({323, collation_info_t("utf8mb4_mn_cyrl_0900_as_cs",
                                          "utf8mb4", 323, false)});
}
