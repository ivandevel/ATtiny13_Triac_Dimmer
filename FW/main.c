#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/wdt.h>
//#include <avr/pgmspace.h>
#include <util/delay.h>
//#include <avr/eeprom.h>

/// текущее значение заданной фазы регул€тора
uint8_t phase = 90;	
// счетчик программной фазы
//volatile uint8_t counter = 0;
volatile uint8_t ADC_stop = 0;

#define RANGE(a,min,max) ((a)<(min))?(min):(((a)>(max))?(max):(a))

/** Обработчик прерывания от таймера, ведущего отсчет фазы.
 * реализует импульсное управление симистором.
 */

ISR(TIM0_COMPA_vect) {		
		TCCR0A = 0x00;
		TCCR0B = 0x00;
		TIMSK0 &= ~(1<<OCIE0A); //останавливаем таймер
		ADC_stop = 0; //разрешаем читать ј÷ѕ
}

/** Обработчик внешнего запроса прерывани€.
 * выполн€ет синхрогнизацию фазорегулятора.
 */
ISR(INT0_vect) {
	OCR0A = phase; //регистру сравнени§ присваиваем значение §ркости	                   
	TCCR0A |= _BV(WGM01) | _BV(COM0A1);    //запускаем таймер 0 в режиме CTC с установкой OC0A в 1 
	TCCR0B |= _BV(CS02) | _BV(CS00); // с периодом счета 75khz (9600000/128) 13 мкс
	TIMSK0 |= (1<<OCIE0A); // запускаем таймер
	ADC_stop = 1;
}

int read_adc (unsigned char adc_input)
{
	// This is the code that selects the channel. AND out the entire mux area,
	// then OR in the desired analog channel.
	//ADMUX = 0;//&= ~((1<<MUX1) | (1<<MUX0));
	ADMUX = adc_input;
	// Delay needed for the stabilization of the ADC input voltage
	_delay_us(10);
	// Start the conversion
	ADCSRA |= (1 << ADSC);
	// Wait for it to finish
	while (ADCSRA & (1 << ADSC));

	return ADC;
}

#define POW_MAX 90
#define POW_MIN 11
#define PidH_Kp   20  //номинальный коэффициент PidH_Kp
#define PidH_Ki   22  //номинальный коэффициент PidH_Ki
#define PidH_Kd   20  //номинальный коэффициент PidH_Kd
#define TSAMPLE   100
 
int16_t PidH(uint16_t ref, int16_t inp)
{
	static int16_t Xp = 0;  //предыдущее значение inp
	static int16_t Xpp = 0; //пред-предыдущее значение inp
	static int32_t Y = 0;
	//вычисление ошибки:
	int16_t e = ref - inp;
	//учет пропорциональной составл€ющей:
	Y = Y - (int16_t)PidH_Kp * (inp - Xp) * 10;
	//учет интегральной составл€ющей:
	Y = Y + (int16_t)PidH_Ki * e * TSAMPLE / 100;
	//учет дифференциальной составл€ющей:
	Y = Y - (int16_t)PidH_Kd * (inp - 2 * Xp + Xpp) * 500 / TSAMPLE;
	//обновление значений:
	Xpp = Xp; Xp = inp;
	//if(!ref) Y = 0;
	//ограничение выходного значени€:
	if(Y > POW_MAX * 256L) Y = POW_MAX * 256L;
	if(Y < POW_MIN * 256L) Y = POW_MIN * 256L;
	return(Y >> 8);
}

int main (void) {
		//инициализаци€ всей периферии
		cli();
		// настройка портов
		DDR			= OUT_PIN | LED2;
		OUT_PORT	= OUT_PIN | ZERO_PIN;
		
		TIMSK0 |= (1<<OCIE0A); //разрешение прерывани€ по переполнению таймера 1		
		MCUCR |= (1<<ISC01) | (1<<ISC00); //rising age of INT0
		GIMSK |= (1<<INT0); //enable INT0

		// Set the prescaler to clock/128 & enable ADC
		ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
		
		sei();

	// основной цикл
	while(1){
		
           if(!ADC_stop)
           {
			   #if 1
			   phase = 90 - read_adc(2)/12;// +5;	//phase controlled by adc input
			   #else
			   phase = (uint8_t)PidH((int16_t)((uint32_t)read_adc(2)*43/100), (int16_t)((uint32_t)read_adc(1)*43/100)); //phase controlled by PID law
			   if (phase < 7) OUT_PORT |= LED2; else  OUT_PORT &= ~LED2;
			   #endif
			   ADC_stop = 1;	           		   
           }
	}
}