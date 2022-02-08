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

	if (readchar_nonblock())
	{
		c[0] = readchar();
		c[1] = 0;
		switch (c[0])
		{
		case 0x7f:
		case 0x08:
			if (ptr > 0)
			{
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
			if (ptr >= (sizeof(s) - 1))
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
	if (c == NULL)
	{
		d = *str;
		*str = *str + strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c + 1;
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
	puts("ir                              - ir test");
}

static void reboot(void)
{
	ctrl_reset_write(1);
}

static void display_test(void)
{
	int i;
	signed char defill = 0; // 1->left, -1->right, 0->.

	char txtToDisplay[40] = {0, DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_A, DISPLAY_B, DISPLAY_C, DISPLAY_D, DISPLAY_E, DISPLAY_F, DISPLAY_G, DISPLAY_H, DISPLAY_I, DISPLAY_J, DISPLAY_K, DISPLAY_L, DISPLAY_M, DISPLAY_N, DISPLAY_O, DISPLAY_P, DISPLAY_Q, DISPLAY_R, DISPLAY_S, DISPLAY_T, DISPLAY_U, DISPLAY_V, DISPLAY_W, DISPLAY_X, DISPLAY_Y, DISPLAY_Z, DISPLAY_DP, DISPLAY_TR, DISPLAY_UR};

	printf("Test del los display de 7 segmentos... se interrumpe con el botton 1\n");

	while (!(buttons_in_read() & 1))
	{
		display(txtToDisplay);
		if (buttons_in_read() & (1 << 1))
			defill = ((defill <= -1) ? -1 : defill - 1);
		if (buttons_in_read() & (1 << 2))
			defill = ((defill >= 1) ? 1 : defill + 1);
		if (defill > 0)
		{
			char tmp = txtToDisplay[0];
			for (i = 0; i < sizeof(txtToDisplay) / sizeof(txtToDisplay[i]); i++)
			{
				txtToDisplay[i] = ((i == sizeof(txtToDisplay) / sizeof(txtToDisplay[i]) - 1) ? tmp : txtToDisplay[i + 1]);
			}
		}
		else if (defill < 0)
		{
			char tmp = txtToDisplay[sizeof(txtToDisplay) / sizeof(txtToDisplay[0]) - 1];
			for (i = sizeof(txtToDisplay) / sizeof(txtToDisplay[i]) - 1; i >= 0; i--)
			{
				txtToDisplay[i] = ((i == 0) ? tmp : txtToDisplay[i - 1]);
			}
		}
		delay_ms(500);
	}
}

static void led_test(void)
{
	unsigned int i;
	printf("Test del los leds... se interrumpe con el botton 1\n");
	while (!(buttons_in_read() & 1))
	{

		for (i = 1; i < 65536; i = i * 2)
		{
			leds_out_write(i);
			delay_ms(50);
		}
		for (i = 32768; i > 1; i = i / 2)
		{
			leds_out_write(i);
			delay_ms(50);
		}
	}
}

static void switch_test(void)
{
	unsigned short temp2 = 0;
	printf("Test del los interruptores... se interrumpe con el botton 1\n");
	while (!(buttons_in_read() & 1))
	{
		unsigned short temp = switchs_in_read();
		if (temp2 != temp)
		{
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

	for (unsigned int j = 0; j < 100; j++)
	{
		ledRGB_1_g_width_write(j);
		for (unsigned int i = 0; i < 100; i++)
		{
			ledRGB_1_r_width_write(100 - i);
			ledRGB_1_b_width_write(i);
			delay_ms(20);
		}
	}
}

static void vga_test(void)
{
	int x, y;

	for (y = 0; y < 480; y++)
	{
		for (x = 0; x < 640; x++)
		{
			vga_cntrl_mem_we_write(0);
			vga_cntrl_mem_adr_write(y * 640 + x);
			if (x < 640 / 3)
				vga_cntrl_mem_data_w_write(((int)(x / 10) % 2 ^ (int)(y / 10) % 2) * 15);
			else if (x < 2 * 640 / 3)
				vga_cntrl_mem_data_w_write((((int)(x / 10) % 2 ^ (int)(y / 10) % 2) * 15) << 4);
			else
				vga_cntrl_mem_data_w_write((((int)(x / 10) % 2 ^ (int)(y / 10) % 2) * 15) << 8);
			vga_cntrl_mem_we_write(1);
		}
	}
}

static void ir_test()
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

	static void console_service(void)
{
	char *str;
	char *token;

	str = readstr();
	if (str == NULL)
		return;
	token = get_token(&str);
	if (strcmp(token, "help") == 0)
		help();
	else if (strcmp(token, "reboot") == 0)
		reboot();
	else if (strcmp(token, "led") == 0)
		led_test();
	else if (strcmp(token, "switch") == 0)
		switch_test();
	// else if(strcmp(token, "display") == 0)
	// display_test();
	else if (strcmp(token, "rgbled") == 0)
		rgbled_test();
	// else if(strcmp(token, "vga") == 0)
	// vga_test();

	else if (strcmp(token, "ir") == 0)
		ir_test();
	prompt();
}

int main(void)
{
#ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);
#endif
	uart_init();

	puts("\nSoC - RiscV project UNAL 2021-2-- CPU testing software built "__DATE__
		 " "__TIME__
		 "\n");
	help();
	prompt();

	while (1)
	{
		console_service();
	}

	return 0;
}