
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "BBBiolib.h"
/* ----------------------------------------------------------- */
#define BUFFER_SIZE 48000
#define SAMPLE_SIZE 48000
/* ----------------------------------------------------------- */
int main(void)
{
	unsigned int sample;
	int i ,j;
	unsigned int buffer_AIN_2[BUFFER_SIZE] ={0};

	time_t rawtime;
	char data_file_name[255];
	FILE* data_file;

	/* BBBIOlib init*/
	iolib_init();

	/* using ADC_CALC toolkit to decide the ADC module argument . Example Sample rate : 10000 sample/s
	 *
	 *	#./ADC_CALC -f 10000 -t 5
	 *
	 *	Suggest Solution :
	 *		Clock Divider : 160 ,   Open Dly : 0 ,  Sample Average : 1 ,    Sample Dly : 1
	 *
	 */
//	const int clk_div = 34 ;

	// ADC sampling settings
	const int clk_div = 25;
	const int open_dly = 5;
	const int sample_dly = 1;

	/*ADC work mode : Timer interrupt mode
	 *	Note : This mode handle SIGALRM using signale() function in BBBIO_ADCTSC_work();
	 */
	BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_TIMER_INT, clk_div);	// use timer interrupts

	/*ADC work mode : Busy polling mode  */
	/* BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_BUSY_POLLING, clk_div);*/


	// microphone is connected to AIN2
	BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN2, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, open_dly, sample_dly, \
				BBBIO_ADC_STEP_AVG_1, buffer_AIN_2, BUFFER_SIZE);
	

	//BBBIO_ADCTSC_module_ctrl(BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_BUSY_POLLING, 1);

	//	BBBIO_ADCTSC_channel_ctrl(BBBIO_ADC_AIN1, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, 0, 1, BBBIO_ADC_STEP_AVG_1, buffer_AIN_1, 100);

	printf("Starting capture...\n");
	// get time
	time(&rawtime);

	// start capture
	BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN2);
	BBBIO_ADCTSC_work(SAMPLE_SIZE);

	// format file name
	snprintf(data_file_name, sizeof(data_file_name), "%s", ctime(&rawtime));	//copy time to string
	data_file_name[strcspn(data_file_name,"\n")] = 0;							//remove trailing newline
	// convert space to _ in filename
	char *p = data_file_name;
	for (i=0; i < strlen(data_file_name); i++){
		if (data_file_name[i] == '\0') break;
		else if (data_file_name[i] == ' ') data_file_name[i] = '_';

	}
	strcat(data_file_name,".dat" );
	
	printf("saving sound data to: %s\n",data_file_name);
	data_file = fopen(data_file_name,"w");	// open file in write mode
	
	// add current time value to top of file
	fprintf(data_file, "%s", ctime(&rawtime));

	// write buffer to file
	for(j = 0 ; j < SAMPLE_SIZE ; j++)
		fprintf( data_file, "%u\n", buffer_AIN_2[j] );

	fclose( data_file );

	iolib_free();
	return 0;
}


