// Timer Library
// Simon Walker, NAIT

#include <compiler.h>
#include "timer.h"

void Timer_Init (Timer_Prescale pre, unsigned int uiInitialOffset)
{
	// start code will power off all modules...
	PRR0 &= ~(1 << PRTIM1);

	// set prescale to requested rate
	TCCR1B = 0;		// noise canceler disabled, waveform generator normal
	TCCR1B |= pre;	// put back requested prescale bits
	
	// setup initial event for output compare 1 A
	OCR1A = TCNT1 + uiInitialOffset;

	// setup interrupt for output compare
	// timer/counter 1, output compare A match interrupt enable
	TIMSK1 = 0b00000010;
}

