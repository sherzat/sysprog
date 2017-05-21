#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#include <time.h>
#include <inttypes.h>

#define FIFO_NAME 	"logFifo"

typedef uint16_t sensor_id_t;
typedef double sensor_value_t;     
typedef time_t sensor_ts_t;         // UTC timestamp as returned by time() - notice that the size of time_t is different on 32/64 bit machine
typedef uint16_t room_id_t;

typedef struct{
	sensor_id_t id;
	sensor_value_t value;
	sensor_ts_t ts;
} sensor_data_t;
			
struct sbuffer_data{
	sensor_data_t sensor_data;
};

#endif /* _CONFIG_H_ */

 
