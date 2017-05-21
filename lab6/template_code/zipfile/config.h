#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#include <time.h>
#include <inttypes.h>

#if defined(RUN_AVG_LENGTH)
#else
	#define RUN_AVG_LENGTH 5
#endif

#if defined(SET_MIN_TEMP)
#else
	#error "SET_MIN_TEMP is not defined"
#endif

#if defined(SET_MAX_TEMP)
#else
	#error "SET_MAX_TEMP is not defined"
#endif

typedef uint16_t sensor_id_t;
typedef double sensor_value_t;     
typedef time_t sensor_ts_t;         // UTC timestamp as returned by time() - notice that the size of time_t is different on 32/64 bit machine
typedef uint16_t room_id_t;

typedef struct {
  sensor_id_t id;
  sensor_value_t value;
  sensor_ts_t ts;
} sensor_data_t;

typedef struct {
	sensor_id_t sensor_id;
	room_id_t room_id;
	sensor_value_t buffer[RUN_AVG_LENGTH];
	sensor_ts_t last_modified;
	int index;
	int buffer_size;
} sensor_node_t;

#include "datamgr.h"
#endif /* _CONFIG_H_ */

