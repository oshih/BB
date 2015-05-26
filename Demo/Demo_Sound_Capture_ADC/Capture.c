
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "BBBiolib.h"
#include "rt_nonfinite.h"
#include "Prep.h"
#include "Prep_emxutil.h"
#include "abs.h"
#include "bsxfun.h"
#include "fft.h"
#include "linspace.h"

/* ----------------------------------------------------------- */
//#define BUFFER_SIZE 48000
//#define SAMPLE_SIZE 48000
// Collect for 1s with 192k
#define BUFFER_SIZE 192000
#define SAMPLE_SIZE 192000
/* ----------------------------------------------------------- */
#define FILTER_FILE "c_20k_23k_500ms.txt"

int main(void)
{
	unsigned int sample;
	int i ,j, count=0, size=5000;
	unsigned int buffer_AIN_2[BUFFER_SIZE] ={0};
	time_t rawtime;
	char data_file_name[255];
	FILE* data_file;
	FILE* filter_fd;
	char fname[] = FILTER_FILE;

	float local_buff[5000] = {0};
	float input[BUFFER_SIZE] = {0};
	float filter[100000] = {0};
	struct emxArray_real32_T prc_data;	
	struct emxArray_real32_T* prc_pt = &prc_data;
	prc_pt->data = (float*)&local_buff;
	prc_pt->size = &size;
	prc_pt->allocatedSize = size*sizeof(float);
	prc_pt->numDimensions = 1;
	prc_pt->canFreeData = false;	

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
	//const int clk_div = 25;
	//const int open_dly = 5;
	//const int sample_dly = 1;
	// setting for 192k
	const int clk_div = 5;
	const int open_dly = 10;
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

	printf("Starting capture with rate %d ...\n", SAMPLE_SIZE);
	// get time
	time(&rawtime);

	// start capture
	BBBIO_ADCTSC_channel_enable(BBBIO_ADC_AIN2);
	BBBIO_ADCTSC_work(SAMPLE_SIZE);
	
	// Preprocessing
	// copy and convert to float
	for(i=0;i<BUFFER_SIZE;i++){
		input[i] = (float)buffer_AIN_2[i];
	} 
	// load filter
	count=0;
	filter_fd =fopen(fname, "r");
	while(count<100000 && fscanf(filter_fd, "%f\n", &filter[count])!= EOF){
		count++;
	}
	fclose(filter_fd);
	printf("Filter loaded #%d\n", count);
	Prep(input, filter, prc_pt);	
	printf("Preprocessing done\n");

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
	
	printf("Saving processed sound data to: %s\n",data_file_name);
	data_file = fopen(data_file_name,"w");	// open file in write mode
	
	// add current time value to top of file
	fprintf(data_file, "%s", ctime(&rawtime));

	// write buffer to file
	//for(j = 0 ; j < SAMPLE_SIZE ; j++)
	//	fprintf( data_file, "%u\n", buffer_AIN_2[j] );

	// Write processed data instead, should be 4096 in length
	for(j = 0 ; j < 4096 ; j++)
		fprintf( data_file, "%f\n", local_buff[j] );
		


	fclose( data_file );

	iolib_free();
	return 0;
}


