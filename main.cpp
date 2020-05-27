#include "load_tables.hpp"

using namespace std;
using namespace pqxx;

int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      //cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }


  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  try{
    string sql = "DROP TABLE IF EXISTS PLAYER;";
    sql += "DROP TABLE IF EXISTS TEAM;";
    sql += "DROP TABLE IF EXISTS STATE;";
    sql += "DROP TABLE IF EXISTS COLOR;";
    sql += "SELECT 'drop sequence ' || c.relname || ';' FROM pg_class c WHERE (c.relkind = 'S');";

    /* Create Four Tables */
    sql += "CREATE TABLE PLAYER("  \
    "PLAYER_ID SERIAL PRIMARY KEY," \
    "TEAM_ID        INT            NOT NULL," \
    "UNIFORM_NUM    INT            NOT NULL," \
    "FIRST_NAME     VARCHAR(50)    NOT NULL," \
    "LAST_NAME      VARCHAR(50)    NOT NULL," \
    "MPG            INT            NOT NULL," \
    "PPG            INT            NOT NULL," \
    "RPG            INT            NOT NULL," \
    "APG            INT            NOT NULL," \
    "SPG            DECIMAL(2,1)   NOT NULL," \
    "BPG            DECIMAL(2,1)   NOT NULL);"; 

    /* Create a transactional object. */
    work W(*C);

    sql += "CREATE TABLE TEAM("\
    "TEAM_ID SERIAL PRIMARY KEY," \
    "NAME           VARCHAR(50)    NOT NULL," \
    "STATE_ID       INT            NOT NULL," \
    "COLOR_ID       INT            NOT NULL," \
    "WINS           INT            NOT NULL," \
    "LOSSES         INT            NOT NULL);";

    sql += "CREATE TABLE STATE("  \
    "STATE_ID SERIAL PRIMARY KEY," \
    "NAME           VARCHAR(50)    NOT NULL);";

    sql += "CREATE TABLE COLOR("  \
    "COLOR_ID SERIAL PRIMARY KEY," \
    "NAME           VARCHAR(50)    NOT NULL);";

    sql += load::load_table("player.txt", 
           "PLAYER(TEAM_ID,UNIFORM_NUM,FIRST_NAME,LAST_NAME,MPG,PPG,RPG,APG,SPG,BPG)");

    sql += load::load_table("team.txt", "TEAM(NAME,STATE_ID,COLOR_ID,WINS,LOSSES)");

    sql += load::load_table("state.txt", "STATE(NAME)");

    sql += load::load_table("color.txt", "COLOR(NAME)");

    W.exec( sql );
    W.commit();
    
  } catch(const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }

  exercise(C);

  //Close database connection
  C->disconnect();

  return 0;
}


