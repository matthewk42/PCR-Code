/* Thermistor_Calibration.c
 *  Author: martijus
 * The thermistor is Rt= 10k Ohms at 25°C. The chart shows voltage to resistance as R/Rt. 
 Thus we multiply the resistances in the table by R*10000 to get the real resistance at temperature.
 The ADC will take the Vcc (~5v) and divide that voltage range into 2^10 resolution. 
 Thus 
 
 */ 
#include <avr/io.h>
#include "ADC.h"			// for ADC read
#include <math.h>		// for exp()
#include "temp.h"



// finds temp in range, then uses equation fitting for 1°C accuracy. If not in range returns 256.
double convTemp(double resist){	
	

	// -55°C to -50°C range:
	if (resist <= 526240 && resist > 384520)
	{
		return -55;
	}
	
	// -50°C to -45°C range:
	else if (resist <= 384520 && resist > 284010)
	{
		return -50;
	}
	
	// -45°C to -40°C range:
	else if (resist <= 284010 && resist > 211940)
	{
		return -45;
	}
	
	// -40°C to -35°C range:
	else if (resist <= 211940 && resist > 159720)
	{
		return -40;
	}
	
	// -35°C to -30°C range:
	else if (resist <= 159720 && resist > 121490)
	{
		return -35;
	}
	
	// -30°C to -25°C range:
	else if (resist <= 121490 && resist > 93246)
	{
		return -30;
	}
	
	// -25°C to -20°C range:
	else if (resist <= 93246 && resist > 72181)
	{
		return -25;
	}
	
	// -20°C to -15°C range:
	else if (resist <= 72181 && resist > 56332)
	{
		return -20;
	}
	
	// -15°C to -10°C range:
	else if (resist <= 56332 && resist > 44308)
	{
		return -15;
	}
	
	// -10°C to -5°C range:
	else if (resist <= 44308 && resist > 35112)
	{
		return -10;
	}
	
	// -5°C to 0°C range:
	else if (resist <= 35112 && resist > 28024)
	{
		return -5;
	}
	
	// 0°C to 5°C range:
	else if (resist <= 28024 && resist > 22520)
	{
		return 0;
	}
	
	// 5°C to 10°C range:
	else if (resist <= 22520 && resist > 18216)
	{
		return 5;
	}
	
	// 10°C to 15°C range:
	else if (resist <= 18216 && resist > 14827)
	{
		return 10;
	}
	
	// 15°C to 20°C range:
	else if (resist <= 14827 && resist > 12142)
	{
		if (resist <= 14827 && resist > 14240){
			return 15;	
		}
		else if (resist <= 14240 && resist > 13678){
			return 16;
		}		
		else if (resist <= 13678 && resist > 13143){
			return 17;
		}
		else if (resist <= 13143 && resist > 12631){
			return 18;
		}
		else if (resist <= 12631 && resist > 12142){
			return 19;
		}		
	}
	
	/*******************************************************************************************************************
	********************* IN PCR RANGE *********************************************************************************
	*******************************************************************************************************************/
	
	// 20°C to 25°C range:
	else if (resist <= 12142 && resist > 10000)
	{
		//return ((12142-resist)/(428.4)) + 20;
		if (resist <= 12142 && resist > 11674)// 12142-10000 = 2142 & 2142/5 = 428.4
		{
		return 20;
		}
		else if (resist <= 11674 && resist > 11227)
		{
			return 21;
		}
		else if (resist <= 11227 && resist > 10856.8)
		{
			return 22;
		}
		else if (resist <= 10800 && resist > 10391)
		{
			return 23;
		}
		else if (resist <= 10391 && resist > 10000)
		{
			return 24;
		}
		
	}
	
	// 25°C to 30°C range:
	else if (resist <= 10000 && resist > 8281.8)
	{
		//return ((10000-resist)/(343.64)) + 25;
		
		if (resist <= 10000 && resist > 9625.8) // 10000-8281.8 = 1718.2 & 1718.2/5 = 343.64
		{
		return 25;
		}
		else if (resist <= 9625.8 && resist > 9267.6)
		{
			return 26;
		}
		else if (resist <= 9267.6 && resist > 8924.7)
		{
			return 27;
		}
		else if (resist <= 8924.7 && resist > 8596.3)
		{
			return 28;
		}
		else if (resist <= 8596.3 && resist > 8281.8)
		{
			return 29;
		}
		
		
	}
	
	// 30°C to 35°C range:
	else if (resist <= 8281.8 && resist > 6895.4)
	{
		//return ((8281.8-resist)/(277.28)) + 30;
		
		if (resist <= 8281.8 && resist > 7980.5) // 8281.8-6895.4= 1386.4 & 1386.4/5= 277.28
		{
		return 30;
		}
		else if (resist <= 7980.5 && resist > 7691.8)
		{
			return 31;
		}
		else if (resist <= 7691.8 && resist > 7415)
		{
			return 32;
		}
		else if (resist <= 7415 && resist > 7149.7)
		{
			return 33;
		}
		else if (resist <= 7149.7 && resist > 6895.4)
		{
			return 34;
		}
		
		
	}
	
	// 35°C to 40°C range:
	else if (resist <= 6895.4 && resist > 5770.3)
	{
		//return ((6895.4-resist)/(225.02)) + 35;
		
		if (resist <= 6895.4 && resist > 6651.4) // 6895.4-5770.3= 1125.1 & 1125.1/5= 225.02
		{
		return 35;
		}
		else if (resist <= 6651.4 && resist > 6417.3)
		{
			return 36;
		}
		else if (resist <= 6417.3 && resist > 6192.7)
		{
			return 37;
		}
		else if (resist <= 6192.7 && resist > 5977.2)
		{
			return 38;
		}
		else if (resist <= 5977.2 && resist > 5770.3){
			return 39;
		}
		
	}
	
	// 40°C to 45°C range:
	else if (resist <= 5770.3 && resist > 4852.5)
	{
		//return ((5770.3-resist)/183.56) + 40;
		
		if (resist <= 5770.3 && resist > 5571.7)//5586.74) //5770.3-4852.5= 917.8 & 917.8/5= 183.56
		{
			return 40;
		}			
		else if (resist <= 5571.7 && resist > 5380.9) 
		{
			return 41;
		}
		else if (resist <= 5380.9 && resist > 5197.7)
		{
			return 42;
		}
		else if (resist <= 5197.7 && resist > 5021.7)
		{
			return 43;
		}
		else if (resist <= 5021.7 && resist > 4852.5)
		{
			return 44;
		}
		
	}
	
	// 45°C to 50°C range:
	else if (resist <= 4852.5 && resist > 4100)
	{
		//return ((4852.5-resist)/150.5) + 45;
		
		if (resist <= 4852.5 && resist > 4690) // 4852.5-4100= 752.5 & 752.5/5= 150.5
		{
		return 45;
		}
		else if (resist <= 4690 && resist > 4533.7)
		{
			return 46;
		}
		else if (resist <= 4533.7 && resist > 4383.5)
		{
			return 47;
		}
		else if (resist <= 4383.5 && resist > 4239)
		{
			return 48;
		}
		else if (resist <= 4239 && resist > 4100)
		{
			return 49;
		}
		
	}
	
	// 50°C to 55°C range:
	else if (resist <= 4100 && resist > 3479.8)
	{
		//return ((4100-resist)/124.04) + 50;
		
		if (resist <= 4100 && resist > 3966.3) //4100-3479.8= 620.2 & 620.2/5= 124.04
		{
		return 50;
		}
		else if (resist <= 3966.3 && resist > 3837.6) 
		{
			return 51;
		}
		else if (resist <= 3837.6 && resist > 3713.8)
		{
			return 52;
		}
		else if (resist <= 3713.8 && resist > 3594.6)
		{
			return 53;
		}
		else if (resist <= 3594.6 && resist > 3479.8)
		{
			return 54;
		}
		
		
	}
	
	// 55°C to 60°C range:
	else if (resist <= 3479.8 && resist > 2966.3)
	{
		//return ((3479.8-resist)/102.7) + 55;
		
		if (resist <= 3479.8 && resist > 3369.3) // 3479.8- 2966.3 = 513.5 & 513.5/5= 102.7 
		{
		return 55;
		}
		else if (resist <= 3369.3 && resist > 3262.9)
		{
			return 56;
		}
		else if (resist <= 3262.9 && resist > 3160.3)
		{
			return 57;
		}
		else if (resist <= 3160.3 && resist > 3061.5)
		{
			return 58;
		}
		else if (resist <= 3061.5 && resist > 2966.3)
		{
			return 59;
		}
		
		
	}
	
	// 60°C to 65°C range:
	else if ((resist <= 2966.3) && (resist > 2539.2))
	{
		//return ((2966.3-resist)/85.42) + 60;
		
		if ((resist <= 2966.3) && (resist > 2874.6)) // 2966.3-2539.2= 427.1 & 427.1/5= 85.42
		{
		return 60;
		}
		else if ((resist <= 2874.6) && (resist > 2786.1))
		{
			return 61;
		}
		else if ((resist <= 2786.1) && (resist > 2700.8))
		{
			return 62;
		}
		else if ((resist <= 2700.8) && (resist > 2618.6))
		{
			return 63;
		}
		else if ((resist <= 2618.6) && (resist > 2539.2))
		{
			return 64;
		}
		
	}
	
	// 65°C to 70°C range:
	else if (resist <= 2539.2 && resist > 2182.4)
	{
		//return ((2539.2-resist)/71.36) + 65;
		
		if (resist <= 2539.2 && resist > 2462.7) //2539.2-2182.4= 356.8 & 356.8/5= 71.36
		{
		return 65;
		}
		else if (resist <= 2462.7 && resist > 2388.8)
		{
			return 66;
		}
		else if (resist <= 2388.8 && resist > 2317.6)
		{
			return 67;
		}
		else if (resist <= 2317.6 && resist > 2248.8)
		{
			return 68;
		}
		else if (resist <= 2248.8 && resist > 2182.4)
		{
			return 69;
		}
		
	}
	
	// 70°C to 75°C range:
	else if (resist <= 2182.4 && resist > 1883)
	{
		//return ((2182.4-resist)/59.88) + 70;
		
		if (resist <= 2182.4 && resist > 2118.3) //2182.4-1883= 299.4 & 299.4/5= 59.88
		{
		return 70;
		}
		else if (resist <= 2118.3 && resist > 2056.4)
		{
			return 71;
		}
		else if (resist <= 2056.4 && resist > 1996.6)
		{
			return 72;
		}
		else if (resist <= 1996.6 && resist > 1938.8)
		{
			return 73;
		}
		else if (resist <= 1938.8 && resist > 1883)
		{
			return 74;
		}
		
	}
	
	// 75°C to 80°C range:
	else if (resist <= 1883 && resist > 1630.7)
	{
		//return ((1883-resist)/50.46) + 75;
		
		if (resist <= 1883 && resist > 1829) //1883-1630.7= 252.3 & 252.3/5=50.46
		{
		return 75;
		}
		else if (resist <= 1829 && resist > 1776.9)
		{
			return 76;
		}
		else if (resist <= 1776.9 && resist > 1726.5)
		{
			return 77;
		}
		else if (resist <= 1726.5 && resist > 1677.8)
		{
			return 78;
		}
		else if (resist <= 1677.8 && resist > 1630.7)
		{
			return 79;
		}
		
	}
	
	// 80°C to 85°C range:
	else if (resist <= 1630.7 && resist > 1417.4)
	{
		//return ((1630.7-resist)/42.66) + 80;
		
		if (resist <= 1630.7 && resist > 1585.2) //1630.7-1417.4= 213.3 & 213.3/5= 42.66
		{
		return 80;
		}
		else if (resist <= 1585.2 && resist > 1541.1)
		{
			return 81;
		}
		else if (resist <= 1541.1 && resist > 1498.5)
		{
			return 82;
		}
		
		else if (resist <= 1498.5 && resist > 1457.3)
		{
			return 83;
		}
		else if (resist <= 1457.3 && resist > 1417.4)
		{
			return 84;
		}
		
	}
	
	// 85°C to 90°C range:
	else if (resist <= 1417.4 && resist > 1236.2)
	{
		//return ((1417.4-resist)/36.24) + 85;
		
		if (resist <= 1417.4 && resist > 1378.7) //1417.4-1236.2= 181.2 & 181.2/5= 36.24
		{
		return 85;
		}
		else if (resist <= 1378.7 && resist > 1341.4)
		{
			return 86;
		}
		else if (resist <= 1341.4 && resist > 1305.2)
		{
			return 87;
		}
		else if (resist <= 1305.2 && resist > 1270.1)
		{
			return 88;
		}
		else if (resist <= 1270.1 && resist > 1236.2)
		{
			return 89;
		}
		
	}
	
	// 90°C to 95°C range:
	else if (resist <= 1236.2 && resist > 1081.8)
	{
		//return ((1236.2-resist)/30.88) + 90;
		
		if (resist <= 1236.2 && resist > 1205.32) // 1236.2-1081.8= 154.4 & 154.4/5= 30.88
		{
		return 90;
		}
		else if (resist <= 1205.32 && resist > 1174.44)
		{
			return 91;
		}
		else if (resist <= 1174.44 && resist > 1143.56)
		{
			return 92;
		}
		else if (resist <= 1143.56 && resist > 1112.68)
		{
			return 93;
		}
		else if (resist <= 1112.68 && resist > 1081.8)
		{
			return 94;
		}
		
	}
	
	// 95°C to 100°C range:
	else if (resist <= 1081.8 && resist > 949.73)
	{
		//return ((1081.8-resist)/26.414) + 95;
		
		if (resist <= 1081.8 && resist > 1055.39) // 1081.8-949.73= 132.07 & 132.07/5= 26.414
		{
		return 95;
		}
		else if (resist <= 1055.39 && resist > 1028.97)
		{
			return 96;
		}
		else if (resist <= 1028.97 && resist > 1002.56)
		{
			return 97;
		}
		else if (resist <= 1002.56 && resist > 976.144)
		{
			return 98;
		}
		else if (resist <= 976.144 && resist > 949.73)
		{
			return 99;
		}
		
	}
	
	/*******************************************************************************************************************
	********************* END PCR RANGE ********************************************************************************
	*******************************************************************************************************************/
	
	// 100°C to 105°C range:
	else if (resist <= 949.73 && resist > 836.4)
	{
		return 100;
	}
	
	// 105°C to 110°C range:
	else if (resist <= 836.4 && resist > 738.81)
	{
		return 105;
	}
	
	// 110°C to 115°C range:
	else if (resist <= 738.81 && resist > 654.5)
	{
		return 110;
	}
	
	// 115°C to 120°C range:
	else if (resist <= 654.5 && resist > 581.44)
	{
		return 115;
	}
	
	// 120°C to 125°C range:
	else if (resist <= 581.44 && resist > 517.94)
	{
		return 120;
	}
	
	// 125°C to 130°C range:
	else if (resist <= 517.94 && resist > 462.59)
	{
		return 125;
	}
	
	// 130°C to 135°C range:
	else if (resist <= 462.59 && resist > 414.2)
	{
		return 130;
	}
	
	// 135°C to 140°C range:
	else if (resist <= 414.2 && resist > 371.79)
	{
		return 135;
	}
	
	// 140°C to 145°C range:
	else if (resist <= 371.79 && resist > 334.51)
	{
		return 140;
	}
	
	// 145°C to 150°C range:
	else if (resist <= 334.51 && resist > 301.66)
	{
		return 145;
	}
	
	// 150°C to 155°C range:
	else if (resist <= 301.66 && resist > 272.64)
	{
		return 150;
	}
	
	//else 
	return 256;
}

double getTemp(void)
{
   // variables:
	double data;
	double resistance;
	double Vref = 5;
	double voltage; 
	double temp;
	//double test=111;
	double offset= 0;

	data = adc_read();	// Taking an ADC reading.
	voltage =0;
	/* Vref = 5v, ADC divides 0-5v into 1024 (2^10) sections. Thus each ADC value is 5/1024=0.004833  
	*/
	
	voltage = data*0.004833; //1024 divisions of Vcc
	resistance = (-10000* voltage)/(voltage -Vref); // rearange the voltage divider equation to get thermistor resistance.
	temp = convTemp(resistance) + offset; // use look-up table to get temperatures
	//send_serial(temp);	// outputs reading through serial.
	return temp;

	
}
