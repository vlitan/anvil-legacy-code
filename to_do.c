#define \
\\\\\\\\TODO NEXT\\\\\\\\\\
write some strategy!\
read real current\
testestestestest!!\
more tests on interrupts, wdt, and overall flow\
unit tests!!!\
read sensors on interrupts\
\\\\\\\\\\\\\\\\\\\\\\\\\\\

//ISR(WDT_vect) on 644 also resets 328\
//assign pins conforming with the scheme\
//wdt_init needs to be more flebile
//watchdog on 328 listening to 644 comands (?)
//log_str test
//request system for current
//request system
//ADC on 328
//ack byte 328 -> 644
//ack byte 644 -> 328

"~~~~~~~~~features~~~~~"
Inverted on one timer
test and use alert system
"~~~~~~~~~~~~~~~~~~~~~~"

//=>read command module on interrupt + use while to wait

=>strategy 1 te-am vazut te-am ftt
	atac directionat de ponderea senzorilor.
	senzorii de linie opresc treaba

//(DONE) test SPEED_LIMIT & SPEED_FACTOR in at644->motors
//(DONE) add SPEED_LIMIT & SPEED_FACTOR in at328 (or not?)
//see instructions

communication
//add current feedback (on 328) + reading on 644 <= test this
//test current logging (on 644) <= test this
//ACK feature
//TEST EVERYTHING AGAIN! - due to type changes

how to test everything:
/*from 644 go trough all the possible values -255 -> 255 write to 328
328 writes them back with oposite sign as current values
start / stop on sensors
display all on screen*/

test cases:
//644 resets
//328 resets
//both reset


//read sensors! (DONE)
