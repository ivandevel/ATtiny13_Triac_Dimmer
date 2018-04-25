/*
 * main.h
 *
 *  Created on: 21.06.2010
 *      Author: ARV
 */

#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 9600000

/// ������������ ������� ������ � �������� �� ��� ������������ ����������
#define	T_STANDBY		600
/// ������������ ������� ������ � �������� �� ��� ��������� ��������������
#define T_AUTOOFF		200
/// ������������ �������� ������� �� �������������� � �������
#define OFF_MIN			30
/// ������������������ �������� � 10 �� � �������������
#define MS10_US			9000
/// ������������ �������� ��������� ��������� � �������� �������� - ����� ����
#define	PULSE			20
/// ����� ������ �������� ���� ��� ������ ��������� ��������
#define MAX_STEP		7

/// ������� �������� ���� � ������
#define	F_PWR			50
/// ������������ ������� :8
#define DIVIDER			8

// ��� ������� ������ ������������ � ������ �����!!!
/// �������� ����
#define	OUT_PORT		PORTB
/// ��� ��� ���������� ����������
#define OUT_PIN			_BV(PB0)
//��� ��������� �������� ����� ����
#define ZERO_PIN		_BV(PB1)
/// ��� ��� ������
#define BTN_PIN			_BV(PB3)
/// ��� ��� ���������� 1
#define LED1			_BV(PB2)
/// ��� ��� ���������� 2
#define LED2			_BV(PB3)
/// ������ ��� ����� ����������� �����
#define ALL_LEDS		(LED1 | LED2)

// �������, ��������� �� ���������� ��
//#if defined(__AVR_ATtiny13__)
/// ������ ���������� �������, ������������ ��� �������� �������������
#define VECTOR 			TIM0_COMPA_vect
/// ������ ��������� ������� �������� �������������
#define config_timer() 				\
	OCR0A	= OCR_VALUE;			\
	TCCR0A 	= _BV(WGM01);			\
	TIMSK0	= _BV(OCIE0A)			\
/// ������ �������
#define start_timer()	TCCR0B = _BV(CS01)
/// ��������� �������
#define stop_timer()	TCCR0B = 0


	
/// �������� ��� ��������� � �������
#define	OCR_VALUE		(F_CPU / (F_PWR * 2UL * 256UL * DIVIDER)-2)
/// ����������� �������� DDR �� ��������� �����
#define DDR				(*(volatile uint8_t*)(&OUT_PORT - 1))
/// ����������� �������� �������� �� ��������� �����
#define PIN				(*(volatile uint8_t*)(&OUT_PORT - 2))

#endif /* MAIN_H_ */
