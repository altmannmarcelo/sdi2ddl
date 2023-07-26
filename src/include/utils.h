#ifndef UTILS_H
#define UTILS_H

using namespace std;
/** escape a string
@param[in]	    text	  string to escape
@return escaped string */
string scape_string(const string &text) {
  string ret = "`";
  ret.append(text);
  ret.append("`");
  return ret;
}

#endif /* UTILS_H */
