/**
  *****************************************************************************
  * @title   main.c
  * @author  Fauzi
  * @date    12 July 2013
  * @brief   Turn on all LEDs in PX4Flow
  *
  *          PE3 (Blue), PE2 (Amber) and PE7 (Red) are configured in output push pull mode, turn ON.
  *           - Set PD12, PD13, PD14 and PD15 by setting corresponding bits in BSRRL register
  *           - Reset PD12, PD13, PD14 and PD15  by setting corresponding bits in BSRRH register
  *
  *******************************************************************************
  **/

#include <stm32f4xx_dcmi.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_i2c.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_usart.h>



/* Constants */
#define TIMEOUT_MAX      				10000

#define BINNING_ROW_A					4
#define BINNING_COLUMN_A				4
#define BINNING_ROW_B					2
#define BINNING_COLUMN_B				2
#define MINIMUM_HORIZONTAL_BLANKING		91 // see datasheet
#define MAX_IMAGE_HEIGHT				480
#define MAX_IMAGE_WIDTH					752
#define MINIMUM_COLUMN_START			1
#define MINIMUM_ROW_START				4

/* Camera I2C registers */
#define mt9v034_DEVICE_WRITE_ADDRESS    0xB8
#define mt9v034_DEVICE_READ_ADDRESS     0xB9

/* Context A */
#define MTV_COLUMN_START_REG_A  		0x01
#define MTV_ROW_START_REG_A     		0x02
#define MTV_WINDOW_HEIGHT_REG_A 		0x03
#define MTV_WINDOW_WIDTH_REG_A  		0x04
#define MTV_HOR_BLANKING_REG_A  		0x05
#define MTV_VER_BLANKING_REG_A  		0x06
#define MTV_COARSE_SW_1_REG_A			0x08
#define MTV_COARSE_SW_2_REG_A			0x09
#define MTV_COARSE_SW_CTRL_REG_A		0x0A
#define MTV_COARSE_SW_TOTAL_REG_A 		0x0B
#define MTV_FINE_SW_1_REG_A				0xD3
#define MTV_FINE_SW_2_REG_A				0xD4
#define MTV_FINE_SW_TOTAL_REG_A			0xD5
#define MTV_READ_MODE_REG_A        		0x0D
#define MTV_V1_CTRL_REG_A				0x31
#define MTV_V2_CTRL_REG_A				0x32
#define MTV_V3_CTRL_REG_A				0x33
#define MTV_V4_CTRL_REG_A				0x34
#define MTV_ANALOG_GAIN_CTRL_REG_A		0x35

/* Context B */
#define MTV_COLUMN_START_REG_B  		0xC9
#define MTV_ROW_START_REG_B     		0xCA
#define MTV_WINDOW_HEIGHT_REG_B 		0xCB
#define MTV_WINDOW_WIDTH_REG_B  		0xCC
#define MTV_HOR_BLANKING_REG_B  		0xCD
#define MTV_VER_BLANKING_REG_B  		0xCE
#define MTV_COARSE_SW_1_REG_B			0xCF
#define MTV_COARSE_SW_2_REG_B			0xD0
#define MTV_COARSE_SW_CTRL_REG_B		0xD1
#define MTV_COARSE_SW_TOTAL_REG_B 		0xD2
#define MTV_FINE_SW_1_REG_B				0xD6
#define MTV_FINE_SW_2_REG_B				0xD7
#define MTV_FINE_SW_TOTAL_REG_B			0xD8
#define MTV_READ_MODE_REG_B        		0x0E
#define MTV_V1_CTRL_REG_B				0x39
#define MTV_V2_CTRL_REG_B				0x3A
#define MTV_V3_CTRL_REG_B				0x3B
#define MTV_V4_CTRL_REG_B				0x3C
#define MTV_ANALOG_GAIN_CTRL_REG_B		0x36

/* General Registers */
#define MTV_CHIP_VERSION_REG    		0x00
#define MTV_CHIP_CONTROL_REG    		0x07
#define MTV_SOFT_RESET_REG      		0x0C

#define MTV_HDR_ENABLE_REG				0x0F
#define MTV_ADC_RES_CTRL_REG			0x1C
#define MTV_ROW_NOISE_CORR_CTRL_REG		0x70
#define MTV_TEST_PATTERN_REG       		0x7F
#define MTV_TILED_DIGITAL_GAIN_REG		0x80
#define MTV_AGC_AEC_DESIRED_BIN_REG		0xA5
#define MTV_MAX_GAIN_REG        		0xAB
#define MTV_MIN_EXPOSURE_REG       		0xAC  // datasheet min coarse shutter width
#define MTV_MAX_EXPOSURE_REG       		0xAD  // datasheet max coarse shutter width
#define MTV_AEC_AGC_ENABLE_REG			0xAF
#define MTV_AGC_AEC_PIXEL_COUNT_REG		0xB0

/*
 * Resolution:
 * ROW_SIZE * BINNING_ROW <= MAX_IMAGE_WIDTH
 * COLUMN_SIZE * BINNING_COLUMN <= MAX_IMAGE_HEIGHT
 */

#define FULL_IMAGE_SIZE (188*120)
#define FULL_IMAGE_ROW_SIZE (188)
#define FULL_IMAGE_COLUMN_SIZE (120)

//Resolution
#define PARAM_IMAGE_WIDTH			188
#define PARAM_IMAGE_HEIGHT			120
#define PARAM_CALIBRATION_ON 		0
#define PARAM_IMAGE_ROW_NOISE_CORR 	0
#define PARAM_IMAGE_LOW_LIGHT		0

//image buffers
uint8_t dcmi_image_buffer_8bit_1[FULL_IMAGE_SIZE];
uint8_t dcmi_image_buffer_8bit_2[FULL_IMAGE_SIZE];
uint8_t dcmi_image_buffer_8bit_3[FULL_IMAGE_SIZE];

volatile uint32_t frame_counter;


void leds_init(void);
void usart3_init(void);
void PrintChar(char c);
void ms_delay(unsigned long msTime);
void enable_image_capture(void);
void dcmi_clock_init();
void dcmi_hw_init(void);
void dcmi_dma_init(uint16_t buffer_size);
uint8_t mt9v034_WriteReg(uint16_t Addr, uint8_t Data);
void mt9v034_context_configuration(void);
uint8_t mt9v034_WriteReg16(uint16_t address, uint16_t Data);
uint8_t mt9v034_ReadReg(uint16_t Addr);
uint16_t mt9v034_ReadReg16(uint8_t address);
void dcmi_dma_enable();
void dcmi_dma_disable();
void send_DCMI_Image_8bit_1(void);
void send_DCMI_Image_8bit_1_SmartUAV(void);
void send_DCMI_Image_8bit_1_Qt(void);

GPIO_InitTypeDef  GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
I2C_InitTypeDef I2C_InitStruct;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
DCMI_InitTypeDef DCMI_InitStructure;
DMA_InitTypeDef DMA_InitStructure;

/*
 * MAIN PROGRAM
 */
int main(void)
{
	SystemInit();
	leds_init();
	usart3_init();
	enable_image_capture();
	GPIO_ResetBits(GPIOE,GPIO_Pin_3);
	GPIO_SetBits(GPIOE,GPIO_Pin_7);
    while(1)
    {
    	send_DCMI_Image_8bit_1_Qt();
    	GPIO_ToggleBits(GPIOE,GPIO_Pin_3);
    	GPIO_ToggleBits(GPIOE,GPIO_Pin_7);
    }
}


/*
 * SUBROUTINES
 */
void leds_init(void)
{
	/* GPIOE Peripheral clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* Configure PE3(Blue), PE2(Amber) and PE7(Red) in output push-pull mode */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2| GPIO_Pin_7;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE, GPIO_Pin_3 | GPIO_Pin_2| GPIO_Pin_7); //all OFF
}

void usart3_init(void)
{
	  /* Enable GPIO clock */
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	  /* Enable UART clock */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	  /* Connect PXx to USARTx_Tx*/
	  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);

	  /* Connect PXx to USARTx_Rx*/
	  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

	  /* Configure USART Tx as alternate function  */
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  /* Configure USART Rx as alternate function  */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  USART_InitStructure.USART_BaudRate = 256000;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	  /* USART configuration */
	  USART_Init(USART3, &USART_InitStructure);

	  /* Enable USART */
	  USART_Cmd(USART3, ENABLE);
}

void PrintChar(char c)
{
	uint8_t ch;
	ch = c;
	USART_SendData(USART3, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

void ms_delay(unsigned long msTime)
{
	unsigned long i,j, nCount=0x1310;
	for(i=0;i<msTime;i++)
		for(j=0;j<nCount;j++);
}

//Camera Image---------------------------------------------------------------------------------
void enable_image_capture(void)
{
	dcmi_clock_init();
	dcmi_hw_init();
	dcmi_dma_init(PARAM_IMAGE_WIDTH * PARAM_IMAGE_HEIGHT);
	mt9v034_context_configuration();
	dcmi_dma_enable();
}

void dcmi_clock_init() //provide 13.0-27.0 (fPWM=21) MHz clock signal to SYSCLK pin
{
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* GPIOC clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* GPIOC Configuration:  TIM3 CH3 (PC8)  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Connect TIM3 pins to AF2 */;
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 3;	 //period = ((fHCLK/2)/fPWM)-1  where fHCLK = 168MHz and fPWM = 21MHz
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //no prescaler
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 2;// 50%DC where (period*Duty_Cycle)/100
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}

void dcmi_hw_init(void)
{
	/*** Configures the DCMI GPIOs to interface with the MT9V034 camera module ***/
	/* Enable DCMI GPIOs clocks */
	RCC_AHB1PeriphClockCmd(
			RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC
					| RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);

	/* Connect DCMI pins to AF13 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI); //PA4 DCMI_HSYNC
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI); //PA6 DCMI_PIXCL

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI); //PB6 DCMI_D5
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI); //PB7 DCMI_VSYNC

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_DCMI); //PC6 DCMI_D0
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_DCMI); //PC7 DCMI_D1

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_DCMI); //PC10 DCMI_D8
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_DCMI); //PC12 DCMI_D9

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_DCMI); //PE0 DCMI_D2
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_DCMI); //PE1 DCMI_D3
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_DCMI); //PE4 DCMI_D4
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_DCMI); //PE5 DCMI_D6
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_DCMI); //PE6 DCMI_D7

	/* DCMI GPIO configuration */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4
			| GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* I2C2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	/* GPIOB clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* Connect I2C2 pins to AF4 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);

	/* Configure I2C2 GPIOs */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* I2C DeInit */
	I2C_DeInit(I2C2);

	/* Enable the I2C peripheral */
	I2C_Cmd(I2C2, ENABLE);

	/* Set the I2C structure parameters */
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0xFE;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = 100000; //100kHz

	/* Initialize the I2C peripheral w/ selected parameters */
	I2C_Init(I2C2, &I2C_InitStruct);
}

void dcmi_dma_init(uint16_t buffer_size)
{
	//reset frame counter
	frame_counter = 0;

	/*** Configures the DCMI to interface with the mt9v034 camera module ***/
	/* Enable DCMI clock */
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);

	/* DCMI configuration */
	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Falling;
	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_Low;
	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;
	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
	DCMI_Init(&DCMI_InitStructure);


	/* Configures the DMA2 to transfer Data from DCMI */
	/* Enable DMA2 clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	/* DMA2 Stream1 Configuration */
	DMA_DeInit(DMA2_Stream1);

	DMA_InitStructure.DMA_Channel = DMA_Channel_1;			//Stream 1 && Channel 1: DCMI
	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x50050028;	//DCMI_DR_ADDRESS
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) dcmi_image_buffer_8bit_1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = buffer_size / 4; // buffer size in date unit (word)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_DoubleBufferModeConfig(DMA2_Stream1,(uint32_t) dcmi_image_buffer_8bit_2, DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA2_Stream1,ENABLE);

	/* DMA2 IRQ channel Configuration */
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
}

void mt9v034_context_configuration(void)
{
	/* Chip Control
	 *
	 * bits           | 15 | ... | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	 * -------------------------------------------------------------------
	 * current values | 0  | ... | 0 | 1 | 1 | 0 | 0 | 0 | 1 | 0 | 0 | 0 |
	 *
	 * (0:2) Scan Mode (Progressive scan)
	 * (3:4) Sensor Operation Mode (Master mode)
	 * (5) Stereoscopy Mode (Disable)
	 * (6) Stereoscopic Master/Slave mode (not used)
	 * (7) Parallel Output Enable (Enable)
	 * (8) Simultaneous/Sequential Mode (Simultaneous mode)
	 * (9) Reserved
	 *
	 * (15)Context A (0) / Context B (1)
	 *
	 */

	uint16_t new_control;

	if (PARAM_CALIBRATION_ON)
		new_control = 0x8188; // Context B
	else
		new_control = 0x0188; // Context A

	/* image dimentions */
	uint16_t new_width_context_a  = PARAM_IMAGE_WIDTH * 4; // windowing off, row + col bin reduce size
	uint16_t new_height_context_a = PARAM_IMAGE_HEIGHT * 4;
	uint16_t new_width_context_b  = FULL_IMAGE_ROW_SIZE * 4; // windowing off, row + col bin reduce size
	uint16_t new_height_context_b = FULL_IMAGE_COLUMN_SIZE * 4;

	/* blanking settings */
	uint16_t new_hor_blanking_context_a = MAX_IMAGE_WIDTH - new_width_context_a + MINIMUM_HORIZONTAL_BLANKING;
	uint16_t new_ver_blanking_context_a = 10; // this value is the first without image errors (dark lines)
	uint16_t new_hor_blanking_context_b = MAX_IMAGE_WIDTH - new_width_context_b + MINIMUM_HORIZONTAL_BLANKING;
	uint16_t new_ver_blanking_context_b = 10;


	/* Read Mode
	 *
	 * bits           | ... | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	 * -------------------------------------------------------------------
	 * current values | ... |  0 | 1 | 1 | 0 | 0 | 0 | 0 | 1 | 0 | 1 | 0 |
	 *
	 * (1:0) Row Bin
	 * (3:2) Column Bin
	 * (9:8) Reserved
	 *
	 */
	uint16_t new_readmode_context_a = 0x30A ; // row + col bin 4 enable, (9:8) default
	uint16_t new_readmode_context_b = 0x305 ; // row bin 2 col bin 4 enable, (9:8) default

	/*
	 * Settings for both context:
	 *
	 * Exposure time should not affect frame time
	 * so we set max on 64 (lines) = 0x44
	 */
	uint16_t min_exposure = 0x0001; // default
	uint16_t max_exposure = 0x01E0; // default
	uint16_t new_max_gain = 64; // VALID RANGE: 16-64 (default)
	uint16_t pixel_count = 4096; //64x64 take all pixels to estimate exposure time // VALID RANGE: 1-65535

	uint16_t desired_brightness = 58; // default
	uint16_t resolution_ctrl = 0x0203; // default
	uint16_t hdr_enabled = 0x0100; // default
	uint16_t aec_agc_enabled = 0x0003; // default
	uint16_t coarse_sw1 = 0x01BB; // default from context A
	uint16_t coarse_sw2 = 0x01D9; // default from context A
	uint16_t shutter_width_ctrl = 0x0164; // default from context A
	uint16_t total_shutter_width = 0x01E0; // default from context A

	if(PARAM_IMAGE_LOW_LIGHT)
	{
		min_exposure = 0x0001;
		max_exposure = 0x0060;
		desired_brightness = 30; // VALID RANGE: 1-64
		resolution_ctrl = 0x0303;
		hdr_enabled = 0x0101; // on
		aec_agc_enabled = 0x0303; // on
		coarse_sw1 = 0x01BB; // default from context A
		coarse_sw2 = 0x01D9; // default from context A
		shutter_width_ctrl = 0x0164; // default from context A
		total_shutter_width = 0x01E0; // default from context A
	}
	else
	{
		min_exposure = 0x0001;
		max_exposure = 0x0080;
		desired_brightness = 50; // VALID RANGE: 1-64
		resolution_ctrl = 0x0202;
		hdr_enabled = 0x0000; // off
		aec_agc_enabled = 0x0303; // on
		coarse_sw1 = 0x01BB; // default from context A
		coarse_sw2 = 0x01D9; // default from context A
		shutter_width_ctrl = 0x0164; // default from context A
		total_shutter_width = 0x01E0; // default from context A
	}

	uint16_t row_noise_correction = 0x0000; // default

	if(PARAM_IMAGE_ROW_NOISE_CORR)
		row_noise_correction = 0x0101;
	else
		row_noise_correction = 0x0000;

	uint16_t version = mt9v034_ReadReg16(MTV_CHIP_VERSION_REG);

	if (version == 0x1324)
	{
		mt9v034_WriteReg16(MTV_CHIP_CONTROL_REG, new_control);

		/* Context A */
		mt9v034_WriteReg16(MTV_WINDOW_WIDTH_REG_A, new_width_context_a);
		mt9v034_WriteReg16(MTV_WINDOW_HEIGHT_REG_A, new_height_context_a);
		mt9v034_WriteReg16(MTV_HOR_BLANKING_REG_A, new_hor_blanking_context_a);
		mt9v034_WriteReg16(MTV_VER_BLANKING_REG_A, new_ver_blanking_context_a);
		mt9v034_WriteReg16(MTV_READ_MODE_REG_A, new_readmode_context_a);
		mt9v034_WriteReg16(MTV_COLUMN_START_REG_A, (MAX_IMAGE_WIDTH - new_width_context_a) / 2 + MINIMUM_COLUMN_START); // Set column/row start point for lower resolutions (center window)
		mt9v034_WriteReg16(MTV_ROW_START_REG_A, (MAX_IMAGE_HEIGHT - new_height_context_a) / 2 + MINIMUM_ROW_START);
		mt9v034_WriteReg16(MTV_COARSE_SW_1_REG_A, coarse_sw1);
		mt9v034_WriteReg16(MTV_COARSE_SW_2_REG_A, coarse_sw2);
		mt9v034_WriteReg16(MTV_COARSE_SW_CTRL_REG_A, shutter_width_ctrl);
		mt9v034_WriteReg16(MTV_V2_CTRL_REG_A, total_shutter_width);

		/* Context B */
		mt9v034_WriteReg16(MTV_WINDOW_WIDTH_REG_B, new_width_context_b);
		mt9v034_WriteReg16(MTV_WINDOW_HEIGHT_REG_B, new_height_context_b);
		mt9v034_WriteReg16(MTV_HOR_BLANKING_REG_B, new_hor_blanking_context_b);
		mt9v034_WriteReg16(MTV_VER_BLANKING_REG_B, new_ver_blanking_context_b);
		mt9v034_WriteReg16(MTV_READ_MODE_REG_B, new_readmode_context_b);
		mt9v034_WriteReg16(MTV_COLUMN_START_REG_B, MINIMUM_COLUMN_START); // default
		mt9v034_WriteReg16(MTV_ROW_START_REG_B, MINIMUM_ROW_START);
		mt9v034_WriteReg16(MTV_COARSE_SW_1_REG_B, coarse_sw1);
		mt9v034_WriteReg16(MTV_COARSE_SW_2_REG_B, coarse_sw2);
		mt9v034_WriteReg16(MTV_COARSE_SW_CTRL_REG_B, shutter_width_ctrl);
		mt9v034_WriteReg16(MTV_V2_CTRL_REG_B, total_shutter_width);

		/* General Settings */
		mt9v034_WriteReg16(MTV_ROW_NOISE_CORR_CTRL_REG, row_noise_correction);
		mt9v034_WriteReg16(MTV_AEC_AGC_ENABLE_REG, aec_agc_enabled); // disable AEC/AGC for both contexts
		mt9v034_WriteReg16(MTV_HDR_ENABLE_REG, hdr_enabled); // disable HDR on both contexts
		mt9v034_WriteReg16(MTV_MIN_EXPOSURE_REG, min_exposure);
		mt9v034_WriteReg16(MTV_MAX_EXPOSURE_REG, max_exposure);
		mt9v034_WriteReg16(MTV_MAX_GAIN_REG, new_max_gain);
		mt9v034_WriteReg16(MTV_AGC_AEC_PIXEL_COUNT_REG, pixel_count);
		mt9v034_WriteReg16(MTV_AGC_AEC_DESIRED_BIN_REG, desired_brightness);
		mt9v034_WriteReg16(MTV_ADC_RES_CTRL_REG, resolution_ctrl); // here is the way to regulate darkness :)

		/* Reset */
		mt9v034_WriteReg16(MTV_SOFT_RESET_REG, 0x01);
	}

}

uint8_t mt9v034_WriteReg(uint16_t Addr, uint8_t Data)
{
  uint32_t timeout = TIMEOUT_MAX;

  /* Generate the Start Condition */
  I2C_GenerateSTART(I2C2, ENABLE);

  /* Test on I2C2 EV5 and clear it */
  timeout = TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
  {
    /* If the timeout delay is exceeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }

  /* Send DCMI selected device slave Address for write */
  I2C_Send7bitAddress(I2C2, mt9v034_DEVICE_WRITE_ADDRESS, I2C_Direction_Transmitter);

  /* Test on I2C2 EV6 and clear it */
  timeout = TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    /* If the timeout delay is exceeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }

  /* Send I2C2 location address LSB */
  I2C_SendData(I2C2, (uint8_t)(Addr));

  /* Test on I2C2 EV8 and clear it */
  timeout = TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    /* If the timeout delay is exceeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }

  /* Send Data */
  I2C_SendData(I2C2, Data);

  /* Test on I2C2 EV8 and clear it */
  timeout = TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }

  /* Send I2C2 STOP Condition */
  I2C_GenerateSTOP(I2C2, ENABLE);

  /* If operation is OK, return 0 */
  return 0;
}

uint8_t mt9v034_WriteReg16(uint16_t address, uint16_t Data)
{
	uint8_t result = mt9v034_WriteReg(address, (uint8_t)( Data >> 8)); // write upper byte
	result |= mt9v034_WriteReg(0xF0, (uint8_t) Data); // write lower byte
	return result;
}

uint8_t mt9v034_ReadReg(uint16_t Addr)
{
  uint32_t timeout = TIMEOUT_MAX;
  uint8_t Data = 0;

  /* Generate the Start Condition */
  I2C_GenerateSTART(I2C2, ENABLE);

  /* Test on I2C2 EV5 and clear it */
  timeout = TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }

  /* Send DCMI selcted device slave Address for write */
  I2C_Send7bitAddress(I2C2, mt9v034_DEVICE_READ_ADDRESS, I2C_Direction_Transmitter);

  /* Test on I2C2 EV6 and clear it */
  timeout = TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }

  /* Send I2C2 location address LSB */
  I2C_SendData(I2C2, (uint8_t)(Addr));

  /* Test on I2C2 EV8 and clear it */
  timeout = TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }

  /* Clear AF flag if arised */
  I2C2->SR1 |= (uint16_t)0x0400;

  /* Generate the Start Condition */
  I2C_GenerateSTART(I2C2, ENABLE);

  /* Test on I2C2 EV6 and clear it */
  timeout = TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }

  /* Send DCMI selcted device slave Address for write */
  I2C_Send7bitAddress(I2C2, mt9v034_DEVICE_READ_ADDRESS, I2C_Direction_Receiver);

  /* Test on I2C2 EV6 and clear it */
  timeout = TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }

  /* Prepare an NACK for the next data received */
  I2C_AcknowledgeConfig(I2C2, DISABLE);

  /* Test on I2C2 EV7 and clear it */
  timeout = TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
    /* If the timeout delay is exeeded, exit with error code */
    if ((timeout--) == 0) return 0xFF;
  }

  /* Prepare Stop after receiving data */
  I2C_GenerateSTOP(I2C2, ENABLE);

  /* Receive the Data */
  Data = I2C_ReceiveData(I2C2);

  /* return the read data */
  return Data;
}

uint16_t mt9v034_ReadReg16(uint8_t address)
{
	uint16_t result = mt9v034_ReadReg(address) << 8; // read upper byte
	result |= mt9v034_ReadReg(0xF0); // read lower byte
	return result;
}

void dcmi_dma_enable()
{
	/* Enable DMA2 stream 1 and DCMI interface then start image capture */
	DMA_Cmd(DMA2_Stream1, ENABLE);
	DCMI_Cmd(ENABLE);
	DCMI_CaptureCmd(ENABLE);
}

void dcmi_dma_disable()
{
	/* Enable DMA2 stream 1 and DCMI interface then start image capture */
	DMA_Cmd(DMA2_Stream1, DISABLE);
	DCMI_Cmd(DISABLE);
	DCMI_CaptureCmd(DISABLE);
}

void send_DCMI_Image_8bit_1(void)
{
	int i;
	unsigned char x, binbyte, d0, d1, d2;

	for(i=0;i<FULL_IMAGE_SIZE;i++)
	{
	binbyte = dcmi_image_buffer_8bit_1[i];
	//Convert from Hex to Decimal to ASCII
	x = binbyte/10;
	d0 = (binbyte%10)|0x30;
	d1 = (x%10)|0x30;
	d2 = (x/10)|0x30;
	//send original data and ASCII d2,d1,d0
	PrintChar('[');
	PrintChar(dcmi_image_buffer_8bit_1[i]);
	PrintChar(']');
	PrintChar('=');
	PrintChar(d2);
	PrintChar(d1);
	PrintChar(d0);
	PrintChar('\n');
	}
}

void send_DCMI_Image_8bit_1_SmartUAV(void)
{
	int i;
	//uint8_t message[] = "\nData\n";
	//Message
	//for(i=0;i<6;i++)
	//		PrintChar(message[i]);
	//Start bytes
	PrintChar(0xff);
	PrintChar(0x00);
	PrintChar(0x00);
	//Image bytes
	for(i=0;i<FULL_IMAGE_SIZE;i++)
		PrintChar(dcmi_image_buffer_8bit_1[i]);
}

void send_DCMI_Image_8bit_1_Qt(void)
{
	unsigned int height;
	unsigned int width;

	for(height=0;height<120;height++)
		for(width=0;width<188;width++)
			dcmi_image_buffer_8bit_3[188*height+width]=dcmi_image_buffer_8bit_1[188*height+width];
	PrintChar(0xFF);
	PrintChar(0x00);
	PrintChar(0x00);
	for(height=0;height<120;height++)
		for(width=0;width<188;width++)
			PrintChar(dcmi_image_buffer_8bit_3[188*height+width]);
}

