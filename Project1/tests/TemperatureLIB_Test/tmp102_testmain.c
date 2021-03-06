/**
 * @brief Test for tmp102 sensor
 * 
 * @file tmp102_testmain.c
 * @author Gunj Manseta
 * @date 2018-03-14
 */

#include "my_i2c.h"
#include "tmp102_sensor.h"
#include <unistd.h>
#include "cmocka.h"

I2C_MASTER_HANDLE_T i2c;



static void testTMP102(void **state)
{
    int ret = I2Cmaster_Init(&i2c);
    assert_int_equal(ret, 0);
    assert_non_null((void*)getMasterI2C_handle());
    assert_ptr_equal(&i2c,getMasterI2C_handle());

    uint16_t *memdump = TMP102_memDump();
    assert_non_null(memdump);
    // printf("----1.SENSOR DUMP-----\n");
    // for(uint8_t i = 0; i < 4; i++)
    // {
    //     (i == 1) ? assert_int_equal(memdump[i], 0x60a0): 0;
    //     (i == 2) ? assert_int_equal(memdump[i], 0x4b00): 0;
    //     (i == 3) ? assert_int_equal(memdump[i], 0x5000): 0;
    //     printf("%02dh : 0x%x\n",i,memdump[i]);
    // }
    // printf("---------\n");

    assert_int_equal(memdump[1], 0x60a0);
    assert_int_equal(memdump[2], 0x4b00);
    assert_int_equal(memdump[3], 0x5000);

    free(memdump);

    ret = TMP102_setMode_ALERT_ActiveHigh();
    assert_int_equal(ret, 0);
    ret = TMP102_setMode_CR_8HZ();
    assert_int_equal(ret, 0);
    ret = TMP102_setMode_SD_PowerSaving();
    assert_int_equal(ret, 0);
    ret = TMP102_setMode_TM_InterruptMode();
    assert_int_equal(ret, 0);
    ret = TMP102_setMode_EM_ExtendedMode();
    assert_int_equal(ret, 0);


    memdump = TMP102_memDump();
    assert_non_null(memdump);
    // printf("----2.SENSOR DUMP-----\n");
    // for(uint8_t i = 0; i < 4; i++)
    // {
    //     (i == 1) ? assert_int_equal(memdump[i], 0x67d0): 0;
    //     (i == 2) ? assert_int_equal(memdump[i], 0x4b00): 0;
    //     (i == 3) ? assert_int_equal(memdump[i], 0x5000): 0;
    //     printf("%02dh : 0x%x\n",i,memdump[i]);
    // }
    // printf("---------\n");

    assert_int_equal(memdump[1], 0x67d0);
    assert_int_equal(memdump[2], 0x4b00);
    assert_int_equal(memdump[3], 0x5000);

    free(memdump);


    
    ret = TMP102_setmode_allDefault();
    assert_int_equal(ret, 0);

    memdump = TMP102_memDump();
    // printf("----3.SENSOR DUMP-----\n");
    // for(uint8_t i = 0; i < 4; i++)
    // {
    //     (i == 1) ? assert_int_equal(memdump[i], 0x60a0): 0;
    //     (i == 2) ? assert_int_equal(memdump[i], 0x4b00): 0;
    //     (i == 3) ? assert_int_equal(memdump[i], 0x5000): 0;
    
    //     printf("%02dh : 0x%x\n",i,memdump[i]);
    // }
    // printf("---------\n");

    assert_int_equal(memdump[1], 0x60a0);
    assert_int_equal(memdump[2], 0x4b00);
    assert_int_equal(memdump[3], 0x5000);

    /* Checking the Tlow = 75deg C and Thigh 80deg C */
    float temp = (float)(memdump[2]>>4) * 0.0625;
    assert_int_equal(temp,75.0);

    temp = (float)(memdump[3]>>4) * 0.0625;
    assert_int_equal(temp,80.0);

    free(memdump);

    float temperature = 0.0, celcius = 0.0f, dummy = 0.0;
    
    int i = 0;

    printf("\n---------TEMPERATURE VALUES--------------\n");
    while(i<3)
    {
		int ret = TMP102_getTemp_Celcius(&temperature);
        assert_int_equal(ret, 0);
		if(ret == 0) printf("C Temp: %.03f\n",temperature);
        celcius = temperature;

		ret = TMP102_getTemp_Fahren(&temperature);
        assert_int_equal(ret, 0);
        dummy = (celcius*1.8) + 32;
        assert_true(temperature == dummy);
		if(ret == 0) printf("F Temp: %.03f\n",temperature);

		ret = TMP102_getTemp_Kelvin(&temperature);
        assert_int_equal(ret, 0);
        dummy = celcius + 273.15;
        assert_true(temperature == dummy);
		if(ret == 0) printf("K Temp: %.03f\n",temperature);
	    
        i++;
        sleep(1);
    } 
    printf("------------------------------------------\n");


    ret = I2Cmaster_Destroy(&i2c);
    assert_int_equal(ret, 0);
    assert_null((void*)getMasterI2C_handle());

}


int main()
{
    const struct CMUnitTest tests[] = {	
	
    cmocka_unit_test(testTMP102)
    
	};

	return cmocka_run_group_tests(tests, NULL, NULL);

}