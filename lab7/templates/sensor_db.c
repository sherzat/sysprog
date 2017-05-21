#include "config.h"
#include <string.h>


DBCONN * init_connection(char clear_up_flag)
{
	sqlite3 *db;

	/* Database filename (UTF-8), 
	 * OUT: SQLite db handle */
	int rc = sqlite3_open(TO_STRING(DB_NAME), &db);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(EXIT_FAILURE);
	}
	char sql[256];
	char *err_msg;
	if (clear_up_flag)
	{
		sprintf(sql, "DROP TABLE IF EXISTS %s;\nCREATE TABLE %s%s",TO_STRING(TABLE_NAME), TO_STRING(TABLE_NAME), 
			"(id INTEGER PRIMARY KEY, sensor_id INTEGER, sensor_value DECIMAL(4,2),timestamp TIMESTAMP);");
	}else{
		sprintf(sql, "CREATE TABLE IF NOT EXISTs %s%s", TO_STRING(TABLE_NAME), 
			"(id INTEGER PRIMARY KEY, sensor_id INTEGER, sensor_value DECIMAL(4,2),timestamp TIMESTAMP);");
	}
	
	//printf("%s\n", sql);
	/* An open database i, 
	 * SQL to be evaluated, 
	 * 1st argument to callback, 
	 * Callback function, 
	 * Error msg written here */
	rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
		sqlite3_close(db);
		exit(EXIT_FAILURE);
	}

	return db;
}

void disconnect(DBCONN *conn)
{
	sqlite3_close(conn);
}

int insert_sensor(DBCONN * conn, sensor_id_t id, sensor_value_t value, sensor_ts_t ts)
{
	char sql[256];
	char *err_msg;
	int rc = 0;
	sprintf(sql, "INSERT INTO SensorData(sensor_id, sensor_value, timestamp) VALUES(%d, %g, %ld);", id, value, ts);
	//printf("%s\n", sql);
	rc = sqlite3_exec(conn, sql, NULL, NULL, &err_msg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
		sqlite3_close(conn);
		return -1;
	}
	return 0;
}
/**/
int insert_sensor_from_file(DBCONN * conn, FILE * sensor_data)
{
	sensor_data_t * measurement = NULL;
	measurement = malloc(sizeof(sensor_data_t));
	if (measurement == NULL)
	{
		fprintf(stderr, "failed to malloc\n");
		return -1;
	}
	while (1)
	{
		fread(&measurement->id, sizeof(sensor_id_t), 1, sensor_data);
		fread(&measurement->value, sizeof(sensor_value_t), 1, sensor_data);
		fread(&measurement->ts, sizeof(sensor_ts_t), 1, sensor_data);
		if (feof(sensor_data) != 0)
			break;
		insert_sensor(conn, measurement->id, measurement->value, measurement->ts);
	} 
	free(measurement);
	return 0;
}

int find_sensor_all(DBCONN * conn, callback_t f)
{
	char sql[256];
	char *err_msg;
	int rc = 0;
	sprintf(sql, "SELECT * FROM %s;", TO_STRING(TABLE_NAME));

	rc = sqlite3_exec(conn, sql, f, NULL, &err_msg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
		sqlite3_close(conn);
		return -1;
	}

	return 0;
}


int find_sensor_by_value(DBCONN * conn, sensor_value_t value, callback_t f)
{
	char sql[256];
	char *err_msg;
	int rc = 0;
	sprintf(sql, "SELECT * FROM %s WHERE sensor_value==%f;", TO_STRING(TABLE_NAME), value);
	
	rc = sqlite3_exec(conn, sql, f, NULL, &err_msg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
		sqlite3_close(conn);
		return -1;
	}
	return 0;
}

int find_sensor_exceed_value(DBCONN * conn, sensor_value_t value, callback_t f)
{
	char sql[256];
	char *err_msg;
	int rc = 0;
	sprintf(sql, "SELECT * FROM %s WHERE sensor_value>%f;", TO_STRING(TABLE_NAME), value);

	rc = sqlite3_exec(conn, sql, f, NULL, &err_msg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
		sqlite3_close(conn);
		return -1;
	}
	return 0;
}


int find_sensor_by_timestamp(DBCONN * conn, sensor_ts_t ts, callback_t f)
{
	char sql[256];
	char *err_msg;
	int rc = 0;
	sprintf(sql, "SELECT * FROM %s WHERE timestamp==%ld;", TO_STRING(TABLE_NAME), ts);

	rc = sqlite3_exec(conn, sql, f, NULL, &err_msg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
		sqlite3_close(conn);
		return -1;
	}
	return 0;
}

int find_sensor_after_timestamp(DBCONN * conn, sensor_ts_t ts, callback_t f)
{
	char sql[256];
	char *err_msg;
	int rc = 0;
	sprintf(sql, "SELECT * FROM %s WHERE timestamp>%ld;", TO_STRING(TABLE_NAME), ts);

	rc = sqlite3_exec(conn, sql, f, NULL, &err_msg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
		sqlite3_close(conn);
		return -1;
	}
	return 0;
}
