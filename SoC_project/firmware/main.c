#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <irq.h>
#include <uart.h>
#include <console.h>
#include <generated/csr.h>

#include "delay.h"
#include "display.h"

static char *readstr(void)
{
	char c[2];
	static char s[64];
	static int ptr = 0;

	if(readchar_nonblock()) {
		c[0] = readchar();
		c[1] = 0;
		switch(c[0]) {
			case 0x7f:
			case 0x08:
				if(ptr > 0) {
					ptr--;
					putsnonl("\x08 \x08");
				}
				break;
			case 0x07:
				break;
			case '\r':
			case '\n':
				s[ptr] = 0x00;
				putsnonl("\n");
				ptr = 0;
				return s;
			default:
				if(ptr >= (sizeof(s) - 1))
					break;
				putsnonl(c);
				s[ptr] = c[0];
				ptr++;
				break;
		}
	}
	return NULL;
}

static char *get_token(char **str)
{
	char *c, *d;

	c = (char *)strchr(*str, ' ');
	if(c == NULL) {
		d = *str;
		*str = *str+strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c+1;
	return d;
}

static void prompt(void)
{
	printf("RUNTIME>");
}

static void help(void)
{
	puts("Available commands:");
	puts("help                            - this command");
	puts("reboot                          - reboot CPU");
	puts("led                             - led test");
	puts("switch                          - switch test");
	puts("display                         - display test");
	puts("rgbled                          - rgb led test");
	puts("vga                             - vga test");
	puts("mt                              - motors test");
	puts("ir                              - infrarrojo test");
	puts("sv                              - servo test");
	puts("us                              - ultrasonido test");
	puts("b                               - bluetooth test");
	puts("ht                              - Humedad - temperatura test");
	puts("ct                              - Cartographer :)");
}

static void reboot(void) 
{
	ctrl_reset_write(1);
}

static void display_test(void)
{
	int i;
	signed char defill = 0;	//1->left, -1->right, 0->.
	
	char txtToDisplay[40] = {0, DISPLAY_0, DISPLAY_1,DISPLAY_2,DISPLAY_3,DISPLAY_4,DISPLAY_5,DISPLAY_6,DISPLAY_7,DISPLAY_8,DISPLAY_9,DISPLAY_A,DISPLAY_B,DISPLAY_C,DISPLAY_D,DISPLAY_E,DISPLAY_F,DISPLAY_G,DISPLAY_H,DISPLAY_I,DISPLAY_J,DISPLAY_K,DISPLAY_L,DISPLAY_M,DISPLAY_N,DISPLAY_O,DISPLAY_P,DISPLAY_Q,DISPLAY_R,DISPLAY_S,DISPLAY_T,DISPLAY_U,DISPLAY_V,DISPLAY_W,DISPLAY_X,DISPLAY_Y,DISPLAY_Z,DISPLAY_DP,DISPLAY_TR,DISPLAY_UR};
	
	printf("Test del los display de 7 segmentos... se interrumpe con el botton 1\n");

	while(!(buttons_in_read()&1)) {
		display(txtToDisplay);
		if(buttons_in_read()&(1<<1)) defill = ((defill<=-1) ? -1 : defill-1);
		if(buttons_in_read()&(1<<2)) defill = ((defill>=1) ? 1 : defill+1);
		if (defill > 0) {
			char tmp = txtToDisplay[0];
			for(i=0; i<sizeof(txtToDisplay)/sizeof(txtToDisplay[i]); i++) {
				txtToDisplay[i] = ((i==sizeof(txtToDisplay)/sizeof(txtToDisplay[i])-1) ? tmp : txtToDisplay[i+1]);
			}
		}
		else if(defill < 0) {
			char tmp = txtToDisplay[sizeof(txtToDisplay)/sizeof(txtToDisplay[0])-1];
			for(i=sizeof(txtToDisplay)/sizeof(txtToDisplay[i])-1; i>=0; i--) {
				txtToDisplay[i] = ((i==0) ? tmp : txtToDisplay[i-1]);
			}
		}
		delay_ms(500);
	}
}

static void led_test(void)
{
	unsigned int i;
	printf("Test del los leds... se interrumpe con el botton 1\n");
	while(!(buttons_in_read()&1)) {

	for(i=1; i<65536; i=i*2) {
		leds_out_write(i);
		delay_ms(50);
	}
	for(i=32768;i>1; i=i/2) {
		leds_out_write(i);
		delay_ms(50);
	}
	}
	
}


static void switch_test(void)
{
	unsigned short temp2 =0;
	printf("Test del los interruptores... se interrumpe con el botton 1\n");
	while(!(buttons_in_read()&1)) {
		unsigned short temp = switchs_in_read();
		if (temp2 != temp){
			printf("switch bus : %i\n", temp);
			leds_out_write(temp);
			temp2 = temp;
		}
	}
}

static void rgbled_test(void)
{
	unsigned int T = 128;
	
	ledRGB_1_r_period_write(T);
	ledRGB_1_g_period_write(T);
	ledRGB_1_b_period_write(T);

	ledRGB_1_r_enable_write(1);
	ledRGB_1_g_enable_write(1);
	ledRGB_1_b_enable_write(1);

	
	/*ledRGB_2_r_period_write(T);
	ledRGB_2_g_period_write(T);
	ledRGB_2_b_period_write(T);
	
	
	ledRGB_2_r_enable_write(1);
	ledRGB_2_g_enable_write(1);
	ledRGB_2_b_enable_write(1);*/

	for (unsigned int j=0; j<100; j++){
		ledRGB_1_g_width_write(j);
		for (unsigned int i=0; i<100; i++){
			ledRGB_1_r_width_write(100-i);
			ledRGB_1_b_width_write(i);	
			delay_ms(20);
		}	
	}
}


static void vga_test(void)
{
	int x,y;
	
	for(y=0; y<480; y++) {
		for(x=0; x<640; x++) {
			vga_cntrl_mem_we_write(0);
			vga_cntrl_mem_adr_write(y*640+x);
			if(x<640/3)	
				vga_cntrl_mem_data_w_write(((int)(x/10)%2^(int)(y/10)%2)*15);
			else if(x<2*640/3) 
				vga_cntrl_mem_data_w_write((((int)(x/10)%2^(int)(y/10)%2)*15)<<4);
			else 
				vga_cntrl_mem_data_w_write((((int)(x/10)%2^(int)(y/10)%2)*15)<<8);
			vga_cntrl_mem_we_write(1);
		}
	}
}

static void motor_test(void)
{
	int state = 0;
	while (1)
	{
		switch (state)
		{
		case 0:
			mt_driver_movimiento_write(1); // 1 - forward
			delay_ms(1000);
			state = 1;
			break;
		case 1:
			mt_driver_movimiento_write(2); // 2 - backward
			delay_ms(1000);
			state = 2;
			break;
		case 2:
			mt_driver_movimiento_write(3); // 3 - right
			delay_ms(1000);
			state = 3;
			break;
		case 3:
			mt_driver_movimiento_write(4); // 4 - left
			delay_ms(1000);
			state = 4;
			break;
		case 4:
			mt_driver_movimiento_write(0); // 0 - stop
			delay_ms(1000);
			return;
			break;
		}
	}
}

static void ir_test(void)
{
	printf("Test del infrarrojo... se interrumpe con el botton 1\n");
	while (!(buttons_in_read() & 1))
	{
		bool L = ir_driver_L_read();
		bool LC = ir_driver_LC_read();
		bool C = ir_driver_C_read();
		bool RC = ir_driver_RC_read();
		bool R = ir_driver_R_read();

		bool IR[5] = {L, LC, C, RC, R};

		for (int i = 0; i < 5; i++)
		{
			printf("%i, ", IR[i]);
		}
		printf("\n");
	}
}

static void servo_test(void)
{
	servo_driver_pos_write(0); // 0 - 0°
	delay_ms(1000);
	servo_driver_pos_write(1); // 1 - 90°
	delay_ms(1000);
	servo_driver_pos_write(2); // 2 - 180°
	delay_ms(1000);
	servo_driver_pos_write(1);
}

static void ultraSound_test(void)
{
	char str[20];
	us_driver_init_write(1);
	// Se esperan 20 ms para dar tiempo a que el registro done se actualice
	delay_ms(20);
	while (1)
	{
		if (us_driver_done_read() == 1)
		{
			int d = us_driver_distance_read();
			//us_driver_init_write(0);
			itoa(d, str, 10); // int to string - 10 significa decimal
			printf(str);
			printf("\n");
			delay_ms(1000);
			
		}
		else if ((buttons_in_read() & 1))
		{
			us_driver_init_write(0);
			break;
		}
		
	}
}

static void TH_test(void)
{
	char str[20];
	int temp;

	i2c_master_w_write(0x44);

		while (1)
	{
		temp = i2c_master_r_read();
		itoa(temp, str, 10); // int to string - 10 significa decimal
		printf(str);
		printf("\n");
		delay_ms(500);

		if ((buttons_in_read() & 1))
		{
			break;
		}
	}
}

static void bt_print(char *str)
{
	for (int i = 0; i < strlen(str); i++)
	{
		uart_bt_rxtx_write(str[i]);
		delay_ms(1);
	}
}

// ------- Robot function ------- //
static void cartographer(void)
{
	bool L, LC, C, RC, R;
	int posIR[5];

	while (1)
	{
		L = ir_driver_L_read();
		LC = ir_driver_LC_read();
		C = ir_driver_C_read();
		RC = ir_driver_RC_read();
		R = ir_driver_R_read();

		posIR[0] = {L, LC, C, RC, R};

		if (posIR == {1,1,0,1,1} )
		{
			mt_driver_movimiento_write(1); // 1 - forward
		}
		else if (posIR == {1,1,1,0,1} || posIR == {1,1,1,1,0} )
		{
			mt_driver_movimiento_write(2); // 2 - backward
		}
		else if (posIR == {0,1,1,1,1} )
		{
			mt_driver_movimiento_write(3); // 3 - right
		}
		else if (posIR == {1,0,1,1,1} )
		{
			mt_driver_movimiento_write(4); // 4 - left
		}
		else if (posIR == {1,1,1,1,1} )
		{
			mt_driver_movimiento_write(0); // 0 - stop
		}
		{
			mt_driver_movimiento_write(3); // 3 - right
		}
		else if ((L || LC) == 1)
		{
			mt_driver_movimiento_write(2); // 2 - backward
		}
		else if ((buttons_in_read() & 1))
		{
			mt_driver_movimiento_write(0); // 0 - stop
			break;
		}
		
	}
	
}

static void console_service(void)
{
	char *str;
	char *token;

	str = readstr();
	if(str == NULL) return;
	token = get_token(&str);
	if(strcmp(token, "help") == 0)
		help();
	else if(strcmp(token, "reboot") == 0)
		reboot();
	else if(strcmp(token, "led") == 0)
		led_test();
	else if(strcmp(token, "switch") == 0)
		switch_test();
	else if(strcmp(token, "rgbled") == 0)
		rgbled_test();
	else if (strcmp(token, "mt") == 0)
		motor_test();
	else if (strcmp(token, "ir") == 0)
		ir_test();
	else if (strcmp(token, "sv") == 0)
		servo_test();
	else if (strcmp(token, "us") == 0)
		ultraSound_test();
	else if (strcmp(token, "b") == 0)
		bt_print("Prueba de funcionamiento del bluetooth \n");
	else if (strcmp(token, "ht") == 0)
		TH_test();
	else if (strcmp(token, "ct") == 0)
		cartographer();
	// else if(strcmp(token, "vga") == 0)
		// vga_test();
	// else if(strcmp(token, "display") == 0)
		// display_test();
	else printf("\nComando desconocido\n\n");
	// help();
	prompt();
}

// ------- Principal function ------- //
int main(void)
{
//#ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);
//#endif
	uart_init();

	puts("\nSoC - RiscV project UNAL 2021-2-- CPU testing software built "__DATE__" "__TIME__"\n");
	help();
	prompt();

	while(1) {
		console_service();
	}

	return 0;
}
