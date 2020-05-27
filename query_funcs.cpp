#include "query_funcs.h"
#include <iomanip>


void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{

    string sql = "INSERT INTO PLAYER (TEAM_ID,UNIFORM_NUM,"\
                 "FIRST_NAME,LAST_NAME,MPG,PPG,RPG,APG,SPG,BPG) VALUES (";
    sql += to_string(team_id) + ", " + to_string(jersey_num) + ", \'" + first_name + "\', \'" +
            last_name + "\', " + to_string(mpg) + ", " + to_string(ppg) + ", " + to_string(rpg) + 
            ", " + to_string(apg) + ", " + to_string(spg) + ", " + to_string(bpg) + ");";
    
    work W(*C); 
    W.exec(sql);
    W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
    string sql = "INSERT INTO TEAM (NAME,STATE_ID,COLOR_ID,WINS,LOSSES) VALUES (";
    sql += " \'" + name + "\', " + to_string(state_id) + ", " + to_string(color_id) + ", " 
            + to_string(wins) + ", " + to_string(losses) + ");";
    
    work W(*C); 
    W.exec(sql);
    W.commit();
}


void add_state(connection *C, string name)
{
    string sql = "INSERT INTO STATE (NAME) VALUES (";
    sql += " \'" + name + "\'); ";
    
    work W(*C); 
    W.exec(sql);
    W.commit();
}


void add_color(connection *C, string name)
{
    string sql = "INSERT INTO COLOR (NAME) VALUES (";
    sql += " \'" + name + "\'); ";
    
    work W(*C); 
    W.exec(sql);
    W.commit();
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
    string sql = 
    "SELECT * FROM PLAYER WHERE PLAYER_ID IN ("\
    "SELECT PLAYER_ID FROM PLAYER GROUP BY PLAYER_ID HAVING ";
    if(use_mpg == 1){
        sql += "(MPG BETWEEN " + to_string(min_mpg) + " and " + to_string(max_mpg) + ") AND ";
    }
    if(use_ppg == 1){
        sql += "(PPG BETWEEN " + to_string(min_ppg) + " and " + to_string(max_ppg) + ") AND ";
    }
    if(use_rpg == 1){
        sql += "(RPG BETWEEN " + to_string(min_rpg) + " and " + to_string(max_rpg) + ") AND ";
    }
    if(use_apg == 1){
        sql += "(APG BETWEEN " + to_string(min_apg) + " and " + to_string(max_apg) + ") AND ";
    }
    if(use_spg == 1){
        sql += "(SPG BETWEEN " + to_string(min_spg) + " and " + to_string(max_spg) + ") AND ";
    }
    if(use_bpg == 1){
        sql += "(BPG BETWEEN " + to_string(min_bpg) + " and " + to_string(max_bpg) + ") AND ";
    }
    sql += "(AVG(PLAYER_ID) > 0) )";

    nontransaction N(*C);
    result res( N.exec(sql) );
    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;
    for (result::const_iterator c = res.begin(); c != res.end(); ++c) {
        cout << c[0].as<int>() << " ";
        cout << c[1].as<int>() << " ";
        cout << c[2].as<int>() << " ";
        cout << c[3].as<string>() << " ";
        cout << c[4].as<string>() << " ";
        cout << c[5].as<int>() << " ";
        cout << c[6].as<int>() << " ";
        cout << c[7].as<int>() << " ";
        cout << c[8].as<int>() << " ";
        cout<< setiosflags(ios::fixed) << setprecision(1);
        cout << c[9] << " ";
        cout << c[10] << endl;
    }
}


void query2(connection *C, string team_color)
{
    string sql = 
    "SELECT t.name FROM team t LEFT JOIN color c ON t.color_id = c.color_id WHERE c.name = \'" 
    + team_color + "\' ";
   
    
    nontransaction N(*C);
    result res( N.exec(sql) );
    cout << "NAME" << endl;
    for (result::const_iterator c = res.begin(); c != res.end(); ++c) {
        cout << c[0].as<string>() << endl;
    }
}


void query3(connection *C, string team_name)
{
    string sql = 
    "SELECT p.first_name, p.last_name FROM player p LEFT JOIN team t ON p.team_id = t.team_id "\
    "WHERE t.name = \'" + team_name + "\' ORDER BY p.ppg DESC";
    
    
    nontransaction N(*C);
    result res( N.exec(sql) );
    cout << "FIRST_NAME LAST_NAME" << endl;
    for (result::const_iterator c = res.begin(); c != res.end(); ++c) {
        cout << c[0].as<string>() << " " << c[1].as<string>() << endl;
    }
}


void query4(connection *C, string team_state, string team_color)
{
    string sql = 
    "SELECT p.first_name, p.last_name, p.uniform_num FROM player p "\
    "LEFT JOIN team t ON p.team_id = t.team_id "\
    "LEFT JOIN state s ON t.state_id = s.state_id "\
    "LEFT JOIN color c ON t.color_id = c.color_id "\
    "WHERE s.name = \'" + team_state + "\' AND c.name = \'" + team_color + "\' ";
    
    
    nontransaction N(*C);
    result res( N.exec(sql) );
    cout << "FIRST_NAME LAST_NAME UNIFORM_NUM" << endl;
    for (result::const_iterator c = res.begin(); c != res.end(); ++c) {
        cout << c[0].as<string>() << " " << c[1].as<string>() << " " << c[2].as<int>() << endl;
    }
}


void query5(connection *C, int num_wins)
{
    string sql = 
    "SELECT p.first_name, p.last_name, t.name, t.wins "\
    "FROM player p LEFT JOIN team t ON p.team_id = t.team_id WHERE t.wins > " 
    + to_string(num_wins);
    
    
    nontransaction N(*C);
    result res( N.exec(sql) );
    cout << "FIRST_NAME LAST_NAME NAME WINS" << endl;
    for (result::const_iterator c = res.begin(); c != res.end(); ++c) {
        cout << c[0].as<string>() << " " << c[1].as<string>() << " " << c[2].as<string>() 
        << " " << c[3].as<int>() << endl;
    }
}