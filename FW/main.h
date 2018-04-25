/*
 * main.h
 *
 *  Created on: 21.06.2010
 *      Author: ARV
 */

#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 9600000

/// длительность нажатия кнопки в ДЕСЯТКАХ МС для немедленного выключения
#define	T_STANDBY		600
/// длительность нажатия кнопки в ДЕСЯТКАХ МС для активации автоотключения
#define T_AUTOOFF		200
/// длительность выдержки времени до автоотключения в МИНУТАХ
#define OFF_MIN			30
/// откорректированная задержка в 10 мс в МИКРОСЕКУНДАХ
#define MS10_US			9000
/// длительность импульса отпирания симистора в условных единицах - шагах фазы
#define	PULSE			20
/// число разных значений фазы для разных скоростей вращения
#define MAX_STEP		7

/// Частота питающей сети в ГЕРЦАХ
#define	F_PWR			50
/// Предделитель таймера :8
#define DIVIDER			8

// все сигналы должны подключаться к одному порту!!!
/// выходной порт
#define	OUT_PORT		PORTB
/// пин для управления симистором
#define OUT_PIN			_BV(PB0)
//пин детектора перехода через ноль
#define ZERO_PIN		_BV(PB1)
/// пин для кнопки
#define BTN_PIN			_BV(PB3)
/// пин для светодиода 1
#define LED1			_BV(PB2)
/// пин для светодоида 2
#define LED2			_BV(PB3)
/// макрос для обоих светодиодов сразу
#define ALL_LEDS		(LED1 | LED2)

// макросы, зависящие от выбранного МК
//#if defined(__AVR_ATtiny13__)
/// вектор прерывания таймера, используемый для фазового регулирования
#define VECTOR 			TIM0_COMPA_vect
/// макрос настройки таймера фазового регулирования
#define config_timer() 				\
	OCR0A	= OCR_VALUE;			\
	TCCR0A 	= _BV(WGM01);			\
	TIMSK0	= _BV(OCIE0A)			\
/// запуск таймера
#define start_timer()	TCCR0B = _BV(CS01)
/// остановка таймера
#define stop_timer()	TCCR0B = 0


	
/// Значение для сравнения в таймере
#define	OCR_VALUE		(F_CPU / (F_PWR * 2UL * 256UL * DIVIDER)-2)
/// определение регистра DDR по заданному порту
#define DDR				(*(volatile uint8_t*)(&OUT_PORT - 1))
/// определение входного регистра по заданному порту
#define PIN				(*(volatile uint8_t*)(&OUT_PORT - 2))

#endif /* MAIN_H_ */
