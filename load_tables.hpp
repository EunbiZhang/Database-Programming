#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "exerciser.h"

/* read information from the source txt files and add rows into the tables */
class load{
public:
    static bool isNum(string& str){
        if(str.length() == 0) return false;
        if(str[0] >= '0' && str[0] <= '9'){
            return true;
        }
        return false;
    }

    static string load_table(const char* filename, string tablename){
        string sql;
        fstream file(filename, fstream::in);
        if (!file) {
            cout << "Unable to open file: " << filename << endl;
            return sql;
        }
        string line;
        while(getline(file, line)){
            sql += "INSERT INTO " + tablename + " VALUES(";
            istringstream ss(line);
            vector<string> attr;
            string tmp;
            while(ss >> tmp){
                attr.push_back(tmp);
            }
            for(int i = 1; i < attr.size(); ++i){
                if(load::isNum(attr[i]) == true){
                    sql += attr[i];
                }
                else{
                    if(attr[i].find('\'') != string::npos)
                        attr[i] = attr[i].replace(attr[i].find('\''), 1, "''");
                    sql = sql + '\'' + attr[i] + '\'';
                }
                if(i < attr.size()-1){
                    sql += ',';
                }
            }
            sql += ");\n";
        }
        file.close();
        return sql;
    }  
};