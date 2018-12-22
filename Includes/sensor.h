


#include <cstdint>
#include "adc.h"
#include "pit.h"



struct InductorData
{
	volatile bool flag_data_updated;
	uint16_t values[6];
};


#ifdef __cplusplus
extern "C"
{
#endif




#ifdef __cplusplus
}
#endif
